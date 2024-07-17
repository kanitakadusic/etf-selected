#ifndef ZADATAK_1_PIECEWISEPOLYNOMIALINTERPOLATOR_H
#define ZADATAK_1_PIECEWISEPOLYNOMIALINTERPOLATOR_H

#include "../main/AbstractInterpolator.h"

class PiecewisePolynomialInterpolator: public AbstractInterpolator {
    int order;

public:
    PiecewisePolynomialInterpolator(const std::vector<std::pair<double, double>>& points, int order): AbstractInterpolator(points), order(order) {
        if (order < 1 || order >= data.size()) throw std::domain_error("Invalid order");
    }

    double operator ()(double) const override;
};

double PiecewisePolynomialInterpolator::operator ()(double x) const {
    int i = Locate(x), start, end;

    if (i == 0) i++;
    else if (i == data.size()) i--;

    if (order % 2 == 0) { start = i - order / 2 - 1; end = i + order / 2 - 1; }
    else { start = i - (order - 1) / 2 - 1; end = i + (order + 1) / 2 - 1; }

    if (start < 0) { start = 0; end = order; }
    else if (end >= data.size()) { start = data.size() - order - 1; end = data.size() - 1; }

    double s = 0.;
    for (i = start; i <= end; i++) {
        double p = data[i].second;

        for (int j = start; j <= end; j++)
            if (j != i)
                p *= (x - data[j].first) / (data[i].first - data[j].first);

        s += p;
    }

    return s;
}

#endif //ZADATAK_1_PIECEWISEPOLYNOMIALINTERPOLATOR_H
