/**
 * @file blkcomp.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 *
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

bool blkcmp(const void *left, size_t llen, const void *right, size_t rlen, bool zfill)
{
    bool          ret;
    bool          flag;
    bool          flag2;
    const unsigned char *c;
    const unsigned char *c2;
    size_t        pad_len;
    size_t        chk_len;
    int           i;

    if (llen != rlen) {
        if (zfill) {
            if (llen > rlen) {
               pad_len = llen - rlen;
               chk_len = rlen;
               c       = left;
               c2      = right;
            }
            else {
               pad_len = rlen - llen;
               chk_len = llen;
               c       = right;
               c2      = left;
            }
            flag = true;
            for (i = 0; i < pad_len; i++) {
                if (0x00 != c[i]) {
                    flag = false;
                }
            }
            flag2 = (0 != memcmp(&(c[pad_len]), c2, chk_len)) ? false : true;
            ret = (flag && flag2) ? true : false;
        }
        else {
            ret = false;
        }
    }
    else {
        ret = (0 != memcmp(left, right, llen)) ? false : true;
    }
}
