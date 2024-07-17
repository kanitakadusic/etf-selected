#ifndef ZADATAK_1_ABSTRACTINTERPOLATOR_H
#define ZADATAK_1_ABSTRACTINTERPOLATOR_H

#include <vector>
#include <algorithm>
#include <stdexcept>

#include "Comparator.h"

class AbstractInterpolator {
    mutable int cache;

protected:
    std::vector<std::pair<double, double>> data;

    int Locate(double) const;

public:
    AbstractInterpolator(const std::vector<std::pair<double, double>>&);

    virtual double operator ()(double) const = 0;
};

AbstractInterpolator::AbstractInterpolator(const std::vector<std::pair<double, double>>& points): data(points), cache(0) {
    if (data.size() < 2) throw std::logic_error("Nonsense");

    std::sort(data.begin(), data.end(), [](const std::pair<double, double>& x, const std::pair<double, double>& y) {
        return x < y;
    });

    for (int i = 0; i < data.size() - 1; i++)
        if (Comparator::isEqual(data[i].first, data[i + 1].first))
            throw std::domain_error("Invalid data set");
}

int AbstractInterpolator::Locate(double x) const {
    if (Comparator::isBetween(x, // current interval
                              (cache == 0 ? std::numeric_limits<double>::lowest() : data[cache - 1].first),
                              (cache == data.size() ? std::numeric_limits<double>::max() : data[cache].first)))
        return cache;
    if (cache != 0 && Comparator::isBetween(x, // interval before
                              (cache == 1 ? std::numeric_limits<double>::lowest() : data[cache - 2].first),
                              (data[cache - 1].first)))
        return cache - 1;
    if (cache != data.size() && Comparator::isBetween(x, // interval after
                              (data[cache].first),
                              (cache == data.size() - 1 ? std::numeric_limits<double>::max() : data[cache + 1].first)))
        return cache + 1;

    auto p = std::lower_bound(data.begin(), data.end(), x, [](const std::pair<double, double>& pair, double value) {
        return Comparator::isLess(pair.first, value);
    });

    return cache = p - data.begin();
}

#endif //ZADATAK_1_ABSTRACTINTERPOLATOR_H
