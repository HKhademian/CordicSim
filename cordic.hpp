#pragma once

#include <iostream>
#include <cmath>

/**
 * this class holds a step of cordic machine.
 * you can use it to simulate real implementation
 * to determine (with INNER_T precision) how many steps
 * (pipelined or resource shared) to reach required constraint
 *
 * @tparam INNER_T numeral type which handles all data precision ops.
 * look fo Fixed<m,n> for more
 */
template<typename INNER_T>
class Cordic {
#define self (*this)
public:
    /// https://en.wikipedia.org/wiki/CORDIC
    constexpr static const long double K = 1.64676025812107;
    constexpr static const long double Kinv = 0.6072529350088812561694;
    static const int TYP_CIRCULAR = 1;
    static const int TYP_LINEAR = 0;
    static const int TYP_HYPERBOLIC = -1;
    static const int MOD_ROTATION = 0;
    static const int MOD_VECTORING = 1;

    int n = 0;
    INNER_T x = 0;
    INNER_T y = 0;
    INNER_T z = 0;

    /**
     * multiplier free cordic step implementation
     * iterate on cordic series
     * @tparam typ 1: circular 0: linear -1: hyperbolic
     * @tparam mod 0: rotation 1: vectoring
     * @return next step of cordic
     */
    template<int typ, int mod>
    static Cordic step(const Cordic &from) {
        const auto n = from.n;
        const auto x = from.x;
        const auto y = from.y;
        const auto z = from.z;

        const auto k = 0; // FIXME: not correct in hyperbolic mode, see textbook
        const auto sig = n - k;
        const auto p2sig = powl(2, -sig);
        const auto w = typ == TYP_CIRCULAR ? atan(p2sig) :
                       typ == TYP_LINEAR ? p2sig :
                       typ == TYP_HYPERBOLIC ? atanh(p2sig) :
                       (exit(1), 0); // unreachable
        const auto d = (mod == MOD_ROTATION) ? (z >= 0 ? 1 : -1) :
                       (mod == MOD_VECTORING) ? (z >= 0 ? -1 : +1) :
                       (exit(1), 0);// unreachable
        const auto m = typ;

        INNER_T zdiff = w;
#if CORDIC_FLOAT
        // FIXME: (INNER_T=double) cannot use shr(n) so use /powl(2,n)
        INNER_T xdiff = y * p2sig;
        INNER_T ydiff = x * p2sig;
#else
        INNER_T xdiff = y >> n;
        INNER_T ydiff = x >> n;
#endif

        if (m == 0) xdiff = 0;
        else if (m == -1) xdiff = -xdiff;

        if (d == -1) {
            xdiff = -xdiff;
            ydiff = -ydiff;
            zdiff = -zdiff;
        }

        return {
                .n = n + 1,
                .x = x - xdiff,
                .y = y + ydiff,
                .z = z - zdiff,
        };
    }

    template<int typ, int mod>
    Cordic step() { return step<typ, mod>(self); }

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
