#include <iostream>
#include <cmath>
#include "fixed/fixed.hpp"
#include "cordic.hpp"

using namespace std;

using CordicT = Cordic<Fixed<2, 30>>;
//using CordicT = Cordic<long double>;

int main() {
    const long double tet = M_PI / 6;
    CordicT cordic{.n=0, .x=1, .y=0, .z=tet};

    cout << cordic << endl;
    for (; cordic.n < 32 && abs((long double) cordic.z) > 0.001;) {
        cordic = cordic.step<CordicT::TYP_CIRCULAR, CordicT::MOD_ROTATION>();
        cout << cordic << endl;
    }

    cout << "tet: " << tet << " deg: " << tet * 180 / M_PI << endl;

    cout << "real   cos: " << cos(tet) << endl;
    cout << "cordic cos: " << ((long double) cordic.x) / CordicT::K << endl;

    cout << "real   sin: " << sin(tet) << endl;
    cout << "cordic sin: " << ((long double) cordic.y) / CordicT::K << endl;

    return 0;
}