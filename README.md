# kkso Edu Artefacts

I collected here my several works from some courses of higher education for [Specialist Degree](https://studyinrussia.ru/en/study-in-russia/info/levels-of-education/#:~:text=Master%27s%20Degree%20course.-,Specialist%20Degree,-Unlike%20Bachelor%27s%20Degrees) in Computer Security. Maybe it can be useful for someone :)

In this repo you can find a bunch of C++ code using of boost lib and gtest for tests. *Warning: In some folders code is garbage and was written as proof of concept rather than a completed programming masterpiece.*

## ShadowBreaker

Subject: Operating Systems

A small program for brute force hash passwords in the format of "/etc/shadow".

[ReadMe](ShadowBreaker/README.md)

## Brainfuck-Parser

Subject: Programming languages theory

A simple Abstract Syntax Tree generator for esotheric [Brainfuck](https://ru.wikipedia.org/wiki/Brainfuck) language.

[ReadMe](Brainfuck-Parser/README.md)

## java_labs_kks, cpp_labs_kks

Subject: Data Structures and Algorithms

A bunch of works for studying the CS theory.

- [ReadMe C++ (redacted list)](cpp_labs_kks/README.md)
- [Readme Java](java_labs_kks/README.md)

## GOST-34.10-2018

Subject: Cryptographic methods in field of Information Security

Implementation of Russian Eliptic Curve Digital Signature Algorithm (GOST R 34.10 2018).

\* SubRepo also contains a third-party implementation of the algorithm GOST R 34.11 (Streebog Hash Function).

[ReadMe](GOST-34.10-2018/)

## ECDL-set

Subject: Cryptographic methods in field of Information Security

Implementation of two ECDLP attacks:
- Pollard's rho algorithm
- Baby-step giant-step algorithm

[ReadMe](ECDL-set/README.md)

## crypto_protocols_labs_kks

Subject: Cryptographic methods in field of Information Security / Cryptographic Protocols

Implementation of:
- GOST R 34.12 2018 64bit Block Cipher in CBC mode. (also known as The GOST Block Cipher (Magma)).
- Dual EC Pseudo-Random Number Generator with security patches.

*Maybe someday will add working implementation of CRISP protocol D:*

[ReadMe](crypto_protocols_labs_kks/README.md)