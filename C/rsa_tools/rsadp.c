/**
 * @file rsadp.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief 
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <tommath.h>

#include "rsa_tools.h"

/**
 * @brief 
 * 
 * @param key 
 * @param emsg 
 * @param emlen 
 * @param msg 
 * @param mlen 
 * @param use_crt 
 * @return int 
 * 
 * 
5.1.2.  RSADP

   RSADP (K, c)

   Input:

         K RSA private key, where K has one of the following forms:

         +  a pair (n, d)
         +  a quintuple (p, q, dP, dQ, qInv) and a possibly empty
            sequence of triplets (r_i, d_i, t_i), i = 3, ..., u

         c ciphertext representative, an integer between 0 and n - 1

   Output:  m message representative, an integer between 0 and n - 1

   Error:  "ciphertext representative out of range"

   Assumption:  RSA private key K is valid

   Steps:

      1.  If the ciphertext representative c is not between 0 and n - 1,
          output "ciphertext representative out of range" and stop.

      2.  The message representative m is computed as follows.

          a.  If the first form (n, d) of K is used, let m = c^d mod n.

          b.  If the second form (p, q, dP, dQ, qInv) and (r_i, d_i,
              t_i) of K is used, proceed as follows:

              i.   Let m_1 = c^dP mod p and m_2 = c^dQ mod q.

              ii.  If u > 2, let m_i = c^(d_i) mod r_i, i = 3, ..., u.

              iii. Let h = (m_1 - m_2) * qInv mod p.

              iv.  Let m = m_2 + q * h.

              v.   If u > 2, let R = r_1 and for i = 3 to u do

                   1.  Let R = R * r_(i-1).

                   2.  Let h = (m_i - m) * t_i mod r_i.

                   3.  Let m = m + R * h.

      3.  Output m.

   Note: Step 2.b can be rewritten as a single loop, provided that one
   reverses the order of p and q.  For consistency with PKCS #1 v2.0,
   however, the first two primes p and q are treated separately from the
   additional primes.

 */
int rsadp(RSA_TOOLS_PRIV_KEY_t key, unsigned char *emsg, size_t emlen, unsigned char *msg, size_t *mlen, bool use_crt)
{
    int    ret;
    size_t crt_len;
	mp_int d, n, p, q, dp, dq, qinv;
	mp_int c, m;
	mp_int m_1, m_2;
	mp_int h;
	mp_int bo;
	mp_int a;
	mp_int b;

    crt_len = key.n_len / 2;
	if ((NULL == emsg) || (NULL == msg) || (NULL == mlen)) {
		ret = -255;
	}
	else {
        if ((key.n_len != key.d_len) ||
            (crt_len != key.p_len) ||
            (crt_len != key.q_len) ||
            (crt_len != key.dp_len) ||
            (crt_len != key.dq_len)||
            (crt_len != key.qinv_len)) {
            ret = -255;
        }
        else {
            switch (key.n_len) {
                case 128 :	/* RSA 1024 bit */
                case 256 :	/* RSA 2048 bit */
                case 384 :	/* RSA 3072 bit */
                case 512 :	/* RSA 4096 bit */
                    if ((key.n_len != emlen) ||
                        (key.n_len != *mlen)) {
                        ret = -255;
                    }
                    else {
                        ret = 0;
                    }
                    break;
                default:
                    ret = -255;
                    break;
            }
        }
	}


    if (0 != ret) {
        /* Error case */
    }
    else {
    	assert(MP_OKAY == mp_init(&n));
    	assert(MP_OKAY == mp_init(&d));
    	assert(MP_OKAY == mp_init(&p));
    	assert(MP_OKAY == mp_init(&q));
	    assert(MP_OKAY == mp_init(&dp));
	    assert(MP_OKAY == mp_init(&dq));
    	assert(MP_OKAY == mp_init(&qinv));
    	assert(MP_OKAY == mp_init(&c));

    	assert(MP_OKAY == mp_read_unsigned_bin(&n,    key.n,    key.n_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&d,    key.d,    key.d_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&p,    key.p,    key.p_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&q,    key.q,    key.q_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&dp,   key.dp,   key.dp_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&dq,   key.dq,   key.dq_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&qinv, key.qinv, key.qinv_len));
	    assert(MP_OKAY == mp_read_unsigned_bin(&c,    emsg,     emlen));

	    assert(MP_OKAY == mp_init(&m));
	    assert(MP_OKAY == mp_init(&m_1));
	    assert(MP_OKAY == mp_init(&m_2));
	    assert(MP_OKAY == mp_init(&h));
	    assert(MP_OKAY == mp_init(&bo));
	    assert(MP_OKAY == mp_init(&a));
	    assert(MP_OKAY == mp_init(&b));

        mp_clear(&m);
        mp_clear(&m_1);
        mp_clear(&m_2);
        mp_clear(&h);
        mp_clear(&bo);
        mp_clear(&a);
        mp_clear(&b);

        if (use_crt) {
            assert(MP_OKAY == mp_exptmod(&c, &dp, &p, &m_1));
            assert(MP_OKAY == mp_exptmod(&c, &dq, &q, &m_2));
            assert(MP_OKAY == mp_sub(&m_1, &m_2, &bo));
            if (MP_NEG == bo.sign) {
                do {
                    assert(MP_OKAY == mp_add(&p, &bo, &b));
                    assert(MP_OKAY == mp_copy(&b, &bo));
                } while (MP_NEG == bo.sign);
            }
            else {
                /* Next */
            }
            assert(MP_OKAY == mp_mulmod(&bo, &qinv, &p, &h));
                
            assert(MP_OKAY == mp_mul(&q, &h, &a));
            ret = mp_add(&a, &m_2, &m);
        }
        else {
            ret = mp_exptmod(&c, &d, &n, &m);
        }
        if (MP_OKAY != ret) {
            ret = -1;
        }
        else {
            *mlen = mp_unsigned_bin_size(&m);
            assert(MP_OKAY == mp_to_unsigned_bin(&m, msg));
            ret = 0;
        }
    }

    return ret;
}