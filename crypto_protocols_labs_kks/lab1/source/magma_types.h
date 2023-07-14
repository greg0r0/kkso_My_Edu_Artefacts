#pragma once
#include <stdint.h>
#include <time.h>

// MGM - MaGMa
#define MGM_256BIT 32
#define MGM_BLOCK_SIZE 8
#define MGM_HBLOCK_SIZE 4

#pragma pack(1)
typedef struct mgm_HalfBlock{
    uint8_t hblock[MGM_HBLOCK_SIZE];
} mgm_HalfBlock;

#pragma pack(1)
typedef struct mgm_Block{
    mgm_HalfBlock block_l;
    mgm_HalfBlock block_r;
} mgm_Block;

#pragma pack(1)
typedef struct mgm_RoundKey{
    uint8_t rkey[MGM_HBLOCK_SIZE];
} mgm_RoundKey;

#pragma pack(1)
typedef struct mgm_Key{
    mgm_RoundKey key_block[8];
    time_t exp_time;
} mgm_Key;

#pragma pack(1)
typedef struct mgm_IV{
    uint8_t iv[MGM_BLOCK_SIZE];
} mgm_IV;

static const uint8_t mgm_SubTable[8][16] = {
    {0b0001, 0b0111, 0b1110, 0b1101, 0b0000, 0b0101, 0b1000, 0b0011, 0b0100, 0b1111, 0b1010, 0b0110, 0b1001, 0b1100, 0b1011, 0b0010},
    {0b1000, 0b1110, 0b0010, 0b0101, 0b0110, 0b1001, 0b0001, 0b1100, 0b1111, 0b0100, 0b1011, 0b0000, 0b1101, 0b1010, 0b0011, 0b0111},
    {0b0101, 0b1101, 0b1111, 0b0110, 0b1001, 0b0010, 0b1100, 0b1010, 0b1011, 0b0111, 0b1000, 0b0001, 0b0100, 0b0011, 0b1110, 0b0000},
    {0b0111, 0b1111, 0b0101, 0b1010, 0b1000, 0b0001, 0b0110, 0b1101, 0b0000, 0b1001, 0b0011, 0b1110, 0b1011, 0b0100, 0b0010, 0b1100},
    {0b1100, 0b1000, 0b0010, 0b0001, 0b1101, 0b0100, 0b1111, 0b0110, 0b0111, 0b0000, 0b1010, 0b0101, 0b0011, 0b1110, 0b1001, 0b1011},
    {0b1011, 0b0011, 0b0101, 0b1000, 0b0010, 0b1111, 0b1010, 0b1101, 0b1110, 0b0001, 0b0111, 0b0100, 0b1100, 0b1001, 0b0110, 0b0000},
    {0b0110, 0b1000, 0b0010, 0b0011, 0b1001, 0b1010, 0b0101, 0b1100, 0b0001, 0b1110, 0b0100, 0b0111, 0b1011, 0b1101, 0b0000, 0b1111},
    {0b1100, 0b0100, 0b0110, 0b0010, 0b1010, 0b0101, 0b1011, 0b1001, 0b1110, 0b1000, 0b1101, 0b0111, 0b0000, 0b0011, 0b1111, 0b0001}
};
