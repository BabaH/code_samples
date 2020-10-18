/**
 * @file pkcs1.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief Refer RFC8017.
 *        PKCS #1: RSA Cryptography Specifications Version 2.2
 *        (https://tools.ietf.org/html/rfc8017)
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <tommath.h>

#include "pkcs1.h"
#include "utils.h"

/**
 * @brief Check a value range as follows.
 *        0 <= a <= (n - 1)
 * 
 * @param a[in]     Buffer a.
 * @param alen[in]  Length of buffer a. 
 * @param n[in]     Buffer which stores a maximum value.
 * @param nlen[in]  Length of buffer n.
 * @return          Result of value range check.
 * 
 * @retval  true    Buffer b's value range is valid.
 * @retval  false   Buffer b's value range is NOT valid.
 */
static bool range_chk(const uint8_t *a, size_t alen, const uint8_t *n, size_t nlen)
{
    bool ret;
	mp_int x;
	mp_int y;

	assert(MP_OKAY == mp_init_multi(&x, &y, NULL));

	assert(MP_OKAY == mp_read_unsigned_bin(&x, a, alen));
	assert(MP_OKAY == mp_read_unsigned_bin(&y, n, nlen));

    ret = (MP_LT == (mp_cmp(&x, &y))) ? true : false;

    return ret;
}


/**
 * @brief RSA encryption primitive (RSAEP)
 * 
 * @param key[in]       RSA Public Key.
 * @param msg[in]       Message buffer.
 * @param mlen[in]      Length of message buffer.
 * @param emsg[out]     Encrypted message buffer.
 * @param emlen[in,out] Length of encrypted message buffer.
 * @return              Status of this function.
 * 
 * @retval PKCS1_E_OK       Success.
 * @retval PKCS1_E_PARAM    Invalid parameter.
 * @retval PKCS1_E_RANGE    Value range error.
 * @retval PKCS1_E_INTERNAL Internal Error.
 *
 * ***************************************************************
 * Reference: PKCS #1: RSA Cryptography Specifications Version 2.2
 * (https://tools.ietf.org/html/rfc8017)
 * 
 * 5.1.1.  RSAEP
 *   RSAEP ((n, e), m)
 *   Input:
 *         (n, e) RSA public key
 *         m message representative, an integer between 0 and n - 1
 *   Output:  c ciphertext representative, an integer between 0 and n - 1
 *   Error:  "message representative out of range"
 *   Assumption:  RSA public key (n, e) is valid
 *   Steps:
 *      1.  If the message representative m is not between 0 and n - 1,
 *          output "message representative out of range" and stop.
 *      2.  Let c = m^e mod n.
 *      3.  Output c.
 * ***************************************************************
 */
int rsaep(RSA_TOOLS_PUB_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *emsg, size_t *emlen)
{
	int    ret;
    int    status;
	mp_int n;
	mp_int e;
	mp_int m;
	mp_int c;

	if ((NULL == msg) || (NULL == emsg) || (NULL == emlen)) {
		ret = PKCS1_E_PARAM;
	}
	else {
		switch (key.n_len) {
			case 128 :	/* RSA 1024 bit */
			case 256 :	/* RSA 2048 bit */
			case 384 :	/* RSA 3072 bit */
			case 512 :	/* RSA 4096 bit */
				if ((key.n_len != mlen) ||
					(key.n_len != *emlen) ||
					(0 == key.e_len) ||
					(key.n_len < key.e_len)) {
					ret = PKCS1_E_PARAM;
				}
				else {
                    if (range_chk(msg, mlen, key.n, key.n_len)) {
                        ret = PKCS1_E_OK;
                    }
                    else {
    					ret = PKCS1_E_RANGE;
                    }
				}
				break;
			default:
				ret = PKCS1_E_PARAM;
				break;
		}
	}

	if (PKCS1_E_OK != ret) {
		/* Error case */
	}
	else {
		assert(MP_OKAY == mp_init_multi(&n, &e, &m, &c, NULL));

		assert(MP_OKAY == mp_read_unsigned_bin(&n, key.n, key.n_len));
		assert(MP_OKAY == mp_read_unsigned_bin(&e, key.e, key.e_len));
		assert(MP_OKAY == mp_read_unsigned_bin(&m, msg,   mlen));

		status = mp_exptmod(&m, &e, &n, &c);
		if (MP_OKAY != status) {
            printf(" (MP Error: %s) ", mp_error_to_string(status));
			ret = PKCS1_E_INTERNAL;
		}
		else {
			*emlen = mp_unsigned_bin_size(&c);
			assert(MP_OKAY == mp_to_unsigned_bin(&c, emsg));
			ret = PKCS1_E_OK;
		}
	}

	return ret;
}


/**
 * @brief RSA encryption primitive (RSADP)
 * 
 * @param key[in]       RSA Private Key.
 * @param emsg[in]      Encrypted message buffer.
 * @param emlen[in]     Length of encrypted message buffer.
 * @param msg[in]       Message buffer.
 * @param mlen[in,out]  Length of message buffer.
 * @param use_crt[in]   CRT flag.
 * @return              Status of this function.
 * 
 * @retval PKCS1_E_OK       Success.
 * @retval PKCS1_E_PARAM    Invalid parameter.
 * @retval PKCS1_E_RANGE    Value range error.
 * @retval PKCS1_E_INTERNAL Internal Error.
 * 
 * ***************************************************************
 * Reference: PKCS #1: RSA Cryptography Specifications Version 2.2
 * (https://tools.ietf.org/html/rfc8017)
 * 
 * 5.1.2.  RSADP
 *   RSADP (K, c)
 *   Input:
 *         K RSA private key, where K has one of the following forms:
 *         +  a pair (n, d)
 *         +  a quintuple (p, q, dP, dQ, qInv) and a possibly empty
 *            sequence of triplets (r_i, d_i, t_i), i = 3, ..., u
 *         c ciphertext representative, an integer between 0 and n - 1
 *   Output:  m message representative, an integer between 0 and n - 1
 *   Error:  "ciphertext representative out of range"
 *   Assumption:  RSA private key K is valid
 *   Steps:
 *      1.  If the ciphertext representative c is not between 0 and n - 1,
 *          output "ciphertext representative out of range" and stop.
 *      2.  The message representative m is computed as follows.
 *          a.  If the first form (n, d) of K is used, let m = c^d mod n.
 *          b.  If the second form (p, q, dP, dQ, qInv) and (r_i, d_i,
 *              t_i) of K is used, proceed as follows:
 *              i.   Let m_1 = c^dP mod p and m_2 = c^dQ mod q.
 *              ii.  If u > 2, let m_i = c^(d_i) mod r_i, i = 3, ..., u.
 *              iii. Let h = (m_1 - m_2) * qInv mod p.
 *              iv.  Let m = m_2 + q * h.
 *              v.   If u > 2, let R = r_1 and for i = 3 to u do
 *                   1.  Let R = R * r_(i-1).
 *                   2.  Let h = (m_i - m) * t_i mod r_i.
 *                   3.  Let m = m + R * h.
 *      3.  Output m.
 *
 *   Note: Step 2.b can be rewritten as a single loop, provided that one
 *   reverses the order of p and q.  For consistency with PKCS #1 v2.0,
 *   however, the first two primes p and q are treated separately from the
 *   additional primes.
 * ***************************************************************
 * ***************************************************************
 * Reference: PKCS #1: RSA Cryptography Specifications Version 2.0
 * (https://tools.ietf.org/html/rfc2437)
 * 
 * 5.1.2 RSADP
 *   RSADP (K, c)
 *   Input:
 *   K         RSA private key, where K has one of the following forms
 *                 -a pair (n, d)
 *                 -a quintuple (p, q, dP, dQ, qInv)
 *   c         ciphertext representative, an integer between 0 and n-1
 *   Output:
 *   m         message representative, an integer between 0 and n-1; or
 *             "ciphertext representative out of range"
 *   Assumptions: private key K is valid
 *   Steps:
 *   1. If the ciphertext representative c is not between 0 and n-1,
 *   output "ciphertext representative out of range" and stop.
 *   2. If the first form (n, d) of K is used:
 *   2.1 Let m = c^d mod n. Else, if the second form (p, q, dP,
 *       dQ, qInv) of K is used:
 *   2.2 Let m_1 = c^dP mod p.
 *   2.3 Let m_2 = c^dQ mod q.
 *   2.4 Let h = qInv ( m_1 - m_2 ) mod p.
 *   2.5 Let m = m_2 + hq.
 *   3. Output m.
 * ***************************************************************
 */
int rsadp(RSA_TOOLS_PRIV_KEY_t key, uint8_t *emsg, size_t emlen, uint8_t *msg, size_t *mlen, bool use_crt)
{
    int    ret;
    int    status;
    size_t crt_len;
	mp_int d, n, p, q, dp, dq, qinv;
	mp_int c, m;
	mp_int m_1, m_2;
	mp_int h;
	mp_int a;

    crt_len = key.n_len / 2;
	if ((NULL == emsg) || (NULL == msg) || (NULL == mlen)) {
		ret = PKCS1_E_PARAM;
	}
	else {
        if (range_chk(emsg, emlen, key.n, key.n_len)) {
            if (use_crt) {
                if ((key.n_len != key.d_len) ||
                    (crt_len != key.p_len) ||
                    (crt_len != key.q_len) ||
                    (crt_len != key.dp_len) ||
                    (crt_len != key.dq_len)||
                    (crt_len != key.qinv_len)) {
                    ret = PKCS1_E_PARAM;
                }
                else {
                    ret = PKCS1_E_OK;
                }
            }
            else {
                if (key.n_len != key.d_len) {
                    ret = PKCS1_E_PARAM;
                }
                else {
                    ret = PKCS1_E_OK;
                }
            }
        }
        else {
            ret = PKCS1_E_RANGE;
        }

        if (PKCS1_E_OK != ret) {
            /* In case of error exit */
        }
        else {
            switch (key.n_len) {
                case 128 :	/* RSA 1024 bit */
                case 256 :	/* RSA 2048 bit */
                case 384 :	/* RSA 3072 bit */
                case 512 :	/* RSA 4096 bit */
                    if ((key.n_len != emlen) ||
                        (key.n_len != *mlen)) {
                        ret = PKCS1_E_PARAM;
                    }
                    else {
                        ret = PKCS1_E_OK;
                    }
                    break;
                default:
                    ret = PKCS1_E_PARAM;
                    break;
            }
        }
	}

    if (PKCS1_E_OK != ret) {
        /* Error case */
    }
    else {
    	assert(MP_OKAY == mp_init_multi(&n, &d, &p, &q, &dp, &dq, &qinv, &c, NULL));

    	assert(MP_OKAY == mp_read_unsigned_bin(&n,    key.n,    key.n_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&d,    key.d,    key.d_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&p,    key.p,    key.p_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&q,    key.q,    key.q_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&dp,   key.dp,   key.dp_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&dq,   key.dq,   key.dq_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&qinv, key.qinv, key.qinv_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&c,    emsg,     emlen));

	    assert(MP_OKAY == mp_init_multi(&m, &m_1, &m_2, &h, &a, NULL));

        mp_clear_multi(&m, &m_1, &m_2, &h, &a, NULL);
        if (use_crt) {
            /* m_1 = c^dP mod p. */
            assert(MP_OKAY == mp_exptmod(&c, &dp, &p, &m_1));
            /* m_2 = c^dQ mod q. */
            assert(MP_OKAY == mp_exptmod(&c, &dq, &q, &m_2));
            /* h = qInv ( m_1 - m_2 ) mod p. */
            assert(MP_OKAY == mp_sub(&m_1, &m_2, &a));
            assert(MP_OKAY == mp_mulmod(&qinv, &a, &p, &h));                
            /* m = m_2 + hq. */
            assert(MP_OKAY == mp_mul(&q, &h, &a));
            status = mp_add(&a, &m_2, &m);
        }
        else {
            status = mp_exptmod(&c, &d, &n, &m);
        }
        if (MP_OKAY != status) {
            ret = PKCS1_E_INTERNAL;
        }
        else {
            *mlen = mp_unsigned_bin_size(&m);
            assert(MP_OKAY == mp_to_unsigned_bin(&m, msg));
            ret = PKCS1_E_OK;
        }
    }

    return ret;
}


/**
 * @brief RSA Signature Primitive, version 1 (RSASP1)
 * 
 * @param key[in]       RSA Private Key.
 * @param msg[in]       Encrypted message buffer.
 * @param mlen[in]      Length of encrypted message buffer.
 * @param sig[out]      Signature buffer.
 * @param slen[in,out]  Length of signature buffer.
 * @param use_crt[in]   CRT flag.
 * @return              Status of this function.
 * 
 * @retval PKCS1_E_OK       Success.
 * @retval PKCS1_E_PARAM    Invalid parameter.
 * @retval PKCS1_E_RANGE    Value range error.
 * @retval PKCS1_E_INTERNAL Internal Error.
 *
 * ***************************************************************
 * Reference: PKCS #1: RSA Cryptography Specifications Version 2.2
 * (https://tools.ietf.org/html/rfc8017)
 * 
 * 5.2.1.  RSASP1
 *   RSASP1 (K, m)
 *   Input:
 *      K        RSA private key, where K has one of the following forms:
 *               - a pair (n, d)
 *               - a quintuple (p, q, dP, dQ, qInv) and a (possibly empty)
 *                 sequence of triplets (r_i, d_i, t_i), i = 3, ..., u
 *      m        message representative, an integer between 0 and n - 1
 *   Output:
 *      s        signature representative, an integer between 0 and n - 1
 *   Error:  "message representative out of range"
 *   Assumption:  RSA private key K is valid
 *   Steps:
 *      1.  If the message representative m is not between 0 and n - 1,
 *          output "message representative out of range" and stop.
 *      2.  The signature representative s is computed as follows.
 *          a.  If the first form (n, d) of K is used, let s = m^d mod n.
 *          b.  If the second form (p, q, dP, dQ, qInv) and (r_i, d_i,
 *              t_i) of K is used, proceed as follows:
 *              1.  Let s_1 = m^dP mod p and s_2 = m^dQ mod q.
 *              2.  If u > 2, let s_i = m^(d_i) mod r_i, i = 3, ..., u.
 *              3.  Let h = (s_1 - s_2) * qInv mod p.
 *              4.  Let s = s_2 + q * h.
 *              5.  If u > 2, let R = r_1 and for i = 3 to u do
 *                  a.  Let R = R * r_(i-1).
 *                  b.  Let h = (s_i - s) * t_i mod r_i.
 *                  c.  Let s = s + R * h.
 *      3.  Output s.
 *
 *   Note: Step 2.b can be rewritten as a single loop, provided that one
 *   reverses the order of p and q.  For consistency with PKCS #1 v2.0,
 *   however, the first two primes p and q are treated separately from the
 *   additional primes.
 * ***************************************************************
 */
int rsasp1(RSA_TOOLS_PRIV_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t *slen, bool use_crt)
{
    int     ret;
    int     status;
    uint8_t *emsg;
    size_t  elen;

    if ((NULL == msg) || (NULL == sig) || (NULL == slen) ||
        (key.n_len != mlen) || (key.n_len > *slen)) {
        ret = PKCS1_E_PARAM;
    }
    else {
        ret = rsadp(key, msg, mlen, sig, slen, use_crt);
    }

    return ret;
}


/*
 * @brief RSA Verification Primitive, version 1 (RSAVP1)
 * 
 * @param key[in]   RSA Public Key.
 * @param msg[in]   Message buffer.
 * @param mlen[in]  Length of message buffer.
 * @param sig[out]   Signature buffer.
 * @param slen[in,out]  Length of signature buffer.
 * @return      Status of this function.
 * 
 * @retval PKCS1_E_OK       Success.
 * @retval PKCS1_E_PARAM    Invalid parameter.
 * @retval PKCS1_E_RANGE    Value range error.
 * @retval PKCS1_E_INTERNAL Internal Error.
 * 
 * ***************************************************************
 * Reference: PKCS #1: RSA Cryptography Specifications Version 2.2
 * (https://tools.ietf.org/html/rfc8017)
 * 
 * 5.2.2.  RSAVP1
 *   RSAVP1 ((n, e), s)
 *   Input:
 *         (n, e) RSA public key
 *         s signature representative, an integer between 0 and n - 1
 *   Output:
 *         m message representative, an integer between 0 and n - 1
 *   Error:  "signature representative out of range"
 *   Assumption:  RSA public key (n, e) is valid
 *   Steps:
 *      1.  If the signature representative s is not between 0 and n - 1,
 *          output "signature representative out of range" and stop.
 *      2.  Let m = s^e mod n.
 *      3.  Output m.
 * ***************************************************************
 */
int rsavp1(RSA_TOOLS_PUB_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t *slen)
{
    int     ret;
    int     status;
    uint8_t *emsg;
    size_t  elen;

    if ((NULL == msg) || (NULL == sig) || (NULL == slen) ||
        (key.n_len != mlen) || (key.n_len > *slen)) {
        ret = PKCS1_E_PARAM;
    }
    else {
        ret = rsaep(key, msg, mlen, sig, slen);
    }

    return ret;
}


/**
 * @brief PKCS1 RSA Sign
 * 
 * @param key[in]       Private Key.
 * @param msg[in]       Message buffer.
 * @param mlen[in]      Length of message buffer.
 * @param sig[out]      Signature buffer.
 * @param slen[in,out]  Length of signature buffer.
 * @param use_crt[in]   Flag
 * @return              Status of this function.
 * 
 * @retval PKCS1_E_OK       Success.
 * @retval PKCS1_E_PARAM    Invalid parameter.
 * @retval PKCS1_E_RANGE    Value range error.
 * @retval PKCS1_E_INTERNAL Internal Error.
 */
int pkcs1_rsa_sign(RSA_TOOLS_PRIV_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t *slen, bool use_crt)
{
    return rsasp1(key, msg, mlen, sig, slen, use_crt);
}

/**
 * @brief PKCS1 RSA Verify
 * 
 * @param key[in]   Public Key.
 * @param msg[in]   Message buffer.
 * @param mlen[in]  Length of message buffer.
 * @param sig[in]   Signature buffer.
 * @param slen[in]  Length of signature buffer.
 * @return          Status of this function.
 *                  If return code is not equal PKCS1_E_OK, it should handle VERIFYCATION ERROR.
 * 
 * @retval PKCS1_E_OK       Success.
 * @retval PKCS1_E_PARAM    Invalid parameter.
 * @retval PKCS1_E_RANGE    Value range error.
 * @retval PKCS1_E_VERIFY   Verify error.
 * @retval PKCS1_E_INTERNAL Internal Error.
 */
int pksc1_rsa_verify(RSA_TOOLS_PUB_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t slen)
{
    int     ret;
    uint8_t *buf;
    size_t  len;
 
    if (key.n_len != slen) {
        ret = PKCS1_E_PARAM;
    }
    else {
        len = slen;
        buf = malloc(len);
        if (NULL != buf) {
            ret = rsavp1(key, msg, mlen, buf, &len);
            if (PKCS1_E_OK != ret) {
                /* In case of error exit */
            }
            else {
                if (blkcmp(sig, slen, buf, len, true)) {
                    ret = PKCS1_E_OK;
                }
                else {
                    ret = PKCS1_E_VERIFY;
                }
            }

            memset(buf, 0, slen);
            free(buf);
            buf = NULL;
        }
        else {
            ret = PKCS1_E_RESOURCE;
        }
    }

    return ret;
}