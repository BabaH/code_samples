/**
 * @file hexdump.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 *
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>

#include "utils.h"

static void separator(bool header)
{
    int i;

    if (header) {
        for (i = 0; i < sizeof(void *); i++) {
            printf("  ");
        }
        for (i = 0; i < 16; i++) {
            printf("+%1x ", i);
        }
    }
    else {
        for (i = 0; i < sizeof(void *); i++) {
            printf("--");
        }
        for (i = 0; i < 16; i++) {
            printf("---");
        }
    }
    printf("\n");
}

/**
 * @brief Hexadecimal dump tool
 * 
 * @param base  Buffer address.
 * @param len   Length of buffer.
 * @param title Title.
 */
void hexdump(void *base, size_t len, const char *title)
{
    int i;
    unsigned char *pos;

    pos = base;
    printf("HEXDUMP: %s, Base: %p, length: %zu\n", title, (void *)base, len);
    separator(true);
    separator(false);
    for (i = 0; i < len; i++) {
        if (0 == (i % 16)) {
            printf("%p: ", pos);
        }
        printf("%02x ", pos[i]);
        if (15 == (i % 16)) {
            printf("\n");
        }
    }
    if (0 != (len % 16)) {
        printf("\n");
    }
    separator(false);
}
