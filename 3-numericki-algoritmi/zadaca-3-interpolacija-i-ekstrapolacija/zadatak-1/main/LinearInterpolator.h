#ifndef ZADATAK_1_LINEARINTERPOLATOR_H
#define ZADATAK_1_LINEARINTERPOLATOR_H

#include "../main/AbstractInterpolator.h"

class LinearInterpolator: public AbstractInterpolator {
public:
    LinearInterpolator(const std::vector<std::pair<double, double>>& points): AbstractInterpolator(points) {}

    double operator ()(double) const override;
};

double LinearInterpolator::operator ()(double x) const {
    int i = Locate(x);

    if (i == 0) i++;
    else if (i == data.size()) i--;

    return ((data[i].first - x) * data[i - 1].second + (x - data[i - 1].first) * data[i].second) / (data[i].first - data[i - 1].first);
}

#endif //ZADATAK_1_LINEARINTERPOLATOR_H
