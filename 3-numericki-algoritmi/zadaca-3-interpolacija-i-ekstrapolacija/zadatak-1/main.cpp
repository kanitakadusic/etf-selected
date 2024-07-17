#include "test/LinearInterpolatorTest.h"
#include "test/PolynomialInterpolatorTest.h"
#include "test/PiecewisePolynomialInterpolatorTest.h"
#include "test/SplineInterpolatorTest.h"
#include "test/BarycentricInterpolatorTest.h"
#include "test/TrigonometricInterpolatorTest.h"

int main() {
    LinearInterpolatorTest::InterpolationTest();

    PolynomialInterpolatorTest::InterpolationTest();
    PolynomialInterpolatorTest::AddPointTest();
    PolynomialInterpolatorTest::GetCoefficientsTest();

    PiecewisePolynomialInterpolatorTest::ConstructorTest();
    PiecewisePolynomialInterpolatorTest::InterpolationTest();

    SplineInterpolatorTest::InterpolationTest();

    BarycentricInterpolatorTest::ConstructorTest();
    BarycentricInterpolatorTest::InterpolationTest();
    BarycentricInterpolatorTest::GetWeightsTest();

    TrigonometricInterpolatorTest::ConstructorTest();
    TrigonometricInterpolatorTest::InterpolationTest();

    return 0;
}
