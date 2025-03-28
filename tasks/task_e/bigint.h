#include <iostream>
#include <vector>
#include <iomanip>

#define NUMBER_LIMIT 1000000000
#define NUMBER_LIMIT_LOG 9

class BigInt
{
private:
    std::vector<unsigned int> digits;

    void trim()
    {
        while (digits.size() > 0 && digits.back() == 0)
        {
            digits.pop_back();
        }
    }

    static unsigned int count_digits(unsigned int n)
    {
        unsigned int res = 0;
        while (n > 0)
        {
            res++;
            n /= 10;
        }
        return res;
    }

public:
    BigInt() {}
    BigInt(unsigned long long n)
    {
        digits.push_back(n);
    }

    BigInt(const std::string s)
    {
        // std::cout << "| " << s << " |" << std::endl;
        int n = s.size();
        digits.resize((n + NUMBER_LIMIT_LOG - 1) / NUMBER_LIMIT_LOG);
        std::string str_num = std::string(digits.size() * NUMBER_LIMIT_LOG - n, '0') + s;
        n = str_num.size();
        for (int i = 0; i < digits.size(); i++)
        {
            std::string slice = str_num.substr(n - NUMBER_LIMIT_LOG * (i + 1), NUMBER_LIMIT_LOG);
            // std::cout << slice << std::endl;
            digits[i] = std::stoi(slice);
        }
    }

    BigInt(const unsigned int *buf, size_t size)
    {
        digits = std::vector<unsigned int>(buf, buf + size);
    }

    size_t get_size()
    {
        return digits.size();
    }

    unsigned int *get_data()
    {
        return digits.data();
    }

    BigInt divide_power_10(unsigned int divider) const
    {
        BigInt res = *this;
        if (divider > digits.size() * (NUMBER_LIMIT_LOG - 1) + count_digits(digits.back()))
        {
            return BigInt(0);
        }
        res.digits.erase(res.digits.begin(), res.digits.begin() + divider / NUMBER_LIMIT_LOG);
        res.digits.push_back(0);
        divider %= NUMBER_LIMIT_LOG;
        unsigned int div_pow = 1;
        for (int i = 0; i < divider; i++)
        {
            div_pow *= 10;
        }
        // std::cout << "DivPow " << div_pow << std::endl;
        for (int i = 0; i < res.digits.size() - 1; i++)
        {
            res.digits[i] = res.digits[i] / div_pow + (res.digits[i + 1] % div_pow) * (NUMBER_LIMIT / div_pow);
        }
        res.trim();
        return res;
    }

    BigInt operator+(const BigInt &other) const
    {
        BigInt result;
        int carry = 0;
        size_t i = 0;

        for (; i < digits.size() && i < other.digits.size(); ++i)
        {
            long long sum = (unsigned long long)digits[i] + other.digits[i] + carry;
            carry = sum / NUMBER_LIMIT;
            result.digits.push_back(sum % NUMBER_LIMIT);
        }

        for (; i < digits.size(); ++i)
        {
            long long sum = (unsigned long long)digits[i] + carry;
            carry = sum / NUMBER_LIMIT;
            result.digits.push_back(sum % NUMBER_LIMIT);
        }

        for (; i < other.digits.size(); ++i)
        {
            long long sum = (unsigned long long)other.digits[i] + carry;
            carry = sum / NUMBER_LIMIT;
            result.digits.push_back(sum % NUMBER_LIMIT);
        }

        if (carry > 0)
        {
            result.digits.push_back(carry);
        }

        return result;
    }

    BigInt operator-(const BigInt &other) const
    {
        BigInt inverted;
        BigInt result;
        inverted.digits.resize(other.digits.size());
        for (int i = 0; i < other.digits.size(); i++)
        {
            inverted.digits[i] = NUMBER_LIMIT - 1 - other.digits[i];
        }
        result = *this + inverted + BigInt(1);
        result.digits.pop_back();
        return result;
    }

    BigInt operator*(const BigInt &other) const
    {
        BigInt result;
        result.digits.resize(this->digits.size() + other.digits.size(), 0);

        for (size_t i = 0; i < digits.size(); ++i)
        {
            for (size_t j = 0; j < other.digits.size(); j++)
            {
                unsigned long long mul = (unsigned long long)digits[i] * other.digits[j];
                int k = i + j;
                while (mul != 0)
                {
                    result.digits[k] += mul % NUMBER_LIMIT;
                    int c = 0;
                    while (result.digits[k + c] >= NUMBER_LIMIT)
                    {
                        result.digits[k + c + 1] += result.digits[k + c] / NUMBER_LIMIT;
                        result.digits[k] %= NUMBER_LIMIT;
                        c++;
                    }
                    k++;
                    mul /= NUMBER_LIMIT;
                }
            }
        }
        result.trim();
        return result;
    }

    BigInt operator>=(const BigInt &other) const
    {
        if (digits.size() > other.digits.size())
        {
            return true;
        }
        else if (digits.size() < other.digits.size())
        {
            return false;
        }
        else
        {
            int n = digits.size();
            for (int i = n - 1; i >= 0; i--)
            {
                if (digits[i] > other.digits[i])
                {
                    return true;
                }
                else if (digits[i] < other.digits[i])
                {
                    return false;
                }
            }
            return true;
        }
    }

    BigInt operator/(unsigned int divider) const
    {
        unsigned long long remainder = 0;

        BigInt res;
        res.digits.resize(digits.size());

        for (int i = digits.size() - 1; i >= 0; i--)
        {
            res.digits[i] = (remainder * NUMBER_LIMIT + digits[i]) / divider;
            remainder = (remainder * NUMBER_LIMIT + digits[i]) % divider;
        }

        res.trim();
        return res;
    }

    unsigned int operator%(unsigned int divider) const
    {
        unsigned long long remainder = 0;

        BigInt res;
        res.digits.resize(digits.size());

        for (int i = digits.size() - 1; i >= 0; i--)
        {
            res.digits[i] = ((unsigned long long)remainder * NUMBER_LIMIT + digits[i]) / divider;
            remainder = ((unsigned long long)remainder * NUMBER_LIMIT + digits[i]) % divider;
        }

        return remainder;
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &obj);
};

std::ostream &operator<<(std::ostream &os, const BigInt &obj)
{
    if (obj.digits.size() == 0)
    {
        os << 0;
        return os;
    }
    os << obj.digits.back();
    for (auto i = obj.digits.rbegin() + 1; i != obj.digits.rend(); i++)
    {
        os << std::setw(NUMBER_LIMIT_LOG) << std::setfill('0') << *i;
    }
    return os;
}
