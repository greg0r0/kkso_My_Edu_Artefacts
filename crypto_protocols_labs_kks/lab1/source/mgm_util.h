#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "magma_types.h"

#ifdef DEBUG
    #define log_data(...) printf(__VA_ARGS__) 
#else
    #define log_data(...) printf("\x00") 
#endif


mgm_Key* util_ReadKeyFromFile(const char* filename);
mgm_IV* util_ReadIVFromFile(const char* filename);
int util_getFileSize(const char* filename);
uint8_t selfcheck_startup();
unsigned int gen_crc32(const unsigned char *buf, int len);