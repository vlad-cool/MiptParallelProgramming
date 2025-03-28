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

    // BigInt a;
    // a = BigInt("1234567890123456789012345678901234567890123456789012345678901234567890");
    std::cout << "SADADSD\n";
    a = BigInt("9876543210");
    std::cout << a << std::endl;
    std::cout << a << std::endl;
    a = BigInt("111222333444555666777888999000");
    std::cout << a << std::endl;
    a = BigInt("98765432109876543210987654321098765432109876543210987654");
    std::cout << a << std::endl;
    a = BigInt("9876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210");
    std::cout << a << std::endl;
    std::cout << "NJKHGJFTRET\n";
    a = BigInt("987654321098765432109876543210987654321098765432109876543210");
    std::cout << a / 1203 << std::endl;
    std::cout << a % 1203 << std::endl;
    std::cout << a / 564655454 << std::endl;
    std::cout << a % 564655454 << std::endl;
}