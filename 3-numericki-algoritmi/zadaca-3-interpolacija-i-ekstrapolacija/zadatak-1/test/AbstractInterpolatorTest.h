#ifndef ZADATAK_1_ABSTRACTINTERPOLATORTEST_H
#define ZADATAK_1_ABSTRACTINTERPOLATORTEST_H

#include <string>

#include "../main/AbstractInterpolator.h"

/*
class AbstractInterpolatorTest {
public:
    static void ConstructorTest() {
        try { AbstractInterpolator({{1, 2}, {1, 3}}); }
        catch (...) { return; }

        throw std::logic_error("AbstractInterpolatorTest: Constructor failed");
    }

    static void LocateTest() {
        std::string test;

        AbstractInterpolator a({{6, 5}, {3, 2}, {5, 4}, {4, 3}, {1, 0}, {2, 1}});

        test += (a.Locate(3.5) == 3); // search (middle)
        test += (a.Locate(3.5) == 3); // current
        test += (a.Locate(2.5) == 2); // before
        test += (a.Locate(4.5) == 4); // after

        test += (a.Locate(0.5) == 0); // search (begin)
        test += (a.Locate(0) == 0); // current
        test += (a.Locate(1.5) == 1); // after
        test += (a.Locate(2.5) == 2); // search

        test += (a.Locate(6.5) == 6); // search (end)
        test += (a.Locate(7) == 6); // current
        test += (a.Locate(5.5) == 5); // before
        test += (a.Locate(4.5) == 4); // search

        test += (a.Locate(3) == 2); // search (exact)
        test += (a.Locate(1) == 0); // search (exact)
        test += (a.Locate(6) == 5); // search (exact)

        AbstractInterpolator b({{0, 0}});

        test += (b.Locate(-0.5) == 0);
        test += (b.Locate(0.5) == 1);

        test += (b.Locate(0) == 0);

        if (std::any_of(test.begin(), test.end(), [](char bit) {
            return bit == 0;
        })) throw std::logic_error("AbstractInterpolatorTest: Locate failed");
    }
};
*/

#endif //ZADATAK_1_ABSTRACTINTERPOLATORTEST_H
