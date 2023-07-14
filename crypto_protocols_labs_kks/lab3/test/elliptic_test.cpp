#include <gtest/gtest.h>
#include "../include/MathUtil.hpp"
#include "../include/EllipricCurve.hpp"
#include "../include/dual_ec_prng.hpp"

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;

#include <time.h>
#include <string>

TEST(_Math_Utils, __pow_test)
{
    ASSERT_EQ( Util::Math<int2048_t>::mod(50,16), 2);
}

TEST(_Math_Utils, __pow_test_inverted)
{
    ASSERT_EQ( Util::Math<int2048_t>::mod(-50,16), 14);
}

TEST(_Math_Utils, __pow_finite)
{
    ASSERT_EQ( Util::Math<int2048_t>::pow_finite(13,14,15), 4);
}

TEST(_Math_Utils, __invert)
{
    ASSERT_EQ( Util::Math<int2048_t>::invert(7,15), 13);
}

TEST(_ECC_EC_points, __mul_simple_1)
{
    ECC::EllipticPoint<int2048_t> a(10,8,23,0);

    ECC::EllipticPoint<int2048_t> res(22,12,23,0);
    ECC::EllipticPoint<int2048_t> check = 5*a; 
    ASSERT_EQ( check , res);
}

TEST(_ECC_EC_points, __mul_simple_2)
{
    ECC::EllipticPoint<int2048_t> b(22,12,23,0);

    ECC::EllipticPoint<int2048_t> res(22,11,23,0);

    ASSERT_EQ( (23*b) , res);
}

TEST(_ECC_EC_points, __sum_simple)
{
    ECC::EllipticPoint<int2048_t> a(22,12,23,0);

    ECC::EllipticPoint<int2048_t> b(10,8,23,0);

    ECC::EllipticPoint<int2048_t> res(9,0,23,0);

    ASSERT_EQ(a+b, res);
}

TEST(_ECC_EC_points, __sum_complex)
{
    ECC::EllipticPoint<int2048_t> a(22,12,23,0);
    int2048_t m1 = 10;
    ECC::EllipticPoint<int2048_t> b(1,10,23,0);
    int2048_t m2 = 3;
    ECC::EllipticPoint<int2048_t> res(8,6,23,0);

    ASSERT_EQ( ((m1*a) + (m2*b)) , res);
}

TEST(_ECC_EC_points, __sum_to_inf)
{
    ECC::EllipticPoint<int2048_t> res1(22,11,23,0);

    ECC::EllipticPoint<int2048_t> res2(22,12,23,0);

    ASSERT_EQ(res1+res2, ECC::EllipticPoint<int2048_t>(true));
}

TEST(_ECC_EC_points, __sum_with_inf)
{
    ECC::EllipticPoint<int2048_t> res1(22,11,23,0);
    ECC::EllipticPoint<int2048_t> i(true);

    ASSERT_EQ(res1+i, res1);
}

TEST(_ECC_EC_curve, __on_curve_check)
{
    ECC::EllipticCurve<int2048_t> ec(0,7,23);
    ECC::EllipticPoint<int2048_t> a(15,1,23,0);

    ASSERT_EQ(ec.CheckPoint(a), true);
}


TEST(_ECC_EC_DUAL_EC_PRNG_original, impl_test1)
{
    DUAL_EC_PRNG_original<int2048_t> gen(ECC::StandartCurves::gost3410_p256, 0x1337);
    auto x = gen.next_state();
    EXPECT_EQ(x, 0x3562a8fc18429bf454f15a050924e2fba12d7d8c1fd1bb526168fe0a4407_cppui);
    x = gen.next_state();
    EXPECT_EQ(x, 0xcf5a9eaf42a962490b57c2dee634e691b0d3aaf022e44cb6fa5b17cae634_cppui);
    x = gen.next_state();
    EXPECT_EQ(x, 0xc630b1ef256a2b5e92d31c745e6333abcc38fa4b751b4643d6c896ad05a7_cppui);
}


TEST(_ECC_EC_DUAL_EC_PRNG_original, impl_test2)
{
    DUAL_EC_PRNG_original<int2048_t> gen(ECC::StandartCurves::gost3410_p256, 0x31337);
    auto x = gen.next_state();
    EXPECT_EQ(x, 0x05dfac537dce95c8de0bb64a1371ded9a3da22798d84d5cb5bfc0a72001b_cppui);
    x = gen.next_state();
    EXPECT_EQ(x, 0xf9d98b2792bce87217b1a9f2c692261a5832386b2878ba2dadf7837cebf8_cppui);
    x = gen.next_state();
    EXPECT_EQ(x, 0xaf3fb3f26275d8acf0f0bfeddb2dd74795d43b1b305dd0008541ba16aae4_cppui);
}

TEST(_ECC_EC_DUAL_EC_PRNG_v2, impl_test1)
{
    DUAL_EC_PRNG_v2<int2048_t> gen(ECC::StandartCurves::gost3410_p256, ECC::StandartCurves::secp256k1, 0x31337, false);
    auto x = gen.next_state();
    EXPECT_EQ(x, 0xdf2b72c850d59621);
    x = gen.next_state();
    EXPECT_EQ(x, 0xf1f2e58a2fc7a2aa);
    x = gen.next_state();
    EXPECT_EQ(x, 0xf25b389c4a1605a4);
}

#pragma pack(1)
typedef struct mgm_Key{
    uint64_t key_block[4];
    time_t exp_time;
} mgm_Key;

TEST(_ECC_EC_DUAL_EC_PRNG_v2, speed_1000keys)
{
    int key_count = 1000;

    FILE* random = fopen("/dev/urandom", "rb");
    size_t seed = 0;
    fread(&seed, sizeof(size_t), 1, random);
    DUAL_EC_PRNG_v2<int2048_t> gen(ECC::StandartCurves::gost3410_p256, ECC::StandartCurves::secp256k1, seed, false);
    fclose(random);

    for (int i = 0; i < key_count; i++)
    {
        mgm_Key newkey;
        newkey.key_block[0] = gen.next_state();
        newkey.key_block[1] = gen.next_state();
        newkey.key_block[2] = gen.next_state();
        newkey.key_block[3] = gen.next_state();
        newkey.exp_time     = time(0) + 1209600; // current time + 2weeks

        std::string filename = "/tmp/examplekey_" + std::to_string(i) + ".mgmkey";
        FILE* file = fopen(filename.c_str(), "wb");
        fwrite(&newkey, sizeof(mgm_Key), 1, file);
        fclose(file);
        
    }
}

TEST(_ECC_EC_DUAL_EC_PRNG_v2, DISABLED_speed1_1mb)
{
    // генератор рандома генерирует 8 байт гаммы
    // нужно нагенерировать 1мб / 8б = 131072 состояний
    // потом этот же файл можно использовать для нистовых тестов

    int state_count = (1*1024*1024) / 8;

    FILE* random = fopen("/dev/urandom", "rb");
    FILE* output_file = fopen("/tmp/dual_ec_v2_random", "wb");
    size_t seed = 0;
    fread(&seed, sizeof(size_t), 1, random);
    DUAL_EC_PRNG_v2<int2048_t> gen(ECC::StandartCurves::gost3410_p256, ECC::StandartCurves::secp256k1, seed, false);
    
    for (int i = 0; i < state_count; i++)
    {   
        size_t next_block = gen.next_state();
        fwrite(&next_block, sizeof(size_t), 1, output_file);
    }

    fclose(random);
    fclose(output_file);
}

TEST(_ECC_EC_DUAL_EC_PRNG_v2, DISABLED_speed1_100mb)
{
    int state_count = (100*1024*1024) / 8;

    FILE* random = fopen("/dev/urandom", "rb");
    FILE* output_file = fopen("/tmp/dual_ec_v2_random_100", "wb");
    size_t seed = 0;
    fread(&seed, sizeof(size_t), 1, random);
    DUAL_EC_PRNG_v2<int2048_t> gen(ECC::StandartCurves::gost3410_p256, ECC::StandartCurves::secp256k1, seed, false);
    
    for (int i = 0; i < state_count; i++)
    {   
        size_t next_block = gen.next_state();
        fwrite(&next_block, sizeof(size_t), 1, output_file);
    }

    fclose(random);
    fclose(output_file);
}

TEST(_ECC_EC_DUAL_EC_PRNG_v2, DISABLED_speed1_1000mb)
{
    int state_count = (1000*1024*1024) / 8;

    FILE* random = fopen("/dev/urandom", "rb");
    FILE* output_file = fopen("/tmp/dual_ec_v2_random_1000", "wb");
    size_t seed = 0;
    fread(&seed, sizeof(size_t), 1, random);
    DUAL_EC_PRNG_v2<int2048_t> gen(ECC::StandartCurves::gost3410_p256, ECC::StandartCurves::secp256k1, seed, false);
    
    for (int i = 0; i < state_count; i++)
    {   
        size_t next_block = gen.next_state();
        fwrite(&next_block, sizeof(size_t), 1, output_file);
    }

    fclose(random);
    fclose(output_file);
}