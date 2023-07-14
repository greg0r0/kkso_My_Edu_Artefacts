#ifndef ELLIPTIC_CURVE_HPP
#define ELLIPTIC_CURVE_HPP

#include "EllipricCurvePoint.hpp"
#include <string>
#include <sstream>
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;

namespace ECC {

    template <typename T> // maybe class t?
    class EllipticCurve
    {
        /*
            Elliptic curve:  y^2 == x^3 + a*x+b (mod p)
        */

        private:
        T modulo;
        T a;
        T b;
        ECC::EllipticPoint<T> G;
        T n;
        T h;

        public:
        EllipticCurve() {}

        EllipticCurve(T _a, T _b, T _modulo) : a(_a), b(_b), modulo(_modulo), G(nullptr), n(nullptr), h(nullptr){}

        EllipticCurve(T _a, T _b, T _modulo, T _G_x, T _G_y, T _order, T _h)
        {
            this->a = _a;
            this->b = _b;
            this->modulo = _modulo;

            T ab_check = Util::Math<T>::mod(4*boost::multiprecision::powm(_a,3, _modulo)+27*boost::multiprecision::powm(b,2,_modulo), _modulo);

            if (ab_check == 0)
            {
                std::cout << "Bad a b parameters - it's singular curve" << std::endl;
                throw "Incorrect point G: Point not on curve";
            }

            this->n=_order;
            this->h=_h; 
            ECC::EllipticPoint<T> point(_G_x, _G_y, _modulo, _a);
            T left = boost::multiprecision::powm(point.getY(),2,_modulo);
            T right = Util::Math<T>::mod(boost::multiprecision::powm(point.getX(), 3, _modulo) + Util::Math<T>::mod(_a*point.getX(), _modulo)+Util::Math<T>::mod(_b, _modulo), _modulo);
            if (left==right){
                this->G = point;
            } else {
                std::cout << "Incorrect point G: Point not on curve" << std::endl;
                throw "Incorrect point G: Point not on curve";
            }
        }

        void setOrder(T order)
        {
            this->n = order;
        }

        ECC::EllipticPoint<T> getGeneratorPoint()
        {
            return this->G;
        }

        T getQ()
        {
            return this->n;
        }

        T getModulo()
        {
            return this->modulo;
        }

        T get_a()
        {
            return this->a;
        }

        bool CheckPoint(ECC::EllipticPoint<T> point){
            if (point.is_in_infinity()){
                return true;
            }
            T left = boost::multiprecision::powm(point.getY(),2,this->modulo);
            T right = boost::multiprecision::powm(point.getX(), 3, this->modulo) + Util::Math<T>::mod(this->a*point.getX(), this->modulo)+Util::Math<T>::mod(this->b, this->modulo);
            return (left == right);
        }

        bool CheckPoint(T x, T y){
            ECC::EllipticPoint<T> point(x,y,this->modulo, this->a);
            T left = boost::multiprecision::powm(point.getY(),2,this->modulo);
            T right = boost::multiprecision::powm(point.getX(), 3, this->modulo) + Util::Math<T>::mod(this->a*point.getX(), this->modulo)+Util::Math<T>::mod(this->b, this->modulo);
            return (left == right);
        }


        std::string ToString(){
            std::stringstream ss;       
            ss << "y == x^3 + " << this->a << "*x + " << this->b << " (mod " << this->modulo << ")";
            return ss.str(); 
        }

        std::pair< ECC::EllipticPoint<T> , ECC::EllipticPoint<T> > producePoint(T x)
        {
            T y = boost::multiprecision::powm(x, 3, this->modulo) + Util::Math<T>::mod(this->a*x, this->modulo)+Util::Math<T>::mod(this->b, this->modulo);
            
            std::pair<T,T> res_points = Util::Math<T>::getYfromX(y, this->modulo);
            if (res_points.first == 0 && res_points.second == 0)
            {
                return std::pair< ECC::EllipticPoint<T> , ECC::EllipticPoint<T> >( ECC::EllipticPoint<T>(true), ECC::EllipticPoint<T>(true) );
            } else {
                return std::pair< ECC::EllipticPoint<T> , ECC::EllipticPoint<T> >( ECC::EllipticPoint<T>(x, res_points.first, this->modulo, this->a), ECC::EllipticPoint<T>(x, res_points.second, this->modulo, this->a) );
            }
        }

        
    };

    namespace StandartCurves
    {
        //Bitcoin Curve, J(E)=0
        EllipticCurve<int2048_t> secp256k1(
        /*a*/      0,
        /*b*/      7,
        /*mod*/    0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f_cppui,
        /*G(x,y)*/ 0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798_cppui, 0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8_cppui,
        /*n*/      0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141_cppui,
        /*h*/      1
        );
                                            
        //J(E)!=0
        EllipticCurve<int2048_t> gost3410_p256(
            7,
            0x5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E_cppui,
            0x8000000000000000000000000000000000000000000000000000000000000431_cppui,
            2,0x8E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8_cppui,
            0x8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3_cppui,
            1
        );
        EllipticCurve<int2048_t> gost3410_p512(
            7,
            0x1cff0806a31116da29d8cfa54e57eb748bc5f377e49400fdd788b649eca1ac4361834013b2ad7322480a89ca58e0cf74bc9e540c2add6897fad0a3084f302adc_cppui,
            0x4531acd1fe0023c7550d267b6b2fee80922b14b2ffb90f04d4eb7c09b5d2d15df1d852741af4704a0458047e80e4546d35b8336fac224dd81664bbf528be6373_cppui,
            0x24d19cc64572ee30f396bf6ebbfd7a6c5213b3b3d7057cc825f91093a68cd762fd60611262cd838dc6b60aa7eee804e28bc849977fac33b4b530f1b120248a9a_cppui,0x2bb312a43bd2ce6e0d020613c857acddcfbf061e91e5f2c3f32447c259f39b2c83ab156d77f1496bf7eb3351e1ee4e43dc1a18b91b24640b6dbb92cb1add371e_cppui,
            0x4531acd1fe0023c7550d267b6b2fee80922b14b2ffb90f04d4eb7c09b5d2d15da82f2d7ecb1dbac719905c5eecc423f1d86e25edbe23c595d644aaf187e6e6df_cppui,
            1
        );
    }

}
#endif