#include <iostream>
#include <cmath>

/// to use Fixed<m,n> use zero or comment it
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

CordicT run(int steps, const InnerT &tet) {
    CordicT cordic{.n=0, .x=1, .y=0, .z=tet};

    cout << cordic << endl;
    for (auto i = 0; (steps <= 0 && cordic.n < 32 && abs((long double) cordic.z) > 0.00001) || i < steps; ++i) {
        cordic = cordic.step<CordicT::TYP_CIRCULAR, CordicT::MOD_ROTATION>();
        cout << cordic << endl;
    }

    return cordic;
}

void log(const CordicT &cordic, const InnerT &tet) {
    cout << "tet: " << ((long double) tet) << " deg: " << ((long double) tet) * 180 / M_PI << endl;
    cout << "real   cos: " << cos((long double) tet) << endl;
    cout << "cordic cos: " << ((long double) cordic.x) / CordicT::K << endl;
    cout << "real   sin: " << sin((long double) tet) << endl;
    cout << "cordic sin: " << ((long double) cordic.y) / CordicT::K << endl;
    cout << endl;
}

int main() {
    /* to find how many steps is needed to reach
     * required precision */
    log(run(0, M_PI / 6), M_PI / 6);
    exit(0);

    const InnerT tets[] = {
            0, M_PI / 12, M_PI / 6, M_PI / 4,
            M_PI / 2, 3 * M_PI / 4, M_PI,
    };

    for (const auto &tet : tets) {
        auto cordic = run(14, tet);
        log(cordic, tet);
    }
    return 0;
}
