#include "mgm_util.h"
#include "magma_algo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <unistd.h>

extern const unsigned char __etext;
static volatile unsigned int crc_code = 0x9c6eefd0;
__attribute__((constructor))
void self_check_text()
{
    unsigned int res = gen_crc32(&__etext, 0x14a4);
    if (res != crc_code)
    {
        syslog(LOG_INFO, "Bad CRC: control: %08x has: %08x", crc_code, res);
        exit(42);
    }
}


int main(int argc, char** argv)
{

    openlog ("GOST 34.12 64bit EnCryption Tool", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    if (!selfcheck_startup())
    {
        syslog(LOG_ERR, "Startup test is failed. Exit...");
        printf("Startup error\n");
        closelog(); 
        exit(-1);
    }
    syslog(LOG_INFO, "Startup test is OK. User ID=%d", getuid());


    if (argc != 6)
    {
        printf("Usage: %s [enc/dec] <IV file> <Key File> <Input file> <Output file>\n > enc - Encrypt file\n > dec - decrypt file\n", argv[0]);

        exit(-1);
    }

    mgm_IV* iv = util_ReadIVFromFile(argv[2]);
    mgm_Key* key = util_ReadKeyFromFile(argv[3]);

    if (strncmp(argv[1], "enc", 3))
    {
        uint8_t res = mgm_EncryptFile(argv[4], argv[5], iv, key);
        if (res == 4)
        {
            printf("Key is Expired. Encryption is blocked.\n");
        }
    } 
    else if (strncmp(argv[1], "dec", 3))
    {
        mgm_DecryptFile(argv[4], argv[5], iv, key);
    } 
    else
    {
        printf("Incorrect input.\n");
        closelog(); 
        exit(-1);
    }
    free(iv);
    free(key);
    closelog(); 
    return 0;
}