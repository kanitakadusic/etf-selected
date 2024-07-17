#include "test/ChebyshevApproximationTest.h"

int main() {
    ChebyshevApproximationTest::constructorTest();

    ChebyshevApproximationTest::setMTest();
    ChebyshevApproximationTest::truncTest();

    ChebyshevApproximationTest::approximationTest();

    ChebyshevApproximationTest::derivativeDoubleTest();
    ChebyshevApproximationTest::derivativeTest();
    ChebyshevApproximationTest::antiderivativeTest();

    ChebyshevApproximationTest::integrateDoubleDoubleTest();
    ChebyshevApproximationTest::integrateTest();

    return 0;
}
