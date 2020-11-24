/**
 * @file utils_ts.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"

/**
 * @brief Allocate timestamps context
 * 
 * @return  Timestamps context
 */
void *utils_ts_alloc()
{
    struct timespec *ctx;

    ctx = malloc(sizeof(struct timespec));
    if (NULL != ctx) {
        ctx->tv_sec  = 0;
        ctx->tv_nsec = 0;
    }

    return ctx;
}

/**
 * @brief Release timestamps context
 * 
 * @param ctx [in]  Timestamps context
 */
void utils_ts_free(void *ctx)
{
    struct timespec *ts;

    if (NULL != ctx) {
        ts = (struct timespec *)ctx;
        ts->tv_sec  = 0;
        ts->tv_nsec = 0;
        free(ctx);
    }
}

/**
 * @brief Get time
 * 
 * @param ctx [in]  Timestamps context
 * 
 * @return          Status of this function
 * @retval  UTILS_E_OK          Success
 * @retval  UTILS_E_PARAM       Invalid Parameter
 * @retval  UTILS_E_INTERNAL    Internal Error
 */
uint32_t utils_ts_gettime(void *ctx)
{
    uint32_t        ret;
    int             res;
    struct timespec *tm;

    if (NULL == ctx) {
        ret = UTILS_E_PARAM;
    }
    else {
        tm = (struct timespec *)ctx;
        res = clock_gettime(CLOCK_MONOTONIC, tm);
        if (0 != res) {
            ret = UTILS_E_INTERNAL;
        }
        else {
            ret = UTILS_E_OK;
        }
    }

    return ret;
}

/**
 * @brief Calcurate delta timestamps
 * 
 * @param ctx1 [in]     Timestamps context 1
 * @param ctx2 [in]     Timestamps context 2
 * @param ctx3 [out]    Timestamps context for result
 * 
 * @return                  Status of this function
 * @retval  UTILS_E_OK      Success
 * @retval  UTILS_E_PARAM   Invalid Parameter
 */
uint32_t utils_ts_diff(void *ctx1, void *ctx2, void *ctx3)
{
    uint32_t ret;
    struct timespec *t1;
    struct timespec *t2;
    struct timespec *t3;

    if ((NULL == ctx1) || (NULL == ctx2) || (NULL == ctx3)) {
        ret = UTILS_E_PARAM;
    }
    else {
        t1 = (struct timespec *)ctx1;
        t2 = (struct timespec *)ctx2;
        t3 = (struct timespec *)ctx3;
        if (t2->tv_nsec < t1->tv_nsec) {
            t2->tv_sec  -= 1;
            t2->tv_nsec += 1000000000;
        }
        t3->tv_sec  = t2->tv_sec  - t1->tv_sec;
        t3->tv_nsec = t2->tv_nsec - t1->tv_nsec;
        ret = UTILS_E_OK;
    }

    return ret;
}

/**
 * @brief Extract number of second from timestamps context.
 * 
 * @param ctx [in]  Timestamp context
 * @return          Number of second
 */
uint64_t utils_ts_sec(void *ctx)
{
    struct timespec *tm;
    uint64_t        sec;

    if (NULL == ctx) {
        sec = 0;
    }
    else {
        tm = ctx;
        sec = tm->tv_sec;
    }

    return sec;
}

/**
 * @brief Extract number of nanosecond from timestamps context.
 * 
 * @param ctx [in]  Timestamp context
 * @return          Number of nanosecond
 */
uint64_t utils_ts_nsec(void *ctx)
{
    struct timespec *tm;
    uint64_t        nsec;

    if (NULL == ctx) {
        nsec = 0;
    }
    else {
        tm = ctx;
        nsec = tm->tv_nsec;
    }

    return nsec;
}