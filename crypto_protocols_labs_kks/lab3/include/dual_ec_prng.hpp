#include "EllipricCurve.hpp"
#include "EllipricCurvePoint.hpp"
#include "MathUtil.hpp"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;
#include <time.h>

template <typename T> 
class DUAL_EC_PRNG_original
{
private:
    ECC::EllipticCurve<T> ecc;
    ECC::EllipticPoint<T> P;
    ECC::EllipticPoint<T> Q;
    T state;        // S

public:
    DUAL_EC_PRNG_original() {}

    //Init generator
    DUAL_EC_PRNG_original(ECC::EllipticCurve<T> base_ec, size_t seed)
    {
        this->ecc = base_ec;
        this->P = this->ecc.getGeneratorPoint();
        this->Q = 31337 * (this->ecc.getGeneratorPoint()); // в оригинальной статье тоже не сказано как выбраны точки :)
        this->state = seed;
    }

    T next_state()
    {
        ECC::EllipticPoint<T> s = this->state*this->P;
        T s_x = s.getX();
        ECC::EllipticPoint<T> r = s_x*this->Q;
        T r_x = r.getX();
        this->state = s_x;
        return r_x & 0x0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff_cppui;
    }
};

template <typename T> 
class DUAL_EC_PRNG_v2
{
private:
    ECC::EllipticCurve<T> ecc1;
    ECC::EllipticCurve<T> ecc2;
    ECC::EllipticPoint<T> P;
    ECC::EllipticPoint<T> Q;
    T state;        // S
    
public:
    DUAL_EC_PRNG_v2() {}

    //Init generator
    DUAL_EC_PRNG_v2(ECC::EllipticCurve<T> base_ec, ECC::EllipticCurve<T> second_ec, size_t seed, bool random_dot)
    {
        this->ecc1 = base_ec;
        this->ecc2 = second_ec;
        this->P = this->ecc1.getGeneratorPoint();
        if (random_dot)
            this->Q = time(0) * (this->ecc2.getGeneratorPoint());
        else
            this->Q = 0x31337 * (this->ecc2.getGeneratorPoint());
        this->state = seed;
    }

    size_t next_state()
    {
        ECC::EllipticPoint<T> s = this->state*this->P;
        T s_x = s.getX();
        ECC::EllipticPoint<T> r = s_x*this->Q;
        T r_x = r.getX();
        this->state = s_x;
        return (size_t)(r_x & 0xffffffffffffffff_cppui); //8 младших байт состояния
    }
};