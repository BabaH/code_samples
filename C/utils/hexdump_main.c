/**
 * @file hexdump_main.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief  Test function for hexdump.
 *
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>

#include "utils.h"

bool hexdump_test()
{
    unsigned char buf[64];
    int           i;

    for (i = 0; i < sizeof(buf); i++) {
        buf[i] = 'A' + (i % 26);
    }

    utils_hexdump(buf, sizeof(buf),        "Test Case 1");
    utils_hexdump(buf, (sizeof(buf) - 1),  "Test Case 2");
    utils_hexdump(buf, (sizeof(buf) - 15), "Test Case 3");
    utils_hexdump(buf, 1,                  "Test Case 4");
    utils_hexdump(buf, 0,                  "Test Case 5");

    return true;
}
