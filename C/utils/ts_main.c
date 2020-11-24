/**
 * @file ts_main.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief Test function for Timestamps
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include <unistd.h>

#include "utils.h"

bool ts_test()
{
    bool ret;
    void *t1;
    void *t2;
    void *t3;

    t1 = utils_ts_alloc();
    t2 = utils_ts_alloc();
    t3 = utils_ts_alloc();

    if ((NULL == t1) || (NULL == t2) || (NULL == t3)) {
        ret = false;
    }
    else {
        utils_ts_gettime(t1);
        sleep(1);
        utils_ts_gettime(t2);

        utils_ts_diff(t1, t2, t3);

        printf("t1: %10" PRIu64 ".%09" PRIu64 "\n", utils_ts_sec(t1), utils_ts_nsec(t1));
        printf("t1: %10" PRIu64 ".%09" PRIu64 "\n", utils_ts_sec(t2), utils_ts_nsec(t2));
        printf("t1: %10" PRIu64 ".%09" PRIu64 "\n", utils_ts_sec(t3), utils_ts_nsec(t3));

        ret = true;
    }

    utils_ts_free(t1);
    utils_ts_free(t2);
    utils_ts_free(t3);
    t1 = NULL;
    t2 = NULL;
    t3 = NULL;

    return ret;
}