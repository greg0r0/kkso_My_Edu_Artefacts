#include <gtest/gtest.h>

#include "../source/magma_algo.h"
#include "../source/mgm_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

TEST(MAGMA_ROUND_PROCESSING, mgm_xor_test_1)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "ABCD", 4);

    mgm_HalfBlock* low = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(low, "\x11\x22\x33\x44", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    
    mgm_xor(high, low, res);

    EXPECT_TRUE(0 == memcmp(res->hblock, "\x50\x60\x70\x00", 4));

    free(res);
    free(low);
    free(high);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_xor_test_2)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "ABCD", 4);

    mgm_HalfBlock* low = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(low, "ABCD", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    
    mgm_xor(high, low, res);

    EXPECT_TRUE(0 == memcmp(res->hblock, "\x00\x00\x00\x00", 4));

    free(res);
    free(low);
    free(high);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_add_test_1)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\x11\x22\x33\x44", 4);

    mgm_RoundKey* low = (mgm_RoundKey*)malloc(sizeof(mgm_RoundKey));
    memcpy(low, "\x11\x22\x33\x44", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));

    mgm_add(high, low, res);

    EXPECT_TRUE(0 == memcmp(res->hblock, "\x22\x44\x66\x88", 4));
    EXPECT_TRUE(0 == memcmp(high->hblock, "\x11\x22\x33\x44", 4));

    free(res);
    free(low);
    free(high);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_add_test_2)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\xaa\xbb\xcc\xdd", 4);

    mgm_RoundKey* low = (mgm_RoundKey*)malloc(sizeof(mgm_RoundKey));
    memcpy(low, "\xcc\xcc\xcc\xcc", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));

    mgm_add(high, low, res);

    EXPECT_TRUE(0 == memcmp(res->hblock, "\x77\x88\x99\xa9", 4));
    EXPECT_TRUE(0 == memcmp(high->hblock, "\xaa\xbb\xcc\xdd", 4));
    free(res);
    free(low);
    free(high);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_add_test_3)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\xff\xff\xff\xff", 4);

    mgm_RoundKey* low = (mgm_RoundKey*)malloc(sizeof(mgm_RoundKey));
    memcpy(low, "\x00\x00\x00\x03", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));

    mgm_add(high, low, res);

    EXPECT_TRUE(0 == memcmp(res->hblock, "\x00\x00\x00\x02", 4));
    EXPECT_TRUE(0 == memcmp(high->hblock, "\xff\xff\xff\xff", 4));

    free(res);
    free(low);
    free(high);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_rol_test_1)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\x01\x00\x00\x01", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));

    mgm_shift(high, res);

    EXPECT_TRUE(0 == memcmp(res->hblock, "\x00\x00\x08\x08", 4));

    free(res);
    free(high);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_rol_test_2)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\xac\x02\xf3\x00", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));

    mgm_shift(high, res);
    EXPECT_TRUE(0 == memcmp(res->hblock, "\x17\x98\x05\x60", 4));

    free(res);
    free(high);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_rol_test_3)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\x01\x02\x03\x04", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_shift(high, res);

    EXPECT_TRUE(0 == memcmp(res->hblock, "\x10\x18\x20\x08", 4));

    free(res);
    free(high);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_sbox_gost_test_1)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\xfd\xb9\x75\x31", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_sbox(high, res);
    
    EXPECT_TRUE(0 == memcmp(res->hblock, "\x2a\x19\x6f\x34", 4));

    free(res);
    free(high);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_sbox_gost_test_2)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\x2a\x19\x6f\x34", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_sbox(high, res);
    
    EXPECT_TRUE(0 == memcmp(res->hblock, "\xeb\xd9\xf0\x3a", 4));

    free(res);
    free(high);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_sbox_gost_test_3)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\xeb\xd9\xf0\x3a", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_sbox(high, res);
    
    EXPECT_TRUE(0 == memcmp(res->hblock, "\xb0\x39\xbb\x3d", 4));

    free(res);
    free(high);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_sbox_gost_test_4)
{
    mgm_HalfBlock* high = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    memcpy(high, "\xb0\x39\xbb\x3d", 4);

    mgm_HalfBlock* res = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_sbox(high, res);
    
    EXPECT_TRUE(0 == memcmp(res->hblock, "\x68\x69\x54\x33", 4));

    free(res);
    free(high);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_round_g_test_1)
{
    mgm_HalfBlock* temp = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_HalfBlock* block = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_RoundKey* rkey = (mgm_RoundKey*)malloc(sizeof(mgm_RoundKey));
    memset(temp, 0, sizeof(mgm_HalfBlock));
    memcpy(block, "\xfe\xdc\xba\x98", 4);
    memcpy(rkey, "\x87\x65\x43\x21", 4);

    mgm_add(block,rkey,temp);
    mgm_sbox(temp, temp);
    mgm_shift(temp, temp);

    EXPECT_TRUE(0 == memcmp(temp->hblock, "\xfd\xcb\xc2\x0c", 4));

    free(temp);
    free(block);
    free(rkey);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_round_g_test_2)
{
    mgm_HalfBlock* temp = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_HalfBlock* block = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_RoundKey* rkey = (mgm_RoundKey*)malloc(sizeof(mgm_RoundKey));
    memset(temp, 0, sizeof(mgm_HalfBlock));
    memcpy(block, "\x87\x65\x43\x21", 4);
    memcpy(rkey, "\xfd\xcb\xc2\x0c", 4);

    mgm_add(block,rkey,temp);
    mgm_sbox(temp, temp);
    mgm_shift(temp, temp);

    EXPECT_TRUE(0 == memcmp(temp->hblock, "\x7e\x79\x1a\x4b", 4));

    free(temp);
    free(block);
    free(rkey);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_round_g_test_3)
{
    mgm_HalfBlock* temp = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_HalfBlock* block = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_RoundKey* rkey = (mgm_RoundKey*)malloc(sizeof(mgm_RoundKey));
    memset(temp, 0, sizeof(mgm_HalfBlock));
    memcpy(block, "\xfd\xcb\xc2\x0c", 4);
    memcpy(rkey, "\x7e\x79\x1a\x4b", 4);

    mgm_add(block,rkey,temp);
    mgm_sbox(temp, temp);
    mgm_shift(temp, temp);

    EXPECT_TRUE(0 == memcmp(temp->hblock, "\xc7\x65\x49\xec", 4));

    free(temp);
    free(block);
    free(rkey);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_round_g_test_4)
{
    mgm_HalfBlock* temp = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_HalfBlock* block = (mgm_HalfBlock*)malloc(sizeof(mgm_HalfBlock));
    mgm_RoundKey* rkey = (mgm_RoundKey*)malloc(sizeof(mgm_RoundKey));
    memset(temp, 0, sizeof(mgm_HalfBlock));
    memcpy(block, "\x7e\x79\x1a\x4b", 4);
    memcpy(rkey, "\xc7\x65\x49\xec", 4);

    mgm_add(block,rkey, temp);
    mgm_sbox(temp, temp);
    mgm_shift(temp, temp);

    EXPECT_TRUE(0 == memcmp(temp->hblock, "\x97\x91\xc8\x49", 4));

    free(temp);
    free(block);
    free(rkey);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_key_shedule_1)
{
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    memcpy(key, "\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key));
    mgm_RoundKey* rkey = mgm_getRoundKey(key, 1);
    EXPECT_TRUE(0 == memcmp(rkey, "\xff\xee\xdd\xcc", 4));
    free(key);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_key_shedule_2)
{
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    memcpy(key, "\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key));
    mgm_RoundKey* rkey = mgm_getRoundKey(key, 17);
    EXPECT_TRUE(0 == memcmp(rkey, "\xff\xee\xdd\xcc", 4));
    free(key);
}

TEST(MAGMA_GOST_CONTOL_TEST, mgm_key_shedule_3)
{
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    memcpy(key, "\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key));
    mgm_RoundKey* rkey = mgm_getRoundKey(key, 31);
    EXPECT_TRUE(0 == memcmp(rkey, "\xbb\xaa\x99\x88", 4));
    free(key);
}

TEST(MAGMA_GOST_CONTOL_TEST, control_enc_test)
{
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    memcpy(key, "\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key));
    
    mgm_Block* block = (mgm_Block*)malloc(sizeof(mgm_Block));
    memcpy(block, "\xfe\xdc\xba\x98\x76\x54\x32\x10", sizeof(mgm_Block));

    mgm_Block* enc_block = (mgm_Block*)malloc(sizeof(mgm_Block));
    mgm_EncryptBlock(block, key, enc_block);

    EXPECT_TRUE(0 == memcmp(enc_block, "\x4e\xe9\x01\xe5\xc2\xd8\xca\x3d", sizeof(mgm_Block)));

    free(key);
    free(block);
    free(enc_block);
}

TEST(MAGMA_GOST_CONTOL_TEST, control_dec_test)
{
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    memcpy(key, "\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key));
    
    mgm_Block* block = (mgm_Block*)malloc(sizeof(mgm_Block));
    memcpy(block, "\x4e\xe9\x01\xe5\xc2\xd8\xca\x3d", sizeof(mgm_Block));

    mgm_Block* enc_block = (mgm_Block*)malloc(sizeof(mgm_Block));
    mgm_DecryptBlock(block, key, enc_block);

    EXPECT_TRUE(0 == memcmp(enc_block, "\xfe\xdc\xba\x98\x76\x54\x32\x10", sizeof(mgm_Block)));

    free(key);
    free(block);
    free(enc_block);
}

void mgm_xor_block_iv_test(mgm_Block* block, mgm_IV* iv)
{
    uint8_t* block_memory = (uint8_t*)malloc(sizeof(mgm_Block));
    memcpy(block_memory, block, sizeof(mgm_Block));

    for (int i=0; i< (uint8_t)sizeof(mgm_Block); i++)
    {
        block_memory[i] ^= iv->iv[i];
    }

    memcpy(block, block_memory, sizeof(mgm_Block));
    free(block_memory);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_block_xor_1)
{
    mgm_Block* block = (mgm_Block*)malloc(sizeof(mgm_Block));
    memcpy(block, "\x4e\xe9\x01\xe5\xc2\xd8\xca\x3d", sizeof(mgm_Block));
    mgm_IV* iv = (mgm_IV*)malloc(sizeof(mgm_IV));
    memcpy(iv, "\x4e\xe9\x01\xe5\xc2\xd8\xca\x3d", sizeof(mgm_Block));

    mgm_xor_block_iv_test(block, iv);
    EXPECT_TRUE(0 == memcmp(block, "\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(mgm_Block)));

    free(block);
    free(iv);
}

TEST(MAGMA_ROUND_PROCESSING, mgm_block_xor_2)
{
    mgm_Block* block = (mgm_Block*)malloc(sizeof(mgm_Block));
    memcpy(block, "\x01\x01\x01\x01\x20\x20\x20\x20", sizeof(mgm_Block));
    mgm_IV* iv = (mgm_IV*)malloc(sizeof(mgm_IV));
    memcpy(iv, "\x20\x20\x20\x20\x01\x01\x01\x01", sizeof(mgm_Block));

    mgm_xor_block_iv_test(block, iv);
    EXPECT_TRUE(0 == memcmp(block, "\x21\x21\x21\x21\x21\x21\x21\x21", sizeof(mgm_Block)));

    free(block);
    free(iv);
}

TEST(MAGMA_GOST_CONTOL_TEST, control_enc_with_iv_test)
{
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    memcpy(key, "\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key));
    
    mgm_Block* block = (mgm_Block*)malloc(sizeof(mgm_Block));
    memcpy(block, "\x00\x00\x00\x00\x76\x54\x32\x10", sizeof(mgm_Block));

    mgm_IV* iv = (mgm_IV*)malloc(sizeof(mgm_IV));
    memcpy(iv, "\xfe\xdc\xba\x98\x00\x00\x00\x00", sizeof(mgm_IV));

    mgm_Block* enc_block = (mgm_Block*)malloc(sizeof(mgm_Block));
    mgm_cbc_EncryptBlock(block, iv, key, enc_block);

    EXPECT_TRUE(0 == memcmp(enc_block, "\x4e\xe9\x01\xe5\xc2\xd8\xca\x3d", sizeof(mgm_Block)));

    free(key);
    free(block);
    free(enc_block);
    free(iv);
}

TEST(MAGMA_GOST_CONTOL_TEST, control_dec_with_iv_test)
{
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    memcpy(key, "\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key));
    
    mgm_Block* block = (mgm_Block*)malloc(sizeof(mgm_Block));
    memcpy(block, "\x4e\xe9\x01\xe5\xc2\xd8\xca\x3d", sizeof(mgm_Block));

    mgm_IV* iv = (mgm_IV*)malloc(sizeof(mgm_IV));
    memcpy(iv, "\xfe\xdc\xba\x98\x00\x00\x00\x00", sizeof(mgm_IV));

    mgm_Block* dec_block = (mgm_Block*)malloc(sizeof(mgm_Block));
    mgm_cbc_DecryptBlock(block, iv, key, dec_block);

    EXPECT_TRUE(0 == memcmp(dec_block, "\x00\x00\x00\x00\x76\x54\x32\x10", sizeof(mgm_Block)));

    free(key);
    free(block);
    free(dec_block);
    free(iv);
}

TEST(MAGMA_CBC_CONTOL_TEST, control_cbc_2b)
{
    syslog(LOG_INFO, "[[test::control_cbc_2b]]");
    FILE* blocks_fd = fopen("/tmp/mgm_blocks", "wb+");
    FILE* key_fd = fopen("/tmp/mgm_key", "wb+");
    FILE* iv_fd = fopen("/tmp/mgm_iv", "wb+");
    fwrite("\x01\x02\x03\x04\x05\x06\xfa\xce", sizeof(mgm_Block), 1, blocks_fd);
    fwrite("\x01\x00\x00\x00\x00\x00\x00\x00", sizeof(mgm_Block), 1, blocks_fd);
    fwrite("\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key), 1, key_fd);
    fwrite("\x01\x02\x03\x04\x05\x06\x07\xff", sizeof(mgm_IV), 1, iv_fd);
    fclose(blocks_fd);
    fclose(key_fd);
    fclose(iv_fd);


    mgm_IV* iv = util_ReadIVFromFile("/tmp/mgm_iv");
    mgm_Key* key = util_ReadKeyFromFile("/tmp/mgm_key");

    mgm_EncryptFile("/tmp/mgm_blocks", "/tmp/mgm_blocks_enc", iv, key);
    free(iv);
    free(key);

    iv = util_ReadIVFromFile("/tmp/mgm_iv");
    key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_DecryptFile("/tmp/mgm_blocks_enc", "/tmp/mgm_blocks_dec", iv, key);

    free(iv);
    free(key);

    FILE* orig_fd = fopen("/tmp/mgm_blocks", "rb+");
    FILE* decrypted_fd = fopen("/tmp/mgm_blocks_dec", "rb+");
    uint8_t flag_is_same = 1;

    // пройтись memcpy по исходнику и шифровке
    for (int i = 0;; i++)
    {
        uint8_t c1 = getc(orig_fd);
        uint8_t c2 = getc(decrypted_fd);

        if (c1 != c2)
            flag_is_same = 0;
        if (feof(orig_fd) || feof(decrypted_fd))
            break;
    }
    fclose(orig_fd);
    fclose(decrypted_fd);

    EXPECT_TRUE(flag_is_same);
}

TEST(MAGMA_CBC_CONTOL_TEST, expired_key_test)
{
    syslog(LOG_INFO, "[[test::expired_key_test]]");
    // Создать файл исходников в /tmp/
    FILE* blocks_fd = fopen("/tmp/mgm_blocks", "wb+");
    // Создать файл ключа в /tmp/
    FILE* key_fd = fopen("/tmp/mgm_key_bad", "wb+");
    // Создать файл iv в /tmp/
    FILE* iv_fd = fopen("/tmp/mgm_iv", "wb+");

    // записать данные в исходники, длина - 2 блока (сделать циклом чтобы масштабировать?)
    fwrite("\x01\x02\x03\x04\x05\x06\xfa\xce", sizeof(mgm_Block), 1, blocks_fd);
    fwrite("\x01\x00\x00\x00\x00\x00\x00\x00", sizeof(mgm_Block), 1, blocks_fd);
    // записать iv и ключ
    fwrite("\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\x00\x00\x00\x00\x00\x00\x00\x00", sizeof(mgm_Key), 1, key_fd);
    fwrite("\x01\x02\x03\x04\x05\x06\x07\xff", sizeof(mgm_IV), 1, iv_fd);

    // fclose
    fclose(blocks_fd);
    fclose(key_fd);
    fclose(iv_fd);

    // основной цикл шифрования
    mgm_IV* iv = util_ReadIVFromFile("/tmp/mgm_iv");
    mgm_Key* key = util_ReadKeyFromFile("/tmp/mgm_key_bad");
    uint8_t res = mgm_EncryptFile("/tmp/mgm_blocks", "/tmp/mgm_blocks_enc", iv, key);
    free(key);
    free(iv);
    EXPECT_TRUE(res == 4);
}

TEST(MAGMA_CBC_CONTOL_TEST, speed_cbc_1mb)
{
    syslog(LOG_INFO, "[[test::speed_cbc_1mb]]");
    const char* blocks_name = "/tmp/mgm_blocks_1mb";
    const char* blocks_enc_name = "/tmp/mgm_blocks_1mb_enc";
    const char* blocks_dec_name = "/tmp/mgm_blocks_1mb_dec";
    
    struct stat blocks_stat;
    if (stat(blocks_name, &blocks_stat) == -1)
    {
        FILE* blocks_fd = fopen(blocks_name, "wb+");
        FILE* random = fopen("/dev/urandom", "r");
        for (unsigned int i = 0; i < (1*1024*1024 / sizeof(mgm_Block)); i++)
        {
            mgm_Block local_block;
            fread(&local_block, sizeof(mgm_Block), 1, random);
            fwrite(&local_block, sizeof(mgm_Block), 1, blocks_fd);
        }
        fclose(random); 
        fclose(blocks_fd);
    }
    
    // записать iv и ключ
    FILE* key_fd = fopen("/tmp/mgm_key", "wb+");
    FILE* iv_fd = fopen("/tmp/mgm_iv", "wb+");
    fwrite("\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key), 1, key_fd);
    fwrite("\x01\x02\x03\x04\x05\x06\x07\xff", sizeof(mgm_IV), 1, iv_fd);

    // fclose
    fclose(key_fd);
    fclose(iv_fd);

    // основной цикл шифрования
    mgm_IV* iv = util_ReadIVFromFile("/tmp/mgm_iv");
    mgm_Key* key = util_ReadKeyFromFile("/tmp/mgm_key");
    
    mgm_EncryptFile(blocks_name, blocks_enc_name, iv, key);
    free(iv);

    // основной цикл расшифрования
    iv = util_ReadIVFromFile("/tmp/mgm_iv");
    key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_DecryptFile(blocks_enc_name, blocks_dec_name, iv, key);
    free(iv);
    free(key);
}

TEST(MAGMA_CBC_CONTOL_TEST, DISABLED_speed_cbc_10mb)
{
    syslog(LOG_INFO, "[[test::speed_cbc_10mb]]");
    const char* blocks_name = "/tmp/mgm_blocks_10mb";
    const char* blocks_enc_name = "/tmp/mgm_blocks_10mb_enc";
    const char* blocks_dec_name = "/tmp/mgm_blocks_10mb_dec";
    
    struct stat blocks_stat;
    if (stat(blocks_name, &blocks_stat) == -1)
    {
        FILE* blocks_fd = fopen(blocks_name, "wb+");
        FILE* random = fopen("/dev/urandom", "r");
        for (unsigned int i = 0; i < (10*1024*1024 / sizeof(mgm_Block)); i++)
        {
            mgm_Block local_block;
            fread(&local_block, sizeof(mgm_Block), 1, random);
            fwrite(&local_block, sizeof(mgm_Block), 1, blocks_fd);
        }
        fclose(random); 
        fclose(blocks_fd);
    }
    
    // записать iv и ключ
    FILE* key_fd = fopen("/tmp/mgm_key", "wb+");
    FILE* iv_fd = fopen("/tmp/mgm_iv", "wb+");
    fwrite("\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key), 1, key_fd);
    fwrite("\x01\x02\x03\x04\x05\x06\x07\xff", sizeof(mgm_IV), 1, iv_fd);

    // fclose
    fclose(key_fd);
    fclose(iv_fd);

    // основной цикл шифрования
    mgm_IV* iv = util_ReadIVFromFile("/tmp/mgm_iv");
    mgm_Key* key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_EncryptFile(blocks_name, blocks_enc_name, iv, key);
    free(iv);

    // основной цикл расшифрования
    iv = util_ReadIVFromFile("/tmp/mgm_iv");
    key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_DecryptFile(blocks_enc_name, blocks_dec_name, iv, key);
    free(iv);
    free(key);
}

TEST(MAGMA_CBC_CONTOL_TEST, DISABLED_speed_cbc_100mb)
{
    syslog(LOG_INFO, "[[test::speed_cbc_100mb]]");
    const char* blocks_name = "/tmp/mgm_blocks_100mb";
    const char* blocks_enc_name = "/tmp/mgm_blocks_100mb_enc";
    const char* blocks_dec_name = "/tmp/mgm_blocks_100mb_dec";
    
    struct stat blocks_stat;
    if (stat(blocks_name, &blocks_stat) == -1)
    {
        FILE* blocks_fd = fopen(blocks_name, "wb+");
        FILE* random = fopen("/dev/urandom", "r");
        for (unsigned int i = 0; i < (100*1024*1024 / sizeof(mgm_Block)); i++)
        {
            mgm_Block local_block;
            fread(&local_block, sizeof(mgm_Block), 1, random);
            fwrite(&local_block, sizeof(mgm_Block), 1, blocks_fd);
        }
        fclose(random); 
        fclose(blocks_fd);
    }
    
    // записать iv и ключ
    FILE* key_fd = fopen("/tmp/mgm_key", "wb+");
    FILE* iv_fd = fopen("/tmp/mgm_iv", "wb+");
    fwrite("\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key), 1, key_fd);
    fwrite("\x01\x02\x03\x04\x05\x06\x07\xff", sizeof(mgm_IV), 1, iv_fd);

    // fclose
    fclose(key_fd);
    fclose(iv_fd);

    // основной цикл шифрования
    mgm_IV* iv = util_ReadIVFromFile("/tmp/mgm_iv");
    mgm_Key* key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_EncryptFile(blocks_name, blocks_enc_name, iv, key);
    free(iv);

    // основной цикл расшифрования
    iv = util_ReadIVFromFile("/tmp/mgm_iv");
    key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_DecryptFile(blocks_enc_name, blocks_dec_name, iv, key);
    free(iv);
    free(key);
}

TEST(MAGMA_CBC_CONTOL_TEST, DISABLED_speed_cbc_1000mb)
{
    syslog(LOG_INFO, "[[test::speed_cbc_1000mb]]");
    const char* blocks_name = "/tmp/mgm_blocks_1000mb";
    const char* blocks_enc_name = "/tmp/mgm_blocks_1000mb_enc";
    const char* blocks_dec_name = "/tmp/mgm_blocks_1000mb_dec";
    
    struct stat blocks_stat;
    if (stat(blocks_name, &blocks_stat) == -1)
    {
        FILE* blocks_fd = fopen(blocks_name, "wb+");
        FILE* random = fopen("/dev/urandom", "r");
        for (unsigned int i = 0; i < (1000*1024*1024 / sizeof(mgm_Block)); i++)
        {
            mgm_Block local_block;
            fread(&local_block, sizeof(mgm_Block), 1, random);
            fwrite(&local_block, sizeof(mgm_Block), 1, blocks_fd);
        }
        fclose(random); 
        fclose(blocks_fd);
    }
    
    // записать iv и ключ
    FILE* key_fd = fopen("/tmp/mgm_key", "wb+");
    FILE* iv_fd = fopen("/tmp/mgm_iv", "wb+");
    fwrite("\xff\xee\xdd\xcc\xbb\xaa\x99\x88\x77\x66\x55\x44\x33\x22\x11\x00\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff\xdd\x11\x52\xca\x00\x00\x00\x00", sizeof(mgm_Key), 1, key_fd);
    fwrite("\x01\x02\x03\x04\x05\x06\x07\xff", sizeof(mgm_IV), 1, iv_fd);

    // fclose
    fclose(key_fd);
    fclose(iv_fd);

    // основной цикл шифрования
    mgm_IV* iv = util_ReadIVFromFile("/tmp/mgm_iv");
    mgm_Key* key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_EncryptFile(blocks_name, blocks_enc_name, iv, key);
    free(iv);

    // основной цикл расшифрования
    iv = util_ReadIVFromFile("/tmp/mgm_iv");
    key = util_ReadKeyFromFile("/tmp/mgm_key");
    mgm_DecryptFile(blocks_enc_name, blocks_dec_name, iv, key);
    free(iv);
    free(key);
}

mgm_Key* generateRandomKey()
{
    FILE* random = fopen("/dev/urandom", "r");
    mgm_Key* key = (mgm_Key*)malloc(sizeof(mgm_Key));
    fread(&(key->key_block), 32, 1, random);
    memcpy(&(key->exp_time), "\xff\xff\xff\xff\x00\x00\x00\x00", 8); // максимальная дата-время для удобства
    fclose(random);
    return key;
}

TEST(MAGMA_CBC_CONTOL_TEST, keys_swap_10)
{
    syslog(LOG_INFO, "[[test::keys_swap_10]]");
    int blocks_count = 10*10*10*10*10*10;
    int change_frequency = 10;

    int keys_count = blocks_count / change_frequency + 1;  // 10^6 / 10
    mgm_Key** keys = (mgm_Key**)malloc(keys_count*sizeof(mgm_Key)); // 10^5 * 40 = ~3MB
    for (int i = 0; i < keys_count; i++)
    {
        keys[i] = generateRandomKey();
    }

    // {main start}
    FILE* random = fopen("/dev/urandom", "r");
    int key_id = 0;
    for (int block_id = 0; block_id < blocks_count; block_id++)
    {
        // создать рандомный iv
        mgm_IV iv;
        fread(&iv, sizeof(mgm_IV), 1, random);
        // создать рандомный блок
        mgm_Block block, out_block, block_dec, xblock;
        fread(&block, sizeof(mgm_Block), 1, random);
        memcpy(&xblock, &block, 8);
        memset(&block_dec, 0, 8);
        memset(&out_block, 0, 8);
        // зашифровать
            
        mgm_cbc_EncryptBlock(&block, &iv, keys[key_id], &out_block);
 
        // расшифровать
        mgm_cbc_DecryptBlock(&out_block, &iv, keys[key_id], &block_dec);

        if (memcmp(&xblock, &block_dec, sizeof(mgm_Block)) != 0)
        {
            FAIL();
        }

        // увеличить индекс ключа при контрольной точке
        if (block_id % change_frequency == 0)
        {
            key_id++;
        }
    }
    fclose(random);
    // {main end}

    for (int i = 0; i < keys_count; i++)
    {
        free(keys[i]);
    }
    free(keys);
}

TEST(MAGMA_CBC_CONTOL_TEST, keys_swap_100)
{
    syslog(LOG_INFO, "[[test::keys_swap_10]]");
    int blocks_count = 10*10*10*10*10*10;
    int change_frequency = 100;

    int keys_count = blocks_count / change_frequency + 1;  // 10^6 / 10
    mgm_Key** keys = (mgm_Key**)malloc(keys_count*sizeof(mgm_Key)); // 10^5 * 40 = ~3MB
    for (int i = 0; i < keys_count; i++)
    {
        keys[i] = generateRandomKey();
    }

    // {main start}
    FILE* random = fopen("/dev/urandom", "r");
    int key_id = 0;
    for (int block_id = 0; block_id < blocks_count; block_id++)
    {
        // создать рандомный iv
        mgm_IV iv;
        fread(&iv, sizeof(mgm_IV), 1, random);
        // создать рандомный блок
        mgm_Block block, out_block, block_dec, xblock;
        fread(&block, sizeof(mgm_Block), 1, random);
        memcpy(&xblock, &block, 8);
        memset(&block_dec, 0, 8);
        memset(&out_block, 0, 8);
        // зашифровать
            
        mgm_cbc_EncryptBlock(&block, &iv, keys[key_id], &out_block);
 
        // расшифровать
        mgm_cbc_DecryptBlock(&out_block, &iv, keys[key_id], &block_dec);

        if (memcmp(&xblock, &block_dec, sizeof(mgm_Block)) != 0)
        {
            FAIL();
        }

        // увеличить индекс ключа при контрольной точке
        if (block_id % change_frequency == 0)
        {
            key_id++;
        }
    }
    fclose(random);
    // {main end}

    for (int i = 0; i < keys_count; i++)
    {
        free(keys[i]);
    }
    free(keys);
}

TEST(MAGMA_CBC_CONTOL_TEST, keys_swap_1000)
{
    syslog(LOG_INFO, "[[test::keys_swap_10]]");
    int blocks_count = 10*10*10*10*10*10;
    int change_frequency = 1000;

    int keys_count = blocks_count / change_frequency + 1;  // 10^6 / 10
    mgm_Key** keys = (mgm_Key**)malloc(keys_count*sizeof(mgm_Key)); // 10^5 * 40 = ~3MB
    for (int i = 0; i < keys_count; i++)
    {
        keys[i] = generateRandomKey();
    }

    // {main start}
    FILE* random = fopen("/dev/urandom", "r");
    int key_id = 0;
    for (int block_id = 0; block_id < blocks_count; block_id++)
    {
        // создать рандомный iv
        mgm_IV iv;
        fread(&iv, sizeof(mgm_IV), 1, random);
        // создать рандомный блок
        mgm_Block block, out_block, block_dec, xblock;
        fread(&block, sizeof(mgm_Block), 1, random);
        memcpy(&xblock, &block, 8);
        memset(&block_dec, 0, 8);
        memset(&out_block, 0, 8);
        // зашифровать
            
        mgm_cbc_EncryptBlock(&block, &iv, keys[key_id], &out_block);
 
        // расшифровать
        mgm_cbc_DecryptBlock(&out_block, &iv, keys[key_id], &block_dec);

        if (memcmp(&xblock, &block_dec, sizeof(mgm_Block)) != 0)
        {
            FAIL();
        }

        // увеличить индекс ключа при контрольной точке
        if (block_id % change_frequency == 0)
        {
            key_id++;
        }
    }
    fclose(random);
    // {main end}

    for (int i = 0; i < keys_count; i++)
    {
        free(keys[i]);
    }
    free(keys);
}