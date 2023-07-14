#include <iostream>
#include <array>
#include <vector>
#include <map>

#include "../include/EllipricCurve.hpp"
#include "../include/MathUtil.hpp"

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;


int2048_t ro_pollard_alg(ECC::EllipticPoint<int2048_t> P, ECC::EllipticPoint<int2048_t> Q, ECC::EllipticCurve<int2048_t> ec)
{
    //stage 1: define L and H
    const int L = 16;
    auto H = [L](int2048_t x) -> int2048_t { //partition func
        return Util::Math<int2048_t>::mod(x, L);
    };

    //support func for next step
    auto gen_triple = [&ec, P, Q]() -> std::tuple<int2048_t, int2048_t, ECC::EllipticPoint<int2048_t> > 
    {
        _subgen:

        auto a = Util::genRandomNumber(ec.getOrder(), 512);
        auto b = Util::genRandomNumber(ec.getOrder(), 512);
        if (a == 0 || b == 0) { goto _subgen; }
        ECC::EllipticPoint<int2048_t> Rl = (a*P);
        if (!ec.CheckPoint(Rl)){
            goto _subgen;
        }

        ECC::EllipticPoint<int2048_t> Rr = (b*Q);
        if (!ec.CheckPoint(Rr)){
            //std::cout << "Rr" <<ec.CheckPoint(Rr) <<" "<<  Rr << " " << b << " " << Q << std::endl;
            goto _subgen;
        }

        if (Rl.getX() == Rr.getX()) {
            goto _subgen;
        }

        ECC::EllipticPoint<int2048_t> R = Rl + Rr;
        if (!ec.CheckPoint(R)){

            goto _subgen;
        }

        return std::make_tuple(a,b,R);
    };


    //stage 2: generate sector table
    std::tuple<int2048_t, int2048_t, ECC::EllipticPoint<int2048_t> > sector_table[L];
    #ifdef DEBUG
        std::cout << "[!] Debug::ro_pollard_alg::partition_table" << std::endl;
    #endif
    for (int i = 0; i < L ; i++)
    {
        sector_table[i] = gen_triple();
        #ifdef DEBUG
            std::cout << i+1 << " " << std::get<0>(sector_table[i]) << " " << std::get<1>(sector_table[i]) << " " << std::get<2>(sector_table[i]) << std::endl;
        #endif
    }

    //std::tuple<int2048_t, int2048_t, ECC::EllipticPoint<int2048_t> > sector_table[L] = {std::make_tuple(79, 163, ECC::EllipticPoint<int2048_t>(135,117,229,1)), std::make_tuple(206, 19, ECC::EllipticPoint<int2048_t>(96,97,229,1)), std::make_tuple(87, 109, ECC::EllipticPoint<int2048_t>(84,62,229,1)), std::make_tuple(219, 68, ECC::EllipticPoint<int2048_t>(72,134,229,1))};
    //stage 3 - start generating probes
    int2048_t s, ss, d, dd;
    ECC::EllipticPoint<int2048_t> X;
    ECC::EllipticPoint<int2048_t> XX;
    std::tie(s,d,X) = gen_triple();
    //s = 54; d = 175; X = s*P + d*Q;
    ss = s; dd = d; XX = X;

    #ifdef DEBUG
        std::cout << "[!] DEBUG::ro_pollard_alg::start_params: s=" << s << " d=" << d <<" X="<<X << std::endl;
    #endif

    //stage 4 - generate probes, while X != XX
    int i = 0;
    for (;;)
    {
        int2048_t p,j;
        #ifdef DEBUG

            std::cout << std::setw(3) <<  i << std::setw(4) << p <<std::setw(4) << s << " " << std::setw(4) << d << " " <<std::setw(8) << X << " | " ;
            std::cout << std::setw(4) << j << std::setw(4) << ss << " " << std::setw(4) << dd << " " <<std::setw(8) << XX << std::endl; 

        #endif
        //1. get section for current state
        p = H(X.getX());

        X = X + std::get<2>( sector_table[(int)p] );

        s = Util::Math<int2048_t>::mod(s + std::get<0>( sector_table[(int)p] ), ec.getOrder());
        d = Util::Math<int2048_t>::mod(d + std::get<1>( sector_table[(int)p] ), ec.getOrder());

        
        for (int k = 0; k < 2; k++)
        {
            j = H(XX.getX());

            XX = XX + std::get<2>( sector_table[(int)j] );
            ss = Util::Math<int2048_t>::mod(ss + std::get<0>( sector_table[(int)j] ), ec.getOrder());
            dd = Util::Math<int2048_t>::mod(dd + std::get<1>( sector_table[(int)j] ), ec.getOrder());
            #ifdef DEBUG

            std::cout << std::setw(35) << " | " ;
            std::cout << std::setw(4) << j << std::setw(4) << ss << " " << std::setw(4) << dd << " " <<std::setw(8) << XX << std::endl; 

            #endif
            
        }
        i++;


        if (X.is_in_infinity() || XX.is_in_infinity())
        {
            return -1; // bad result
        }
    
        if (X == XX)
        {
            #ifdef DEBUG
            std::cout << "[!] DEBUG::ro_pollard_alg::FOUND(X,XX)" << std::endl;
            std::cout << std::setw(3) <<  i << std::setw(4) << s << " " << std::setw(4) << d << " " <<std::setw(8) << X << " | " ;
            std::cout << std::setw(4) << ss << " " << std::setw(4) << dd << " " <<std::setw(8) << XX << std::endl; 
            #endif

            break;
        }   
    }

    if (d == dd)
    {
        return -1; // bad result
    }

    //auto l = Util::Math<int2048_t>::mod( (s - ss) * ( Util::Math<int2048_t>::invert((d - dd), ec.getOrder() ) ) , ec.getOrder());
    auto ls = s-ss;
    auto ld = Util::Math<int2048_t>::invert((d - dd), ec.getOrder() ) ;
    auto ll = ls*ld;
    auto l = Util::Math<int2048_t>::mod( ll, ec.getOrder());
    #ifdef DEBUG
        std::cout << "[!] DEBUG::Found l=" << l << std::endl;
    #endif
    return l;
    //дописать самопроверку, в случае провала - выпасть в рекурсию
}


// do from P(x,y) to P(x,-y)
ECC::EllipticPoint<int2048_t> invert_point(ECC::EllipticPoint<int2048_t> P)
{
    return ECC::EllipticPoint<int2048_t>(P.getX(), -1*P.getY(), P.getModulo(), P.getCurveParam());
}

int2048_t lsbs_alg(ECC::EllipticPoint<int2048_t> P, ECC::EllipticPoint<int2048_t> Q, ECC::EllipticCurve<int2048_t> ec)
{
    std::vector< ECC::EllipticPoint<int2048_t> > ls; //little step
    std::vector< ECC::EllipticPoint<int2048_t> > bs; //big step

    // step 1: compute m = sqrt(n)
    auto m = boost::multiprecision::sqrt(ec.getOrder()) + 1;
    
    #ifdef DEBUG
        std::cout << "[!] DEBUG::lsbs_alg::m="<< m << std::endl;
    #endif

    // step 2: calculating mP
    auto mp = invert_point(m * P);

    for (int i = 0; i < m; i++)
    {
        ls.push_back(i*P);
    }

    for (int j = 0; j < m; j++)
    {
        bs.push_back(Q + j*mp);
    }

    #ifdef DEBUG
        std::cout << "[!] DEBUG::lsbs_alg::ls" << std::endl;
        for (auto key : ls)
        {
            std::cout << key << ":" << key.getModulo() << " ";
        }
        std::cout << std::endl;
        std::cout << "[!] DEBUG::lsbs_alg::bs" << std::endl;
        for (auto key : bs)
        {
            std::cout << key << ":" << key.getModulo() << " ";
        }
        std::cout << std::endl;
    #endif

    //step 3: find collision
    //O(m^2) :(
    for (int i = 0; i < ls.size(); i++)
    {
        for (int j = 0; j < bs.size(); j++)
        {
            if (ls[i].is_in_infinity() || bs[j].is_in_infinity())
            {
                continue;
            }

            if (ls[i] == bs[j])
            {
                auto k = Util::Math<int2048_t>::mod( i+j*m , ec.getOrder());
                #ifdef DEBUG
                    std::cout << "[!] DEBUG::lsbs_alg::FOUND i="<< i << " j=" << j << " => k=" << k<< std::endl;
                #endif
                return k;
            }
        }
    }
    return -1;
}

int main()
{
    ECC::EllipticCurve<int2048_t> ecc(0x4f5ba4fbbd460e801077062e5fb21d02fcc74f59_cppui,0xa423694fe0d5ec5a82e3e08d8e710d36aff68920_cppui,0x2d5112a6a0b6cb90615a977f7678d9573d1bbdc61_cppui);
    ecc.setOrder(0x2d5112a6a0b6cb90615aae22427483a5cdd40a3cf_cppui);
    ECC::EllipticPoint<int2048_t> a(0x21ce6ae72693c4d0fbb0023482d00efbcea5f778b_cppui,0x262be28883d823a380fea27d8a12aa1c61bbdbe34_cppui, 0x2d5112a6a0b6cb90615a977f7678d9573d1bbdc61_cppui, 0x4f5ba4fbbd460e801077062e5fb21d02fcc74f59_cppui);
    int n = 822;
    ECC::EllipticPoint<int2048_t> b(0x12ab24eea1e3f3c83cc910abed1b3bba2ddd5b5b7_cppui,0x284bcc533e4cdc01e98581d6987672825b3a430a6_cppui, 0x2d5112a6a0b6cb90615a977f7678d9573d1bbdc61_cppui, 0x4f5ba4fbbd460e801077062e5fb21d02fcc74f59_cppui);

    // for (;;)
    // {   try 
    //     {
    //         auto res = ro_pollard_alg(a,b,ecc);
    //         auto check = res*a;
    //         if (res != -1 && check == b)
    //         {
    //             std::cout << "Found, log=" << res << std::endl << a << "*" << res << "=" << b <<std::endl;
    //             break;
    //         }
    //     } catch (const std::string& err)
    //     {
    //         std::cout << "Error ocured: " << err << std::endl;
    //     }
    // }

    // ECC::EllipticCurve<int2048_t> ecc(0,734,829);
    // ecc.setOrder(784);
    // ECC::EllipticPoint<int2048_t> P(6,11,829,0);
    // ECC::EllipticPoint<int2048_t> Q = 34*P;//( 293, 12,829,0);
    std::cout << lsbs_alg(a, b, ecc) << std::endl;
    return 0;
}