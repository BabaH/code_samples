/**
 * @file rsa_tools.h
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief 
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef __RSA_TOOLS_H__
#define __RSA_TOOLS_H__

typedef struct {
    unsigned char *n;
    unsigned char *e;
    unsigned char *d;
    unsigned char *p;
    unsigned char *q;
    unsigned char *dp;
    unsigned char *dq;
    unsigned char *qinv;
    size_t        n_len;
    size_t        e_len;
    size_t        d_len;
    size_t        p_len;
    size_t        q_len;
    size_t        dp_len;
    size_t        dq_len;
    size_t        qinv_len;
} RSA_TOOLS_PRIV_KEY_t;

typedef struct {
    unsigned char *n;
    unsigned char *e;
    size_t        n_len;
    size_t        e_len;
} RSA_TOOLS_PUB_KEY_t;

int rsaep(RSA_TOOLS_PUB_KEY_t key, unsigned char *msg, size_t mlen, unsigned char *emsg, size_t *emlen);
int rsadp(RSA_TOOLS_PRIV_KEY_t key, unsigned char *emsg, size_t emlen, unsigned char *msg, size_t *mlen, bool use_crt);

#endif  /* __RSA_TOOLS_H__ */