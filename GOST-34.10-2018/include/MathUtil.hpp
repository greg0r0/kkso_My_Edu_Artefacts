#ifndef UTIL_HPP
#define UTIL_HPP

#include <fstream>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <ctime>
#include <cstdint>
#include <random>
#include <sys/stat.h>

using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;
typedef number<cpp_int_backend<2048, 2048, signed_magnitude, unchecked, void> > int2048_t;
namespace Util {

    template <typename T>
    class Math {
    public:
        class AffinePoint
        {
            public:
                int2048_t x;
                int2048_t y;
                int2048_t z;
            
                //AffinePoint(boost::multiprecision::int2048_t _x, boost::multiprecision::int2048_t _y, boost::multiprecision::int2048_t _z) : x( _x), y(_y), z(_z) {}
                AffinePoint(T _x, T _y, T _z)
                {
                    this->x = _x.template convert_to<int2048_t>();
                    this->y = _y.template convert_to<int2048_t>();
                    this->z = _z.template convert_to<int2048_t>();
                }
                
        };

        /*
            c++ operation% is shitty ._. (for Z/Zp)
        */
        static T mod(T a, T p){
            T res = a % p;
            return (res<0)?res+p:res;
        }

        /*
            compute y == a^b (mod n)
            return y
        */
        static T pow_finite(T a, T b, T n) {
            T x = 1, y = a;
            while (b > 0) {
                if (b % 2 == 1) {
                    x = (x * y) % n;
                }
                y = (y * y) % n;
                b /= 2;
            }
            return x % n;
        }

        static T gcd(T a, T b){
            while (b != 0)
            {
                T r = a % b;
                a = b;
                b = r;
            }

            return a;
        }

        /*
            au+bv=gcd(a,b)
            res.x = gcd(a,b), res.y = u, res.z = v 
        */
        static AffinePoint extended_gcd(T a, T b){
            AffinePoint U(a.template convert_to<int2048_t>(), 1, 0);
            AffinePoint V(b.template convert_to<int2048_t>(), 0, 1);
            while (V.x != 0){
                int2048_t q = U.x / V.x;
                AffinePoint Tmp(U.x % V.x,  U.y - q * V.y, U.z - q*V.z);
                U = V;
                V = Tmp;
            }
            return U;
        }

        /*
        solve n * n^-1 == 1 (mod modulus)
        return n^-1
        */
        static T invert(T n, T modulus){
            AffinePoint res = extended_gcd(n, modulus);
            
            if (res.y < 0)
                res.y += modulus;

            return T(res.y);
        }

        static AffinePoint Tonelli_Shanks_Alg(T n, T p) {
            T q = p - 1;
            T ss = 0;
            T z = 2;
            T c, r, t, m;
            T minus = -1;
        
            if (pow_finite(n, (p - 1) / 2, p) != 1) {
                return AffinePoint(minus, minus, 0);
            }
        
            while ((q & 1) == 0) {
                ss += 1;
                q >>= 1;
            }
        
            if (ss == 1) {
                T r1 = pow_finite(n, (p + 1) / 4, p);
                return AffinePoint(r1, p - r1, 1);
            }
        
            while (pow_finite(z, (p - 1) / 2, p) != p - 1) {
                z++;
            }
        
            c = pow_finite(z, q, p);
            r = pow_finite(n, (q + 1) / 2, p);
            t = pow_finite(n, q, p);
            m = ss;
        
            while (true) {
                T i = 0, zz = t;
                T b = c, e;
                if (t == 1) {
                    return AffinePoint(r, p - r, 1);
                }
                while (zz != 1 && i < (m - 1)) {
                    zz = zz * zz % p;
                    i++;
                }
                e = m - i - 1;
                while (e > 0) {
                    b = b * b % p;
                    e--;
                }
                r = r * b % p;
                c = b * b % p;
                t = t * c % p;
                m = i;
            }
        }

        static std::pair<T,T> getYfromX(T x, T modulo){
            AffinePoint a = Tonelli_Shanks_Alg(x , modulo);
            if (a.z) {
                T res_a = a.x.template convert_to<T>();
                T res_b = a.y.template convert_to<T>();
                return std::pair<T,T>(res_a, res_b);
            } else {
                return std::pair<T,T>(0,0);
            }
        }

        //just WTF func because boost multiprecision ops is broken
        static std::vector<bool> getBits(T n){
            std::vector<bool> res;
            int len = log2(n.template convert_to<double>());
            int i = 0;
            while (i <=len){
                res.push_back((n % 2)==1);
                n /= 2;
                i++;
            }
            std::reverse(res.begin(), res.end());
            return res;
        }

    };

    uint256_t convertArrToNumber_256(std::vector<unsigned char> arr)
        {
            assert( arr.size() == 32);
            uint256_t res;
            std::reverse(arr.begin(), arr.end());
            for (int i = 0; i < arr.size(); i++)
            {
                res |= ((uint256_t)arr[i]<<(8*i));
            } 
            
            return res;
        }

    uint512_t convertArrToNumber_512(std::vector<unsigned char> arr)
    {
        assert( arr.size() == 64);
        uint512_t res;
        std::reverse(arr.begin(), arr.end());
        for (int i = 0; i < arr.size(); i++)
        {
            res |= ((uint512_t)arr[i]<<(8*i));
        } 
        return res;
    }
    //convert unsigned char to vector< unsigned char > 
    template<int N>
    std::vector<unsigned char> convertUCtoVUC(unsigned char s[])
    {
        std::vector<unsigned char> res;
        //std::cout << s << std::endl;
        for (int i = 0; i < N; i++)
        {
            res.push_back(s[i]);
        }
        
        return res;
    }

    bool is_file_exist(const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
    }

    std::string readFile(std::string filepath)
    {
        std::ifstream fd(filepath);
        std::stringstream buf;
        if (fd) { buf << fd.rdbuf(); }
#ifdef DEBUG
        std::cout << "[!] Data read:\n" << buf.str().size() << std::endl;
#endif
        return buf.str();
    }
    
    int2048_t genRandomNumber(int2048_t modulo, int bits)
    {
        assert( (bits % 32) == 0);
        int2048_t res(0);
        std::time_t now = std::time(0);
        boost::random::mt19937 gen{ std::random_device()() };
        for (int i = 0; i < bits/32; i ++)
        {
            res |= ((int2048_t)gen())<<(i*32);
        }
        return Util::Math<int2048_t>::mod(res, modulo);
    }

    int2048_t readData_256b(std::string filepath, int pos=0)
    {
        unsigned char arr[32]; // 256 bit == 32*8 bit == 32 byte
        std::ifstream fd(filepath, std::ios::binary);
        fd.seekg(pos);  
        if (fd)
        {   
            for (int i = 0; i < 32; i++)
            {
                fd.read((char*)&arr[i], 1);
            }

        }
        fd.close();
        return convertArrToNumber_256(convertUCtoVUC<256/8>(arr));
    }

    void writeData_256b(int2048_t data, std::string filepath, int pos=0)
    {
        std::fstream fd;
        if (is_file_exist(filepath))
        {
            fd.open(filepath, std::ios::in|std::ios::out|std::ios::binary);
        } 
        else 
        {
            fd.open(filepath, std::ios::in|std::ios::out|std::ios::binary|std::ios::trunc);
        }
        fd.seekg(pos);  
        if (fd)
        {   
            for (int i = 0; i < 32; i++)
            {
                fd << (unsigned char)( (data >> (8*(31-i))) & 0xff);
            }
        }
        fd.close();
    }

    int2048_t readData_512b(std::string filepath, int pos=0)
    {
        unsigned char arr[64]; // 256 bit == 32*8 bit == 32 byte
        std::ifstream fd(filepath, std::ios::binary);
        fd.seekg(pos);  
        if (fd)
        {   
            for (int i = 0; i < 64; i++)
            {
                fd.read((char*)&arr[i], 1);
            }

        }
        fd.close();
        return convertArrToNumber_512(convertUCtoVUC<512/8>(arr));
    }

    void writeData_512b(int2048_t data, std::string filepath, int pos=0)
    {
        std::fstream fd;
        if (is_file_exist(filepath))
        {
            fd.open(filepath, std::ios::in|std::ios::out|std::ios::binary);
        } 
        else 
        {
            fd.open(filepath, std::ios::in|std::ios::out|std::ios::binary|std::ios::trunc);
        }
        fd.seekg(pos);  
        if (fd)
        {   
            for (int i = 0; i < 64; i++)
            {
                fd << (unsigned char)( (data >> (8*(63-i))) & 0xff);
            }
        }
        fd.close();
    }
}

#endif