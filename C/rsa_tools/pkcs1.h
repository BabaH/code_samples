/**
 * @file pkcs1.h
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief 
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef __PKCS1_H__
#define __PKCS1_H__

#define PKCS1_E_OK          (0)
#define PKCS1_E_PARAM       (-1)
#define PKCS1_E_RANGE       (-2)
#define PKCS1_E_VERIFY      (-3)
#define PKCS1_E_RESOURCE    (-254)
#define PKCS1_E_INTERNAL    (-255)

#ifdef PKCS1_TRACE
#define PKCS1_DEBUG_TRACE (1)
#endif  /* PKCS1TRACE */

typedef struct {
    uint8_t *n;
    uint8_t *e;
    uint8_t *d;
    uint8_t *p;
    uint8_t *q;
    uint8_t *dp;
    uint8_t *dq;
    uint8_t *qinv;
    size_t  n_len;
    size_t  e_len;
    size_t  d_len;
    size_t  p_len;
    size_t  q_len;
    size_t  dp_len;
    size_t  dq_len;
    size_t  qinv_len;
} RSA_TOOLS_PRIV_KEY_t;

typedef struct {
    uint8_t *n;
    uint8_t *e;
    size_t  n_len;
    size_t  e_len;
} RSA_TOOLS_PUB_KEY_t;

int rsaep(RSA_TOOLS_PUB_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *emsg, size_t *emlen);
int rsadp(RSA_TOOLS_PRIV_KEY_t key, uint8_t *emsg, size_t emlen, uint8_t *msg, size_t *mlen, bool use_crt);
int rsasp1(RSA_TOOLS_PRIV_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t *slen, bool use_crt);
int rsavp1(RSA_TOOLS_PUB_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t *slen);

int pkcs1_rsa_sign(RSA_TOOLS_PRIV_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t *slen, bool use_crt);
int pksc1_rsa_verify(RSA_TOOLS_PUB_KEY_t key, uint8_t *msg, size_t mlen, uint8_t *sig, size_t slen);

#endif  /* __PKCS1_H__ */