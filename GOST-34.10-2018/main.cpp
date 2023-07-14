#include <iostream>

#include <string>
#include "./include/GOST_34-10-2018.hpp"

#include <boost/program_options.hpp>

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;


std::map< std::string, ECC::EllipticCurve<int2048_t> > standart_curves = {
    {"gost3410_256p", ECC::StandartCurves::gost3410_p256},
    {"gost3410_512p", ECC::StandartCurves::gost3410_p512},
    {"secp256k1", ECC::StandartCurves::secp256k1}
};


void create(const boost::program_options::variables_map& vm, ECC::EllipticCurve<int2048_t> curve, int bits);
void check(const boost::program_options::variables_map& vm, ECC::EllipticCurve<int2048_t> curve, int bits);
void genkeys(const boost::program_options::variables_map& vm, ECC::EllipticCurve<int2048_t> curve, int bits);
void list_curves();

int main(int argc, const char* argv[]){

    std::string mode; // mode - create sign, check sign, gen keys
    try
    {
        boost::program_options::options_description desc{"[~] GOST 34.10-2018\n\tRussian Eliptic Curve Digital Signature Algorithm.\n\tImplementation By George Zaytsev, 2020.\nOptions"};
        desc.add_options()
            ("help,h", "Show this menu")
            ("mode,m", boost::program_options::value<std::string>(&mode), "Select mode: Create, Check, Genkeys")
            ("bits,b", boost::program_options::value<int>(), "Length of digital signature. Possible value: 256, 512 bits. 256 by default")
            ("curve,c", boost::program_options::value<std::string>(), "Select curve. gost3410_p256 by default.")
            ("list-curves", "List available curves");


        boost::program_options::options_description create_desc{"Create Digital Signature options"};
        create_desc.add_options()
            ("input,I", boost::program_options::value<std::string>(), "Input file for signing")
            ("private-key", boost::program_options::value<std::string>(), "Input file with private key")
            ("write-mode", boost::program_options::value<std::string>(), "Output mode: file, join.\n\t[!] join mode concatenate bytes of digital signature to input file.");


        boost::program_options::options_description check_desc{"Check Digital Signature options"};
        check_desc.add_options()
            ("input,I", boost::program_options::value<std::string>(), "Input file for checking")
            ("public-key", boost::program_options::value<std::string>(), "Input file with public key")
            ("read-mode", boost::program_options::value<std::string>(), "Read mode: file, join.\n\t[!] join mode read bytes of digital signature from input file and erase them.")
            ("signature", boost::program_options::value<std::string>(), "Specify file with digital signature. For `join` read mode.");

        boost::program_options::options_description genkey_desc{"Generate keys options"};
        genkey_desc.add_options()
            ("public-key", boost::program_options::value<std::string>(), "Output directory for public key.")
            ("private-key", boost::program_options::value<std::string>(), "Output directory for private key.")
            ("name,n", boost::program_options::value<std::string>(), "Name for keypair without extension.");
        

        boost::program_options::variables_map vm;
        boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc,argv).options(desc).allow_unregistered().run();
        boost::program_options::store(parsed, vm);
        boost::program_options::notify(vm);

        if (vm.count("list-curves"))
        {
            list_curves();
            return 0;
        }
        int bits = 0;
        if (vm.count("bits"))
        {
            int value = vm["bits"].as<int>();
            bits = (value == 256||value == 512) ? value : 256;
        } 
        else 
        {
            bits = 256;
        }

        ECC::EllipticCurve<int2048_t> curve;
        if (vm.count("curve"))
        {
            curve = standart_curves[vm["curve"].as<std::string>()];
        } else {
            curve = ECC::StandartCurves::gost3410_p256;
            bits = 256;
        }
        
        try {
            if (mode == "Create")
            {
                desc.add(create_desc);
                boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc), vm);
                create(vm, curve, bits);
            }
            else if (mode == "Check")
            {
                desc.add(check_desc);
                boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc), vm);
                check(vm, curve, bits);
            }
            else if (mode == "GenKeys")
            {
                desc.add(genkey_desc);
                boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc), vm);
                genkeys(vm, curve, bits);
            }
            else 
            {
                desc.add(create_desc).add(check_desc).add(genkey_desc);
                std::cout<<desc<<std::endl;
                std::cout << "Bad mode. Possible values: [Create|Check|GenKeys]" << std::endl;
            }
        } catch (const std::string& ex) {
            std::cout << ex << std::endl;
        }

    }
    catch (const boost::program_options::error &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}

void create(const boost::program_options::variables_map& vm, ECC::EllipticCurve<int2048_t> curve, int bits)
{
    std::string input_file, input_key;
    try{
        if (vm.count("input"))
        {
            input_file = vm["input"].as<std::string>();
        } 
        else
        {
            throw "Must specify input file.";
        }
        if (vm.count("private-key"))
        {
            input_key = vm["private-key"].as<std::string>();
        } 
        else
        {
            throw "Must specify file with private key.";
        }   
    } catch (const std::string& ex) {
        std::cerr << ex << std::endl;
        return;
    }
    std::string data = Util::readFile(input_file);
    int2048_t key;
    if (bits == 256)
        key = Util::readData_256b(input_key);
    else
        key = Util::readData_512b(input_key);

    GOST_34_10_2018::Types::Sign<int2048_t> sign = GOST_34_10_2018::Algorithms::createSign(data, GOST_34_10_2018::Types::PrivateKey<int2048_t>(key), curve, bits);

    std::string mode;
    if (vm.count("write-mode"))
    {
        mode = vm["write-mode"].as<std::string>();
    }
    else 
    {
        mode = "file";
    }

    if (mode == "file")
    {
        std::string filename = input_file+".gsign";
        if (bits == 256)
        {
            Util::writeData_256b(sign.getR(), filename);
            Util::writeData_256b(sign.getS(), filename, bits/8);
        } 
        else 
        {
            Util::writeData_512b(sign.getR(), filename);
            Util::writeData_512b(sign.getS(), filename, bits/8);
        }
        std::cout << "[+] Sign Created. File with digital signature - " << filename << std::endl;
    } 
    else if (mode == "join")
    {
        std::ifstream in(input_file, std::ios::ate | std::ios::binary);
        int sz = in.tellg();
        in.close();
        if (bits == 256)
        {
            Util::writeData_256b(sign.getR(), input_file, sz);
            Util::writeData_256b(sign.getS(), input_file, sz+bits/8);
        } else {
            Util::writeData_512b(sign.getR(), input_file, sz);
            Util::writeData_512b(sign.getS(), input_file, sz+bits/8);
        }
        std::fstream in_end(input_file, std::ios::in |std::ios::out | std::ios::ate | std::ios::binary);
        in_end.write("gost", 4);
        in_end.close();
        std::cout << "[+] Sign Created. Digital signature stored if input file. Last " << std::dec <<4+2*bits/8 << " bytes."<< std::endl;
    }
}


void check(const boost::program_options::variables_map& vm, ECC::EllipticCurve<int2048_t> curve, int bits)
{
    std::string input_file, input_key;
    try {
        if (vm.count("input"))
        {
            input_file = vm["input"].as<std::string>();
        } 
        else
        {
            throw "Must specify input file.";
        }
        if (vm.count("public-key"))
        {
            input_key = vm["public-key"].as<std::string>();
        } 
        else
        {
            throw "Must specify file with private key.";
        }
    } catch (const std::string& ex) {
        std::cerr << ex << std::endl;
        return;
    }

    // we need to read signature+'gost' and erase them from input file.
    std::string mode;
    if (vm.count("read-mode"))
    {
        mode = vm["read-mode"].as<std::string>();
    }
    else 
    {
        mode = "file";
    }

    if (mode == "join")
    {
        std::string data = Util::readFile(input_file);
        int sz = data.size();
        #ifdef DEBUG
        std::cout << "[!] DEBUG. Readed file len="<<std::dec<<sz<<std::endl;
        #endif
        bool flag_is_gost_signature = false;
        if (data.substr(data.size()-4) == "gost")
            flag_is_gost_signature = true;
        else
            std::cout << "[!] Doesn't found token of signature." << std::endl;
        #ifdef DEBUG
            std::cout<< "[!] DEBUG (check-read): Last 4 bytes of file: " << data.substr(data.size()-4) << std::endl; 
        #endif
        data.erase(data.size()-4-2*(bits/8)); //delete last 68/132 bytes of signature

        int2048_t r,s,qx,qy;
        if (bits == 256)
        {
            r  = Util::readData_256b(input_file, sz-4-2*bits/8);
            s  = Util::readData_256b(input_file, sz-4-bits/8);
            qx = Util::readData_256b(input_key, 0);
            qy = Util::readData_256b(input_key, bits/8);
        } else {
            r  = Util::readData_512b(input_file, sz-4-2*bits/8);
            s  = Util::readData_512b(input_file, sz-4-bits/8);\
            qx = Util::readData_512b(input_key, 0);
            qy = Util::readData_512b(input_key, bits/8);
        }
        #ifdef DEBUG
        std::cout << "[!] DEBUG (check) Readed:\nr=0x"<<std::hex<<r<<"\ns=0x"<<s<<"\nQx=0x"<<qx<<"\nQy=0x"<<qy<<std::endl;
        #endif
        
        GOST_34_10_2018::Types::Sign<int2048_t> sign(r,s);
        
        ECC::EllipticPoint<int2048_t> Q(qx,qy, curve.getModulo(), curve.get_a());
        
        GOST_34_10_2018::Types::PublicKey<int2048_t> pubkey(Q);
        
        if (flag_is_gost_signature && GOST_34_10_2018::Algorithms::checkSign(data, sign, pubkey, curve, bits))
        {
            std::cout << "[+] Digital signature is correct." << std::endl;
             //now delete sig
            std::ofstream new_data(input_file, std::ios::binary);
            new_data << data;
            new_data.close();
        } 
        else
        {
            std::cout << "[-] Digital signature is incorrect." << std::endl;
        }

    }
    else
    {
        std::string sig;
        if (vm.count("signature"))
        {
            sig = vm["signature"].as<std::string>();
        }
        else 
        {
            std::cerr << "Please specify file with signature." <<std::endl;
            return;
        }

        std::string data = Util::readFile(input_file);
        #ifdef DEBUG
        std::cout << "[!] DEBUG. Readed file len="<<std::dec<<data.size()<<std::endl;
        #endif

        int2048_t r,s,qx,qy;
        if (bits == 256)
        {
            r  = Util::readData_256b(sig, 0);
            s  = Util::readData_256b(sig, bits/8);
            qx = Util::readData_256b(input_key, 0);
            qy = Util::readData_256b(input_key, bits/8);
        } else {
            r  = Util::readData_512b(sig, 0);
            s  = Util::readData_512b(sig, bits/8);
            qx = Util::readData_512b(input_key, 0);
            qy = Util::readData_512b(input_key, bits/8);
        }
        #ifdef DEBUG
        std::cout << "[!] DEBUG (check) Readed:\nr=0x"<<std::hex<<r<<"\ns=0x"<<s<<"\nQx=0x"<<qx<<"\nQy=0x"<<qy<<std::endl;
        #endif
        GOST_34_10_2018::Types::Sign<int2048_t> sign(r,s);
        ECC::EllipticPoint<int2048_t> Q(qx,qy, curve.getModulo(), curve.get_a());
        GOST_34_10_2018::Types::PublicKey<int2048_t> pubkey(Q);
        if (GOST_34_10_2018::Algorithms::checkSign(data, sign, pubkey, curve, bits))
        {
            std::cout << "[+] Digital signature is correct." << std::endl;
        } 
        else
        {
            std::cout << "[-] Digital signature is incorrect." << std::endl;
        }
    }
}


void genkeys(const boost::program_options::variables_map& vm, ECC::EllipticCurve<int2048_t> curve, int bits)
{
    // Step1. Generate file names
    std::string input_pub="", input_priv="";
    if (vm.count("public-key"))
    {
        input_pub = vm["public-key"].as<std::string>();       
    } 

    if (vm.count("private-key"))
    {
        input_priv = vm["private-key"].as<std::string>();       
    }

    if (vm.count("name"))
    {
        input_pub  = input_pub + (input_pub.back() == '/' ? "":"/") + vm["name"].as<std::string>() + ".gpub";
        input_priv = input_priv + (input_priv.back() == '/' ? "":"/") + vm["name"].as<std::string>() + ".gpriv";
    } 
    else
    {
        input_pub = input_pub   + "key.gpub";
        input_priv = input_priv + "key.gpriv";
    } 
#ifdef DEBUG
    std::cout << "[!] DEBUG (GenKey):\n"<<input_pub << std::endl << input_priv << std::endl;
#endif

    // Step2. Generate keys!
    std::pair< GOST_34_10_2018::Types::PublicKey<int2048_t>, GOST_34_10_2018::Types::PrivateKey<int2048_t> > keys = GOST_34_10_2018::Algorithms::genKeys(curve, bits);
    if (bits == 256){
        Util::writeData_256b(keys.second.getKey(),input_priv);
        Util::writeData_256b(keys.first.getKey().getX(), input_pub);
        Util::writeData_256b(keys.first.getKey().getY(), input_pub, bits/8);
        #ifdef DEBUG
        std::cout << "[!] DEBUG (GenKey). Files Created." << std::endl;
        #endif
    }
    else
    {
        Util::writeData_512b(keys.second.getKey(),input_priv);
        Util::writeData_512b(keys.first.getKey().getX(), input_pub);
        Util::writeData_512b(keys.first.getKey().getY(), input_pub, bits/8);
        #ifdef DEBUG
        std::cout << "[!] DEBUG (GenKey). Files Created." << std::endl;
        #endif
    }

    std::cout << "[+] Keys created:\n"<<input_pub << std::endl << input_priv << std::endl;
}

void list_curves()
{
    for(auto n: standart_curves)
        std::cout << n.first << std::endl;
}
