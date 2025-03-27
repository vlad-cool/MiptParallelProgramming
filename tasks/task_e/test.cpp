#include <iostream>

#include "bigint.h"

int main()
{
    BigInt a(7777777);
    BigInt b(3333333);
    BigInt c = a + b;
    BigInt d = a * b;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    std::cout << d << std::endl;
    std::cout << a - b << std::endl;

    std::cout << (BigInt(8) >= BigInt(9)) << std::endl;
    std::cout << (BigInt(89) >= BigInt(9)) << std::endl;
}