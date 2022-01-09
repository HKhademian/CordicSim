#include <iostream>
#include <cmath>

/// to use Fixed<m,n> use false or 0 or comment it
#define CORDIC_FLOAT false

#include "cordic.hpp"

#if CORDIC_FLOAT
using InnerT = long double; // float / double / long double
#else
// optional: just use float/double/long double types
#include "../fixedsim/fixed.hpp"

using InnerT = Fixed<2, 6>; // Fixed<5,9> Fixed<2, 6> Fixed<2, 14> Fixed<2, 30> Fixed<2, 62> ...
#endif

using CordicT = Cordic<InnerT>;

using namespace std;

CordicT run(int steps, bool log, const InnerT &tet) {
    CordicT cordic{.n=0, .x=CordicT::Kinv, .y=0, .z=tet};

    if (log) cout << cordic << endl;
    for (auto i = 0; (steps <= 0 && cordic.n < 32 && abs((long double) cordic.z) > 0.0001) || i < steps; ++i) {
        cordic = cordic.step<CordicT::TYP_CIRCULAR, CordicT::MOD_ROTATION>();
        if (log) cout << cordic << endl;
    }

    return cordic;
}

void print(const CordicT &cordic, const InnerT &tet) {
    const auto realC = cos((long double) tet);
    const auto realS = sin((long double) tet);
    const auto cordicC = ((long double) cordic.x);
    const auto cordicS = ((long double) cordic.y);
    cout << "tet: " << ((long double) tet) << " deg: " << ((long double) tet) * 180 / M_PI << endl;
    // cout << "real   cos: " << realC << endl;
    // cout << "cordic cos: " << cordicC << endl;
    cout << "acc cos: " << 100 - abs(realC - cordicC) * 100 << endl;
    // cout << "real   sin: " << realS << endl;
    // cout << "cordic sin: " << cordicS << endl;
    cout << "acc sin: " << 100 - abs(realS - cordicS) * 100 << endl;
    cout << endl;
}

void test(int step, bool log, const InnerT &tet) {
    auto cordic = run(step, log, tet);
    print(cordic, tet);
}

int main() {
    /* to find how many steps is needed to reach
     * required precision */
//    test(0, true, M_PI / 6), exit(0);

    const InnerT tets[] = {
            0, M_PI / 12, M_PI / 6, M_PI / 4, M_PI / 2,
    };

    for (const auto &tet : tets) {
        test(13, false, tet);
    }
    return 0;
}
