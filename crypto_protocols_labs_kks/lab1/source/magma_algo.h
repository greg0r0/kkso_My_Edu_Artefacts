#pragma once
#include <stdint.h>
#include "magma_types.h"


// Round processing, xor operation
void mgm_xor(mgm_HalfBlock* block_h, mgm_HalfBlock* block_l, mgm_HalfBlock* output);
// Round processing, add operation
void mgm_add(mgm_HalfBlock* block_h, mgm_RoundKey* block_l, mgm_HalfBlock* output);
// Round processing, cyclic shift operation
void mgm_shift(mgm_HalfBlock* block, mgm_HalfBlock* output);
// Round processing, substitution operation
void mgm_sbox(mgm_HalfBlock* block, mgm_HalfBlock* output);
//Round processing, core function
void mgm_round_func(mgm_HalfBlock* left, mgm_HalfBlock* right, mgm_RoundKey* rkey);
//Round processing, key sheduler
mgm_RoundKey* mgm_getRoundKey(mgm_Key* key, uint8_t index);
//Encrypt block func
void mgm_EncryptBlock(mgm_Block* block, mgm_Key* Key, mgm_Block* encrypted_block);
//Decrypt block func
void mgm_DecryptBlock(mgm_Block* block, mgm_Key* Key, mgm_Block* encrypted_block);
//Encrypt block with IV func
void mgm_cbc_EncryptBlock(mgm_Block* block, mgm_IV* iv, mgm_Key* Key, mgm_Block* encrypted_block);
//Decrypt block with IV func
void mgm_cbc_DecryptBlock(mgm_Block* block, mgm_IV* iv, mgm_Key* Key, mgm_Block* encrypted_block);

//Encrypt file in CBC mode
//input_filename - filename for encryption with cbc.
//output_filename - encrypted file. Open in "wb" mode.
//iv - iv stuct offset
//key - key file struct offset
//return - error code (0 by default is ok)
uint8_t mgm_EncryptFile(const char* input_filename, const char* output_filename, mgm_IV* iv, mgm_Key* key);
//Decrypt file in CBC mode
//input_filename - filename for encryption with cbc.
//output_filename - encrypted file. Open in "wb" mode.
//iv - iv stuct offset
//key - key file struct offset
//return - error code (0 by default is ok)
uint8_t mgm_DecryptFile(const char* input_filename, const char* output_filename, mgm_IV* iv, mgm_Key* key);