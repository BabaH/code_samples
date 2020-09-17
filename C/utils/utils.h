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

#include <stdbool.h>

void hexdump(void *base, size_t len, const char *title);
bool blkcmp(const void *left, size_t llen, const void *right, size_t rlen, bool fill);

#endif  /* __UTILS_H__ */
