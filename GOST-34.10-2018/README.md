# GOST-34.10-2018

## Compile

* Need boost

`g++ main.cpp sources/cpuinfo.cpp sources/streebog.cpp sources/crypto_hash.cpp sources/gost3411-2012-sse2.c sources/gost3411-2012-sse41.c -lboost_program_options -o ECC`

TODO: makefile ._.

## Usage
```
[~] GOST 34.10-2018
        Russian Eliptic Curve Digital Signature Algorithm.
        Implementation By George Zaytsev, 2020.
Options:
  -h [ --help ]          Show this menu
  -m [ --mode ] arg      Select mode: Create, Check, Genkeys
  -b [ --bits ] arg      Length of digital signature. Possible value: 256, 512
                         bits. 256 by default
  -c [ --curve ] arg     Select curve. gost3410_p256 by default.
  --list-curves          List available curves

Create Digital Signature options:
  -I [ --input ] arg     Input file for signing
  --private-key arg      Input file with private key
  --write-mode arg       Output mode: file, join.
                         [!] join mode concatenate bytes of digital signature
                         to input file.

Check Digital Signature options:
  -I [ --input ] arg     Input file for checking
  --public-key arg       Input file with public key
  --read-mode arg        Read mode: file, join.
                         [!] join mode read bytes of digital signature from
                         input file and erase them.
  --signature arg        Specify file with digital signature. For `join` read
                         mode.

Generate keys options:
  --public-key arg       Output directory for public key.
  --private-key arg      Output directory for private key.
  -n [ --name ] arg      Name for keypair without extension.
```

## Example

In `./keys` directory you can find keys examples.

- Generate keys

  `./ECC -m GenKeys --public-key /tmp/keys --private-key /tmp/keys -n key_secp -b 256 -c secp256k1 `

- Create sign

  `echo 'Hello world!' > /tmp/keys/hello.txt`

  `./ECC -m Create -b 256 -c secp256k1 --input /tmp/keys/hello.txt --private-key /tmp/keys/key_secp.gpriv --write-mode join`

- Check sign
  `./ECC -m Check -b 256 -c secp256k1 --public-key /tmp/keys/key_secp.gpub --input /tmp/keys/hello.txt --read-mode join`