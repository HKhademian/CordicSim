#include <iostream>
#include <cmath>
#include "fixed/fixed.hpp"
#include "cordic.hpp"

using namespace std;

using InnerT = Fixed<2, 14>;
//using InnerT = Fixed<2, 30>;
//using InnerT = long double;
using CordicT = Cordic<InnerT>;

CordicT run(int steps, InnerT tet) {
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