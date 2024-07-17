#ifndef ZADATAK_1_POLYNOMIALINTERPOLATOR_H
#define ZADATAK_1_POLYNOMIALINTERPOLATOR_H

#include "../main/AbstractInterpolator.h"

class PolynomialInterpolator: public AbstractInterpolator {
    std::vector<double> newton, divDiff;

public:
    PolynomialInterpolator(const std::vector<std::pair<double, double>>&);

    double operator ()(double) const override;

    void AddPoint(const std::pair<double, double>&);
    std::vector<double> GetCoefficients() const;
};

PolynomialInterpolator::PolynomialInterpolator(const std::vector<std::pair<double, double>>& points): AbstractInterpolator(points), newton(points.size()), divDiff(points.size()) {
    std::transform(data.begin(), data.end(), newton.begin(), [](std::pair<double, double> pair) {
        return pair.second;
    });

    divDiff[0] = newton[newton.size() - 1];

    for (int j = 1; j < newton.size(); j++) {
        for (int i = newton.size() - 1; i >= j; i--)
            newton[i] = (newton[i] - newton[i - 1]) / (data[i].first - data[i - j].first);

        divDiff[j] = newton[newton.size() - 1];
    }
}

double PolynomialInterpolator::operator ()(double x) const {
    double y = newton[newton.size() - 1];

    for (int i = newton.size() - 2; i > -1; i--)
        y = y * (x - data[i].first) + newton[i];

    return y;
}

void PolynomialInterpolator::AddPoint(const std::pair<double, double>& point) {
    if (std::any_of(data.begin(), data.end(), [point](std::pair<double, double> x){
        return Comparator::isEqual(x.first, point.first);
    })) throw std::domain_error("Invalid point");

    data.push_back(point);
    newton.push_back(point.second);

    int size = data.size() - 1;
    double coeff = point.second;

    for (int i = 0; i < size; i++) {
        double temp = coeff;
        coeff = (coeff - divDiff[i]) / (data[size].first - data[size - i - 1].first);
        divDiff[i] = temp;
    }

    newton[size] = coeff;
    divDiff.push_back(coeff);
}

std::vector<double> PolynomialInterpolator::GetCoefficients() const {
    std::vector<double> coeff(data.size()), master(data.size() + 1);

    master[0] = 1.;
    for (int i = 0; i < data.size(); i++) {
        master[i + 1] = master[i];

        for (int j = i; j > 0; j--)
            master[j] = master[j - 1] - data[i].first * master[j];

        master[0] *= -data[i].first;
    }

    for (int i = 0; i < data.size(); i++) {
        double a = 1.;
        for (int j = 0; j < data.size(); j++)
            if (j != i)
                a *= data[i].first - data[j].first;

        a = data[i].second / a;

        std::vector<double> temp(master);
        for (int j = data.size() - 1; j > -1; j--) {
            temp[j] += data[i].first * temp[j + 1];
            coeff[j] += a * temp[j + 1];
        }
    }

    return coeff;
}

#endif //ZADATAK_1_POLYNOMIALINTERPOLATOR_H
