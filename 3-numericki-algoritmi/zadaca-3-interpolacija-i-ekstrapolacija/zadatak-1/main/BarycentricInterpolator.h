#ifndef ZADATAK_1_BARYCENTRICINTERPOLATOR_H
#define ZADATAK_1_BARYCENTRICINTERPOLATOR_H

#include "../main/AbstractInterpolator.h"

class BarycentricInterpolator: public AbstractInterpolator {
    std::vector<double> weights;

public:
    BarycentricInterpolator(const std::vector<std::pair<double, double>>&, int);

    double operator ()(double) const override;

    std::vector<double> GetWeights() const { return weights; }
};

BarycentricInterpolator::BarycentricInterpolator(const std::vector<std::pair<double, double>>& points, int order): AbstractInterpolator(points), weights(points.size()) {
    if (order < 0 || order >= data.size()) throw std::domain_error("Invalid order");

    for (int i = 0; i < data.size(); i++)
        for (int k = (0 > i - order ? 1 : i + 1 - order); k <= (i + 1 < data.size() - order ? i + 1 : data.size() - order); k++) {
            double p = 1.;

            for (int j = k - 1; j < k + order; j++)
                if (j != i)
                    p /= data[i].first - data[j].first;

            if (k % 2 == 0) p *= -1;
            weights[i] += p;
        }
}

double BarycentricInterpolator::operator ()(double x) const {
    double p = 0., q = 0.;

    for (int i = 0; i < data.size(); i++) {
        if (Comparator::isEqual(x, data[i].first)) return data[i].second;

        double u = weights[i] / (x - data[i].first);
        p += u * data[i].second;
        q += u;
    }

    return p / q;
}

#endif //ZADATAK_1_BARYCENTRICINTERPOLATOR_H
