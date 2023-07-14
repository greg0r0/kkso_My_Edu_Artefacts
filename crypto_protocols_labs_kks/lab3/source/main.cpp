#include <iostream>
#include "../include/EllipricCurve.hpp"
#include "../include/EllipricCurvePoint.hpp"
#include "../include/MathUtil.hpp"
#include "../include/dual_ec_prng.hpp"

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;

#include <time.h>


#pragma pack(1)
typedef struct mgm_Key{
    uint64_t key_block[4];
    time_t exp_time;
} mgm_Key;

int main()
{
    FILE* random = fopen("/dev/urandom", "rb");
    size_t seed = 0;
    fread(&seed, sizeof(size_t), 1, random);
    DUAL_EC_PRNG_v2<int2048_t> gen(ECC::StandartCurves::gost3410_p256, ECC::StandartCurves::secp256k1, seed, true);
    fclose(random);

    mgm_Key newkey;
    newkey.key_block[0] = gen.next_state();
    newkey.key_block[1] = gen.next_state();
    newkey.key_block[2] = gen.next_state();
    newkey.key_block[3] = gen.next_state();
    newkey.exp_time     = time(0) + 1209600; // current time + 2weeks
    std::string input;
    std::cout << "Enter Key Name: ";
    std::cin  >> input;
    std::string filename = "./"+ input + ".mgmkey";
    FILE* file = fopen(filename.c_str(), "wb");
    fwrite(&newkey, sizeof(mgm_Key), 1, file);
    fclose(file);
        
    return 0;
}