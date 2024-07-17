#include "test/NonlinearEquationsTest.h"

int main() {

    BracketRootTest();

    RegulaFalsiSolveTest();
    RiddersSolveTest();
    NewtonRaphsonSolveTest();

    PolyRootsComplexTest();
    PolyRootsTest();

    return 0;
}
