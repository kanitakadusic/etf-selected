#ifndef ZADATAK_1_TRIGONOMETRICINTERPOLATOR_H
#define ZADATAK_1_TRIGONOMETRICINTERPOLATOR_H

#include "../main/AbstractInterpolator.h"

class TrigonometricInterpolator: public AbstractInterpolator {
public:
    TrigonometricInterpolator(const std::vector<std::pair<double, double>>& points): AbstractInterpolator(points) {
        if (!Comparator::isEqual(data[0].second, data[data.size() - 1].second)) throw std::domain_error("Function is not periodic");
    }

    double operator ()(double) const override;
};

double TrigonometricInterpolator::operator ()(double x) const {
    double o = 2 * M_PI / (data[data.size() - 1].first - data[0].first), s = 0.;

    if (data.size() % 2 == 0) {
        for (int k = 0; k < data.size() - 1; k++) {
            double p = 1.;

            for (int j = 0; j < data.size() - 1; j++)
                if (j != k)
                    p *= std::sin(o * (x - data[j].first) / 2) / std::sin(o * (data[k].first - data[j].first) / 2);

            s += data[k].second * p;
        }
    } else {
        for (int k = 0; k < data.size() - 1; k++) {
            double p = 1., a = 0.;

            for (int j = 0; j < data.size() - 1; j++)
                if (j != k) {
                    p *= std::sin(o * (x - data[j].first) / 2) / std::sin(o * (data[k].first - data[j].first) / 2);
                    a += data[j].first;
                }

            s += data[k].second * p * std::sin(o * (x + a) / 2) / std::sin(o * (data[k].first + a) / 2);
        }
    }

    return s;
}

#endif //ZADATAK_1_TRIGONOMETRICINTERPOLATOR_H
