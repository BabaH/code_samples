/**
 * @file utils_main.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief  Test Application for utilities.
 *
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define TEST_UTILS_HEXDUMP  (1)
//#define TEST_UTILS_BLKCMP   (1)
#define TEST_UTILS_TIMESPEC (1)

extern bool hexdump_test();
extern bool blkcmp_test();
extern bool ts_test();

int main(int argc, char *argv[])
{
    int ret;

    ret = EXIT_FAILURE;
#ifdef TEST_UTILS_HEXDUMP
    ret = hexdump_test() ? EXIT_SUCCESS : EXIT_FAILURE;
#endif  /* TEST_UTILS_HEXDUMP */
#ifdef TEST_UTILS_BLKCMP
    ret = blkcmp_test() ? EXIT_SUCCESS : EXIT_FAILURE;
#endif  /* TEST_UTILS_BLKCMP */
#ifdef TEST_UTILS_TIMESPEC
    ret = ts_test() ? EXIT_SUCCESS : EXIT_FAILURE;
#endif  /* TEST_UTILS_TIMESPEC */

    return ret;
}
