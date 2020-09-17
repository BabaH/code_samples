/**
 * @file rsa_main.c
 * @author Hidenori BABA (BabaH@dotpro.jp)
 * @brief 
 * 
 * @copyright Copyright (c) 2020 Hidenori BABA
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include "rsa_tools.h"
#include "utils.h"

unsigned char rsa_n[] = {
0xa7, 0x78, 0x35, 0xe7, 0x98, 0x1e, 0x2c, 0xbd, 0x2c, 0x91, 0x61, 0x49, 0x10, 0xb2, 0x1c, 0x3e,
0x65, 0x0d, 0x1e, 0x08, 0x16, 0x21, 0xc2, 0x0a, 0x3a, 0x28, 0x51, 0xbd, 0xf5, 0xb3, 0x38, 0xae,
0x18, 0xdd, 0x05, 0xdd, 0x9b, 0x76, 0x51, 0x24, 0x9a, 0xcc, 0x23, 0xc5, 0x0e, 0xa0, 0x4a, 0x93,
0xa2, 0xf1, 0x46, 0x02, 0x11, 0x4d, 0x95, 0x72, 0x53, 0x92, 0xfd, 0xe6, 0xeb, 0xc6, 0x9a, 0x77,
0x6c, 0xcd, 0xa7, 0x58, 0x89, 0x5d, 0x26, 0x41, 0x89, 0x54, 0xf8, 0xf8, 0xfe, 0x33, 0x18, 0x18,
0x94, 0x8f, 0xde, 0x2f, 0x0d, 0xdb, 0x91, 0x43, 0x51, 0x6f, 0x0c, 0x7d, 0xc9, 0xd5, 0xb9, 0x56,
0x8f, 0x14, 0xd7, 0x04, 0x17, 0x99, 0x75, 0x53, 0x81, 0x55, 0x9e, 0x16, 0xa5, 0x86, 0xf5, 0x12,
0xf2, 0xfb, 0x50, 0xbc, 0xe1, 0xac, 0xd6, 0x0b, 0xcf, 0x83, 0x1e, 0xf0, 0xac, 0x3c, 0x24, 0x85,
0x54, 0x88, 0x03, 0x40, 0x87, 0x4d, 0x06, 0xfc, 0x97, 0x68, 0x0b, 0xc1, 0xff, 0xb0, 0xdf, 0x4d,
0xe7, 0x46, 0xa8, 0x46, 0x88, 0xb7, 0x33, 0x27, 0x9d, 0xfb, 0x30, 0x1f, 0x25, 0x03, 0x07, 0x4f,
0xc9, 0xfc, 0xe2, 0xcd, 0x56, 0x23, 0x37, 0x73, 0x90, 0x2c, 0xf5, 0x89, 0x19, 0xa7, 0x89, 0x82,
0x5e, 0x84, 0x93, 0x4c, 0x47, 0x6b, 0x39, 0x4e, 0x16, 0x2b, 0x7a, 0xdf, 0xb4, 0xbd, 0xa1, 0x49,
0x4d, 0x47, 0x48, 0x07, 0x1a, 0x75, 0xf0, 0x2b, 0x95, 0xcf, 0xd6, 0x20, 0x2e, 0x0b, 0x2d, 0x29,
0x94, 0x3a, 0x62, 0xd8, 0xce, 0x50, 0x43, 0x3d, 0xc1, 0x17, 0xa5, 0xeb, 0xb7, 0x36, 0x1c, 0xc1,
0x3c, 0xc9, 0x32, 0x2f, 0x51, 0xc5, 0xf6, 0x75, 0xe7, 0x05, 0xd7, 0x00, 0x0b, 0x7d, 0xfc, 0x5f,
0x83, 0x3c, 0xc5, 0x3c, 0x65, 0xde, 0x18, 0x2d, 0xf1, 0x45, 0x69, 0x7e, 0xec, 0x7d, 0x0e, 0x01,
};

unsigned char rsa_d[] = {
0x02, 0x53, 0xf2, 0x73, 0xe0, 0xb2, 0x72, 0xae, 0xc2, 0x82, 0xd6, 0xd6, 0x5c, 0x2c, 0x0d, 0xa6,
0x74, 0x63, 0x25, 0x4a, 0xb9, 0x7c, 0x08, 0xa5, 0x79, 0xbf, 0x7b, 0xbb, 0x67, 0x41, 0xf1, 0x84,
0xc2, 0x05, 0xb8, 0x82, 0x27, 0x1a, 0x12, 0xf0, 0xae, 0x52, 0x2e, 0x21, 0x1c, 0x1d, 0xf0, 0x4b,
0x73, 0x15, 0x65, 0x82, 0xbb, 0xd8, 0xd7, 0x3d, 0xfa, 0x99, 0x6d, 0x7c, 0x1b, 0x44, 0xec, 0x7a,
0xc2, 0x81, 0x6d, 0x55, 0x16, 0xbb, 0xbb, 0xbf, 0x67, 0x78, 0x4d, 0xe9, 0xb0, 0x26, 0xe7, 0xab,
0xd7, 0xcc, 0xd4, 0x2a, 0xd5, 0x47, 0x7b, 0x12, 0x73, 0x7c, 0x56, 0xb1, 0x1c, 0xf0, 0x22, 0x9e,
0x68, 0x68, 0xc2, 0xc0, 0x64, 0xcf, 0x96, 0x48, 0xed, 0xe0, 0x51, 0xcd, 0x40, 0xc9, 0xca, 0x78,
0x53, 0xf6, 0x58, 0x5d, 0xf7, 0xc9, 0x1e, 0x44, 0x95, 0xc2, 0xdd, 0xd8, 0x3c, 0x9c, 0xad, 0x69,
0x43, 0x31, 0x31, 0xbc, 0x55, 0xdb, 0x21, 0xcc, 0x6a, 0xfa, 0x59, 0x35, 0x60, 0xd7, 0x67, 0x5c,
0x23, 0x64, 0xf4, 0x45, 0x79, 0xd5, 0x7d, 0x5d, 0xe2, 0x19, 0x4a, 0xc3, 0xd4, 0x7d, 0x17, 0x1f,
0x75, 0x87, 0x9b, 0xe7, 0x15, 0x4e, 0xbf, 0xb7, 0x62, 0x13, 0x30, 0x63, 0x7a, 0x82, 0x09, 0xc2,
0xc6, 0x38, 0xcb, 0x97, 0x84, 0xaf, 0xb1, 0x56, 0x79, 0xf5, 0x6a, 0x6a, 0xee, 0x20, 0xe2, 0xb3,
0x11, 0x00, 0x81, 0xa1, 0x8d, 0x2a, 0xd3, 0x9c, 0x64, 0xcd, 0xaf, 0xc8, 0xa4, 0x3b, 0x60, 0x2b,
0xc2, 0x93, 0x3f, 0x5b, 0xa4, 0x5d, 0xbb, 0x5d, 0x93, 0xd7, 0x6e, 0x65, 0xdc, 0xb8, 0xc4, 0xe7,
0x01, 0xb5, 0xb3, 0x4d, 0x2d, 0x0e, 0x51, 0xe4, 0x6c, 0xa2, 0x91, 0x8b, 0x76, 0x0c, 0x43, 0x6a,
0x03, 0x37, 0x44, 0x21, 0x5e, 0x4d, 0x06, 0x0b, 0x02, 0x5a, 0x36, 0x04, 0xf8, 0xea, 0x59, 0x01,
};

unsigned char rsa_p[] = {
0xaa, 0x9d, 0x3b, 0x7b, 0x3c, 0xa7, 0xbf, 0x43, 0xca, 0xe6, 0x47, 0xcf, 0x1d, 0x73, 0xd6, 0x14,
0xef, 0x06, 0xfd, 0xa1, 0x3b, 0xa9, 0x58, 0x27, 0x34, 0xb3, 0x44, 0x56, 0x92, 0x07, 0xc2, 0x9f,
0x36, 0xc2, 0x3f, 0xeb, 0x70, 0x13, 0x6e, 0x0e, 0xde, 0x2b, 0xa0, 0xdf, 0x19, 0xdc, 0xf0, 0x31,
0x65, 0x23, 0xb4, 0xc8, 0x2f, 0x8e, 0x24, 0xba, 0xd6, 0x9e, 0x9e, 0x87, 0x81, 0x61, 0x13, 0xaf,
0xd9, 0xbd, 0x27, 0x8e, 0xa3, 0xdb, 0xb3, 0x15, 0x98, 0xc6, 0xba, 0xf5, 0x0c, 0x68, 0x87, 0x55,
0x13, 0xd3, 0x82, 0xc0, 0x29, 0xc5, 0x1f, 0xad, 0x88, 0x41, 0x0b, 0xef, 0xc5, 0xbd, 0x9a, 0x7a,
0xe6, 0x04, 0x6a, 0x8f, 0x37, 0xd9, 0xf1, 0xc2, 0xe9, 0x8c, 0xea, 0xc8, 0x83, 0x81, 0xbb, 0x8c,
0x32, 0xdb, 0x09, 0xb3, 0xd6, 0x74, 0xd0, 0xb9, 0x6a, 0x5e, 0xab, 0x31, 0x52, 0xd4, 0x72, 0xb1,
};

unsigned char rsa_q[] = {
0xfb, 0x48, 0x18, 0x8d, 0x7c, 0x40, 0xc6, 0x95, 0x74, 0x55, 0xe0, 0x20, 0x64, 0xb0, 0x31, 0x89,
0x20, 0x68, 0x39, 0xd7, 0x02, 0x98, 0x9e, 0x32, 0xdc, 0xba, 0x0c, 0xa0, 0x99, 0x77, 0x95, 0x77,
0x58, 0xc2, 0x86, 0x06, 0x5e, 0xf1, 0x6e, 0x4f, 0x1d, 0xc0, 0x91, 0x74, 0xa2, 0xe9, 0xb7, 0x27,
0xf3, 0x99, 0x1f, 0x9f, 0x45, 0xe3, 0xd1, 0x5d, 0xc5, 0xe7, 0x3d, 0x7a, 0x0c, 0xa2, 0x4a, 0xbb,
0x89, 0x94, 0x10, 0xb7, 0xbf, 0x47, 0x1f, 0x3c, 0xa6, 0xbe, 0x31, 0x46, 0xaa, 0x7b, 0x79, 0x64,
0x73, 0x31, 0x38, 0xa5, 0x39, 0xa7, 0x98, 0xa8, 0x54, 0xa7, 0xe2, 0xf6, 0xd9, 0xf8, 0x31, 0x68,
0x2b, 0x4b, 0x61, 0x5f, 0x5e, 0x59, 0x58, 0x83, 0x11, 0xfc, 0xf2, 0x64, 0x0b, 0x93, 0x4e, 0x84,
0x74, 0x45, 0xd0, 0xb6, 0x3e, 0x5e, 0xc8, 0xfd, 0xd4, 0x65, 0xef, 0xb5, 0x97, 0x9a, 0x04, 0x51,
};

unsigned char rsa_dp[] = {
0x4e, 0xf6, 0xce, 0x9c, 0xcf, 0x09, 0xe4, 0x5f, 0xff, 0xfc, 0x6c, 0xa7, 0xd3, 0x3d, 0xd5, 0x8b,
0x82, 0xc5, 0x7d, 0x85, 0xae, 0x13, 0x46, 0x95, 0x7e, 0xb2, 0xa9, 0xa2, 0x57, 0x9b, 0xf7, 0xdb,
0xb0, 0x80, 0x5f, 0x0d, 0x1c, 0xa6, 0x21, 0x7e, 0x08, 0x1d, 0xb1, 0x2b, 0x10, 0xd1, 0x4c, 0xe9,
0xcf, 0xa1, 0xa2, 0xfe, 0x33, 0xe4, 0x59, 0xe3, 0x8f, 0x89, 0x82, 0x5d, 0xbc, 0xe7, 0xad, 0x30,
0xea, 0x59, 0xbd, 0x81, 0x2b, 0x1d, 0x77, 0xd9, 0x3b, 0x08, 0x5f, 0x67, 0x23, 0x03, 0x0d, 0xff,
0xb6, 0x59, 0x62, 0x5e, 0x85, 0xf6, 0x92, 0x1e, 0xc2, 0xbd, 0x38, 0x45, 0x15, 0x04, 0x18, 0x07,
0x61, 0x86, 0x11, 0xbb, 0x8f, 0x61, 0xf3, 0x9e, 0xfe, 0x71, 0x16, 0x09, 0x0f, 0xb4, 0x6f, 0x95,
0x07, 0xc4, 0x90, 0xcd, 0x07, 0x28, 0xb1, 0xa1, 0xdf, 0x35, 0x49, 0x8e, 0x5e, 0x83, 0xad, 0x41,
};


unsigned char rsa_dq[] = {
0x9a, 0x14, 0xb9, 0x49, 0x78, 0x99, 0x57, 0xab, 0xa9, 0x2c, 0x47, 0x44, 0x55, 0xb9, 0x5c, 0x86,
0x93, 0x22, 0x95, 0xe4, 0xeb, 0xa8, 0x9d, 0xc8, 0xde, 0x64, 0x0e, 0xfe, 0x23, 0x5c, 0x9a, 0x80,
0x16, 0x23, 0x21, 0x7f, 0xa2, 0x94, 0x34, 0x69, 0xba, 0xfa, 0xab, 0xb6, 0x73, 0x77, 0x10, 0x62,
0x25, 0x13, 0x0d, 0xba, 0xe2, 0x66, 0x31, 0xa9, 0x68, 0x82, 0xc4, 0xc5, 0xa4, 0x1d, 0x97, 0xe9,
0xab, 0x10, 0xe9, 0xb7, 0x62, 0x48, 0xc0, 0x34, 0xb6, 0xa2, 0x05, 0x21, 0x29, 0xc4, 0x90, 0xc9,
0x31, 0x81, 0x4b, 0x14, 0xb5, 0x67, 0xb9, 0x49, 0xee, 0xaf, 0x0b, 0x93, 0x06, 0x51, 0x14, 0x2a,
0xa5, 0xf1, 0x91, 0x01, 0xa3, 0x9e, 0xf1, 0x91, 0x5d, 0x64, 0x57, 0x55, 0x55, 0x6b, 0xbc, 0x47,
0xa7, 0xec, 0x6f, 0x70, 0x70, 0x8a, 0x3a, 0x80, 0xba, 0xce, 0xce, 0xcf, 0xf0, 0xe7, 0xf6, 0x21,
};

unsigned char rsa_qinv[] = {
0x13, 0xce, 0xff, 0xd3, 0x93, 0x9d, 0x01, 0xc7, 0xf5, 0xa7, 0xe9, 0x24, 0xdb, 0xb2, 0x32, 0xa0,
0x45, 0x65, 0x7c, 0x9b, 0xa8, 0x79, 0x40, 0x73, 0x29, 0x6f, 0xd0, 0x79, 0x44, 0xee, 0xba, 0x08,
0x77, 0x53, 0x1c, 0x08, 0x07, 0x50, 0x3e, 0x70, 0x78, 0xb7, 0x9f, 0x80, 0xba, 0x4f, 0x67, 0xe4,
0x59, 0x3f, 0x41, 0xeb, 0x58, 0x61, 0xfc, 0x31, 0xfb, 0x40, 0xc4, 0x9a, 0x2a, 0x71, 0xe9, 0x8e,
0x61, 0xf5, 0x84, 0xee, 0x72, 0x8f, 0x7c, 0xec, 0xa1, 0x39, 0xaf, 0x13, 0xb9, 0xa4, 0x23, 0xa6,
0xfc, 0xba, 0x34, 0xe7, 0xd7, 0x38, 0xd8, 0xaa, 0xbb, 0xad, 0xb5, 0xcc, 0xc3, 0xbf, 0x00, 0x5f,
0xb7, 0x20, 0x47, 0x64, 0x8c, 0x5f, 0x65, 0x08, 0xf7, 0xb4, 0x2f, 0xa1, 0xca, 0x7a, 0x66, 0x47,
0x49, 0xa4, 0x1f, 0xdf, 0xd0, 0x81, 0xce, 0x97, 0x48, 0x8a, 0x3a, 0x2b, 0x1a, 0x53, 0x08, 0x35,
};

unsigned char rsa_e[] = {
0x01, 0x00, 0x01,
};

unsigned char EM[] = {
0x00, 0xaa, 0xb7, 0x52, 0xf8, 0x10, 0x7b, 0x24, 0x18, 0x3f, 0xf0, 0xae, 0xf7, 0xe2, 0xc0, 0xa5,
0x45, 0x09, 0x3a, 0x2c, 0xd7, 0x5e, 0x2b, 0xae, 0xd9, 0x2c, 0x98, 0x89, 0x6d, 0x1c, 0x61, 0xf0,
0x14, 0x08, 0xb7, 0xfd, 0xed, 0xe0, 0x1e, 0x91, 0x29, 0xa0, 0x52, 0x99, 0xc9, 0x5b, 0x02, 0xdf,
0xe5, 0xfe, 0x04, 0xba, 0x28, 0x0e, 0x31, 0x99, 0xfc, 0x79, 0x58, 0x5f, 0xe2, 0xed, 0x04, 0x85,
0x4d, 0x31, 0x1b, 0x96, 0x46, 0x4c, 0x01, 0x5d, 0xaf, 0xcf, 0xd3, 0x83, 0x62, 0x22, 0xd8, 0xbd,
0x97, 0xd2, 0xcb, 0xae, 0x82, 0x09, 0x51, 0xdd, 0x66, 0x78, 0x5e, 0x65, 0xb8, 0xed, 0xfa, 0xea,
0xd6, 0x38, 0x05, 0x6c, 0x2a, 0x2a, 0xbc, 0x3f, 0x39, 0x6c, 0x76, 0xd2, 0xcf, 0xa0, 0x59, 0xe9,
0x6c, 0xfe, 0xb3, 0xbc, 0xcd, 0x49, 0xf0, 0x63, 0x0b, 0x16, 0x20, 0xd0, 0x51, 0xa9, 0x3e, 0x0e,
0x23, 0x40, 0xd4, 0x42, 0x1f, 0xd7, 0xe3, 0x91, 0x45, 0xa5, 0x2a, 0x48, 0x2d, 0x1d, 0x64, 0x7c,
0x0a, 0xbc, 0xf2, 0x41, 0xea, 0x64, 0x3a, 0xea, 0xda, 0x28, 0x30, 0x1d, 0x11, 0x48, 0x93, 0x81,
0x7d, 0x79, 0x66, 0x86, 0x8c, 0xa5, 0xe2, 0x56, 0xd2, 0xd1, 0x2f, 0xe4, 0xbf, 0xe3, 0xc1, 0x48,
0xf2, 0xdc, 0xf5, 0x47, 0x54, 0xab, 0x0e, 0xc2, 0x04, 0x22, 0x2b, 0xef, 0x3a, 0x80, 0x44, 0x3d,
0x99, 0x52, 0x47, 0x60, 0xa7, 0xe1, 0x9c, 0xcf, 0xbd, 0xa5, 0x3a, 0x53, 0x21, 0x65, 0xd6, 0x58,
0x55, 0xa6, 0x7c, 0x48, 0x25, 0x1e, 0xe7, 0x03, 0x50, 0x4b, 0x1c, 0xfd, 0xf8, 0xa7, 0x81, 0x2f,
0x0f, 0xd6, 0xc7, 0x07, 0x57, 0x5d, 0xe9, 0x6e, 0x91, 0x9b, 0xa2, 0x0c, 0x36, 0x57, 0xe4, 0x49,
0x99, 0xcd, 0x39, 0xcc, 0x09, 0xb2, 0x18, 0x95, 0x70, 0x0f, 0xe4, 0x18, 0xdd, 0xe7, 0x7d, 0x1a,
};

unsigned char expected_encEM[] = {
0x8a, 0x5a, 0xad, 0x9b, 0xef, 0x64, 0x5e, 0x13, 0x32, 0x47, 0x7b, 0x53, 0xad, 0xa3, 0xcb, 0xcc,
0x69, 0xd1, 0xc3, 0xf2, 0x4a, 0x45, 0x99, 0x43, 0xda, 0xf8, 0x6c, 0x4c, 0x58, 0xb2, 0x73, 0xe6,
0xab, 0x15, 0x8d, 0x79, 0xf4, 0x33, 0x9d, 0x00, 0x39, 0xef, 0xdf, 0x9d, 0x53, 0xfa, 0x70, 0x7a,
0x1a, 0x74, 0x3a, 0x9c, 0xba, 0xe2, 0x62, 0xb6, 0x7c, 0xe7, 0x54, 0x8a, 0x4b, 0xbe, 0x3b, 0xe4,
0x2a, 0x39, 0x49, 0x48, 0x57, 0x29, 0x9d, 0x0b, 0x09, 0x72, 0x93, 0xba, 0x23, 0x11, 0xeb, 0x39,
0x25, 0x3a, 0x67, 0x01, 0x5b, 0xba, 0x35, 0x59, 0x92, 0xc1, 0xde, 0xe2, 0x4d, 0xd7, 0x4b, 0xc7,
0x03, 0x73, 0xb8, 0xcc, 0x38, 0x42, 0xe9, 0xf5, 0xb8, 0xce, 0x8e, 0x17, 0xe8, 0x43, 0xde, 0x17,
0x12, 0x10, 0xa2, 0x55, 0x4e, 0x60, 0x86, 0x29, 0x3e, 0x16, 0x06, 0xc3, 0x71, 0x6d, 0x3d, 0xa5,
0x75, 0x10, 0xce, 0x68, 0x54, 0xfb, 0xe2, 0xff, 0xdf, 0xba, 0x5e, 0xb0, 0xe3, 0x54, 0x27, 0x42,
0x7d, 0x0e, 0x36, 0x74, 0x7a, 0x77, 0xbb, 0x97, 0x7d, 0xd6, 0x48, 0xd0, 0x42, 0x9f, 0x3f, 0x2f,
0x4a, 0x10, 0xc6, 0xef, 0x93, 0x0b, 0x46, 0x30, 0x66, 0x3b, 0x2a, 0x21, 0xbe, 0x28, 0x24, 0x4e,
0x88, 0xe0, 0x65, 0xc0, 0x4c, 0xaf, 0x2e, 0x39, 0x6d, 0xe5, 0x0a, 0x7d, 0xcb, 0xf1, 0xd4, 0x7b,
0xf7, 0xd3, 0xe6, 0xab, 0xb7, 0xd5, 0x18, 0x7d, 0x4b, 0x1a, 0x05, 0xf8, 0xda, 0x91, 0x94, 0x6f,
0x5e, 0xee, 0xee, 0x69, 0xe6, 0x67, 0x07, 0xcd, 0xb8, 0x36, 0xae, 0xc5, 0x61, 0x73, 0x60, 0x03,
0xaf, 0x0c, 0x21, 0x8f, 0x85, 0x50, 0x39, 0x73, 0x5a, 0xd3, 0xec, 0x74, 0x0a, 0x2b, 0x22, 0x46,
0x57, 0x7f, 0x07, 0x12, 0xdb, 0x23, 0x06, 0x32, 0x11, 0x5d, 0xd7, 0x39, 0x41, 0xc1, 0xa2, 0xbd,
};

int main(int argc, char *argv[])
{
    int ret;
    RSA_TOOLS_PRIV_KEY_t priv;
    RSA_TOOLS_PUB_KEY_t  pub;
    unsigned char *plaintext;
    unsigned char encrypttext[512];
    unsigned char decrypttext[512];
    size_t        plen;
    size_t        elen;
    size_t        dlen;
    int           i;
	char          buf[10000];

    priv.n        = rsa_n;
    priv.n_len    = sizeof(rsa_n);
    priv.d        = rsa_d;
    priv.d_len    = sizeof(rsa_d);
    priv.p        = rsa_p;
    priv.p_len    = sizeof(rsa_p);
    priv.q        = rsa_q;
    priv.q_len    = sizeof(rsa_q);
    priv.dp       = rsa_dp;
    priv.dp_len   = sizeof(rsa_dp);
    priv.dq       = rsa_dq;
    priv.dq_len   = sizeof(rsa_dq);
    priv.qinv     = rsa_qinv;
    priv.qinv_len = sizeof(rsa_qinv);

    pub.n = rsa_n;
    pub.e = rsa_e;
    pub.n_len = sizeof(rsa_n);
    pub.e_len = sizeof(rsa_e);

    plaintext = EM;
    plen = sizeof(EM);
    elen = plen;
    dlen = plen;
    memset(encrypttext, 0, sizeof(encrypttext));
    memset(decrypttext, 0, sizeof(decrypttext));

    ret = rsaep(pub, plaintext, plen, encrypttext, &elen);
    if (0 != ret) {
        printf("Error.  ret=%d\n", ret);
    }
    else {
        hexdump(encrypttext,    elen, "Encrypted message");
        hexdump(expected_encEM, elen, "Expected message");
        if (0 != memcmp(encrypttext, expected_encEM, elen)) {
            printf("Compare NG.\n");
        }
        else {
            printf("Compare OK.\n");
        }

        ret = rsadp(priv, encrypttext, elen, decrypttext, &dlen, true);
        if (0 != ret) {
            printf("Error.  ret=%d\n", ret);
        }
        else {
            hexdump(decrypttext, dlen, "Decrypted message");
            hexdump(plaintext, plen, "Expected message");
            if (blkcmp(decrypttext, dlen, plaintext, plen, true)) {
                printf("Compare OK.\n");
            }
            else {
                printf("Compare NG.\n");
            }
        }
    }

    return 0;
}