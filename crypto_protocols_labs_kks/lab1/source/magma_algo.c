#include "magma_algo.h"
#include "mgm_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

void mgm_xor(mgm_HalfBlock* block_h, mgm_HalfBlock* block_l, mgm_HalfBlock* output)
{
    __asm__ volatile (
        "mov eax, [rax] \n"
        "mov ebx, [rbx] \n"
        "xor eax, ebx \n"
        "mov [rcx], eax \n"
        : 
        : "a" (block_h), "b" (block_l), "c" (output)
    ); 

    //log_data("\t After xor ::  h:0x%08x l:0x%08x out:0x%08x \n", *block_h, *block_l, *output);
}

void mgm_add(mgm_HalfBlock* block_h, mgm_RoundKey* key, mgm_HalfBlock* output)
{
    __asm__ volatile (
        "mov eax, [rax] \n"
        "bswap eax \n"       // fix this shit, it swap endianess then load from memory
        "mov ebx, [rbx] \n"
        "bswap ebx \n"
        "add eax, ebx \n"
        "bswap eax \n"
        "mov [rcx], eax \n"
        : 
        : "a" (block_h), "b" (key), "c" (output)
    ); 
    //log_data("\t After add ::  h:0x%08x k:0x%08x out:0x%08x \n", *block_h, *key, *output);
}

void mgm_shift(mgm_HalfBlock* block, mgm_HalfBlock* output)
{
    __asm__ volatile (
        "mov eax, [rax] \n"
        "bswap eax \n"
        "rol eax, 11 \n"
        "bswap eax \n"
        "mov [rcx], eax \n"
        : 
        : "a" (block), "c" (output)
    );
    //log_data("\t After shift ::  h:0x%08x k:0x%08x  \n", *block, *output);
}

void mgm_sbox(mgm_HalfBlock* block, mgm_HalfBlock* output)
{
    //too lazy for asm implementation
    for (int i = 0; i < MGM_HBLOCK_SIZE; i++)
    {
        //TODO: в одну строчку :D
        uint8_t high = (block->hblock[i] & 0xf0) >> 4;
        uint8_t low = block->hblock[i] & 0x0f;  
        //log_data("SBOX high=%d %08x low=%d %08x ==>", high,high,low, low);
        high = mgm_SubTable[2*i][high];
        low = mgm_SubTable[2*i+1][low];
        //log_data("high=%d %08x low=%d %08x \n",  high,high,low, low);
        output->hblock[i] = (high << 4) | low;
    }   
    //log_data("\t After Sbox ::  h:0x%08x k:0x%08x  \n", *block, *output);
} 

mgm_RoundKey* mgm_getRoundKey(mgm_Key* key, uint8_t index)
{
    mgm_RoundKey* rkey;
    //wtf, but crossplatform
    switch (index)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
        rkey = &key->key_block[(index-1)%8];
        break;
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
        rkey = &key->key_block[7-((index-1)%8)];
        break;
        
    default:
        rkey = NULL;
        break;
    }

    return rkey;
}

void mgm_round_func(mgm_HalfBlock* left, mgm_HalfBlock* right, mgm_RoundKey* rkey)
{
    mgm_HalfBlock temp;
    memset(&temp, 0, sizeof(mgm_HalfBlock));

    mgm_add(right,rkey,&temp);    
    mgm_sbox(&temp, &temp);
    mgm_shift(&temp, &temp);
    mgm_xor(left,&temp,&temp);

    memcpy(left, right, sizeof(mgm_HalfBlock));
    memcpy(right, &temp, sizeof(mgm_HalfBlock));
    memset(&temp, 0, sizeof(mgm_HalfBlock));

    //free(temp);
}

void mgm_EncryptBlock(mgm_Block* block, mgm_Key* Key, mgm_Block* encrypted_block)
{
    //распарсить блок в полублоки
    mgm_HalfBlock lblock, rblock;
    memcpy(&lblock, &block->block_l, sizeof(mgm_HalfBlock));
    memcpy(&rblock, &block->block_r, sizeof(mgm_HalfBlock));
   
    //32 раза пихнуть  round_func с шедулером ключа
    for (int round = 1; round <= 32; round++)
    {
        mgm_RoundKey* rkey = mgm_getRoundKey(Key, round);
        mgm_round_func(&lblock, &rblock, rkey);
    }
    //результат скопировать обратно в блок
    memcpy(&encrypted_block->block_l, &rblock, sizeof(mgm_HalfBlock)); //32 раунд - без замешивания
    memcpy(&encrypted_block->block_r, &lblock, sizeof(mgm_HalfBlock));
}

void mgm_DecryptBlock(mgm_Block* block, mgm_Key* Key, mgm_Block* decrypted_block)
{
    //распарсить блок в полублоки
    mgm_HalfBlock lblock, rblock;
    memcpy(&lblock, &block->block_l, sizeof(mgm_HalfBlock));
    memcpy(&rblock, &block->block_r, sizeof(mgm_HalfBlock));
   
    //32 раза пихнуть  round_func с шедулером ключа
    for (int round = 32; round >= 1; round--)
    {
        mgm_RoundKey* rkey = mgm_getRoundKey(Key, round);
        mgm_round_func(&lblock, &rblock, rkey);
    }
    //результат скопировать обратно в блок
    memcpy(&decrypted_block->block_l, &rblock, sizeof(mgm_HalfBlock)); //32 раунд - без замешивания
    memcpy(&decrypted_block->block_r, &lblock, sizeof(mgm_HalfBlock));
}

void mgm_xor_block_iv(mgm_Block* block, mgm_IV* iv)
{
    uint8_t* block_memory = (uint8_t*)malloc(sizeof(mgm_Block));
    memcpy(block_memory, block, sizeof(mgm_Block));

    for (int i=0; i < (int)sizeof(mgm_Block); i++)
    {
        block_memory[i] ^= iv->iv[i];
    }

    memcpy(block, block_memory, sizeof(mgm_Block));
    free(block_memory);
}

void mgm_cbc_EncryptBlock(mgm_Block* block, mgm_IV* iv, mgm_Key* Key, mgm_Block* encrypted_block)
{
    //Поксорить с IV
    mgm_xor_block_iv(block, iv);

    //распарсить блок в полублоки
    mgm_HalfBlock lblock, rblock;
    memcpy(&lblock, &block->block_l, sizeof(mgm_HalfBlock));
    memcpy(&rblock, &block->block_r, sizeof(mgm_HalfBlock));
    
    //32 раза пихнуть  round_func с шедулером ключа
    for (int round = 1; round <= 32; round++)
    {
        mgm_RoundKey* rkey = mgm_getRoundKey(Key, round);
        mgm_round_func(&lblock, &rblock, rkey);
    }
    //результат скопировать обратно в блок
    memcpy(&encrypted_block->block_l, &rblock, sizeof(mgm_HalfBlock)); //32 раунд - без замешивания
    memcpy(&encrypted_block->block_r, &lblock, sizeof(mgm_HalfBlock));
}

void mgm_cbc_DecryptBlock(mgm_Block* block, mgm_IV* iv, mgm_Key* Key, mgm_Block* decrypted_block)
{
    //распарсить блок в полублоки
    mgm_HalfBlock lblock, rblock;
    memcpy(&lblock, &block->block_l, sizeof(mgm_HalfBlock));
    memcpy(&rblock, &block->block_r, sizeof(mgm_HalfBlock));
   
    //32 раза пихнуть  round_func с шедулером ключа
    for (uint8_t round = 32; round >= 1; round--)
    {
        mgm_RoundKey* rkey = mgm_getRoundKey(Key, round);
        mgm_round_func(&lblock, &rblock, rkey);
    }
    //результат скопировать обратно в блок
    memcpy(&decrypted_block->block_l, &rblock, sizeof(mgm_HalfBlock)); //32 раунд - без замешивания
    memcpy(&decrypted_block->block_r, &lblock, sizeof(mgm_HalfBlock));

    //поксорить с IV
    mgm_xor_block_iv(decrypted_block, iv);
}

uint8_t mgm_EncryptFile(const char* input_filename, const char* output_filename, mgm_IV* iv, mgm_Key* key)
{
    uint8_t flag_is_error = 0;
    if (iv == NULL|| key == NULL)
    {
        syslog(LOG_INFO, "Key or IV data is missing.");
        flag_is_error = 3;
    }

    time_t key_time = key->exp_time;
    time_t current_time = time(0);

    if ((long)difftime(key_time, current_time) < 0)
    {
        syslog(LOG_INFO, "Key is Expired. Encryption is blocked. difftime=%ld", (unsigned long)difftime(key_time, current_time));
        flag_is_error = 4;
    }

    FILE* fin = fopen(input_filename, "rb");
    FILE* fout = fopen(output_filename, "wb");
    
    if (!fin)
    {
        syslog(LOG_INFO, "File %s not found.", input_filename);
        flag_is_error = 1;
    }

    if (!fout)
    {
        syslog(LOG_INFO, "File %s cant be opened.", output_filename);
        flag_is_error = 2;
    }

    unsigned long  size = util_getFileSize(input_filename);
    syslog(LOG_INFO, "File: %s Filesize: %ld Blocks: %ld\n", input_filename, size, size / sizeof(mgm_Block) + (size % sizeof(mgm_Block) == 0 ? 0:1));


    mgm_Block* buffer = (mgm_Block*)malloc(sizeof(mgm_Block));
    memset(buffer, 0, sizeof(mgm_Block));

    mgm_Block* buffer_enc = (mgm_Block*)malloc(sizeof(mgm_Block));
    memset(buffer, 0, sizeof(mgm_Block));

    //технически, комбинация memset(0)+fread выполняет процедуру 1 по дополнению блока (ГОСТ 34.13.2018, 4.1.2)

    if (!flag_is_error)
    {
        unsigned int key_crc = gen_crc32((unsigned char*)key, sizeof(mgm_Key));
        syslog(LOG_INFO, "Start encryption. Key stamp = %08x", key_crc);
        while( fread(buffer, 1, sizeof(mgm_Block), fin) )
        {
            mgm_cbc_EncryptBlock(buffer, iv, key, buffer_enc);
            fwrite(buffer_enc, 1, sizeof(mgm_Block), fout);
            memset(buffer, 0, sizeof(mgm_Block));
            memcpy(iv, buffer_enc, sizeof(mgm_Block));
            memset(buffer_enc, 0, sizeof(mgm_Block));
        }   
        syslog(LOG_INFO, "Encryption complete.");
    } 
    else
    {
        syslog(LOG_INFO, "Encryption NOT complete. Error=%d", flag_is_error);
    }

    memset(key, 0, sizeof(mgm_Key));
    memset(iv, 0, sizeof(mgm_IV));

    free(buffer);
    free(buffer_enc);
    fclose(fout);
    fclose(fin);
    return flag_is_error;
}

uint8_t mgm_DecryptFile(const char* input_filename, const char* output_filename, mgm_IV* iv, mgm_Key* key)
{
    uint8_t flag_is_error = 0;
    if (iv == NULL || key == NULL)
    {
        syslog(LOG_INFO, "Key or IV data is missing.");
        flag_is_error = 3;
    }

    FILE* fin = fopen(input_filename, "rb");
    FILE* fout = fopen(output_filename, "wb");

    if (!fin)
    {
        syslog(LOG_INFO, "File %s not found.", input_filename);
        flag_is_error = 1;
    }

    if (!fout)
    {
        syslog(LOG_INFO, "File %s cant be opened.", output_filename);
        flag_is_error = 2; 
    }


    unsigned long  size = util_getFileSize(input_filename);
    syslog(LOG_INFO, "File: %s Filesize: %ld Blocks: %ld\n", input_filename, size, size / sizeof(mgm_Block) + (size % sizeof(mgm_Block) == 0 ? 0:1));

    mgm_Block* buffer = (mgm_Block*)malloc(sizeof(mgm_Block));
    memset(buffer, 0, sizeof(mgm_Block));

    mgm_Block* buffer_enc = (mgm_Block*)malloc(sizeof(mgm_Block));
    memset(buffer, 0, sizeof(mgm_Block));

    if (!flag_is_error)
    {    
        unsigned int key_crc = gen_crc32((unsigned char*)key, sizeof(mgm_Key));
        syslog(LOG_INFO, "Start decryption. Key stamp = %08x", key_crc);
        //технически, комбинация memset(0)+fread выполняет процедуру 1 по дополнению блока (ГОСТ 34.13.2018, 4.1.2)
        while( fread(buffer, 1, sizeof(mgm_Block), fin) )
        {
            mgm_cbc_DecryptBlock(buffer, iv, key, buffer_enc);
            fwrite(buffer_enc, 1, sizeof(mgm_Block), fout);
            memcpy(iv, buffer, sizeof(mgm_Block));
            memset(buffer, 0, sizeof(mgm_Block));
            memset(buffer_enc, 0, sizeof(mgm_Block));
        }
                   
        syslog(LOG_INFO, "Decryption complete.");
    } 
    else
    {
        syslog(LOG_INFO, "Decryption NOT complete. Error=%d", flag_is_error);
    }

    memset(key, 0, sizeof(mgm_Key));
    memset(iv, 0, sizeof(mgm_IV));

    free(buffer);
    free(buffer_enc);
    fclose(fout);
    fclose(fin);
    return flag_is_error;
}
