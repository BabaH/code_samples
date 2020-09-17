/**
 * @file rsaes.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief RSA encryption primitive
 *        Refer 5.1.1 RSAEP of RFC8017.
 *        PKCS #1: RSA Cryptography Specifications Version 2.2
 *        (https://tools.ietf.org/html/rfc8017)
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <assert.h>
#include <tommath.h>

#include "rsa_tools.h"

/**
 * @brief RSA encryption primitive
 * 
 * @param key   RSA Public Key
 * @param src   Message buffer
 * @param slen  Length of Source Buffer
 * @param dst   Encrypted Message buffer
 * @param dlen  Length of Encrypted Message buffer
 * @return      Status of this function
 * @retval 0    Success
 * @retval -1   Fail
 * @retval -255	Invalid parameter

5.1.1.  RSAEP

   RSAEP ((n, e), m)

   Input:

         (n, e) RSA public key

         m message representative, an integer between 0 and n - 1

   Output:  c ciphertext representative, an integer between 0 and n - 1

   Error:  "message representative out of range"

   Assumption:  RSA public key (n, e) is valid

   Steps:

      1.  If the message representative m is not between 0 and n - 1,
          output "message representative out of range" and stop.

      2.  Let c = m^e mod n.

      3.  Output c.

 */
int rsaep(RSA_TOOLS_PUB_KEY_t key, unsigned char *msg, size_t mlen, unsigned char *emsg, size_t *emlen)
{
	int    ret;
	mp_int n;
	mp_int e;
	mp_int m;
	mp_int c;

	if ((NULL == msg) || (NULL == emsg) || (NULL == emlen)) {
		ret = -255;
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
					ret = -254;
				}
				else {
					ret = 0;
				}
				break;
			default:
				ret = -253;
				break;
		}
	}

	if (0 != ret) {
		/* Error case */
	}
	else {
		assert(MP_OKAY == mp_init(&n));
		assert(MP_OKAY == mp_init(&e));
		assert(MP_OKAY == mp_init(&m));
		assert(MP_OKAY == mp_init(&c));

		assert(MP_OKAY == mp_read_unsigned_bin(&n, key.n, key.n_len));
		assert(MP_OKAY == mp_read_unsigned_bin(&e, key.e, key.e_len));
		assert(MP_OKAY == mp_read_unsigned_bin(&m, msg,   mlen));

		ret = mp_exptmod(&m, &e, &n, &c);
		if (MP_OKAY != ret) {
			ret = -1;
		}
		else {
			*emlen = mp_unsigned_bin_size(&c);
			assert(MP_OKAY == mp_to_unsigned_bin(&c, emsg));
			ret = 0;
		}
	}

	return ret;
}