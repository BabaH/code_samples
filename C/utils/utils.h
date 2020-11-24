/**
 * @file utils.h
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief Utility function library.
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#ifndef __UTILS_H__
#define __UTILS_H__

//#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define UTILS_E_OK       (0)
#define UTILS_E_PARAM    (-1)
//#define UTILS_E_RANGE    (-2)
#define UTILS_E_RESOURCE (-254)
#define UTILS_E_INTERNAL (-255)

void *utils_ts_alloc();
void utils_ts_free(void *ctx);
uint32_t utils_ts_gettime(void *ctx);
uint32_t utils_ts_diff(void *ctx1, void *ctx2, void *ctx3);
uint64_t utils_ts_sec(void *ctx);
uint64_t utils_ts_nsec(void *ctx);
void utils_hexdump(void *base, size_t len, const char *title);
bool utils_blkcmp(const void *left, size_t llen, const void *right, size_t rlen, bool fill);

#endif  /* __UTILS_H__ */
