#ifndef GOST_34_10_2018_HPP
#define GOST_34_10_2018_HPP

#include "EllipricCurve.hpp"
#include "streebog.h"

namespace GOST_34_10_2018
{
    namespace StreebogUtil
    {
        uint256_t getHash_256(std::string data)
        {
            unsigned char hash[256/8];
            cppcrypto::streebog f(256);
            f.init();
            auto t = data.c_str();
            
            f.update(reinterpret_cast<const unsigned char *>(t), strlen(t));
            f.final(hash);
#ifdef DEBUG
            std::cout << "[!] Hash 256bit" << std::endl;    
            std::cout.fill('0');        
            for (auto i : hash)
            {
                std::cout << std::setw(2) << std::hex << (int)i << " ";
            }
            std::cout << std::endl;
#endif

            return Util::convertArrToNumber_256( Util::convertUCtoVUC<256/8>(hash));
        }

        uint512_t getHash_512(std::string data)
        {
            unsigned char hash[512/8];
            cppcrypto::streebog f(512);
            f.init();
            auto t = data.c_str();
            f.update(reinterpret_cast<const unsigned char *>(t), strlen(t));
            f.final(hash);
#ifdef DEBUG
            std::cout << "[!] Hash 512bit" << std::endl;
            std::cout.fill('0');        
            for (auto i : hash)
            {
                std::cout << std::setw(2) << std::hex << (int)i << " ";
            }
            std::cout << std::endl;
#endif
            return Util::convertArrToNumber_512( Util::convertUCtoVUC<512/8>(hash));
        }
    }
    
    namespace Types
    {   

        template<typename T>
        class PublicKey
        {
            private:
                ECC::EllipticPoint<T> key;
            public:
                PublicKey(ECC::EllipticPoint<T> point): key(point) {}
                PublicKey(T qx, T qy) : key(ECC::EllipticPoint<T>(qx,qy)) {}
                ECC::EllipticPoint<T> getKey() { return this->key;}
        };

        template<typename T>
        class PrivateKey
        {
            private:
                T key;
            public:
                PrivateKey(T n) : key(n) {}
                T getKey() { return this->key;}
        };

        template<typename T>
        class Sign
        {
            private:
                T r;
                T s;
            public:
                Sign(T _r, T _s) : r(_r), s(_s) {}
                T getR() { return this->r;}
                T getS() { return this->s;}
        };

    }

    namespace Algorithms
    {
        //Return std::pair: first == PublicKey, second == PrivateKey
        //GOST doesn't describe how user must gen Data - i just get random number
        //bits == 256 || 512
        std::pair< Types::PublicKey<int2048_t>, Types::PrivateKey<int2048_t> > genKeys(ECC::EllipticCurve<int2048_t> curve, int bits)
        {
            assert(bits == 256 || bits == 512);
            int2048_t d = Util::genRandomNumber(curve.getQ(), bits);
            ECC::EllipticPoint<int2048_t> Q = d*curve.getGeneratorPoint();
            #ifdef DEBUG
            std::cout << "[!] DEBUG (genkeys)\nd=0x"<<std::hex<<d<<std::endl<<"Q="<<Q.ToString()<<std::endl;
            #endif
            return std::pair< Types::PublicKey<int2048_t>, Types::PrivateKey<int2048_t> >(Types::PublicKey<int2048_t>(Q), Types::PrivateKey<int2048_t>(d));
        }

        bool checkSign(std::string data, Types::Sign<int2048_t> sign,Types::PublicKey<int2048_t> pub, ECC::EllipticCurve<int2048_t> curve, int bits)
        {
            // Step 1. Check r && s 
            if ((sign.getR() < 0) || ( sign.getR() > curve.getQ()))
            {
                return false;
            }
            if ((sign.getS() < 0) || ( sign.getS() > curve.getQ()))
            {
                return false;
            }

            // Step 2. Get Hash (described as GOST 34.11 "Streebog") and compute `e`.
            int2048_t hash_sum = bits==256 ? StreebogUtil::getHash_256(data) : StreebogUtil::getHash_512(data);
            int2048_t e = Util::Math<int2048_t>::mod(hash_sum, curve.getQ());
            if (e.is_zero())
            {
                e = 1;
            }
            #ifdef DEBUG
            std::cout << "[!] DEBUG (check): hash=0x"<<std::hex << hash_sum << std::endl;
            std::cout << "[!] DEBUG (check): e=0x"<<std::hex << e << std::endl;
            #endif 

            // Step 3. Compute invert of e.
            int2048_t v = Util::Math<int2048_t>::invert(e, curve.getQ());

            #ifdef DEBUG
            std::cout << "[!] DEBUG (check): v=0x"<<std::hex << v << std::endl;
            #endif 

            // Step 4. Compute z1 and z2.
            int2048_t z1 = Util::Math<int2048_t>::mod(
                sign.getS()*v,
                curve.getQ()
            );
            int2048_t z2 = Util::Math<int2048_t>::mod(
                -1*sign.getR()*v,
                curve.getQ()
            );
            #ifdef DEBUG
            std::cout << "[!] DEBUG (check): z1=0x"<<std::hex << z1 << std::endl;
            std::cout << "[!] DEBUG (check): z2=0x"<<std::hex << z2 << std::endl;
            #endif 

            // Step 5. Compute Point C=z1*P + z2*Q (P in generator of elliptic group, Q is pubkey)
            ECC::EllipticPoint<int2048_t> new_C = (z1*curve.getGeneratorPoint()) + (z2*pub.getKey());
            #ifdef DEBUG
            std::cout << "[!] DEBUG (check): C="<< new_C.ToString() << std::endl;
            #endif

            // Step 6. Check sign!
            int2048_t R = Util::Math<int2048_t>::mod(new_C.getX(), curve.getQ());
            #ifdef DEBUG
            std::cout << "[!] DEBUG (check): R=0x"<<std::hex << R << std::endl;
            std::cout << "[!] DEBUG (check): r=0x"<<std::hex << sign.getR() << std::endl;
            #endif 
            if (R == sign.getR())
            {
                return true;
            } 
            else 
            {
                return false;
            }

        }

        //Generate sign numbers - r && s.
        Types::Sign<int2048_t> createSign(std::string data, Types::PrivateKey<int2048_t> priv, ECC::EllipticCurve<int2048_t> curve, int bits)
        {
            // Step 1. Get Hash (described as GOST 34.11 "Streebog") and compute `e`.
            int2048_t hash_sum = bits==256 ? StreebogUtil::getHash_256(data) : StreebogUtil::getHash_512(data);
            int2048_t e = Util::Math<int2048_t>::mod(hash_sum, curve.getQ());
            if (e.is_zero())
            {
                e = 1;
            }
            #ifdef DEBUG
            std::cout << "[!] DEBUG (create): hash=0x"<<std::hex << hash_sum << std::endl;
            std::cout << "[!] DEBUG (create): e=0x"<<std::hex << e << std::endl;
            #endif
            
            bad_k:
            // Step 2. Generate pseudo-random number k.
            int2048_t k = Util::genRandomNumber(curve.getQ(), bits);
            

            #ifdef DEBUG
            std::cout << "[!] DEBUG (create): k=0x"<<std::hex << k << std::endl;
            #endif

            // Step 3. Compute point C=k*P (where P - is generator of elliptic curve group)
            ECC::EllipticPoint<int2048_t> C = k*curve.getGeneratorPoint();
            #ifdef DEBUG
            std::cout << "[!] DEBUG (create): C="<< C.ToString() << std::endl;
            #endif

            // Step 4. Compute r. If r == 0 => return to step 2.
            int2048_t r = Util::Math<int2048_t>::mod(C.getX(), curve.getQ());
            if (r.is_zero())
            {
                //TODO: goto is bad, must be rewritten
                goto bad_k;
            }
            #ifdef DEBUG
            std::cout << "[!] DEBUG (create): r=0x"<<std::hex << r << std::endl;
            #endif

            // Step 5. Compute s. If s == 0 => return to step 2
            int2048_t s = Util::Math<int2048_t>::mod(
                r*priv.getKey()+k*e,
                curve.getQ()
            );
            if (s.is_zero())
            {
                goto bad_k;
            }
            #ifdef DEBUG
            std::cout << "[!] DEBUG (create): s=0x"<<std::hex << s << std::endl;
            #endif
            Types::Sign<int2048_t> gsign(r,s);

            return gsign;
        }
    }


}

#endif