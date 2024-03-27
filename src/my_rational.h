#pragma once

#include <cmath>
#include <numeric>
#include <string>

#include <ostream>

class MyRational {
  public:
    MyRational(std::string const &raw) {
        minus = raw.front() == '-';

        auto pos_raw = raw.substr(minus ? 1 : 0);
        if (auto itor_dot = pos_raw.find('.'); itor_dot != std::string::npos) {
            auto decimal_pow = std::pow(10, pos_raw.size() - itor_dot - 1);

            numerator = std::stol(pos_raw.substr(0, itor_dot)) * decimal_pow + std::stol(pos_raw.substr(itor_dot + 1));
            denominator = decimal_pow;
        } else if (auto itor_div = pos_raw.find('/'); itor_div != std::string::npos) {
            numerator   = std::stol(pos_raw.substr(0, itor_div));
            denominator = std::stol(pos_raw.substr(itor_div + 1));
        } else {
            numerator   = std::stol(pos_raw);
            denominator = 1;
        }
        simplify();
    }

    friend std::ostream &operator<<(std::ostream &os, MyRational const &obj) {
        os << (obj.minus ? "-" : "") << obj.numerator << " / " << obj.denominator;
        return os;
    }

  private:
    bool minus;
    long numerator, denominator;

    void simplify() {
        auto gcd = std::gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
    }
};
