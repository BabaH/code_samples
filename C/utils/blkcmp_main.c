/**
 * @file blkcmp_main.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief  Test Function for blkcmp.
 *
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

static void fill_data(unsigned char *buf, size_t len, size_t flen, size_t zlen) {
    size_t        i;

    memset(buf, 0, len);
    for (i = 0; i < (flen - zlen); i++) {
        buf[i + zlen]  = 'A' + (i % 26);
    }
}

#define BUF_SIZE (32)

bool blkcmp_test()
{
    bool          ret;
    unsigned char buf[BUF_SIZE];
    unsigned char buf2[BUF_SIZE];
    unsigned char buf3[BUF_SIZE];
    int           i;
    size_t        len;
    size_t        len2;

    ret = true;

    printf("Test Case 1: ");
    len  = BUF_SIZE;
    len2 = BUF_SIZE;
    fill_data(buf,  BUF_SIZE,  len,  0);
    fill_data(buf2, BUF_SIZE, len2, 0);
    if (utils_blkcmp(buf, len, buf2, len2, true)) {
        printf("OK.\n");
    }
    else {
        printf("NG.\n");
        ret = false;
    }
    utils_hexdump(buf,  len,  "left");
    utils_hexdump(buf2, len2, "right");

    printf("Test Case 2: ");
    len  = BUF_SIZE;
    len2 = BUF_SIZE;
    fill_data(buf,  BUF_SIZE, len,  0);
    fill_data(buf2, BUF_SIZE, len2, 3);
    if (utils_blkcmp(buf, len, buf2, len2, true)) {
        printf("NG.\n");
        ret = false;
    }
    else {
        printf("OK.\n");
    }
    utils_hexdump(buf,  len,  "left");
    utils_hexdump(buf2, len2, "right");

    printf("Test Case 3: ");
    len  = BUF_SIZE;
    len2 = BUF_SIZE - 1;
    fill_data(buf,  BUF_SIZE, len,  1);
    fill_data(buf2, BUF_SIZE, len2, 0);
    if (utils_blkcmp(buf, len, buf2, len2, true)) {
        printf("OK.\n");
    }
    else {
        printf("NG.\n");
        ret = false;
    }
    utils_hexdump(buf,  len,  "left");
    utils_hexdump(buf2, len2, "right");

    printf("Test Case 4: ");
    len  = BUF_SIZE - 1;
    len2 = BUF_SIZE;
    fill_data(buf,  BUF_SIZE, len,  0);
    fill_data(buf2, BUF_SIZE, len2, 1);
    if (utils_blkcmp(buf, len, buf2, len2, true)) {
        printf("OK.\n");
    }
    else {
        printf("NG.\n");
        ret = false;
    }
    utils_hexdump(buf,  len,  "left");
    utils_hexdump(buf2, len2, "right");

    printf("Test Case 5: ");
    len  = BUF_SIZE;
    len2 = BUF_SIZE;
    fill_data(buf,  BUF_SIZE, len,  5);
    fill_data(buf2, BUF_SIZE, len2, 5);
    if (utils_blkcmp(buf, len, buf2, len2, true)) {
        printf("OK.\n");
    }
    else {
        printf("NGp.\n");
        ret = false;
    }
    utils_hexdump(buf,  len,  "left");
    utils_hexdump(buf2, len2, "right");

    printf("Test Case 6: ");
    len  = BUF_SIZE;
    len2 = BUF_SIZE;
    fill_data(buf,  BUF_SIZE, len,  5);
    fill_data(buf2, BUF_SIZE, len2, 5);
    buf2[3] = 0x31;
    if (utils_blkcmp(buf, len, buf2, len2, true)) {
        printf("NG.\n");
        ret = false;
    }
    else {
        printf("OK.\n");
    }
    utils_hexdump(buf,  len,  "left");
    utils_hexdump(buf2, len2, "right");

    return ret;
}
