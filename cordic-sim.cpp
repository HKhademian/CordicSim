// clear && g++ -o cordic cordic-sim.cpp && ./cordic && rm ./cordic
#include <iostream>
#include <cmath>

/// to use Fixed<m,n> use false or 0 or comment it
#define CORDIC_FLOAT false

#include "cordic.hpp"

#if !CORDIC_FLOAT

#include "../fixedsim/fixed.hpp"

#endif

using namespace std;

template<typename DataT>
Cordic<DataT> run(int steps, bool log, long double tet) {
    using CordicT = Cordic<DataT>;
    CordicT cordic{.n=0, .x=CordicT::Kinv, .y=0, .z=tet};

    if (log) cout << cordic << endl;
    for (auto i = 0; (steps <= 0 && cordic.n < 32 && abs((long double) cordic.z) > 0.0001) || i < steps; ++i) {
        cordic = CordicT::step(cordic, CordicT::TYP_CIRCULAR, CordicT::MOD_ROTATION);
        if (log) cout << cordic << endl;
    }

    return cordic;
}

template<typename DataT>
void print(const Cordic<DataT> &cordic, long double tet) {
    const auto cordicTet = (long double) DataT(tet);
    const auto targetC = cos(tet);
    const auto targetS = sin(tet);
    const auto cordicC = (long double) cordic.x;
    const auto cordicS = (long double) cordic.y;
    const auto realC = cos(cordicTet);
    const auto realS = sin(cordicTet);

    // cordic result accuracy can affect by input angle accuracy (data type accuracy)
    // so there is two acc one for target (according to input teta)
    // and one for real (teta holds by cordic machine input type)
    // in floating-point systems , it most be equal but in fixed point systems
    // specially in low bit counts (fractional part) there might be difference

    cout << " deg: " << (tet * 180 / M_PI)
         << " tet: " << tet
         << " cordic tet: " << cordicTet
         << endl;
    cout << "acc tet: " << 100 - abs(tet - cordicTet) / M_PI * 100 << endl;
    // cout << "target   cos: " << targetC << endl;
    // cout << "cordic cos: " << cordicC << endl;
    // cout << "real   cos: " << targetC << endl;
    cout << "acc real/cordic   cos: " << 100 - abs(realC - cordicC) * 100 << endl;
    cout << "acc target/cordic cos: " << 100 - abs(targetC - cordicC) * 100 << endl;
    // cout << "target   sin: " << targetC << endl;
    // cout << "cordic sin: " << cordicS << endl;
    // cout << "real   sin: " << targetC << endl;
    cout << "acc real/cordic   sin: " << 100 - abs(realS - cordicS) * 100 << endl;
    cout << "acc target/cordic sin: " << 100 - abs(targetS - cordicS) * 100 << endl;
    cout << endl;
}

template<typename DataT>
Cordic<DataT> test(int step, bool log, long double tet) {
    auto cordic = run<DataT>(step, log, tet);
    cout << "Steps: " << step;
    print<DataT>(cordic, tet);
    return cordic;
}

int main() {
#if CORDIC_FLOAT
    using InnerT = float; // float / double / long double
#else
    using InnerT = Fixed<3, 5>; // Fixed<5,9> Fixed<3, 5> Fixed<3, 13> Fixed<3, 29> Fixed<3, 61> ...
#endif
    using CordicT = Cordic<InnerT>;

    cout << "InnerT: " << (InnerT) 0 << endl;
    cout << "K: " << (InnerT) CordicT::K << endl;
    cout << "1/K: " << (InnerT) CordicT::Kinv << endl;
    cout << endl;

    /* to find how many steps is needed to reach required precision */
    const auto cordic = test<InnerT>(0, true, M_PI / 6);
    // exit(0);

    const long double tets[] = {
            0, M_PI / 12, M_PI / 6, M_PI / 4, M_PI / 2,
    };

    for (const auto &tet : tets) {
        test<InnerT>(1 + cordic.n, false, tet);
    }
    return 0;
}
