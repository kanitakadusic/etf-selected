#ifndef ZADATAK_1_SPLINEINTERPOLATOR_H
#define ZADATAK_1_SPLINEINTERPOLATOR_H

#include "../main/AbstractInterpolator.h"

class SplineInterpolator: public AbstractInterpolator {
    std::vector<double> r, a, s, q;

public:
    SplineInterpolator(const std::vector<std::pair<double, double>>&);

    double operator ()(double) const override;
};

SplineInterpolator::SplineInterpolator(const std::vector<std::pair<double, double>>& points): AbstractInterpolator(points), r(points.size()), a(points.size()), s(points.size()), q(points.size()) {
    r[0] = r[r.size() - 1] = 0.;

    for (int i = 1; i < r.size() - 1; i++) {
        a[i] = 2 * (data[i + 1].first - data[i - 1].first);
        r[i] = 3 * ((data[i + 1].second - data[i].second) / (data[i + 1].first - data[i].first) - (data[i].second - data[i - 1].second) / (data[i].first - data[i - 1].first));
    }

    for (int i = 1; i < r.size() - 2; i++) {
        double u = (data[i + 1].first - data[i].first) / a[i];

        a[i + 1] -= u * (data[i + 1].first - data[i].first);
        r[i + 1] -= u * r[i];
    }

    r[r.size() - 2] /= a[a.size() - 2];

    for (int i = r.size() - 3; i > 0; i--)
        r[i] = (r[i] - (data[i + 1].first - data[i].first) * r[i + 1]) / a[i];

    for (int i = 1; i <= r.size() - 1; i++) {
        double d = data[i].first - data[i - 1].first;

        s[i - 1] = (r[i] - r[i - 1]) / (3 * d);
        q[i - 1] = (data[i].second - data[i - 1].second) / d - d * (r[i] + 2 * r[i - 1]) / 3;
    }
}

double SplineInterpolator::operator ()(double x) const {
    int i = Locate(x) - 1;

    if (i == -1) i++;
    else if (i == data.size() - 1) i--;

    double t = x - data[i].first;
    return data[i].second + t * (q[i] + t * (r[i] + t * s[i]));
}

#endif //ZADATAK_1_SPLINEINTERPOLATOR_H
