/**
 * @file pkcs1_main.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief 
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include "pkcs1.h"
#include "utils.h"
#include "nist_tv_rsadp.h"
#include "nist_tv_rsasp1.h"

/**
 * @brief Verification Test for RSADP/RSAEP.
 * 
 * @return Status of this function.
 * 
 * @retval  PKCS1_E_OK  Success.
 */
int pkcs1_rsadp_test()
{
    int ret;
    uint8_t              buf[512];
    size_t               len;
    int                  i;
    int                  tv_cnt;
    NIST_TV_RSADP_t      *tv;

    tv_cnt = (sizeof(nist_rsadp_tv_param) / sizeof(NIST_TV_RSADP_t));
    for (i = 0; i < tv_cnt; i++) {
        tv = &(nist_rsadp_tv_param[i]);
        printf("Start RSADP Test %02d...  ", i);
        if (tv->e_result) {
            memset(buf, 0, sizeof(buf));
            len = tv->pubkey.n_len;
            ret = rsaep(tv->pubkey, tv->k, tv->k_len, buf, &len);
            if (0 == ret) {
                if (blkcmp(tv->c, tv->c_len, buf, len, true)) {
                    printf("RSAEP: OK.       ");
                }
                else {
                    printf("RSAEP: NG.\n");
                    hexdump(tv->c, tv->c_len, "Expected Ciphertext.");
                    hexdump(buf, len, "Calculated Chipertext.");
                }
            }
            else {
                printf("RSAEP: Error. ret=%d  ", ret);
            }
        }
        else {
            printf("RSAEP: Skipped.  ");
        }

        memset(buf, 0, sizeof(buf));
        len = tv->privkey.n_len;
        ret = rsadp(tv->privkey, tv->c, tv->c_len, buf, &len, false);
        if (0 == ret) {
            if (blkcmp(tv->k, tv->k_len, buf, len, true)) {
                printf("RSADP: OK.               ");
            }
            else {
                printf("RSADP: NG.\n");
                hexdump(tv->k, tv->k_len, "Expected Ciphertext.");
                hexdump(buf, len, "Calculated Chipertext.");
            }
        }
        else {
            if (tv->e_result) {
                printf("RSADP: Error. ret=%d  ", ret);
            }
            else {
                printf("RSADP: Expected Result.  ");
            }
        }
        printf("Finish Test %02d.\n", i);
    }

   return PKCS1_E_OK;
}

/**
 * @brief Verification Test for RSASP1/RSAVP1.
 * 
 * @return Status of this function.
 * 
 * @retval  PKCS1_E_OK  Success.
 */
int pkcs1_rsasp1_test()
{
    int ret;
    uint8_t              buf[512];
    size_t               len;
    int                  i;
    int                  tv_cnt;
    NIST_TV_RSASP1_t     *tv;

    tv_cnt = (sizeof(nist_rsasp1_tv_param) / sizeof(NIST_TV_RSASP1_t));
    for (i = 0; i < tv_cnt; i++) {
        tv = &(nist_rsasp1_tv_param[i]);
        printf("Start RSASP1 Test %02d...  ", i);
        if (tv->e_result) {
            memset(buf, 0, sizeof(buf));
            len = tv->privkey.n_len;
            ret = rsasp1(tv->privkey, tv->EM, tv->em_len, buf, &len, false);
            if (0 == ret) {
                if (blkcmp(tv->Sig, tv->sig_len, buf, len, true)) {
                    printf("RSASP1: OK.       ");
                }
                else {
                    printf("RSASP1: NG.\n");
                    hexdump(tv->Sig, tv->sig_len, "Expected Signature.");
                    hexdump(buf, len, "Calculated Signature.");
                }
            }
            else {
                printf("RSASP1: Error. ret=%d  ", ret);
            }
        }
        else {
            printf("RSASP1: Skipped.  ");
        }

        memset(buf, 0, sizeof(buf));
        len = tv->privkey.n_len;
        ret = rsavp1(tv->pubkey, tv->Sig, tv->sig_len, buf, &len);
        if (0 == ret) {
            if (blkcmp(tv->EM, tv->em_len, buf, len, true)) {
                printf("RSAVP1: OK.               ");
            }
            else {
                printf("RSAVP1: NG.\n");
                hexdump(tv->EM, tv->em_len, "Expected Signature.");
                hexdump(buf, len, "Calculated Signature.");
            }
       }
        else {
            if (tv->e_result) {
                printf("RSAVP1: Error. ret=%d  ", ret);
            }
            else {
                printf("RSAVP1: Expected Result.  ");
            }
        }
        printf("Finish Test %02d.\n", i);
    }

   return PKCS1_E_OK;
}

/**
 * @brief Verification Test for RSA sign feature.
 * 
 * @return Status of this function.
 * 
 * @retval  PKCS1_E_OK  Success.
 */
int pkcs1_rsa_sign_test()
{
    int ret;
    uint8_t              buf[512];
    size_t               len;
    int                  i;
    int                  tv_cnt;
    NIST_TV_RSASP1_t     *tv;

    printf("Start RSA Sign Test\n");
    tv_cnt = (sizeof(nist_rsasp1_tv_param) / sizeof(NIST_TV_RSASP1_t));
    printf("*** No Padding ***\n");
    for (i = 0; i < tv_cnt; i++) {
        tv = &(nist_rsasp1_tv_param[i]);
        if (tv->e_result) {
            printf("Test Vector %02d: ", i);
            memset(buf, 0, sizeof(buf));
            len = tv->privkey.n_len;
            ret = pkcs1_rsa_sign(tv->privkey, tv->EM, tv->em_len, buf, &len, false);
            if (0 == ret) {
                if (blkcmp(tv->Sig, tv->sig_len, buf, len, true)) {
                    printf("OK.\n");
                }
                else {
                    printf("NG.\n");
                }
            }
            else {
                printf("Error. ret=%d\n", ret);
            }
        }
        else {
            /* Try next test vector. */
        }
    }
 
    printf("Finish RSA Sign Test\n");
 
   return PKCS1_E_OK;
}

/**
 * @brief Verification Test for RSA verify feature.
 * 
 * @return Status of this function.
 * 
 * @retval  PKCS1_E_OK  Success.
 */
int pkcs1_rsa_verify_test()
{
    int ret;
    uint8_t              buf[512];
    size_t               len;
    int                  i;
    int                  tv_cnt;
    NIST_TV_RSASP1_t     *tv;

    printf("Start RSA Verify Test\n");
    tv_cnt = (sizeof(nist_rsasp1_tv_param) / sizeof(NIST_TV_RSASP1_t));
    printf("*** No Padding ***\n");
    for (i = 0; i < tv_cnt; i++) {
        tv = &(nist_rsasp1_tv_param[i]);
        if (tv->e_result) {
            printf("Test Vector %02d: ", i);
            memset(buf, 0, sizeof(buf));
            len = tv->pubkey.n_len;
            ret = rsavp1(tv->pubkey, tv->Sig, tv->sig_len, buf, &len);
            if (0 == ret) {
                if (blkcmp(tv->EM, tv->em_len, buf, len, true)) {
                    printf("OK.\n");
                }
                else {
                    printf("NG.\n");
                }
            }
            else {
                printf("Error. ret=%d\n", ret);
            }
        }
        else {
            /* Try next test vector. */
        }
    }
 
    printf("Finish RSA Verify Test\n");
 
   return PKCS1_E_OK;
}