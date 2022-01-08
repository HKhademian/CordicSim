#pragma once

#include <iostream>
#include <cmath>

template<typename INNER_T>
class Cordic {
#define self (*this)
public:
    constexpr static const long double K = 1.6467603;
    static const int TYP_CIRCULAR = 1;
    static const int TYP_LINEAR = 0;
    static const int TYP_HYPERBOLIC = -1;
    static const int MOD_ROTATION = 0;
    static const int MOD_VECTORING = 1;

    int n = 0;
    INNER_T x = 0;
    INNER_T y = 0;
    INNER_T z = 0;

    template<int typ, int mod>
    static Cordic step(const Cordic &from) {
        const auto n = from.n;
        const auto x = (long double) from.x;
        const auto y = (long double) from.y;
        const auto z = (long double) from.z;

        const auto m = typ;
        const auto d = (mod == MOD_ROTATION) ? (z >= 0 ? 1 : -1) : /* rotation mode */
                       (mod == MOD_VECTORING) ? (z >= 0 ? 1 : -1) : /* vectoring mode */
                       (exit(1), 0);// unreachable
        const auto k = 0; // FIXME: not correct in hyperbolic mode
        const auto sig = n - k;
        const auto p2sig = pow((long double) 2, -sig);
        const auto w = typ == TYP_CIRCULAR ? atan(p2sig) : // circular type
                       typ == TYP_LINEAR ? p2sig : // linear type
                       typ == TYP_HYPERBOLIC ? atanh(p2sig) : // hyperbolic type
                       (exit(1), 0); // unreachable

        return {
                .n = n + 1,
                .x = (x - m * d * y * p2sig),
                .y = (y + d * x * p2sig),
                .z = (z - d * w),
        };
    }

    /**
     * iterate on cordic series
     * @tparam typ 1: circular 0: linear -1: hyperbolic
     * @tparam mod 0: rotation 1: vectoring
     * @return next step of cordic
     */
    template<int typ, int mod>
    Cordic step() {
        return step<typ, mod>(self);
        /* original multiplier free algorithm : circular rotation
        const auto p2n = pow((long double) 2, -n);
        const INNER_T arc = atan(p2n);
        return {
                .n = n + 1,
                .x = z >= 0 ? (x - (y >>n)) : (x + (y >>n)),
                .y = z >= 0 ? (y + (x >>n)) : (y - (x >>n)),
                .z = z >= 0 ? (y - arc) : (y + arc),
        };
        */
    }

    Cordic &operator=(const Cordic &c) {
        n = c.n;
        x = c.x;
        y = c.y;
        z = c.z;
    }

    friend std::ostream &operator<<(std::ostream &os, const Cordic &cordic) {
        return os << "Cordic[n=" << cordic.n
                  << "](x:" << (long double) cordic.x
                  << ",y:" << (long double) cordic.y
                  << ",z:" << (long double) cordic.z
                  << ")";
    }

#undef self
};
