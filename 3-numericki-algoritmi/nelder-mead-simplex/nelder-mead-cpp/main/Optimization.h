#ifndef NELDER_MEAD_CPP_OPTIMIZATION_H
#define NELDER_MEAD_CPP_OPTIMIZATION_H

#include <stdexcept>
#include <algorithm>
#include <vector>

#include "Comparator.h"

using VECTOR = std::vector<double>;
using MATRIX = std::vector<VECTOR>;

/**
 * @param n number of vertices
 * @param d size of simplex
 * @return d * [0 ; E]
 */
MATRIX getSimplex(unsigned int n, double d = 1.0) {
    MATRIX simplex(n, VECTOR(n - 1));
    for (int i = 1; i < n; i++) simplex[i][i - 1] = d;
    return simplex;
}

/**
 * domain convergence test
 * becomes true when the working simplex is sufficiently small i.e. all vertices are close enough
 */
bool domainConvergenceTestPassed(const MATRIX& simplex, double tolerance) {
    for (int i = 1; i < simplex.size(); i++)
        for (int j = 0; j < simplex.size() - 1; j++)
            if (std::fabs(simplex[0][j] - simplex[i][j]) > tolerance) return false;

    return true;
}

/**
 * function-value convergence test
 * becomes true when the values of the function in all vertices of the simplex are close enough
 */
template <typename F>
bool valueConvergenceTestPassed(F f, const MATRIX& simplex, double tolerance) {
    for (int i = 1; i < simplex.size(); i++)
        if (std::fabs(f(simplex[0]) - f(simplex[i])) > tolerance) return false;

    return true;
}

/**
 * @tparam F function or functor
 * @param f dimension >= 2
 * @param simplex n /vertices/ x (n - 1) /function dimension/
 * @param tolerance non-negative
 * @param maxIterations non-negative
 * @return {accuracy achieved, minimum}
 */
template <typename F>
std::pair<bool, VECTOR> NelderMead(F f, MATRIX simplex, double tolerance = 1e-10, int maxIterations = 500) {
    if (simplex.size() < 3 || tolerance < 0 || maxIterations < 0) throw std::domain_error("Invalid parameters");
    for (const VECTOR& vertex : simplex)
        if (vertex.size() != simplex.size() - 1) throw std::domain_error("Invalid parameters");

    /**
     * N - number of vertices of the simplex
     * D - dimension of the function
     */
    const unsigned int N = simplex.size(), D = N - 1;

    /**
     * iBest - index of the best performing vertex
     * iWorst - index of the worst performing vertex
     * iSecondWorst - index of the second worst performing vertex
     */
    const unsigned int iBest = 0, iWorst = N - 1, iSecondWorst = N - 2;

    /**
     * ALPHA - parameter for reflection
     * BETA - parameter for contraction
     * GAMMA - parameter for expansion
     * DELTA - parameter for shrinkage
     */
    const double ALPHA = 1.0, BETA = 0.5, GAMMA = 2.0, DELTA = 0.5;

    while (--maxIterations) {
        /**
         * sort the vertices by corresponding function values in ascending order
         * f(simplex[0]) < f(simplex[1]) < ... < f(simplex[N - 1])
         */
        std::sort(simplex.begin(), simplex.end(), [f](const VECTOR& A, const VECTOR& B) {
            return f(A) < f(B);
        });

        if (domainConvergenceTestPassed(simplex, tolerance) || valueConvergenceTestPassed(f, simplex, tolerance)) break;

        /**
         * calculate the centroid of the best side - the one opposite the worst vertex
         * the worst vertex is the last one after sorting
         */
        VECTOR centroid(D);
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < N - 1; j++)
                centroid[i] += simplex[j][i];

            centroid[i] /= N - 1;
        }

        /**
         * compute the reflection point
         */
        VECTOR reflection(D);
        for (int i = 0; i < D; i++)
            reflection[i] = centroid[i] + ALPHA * (centroid[i] - simplex[iWorst][i]);

        if (Comparator::isLess(f(reflection), f(simplex[iSecondWorst]))) {
            /**
             * the reflection point is better than the second worst point
             * check if it is better than the best point
             */
            if (Comparator::isLess(f(reflection), f(simplex[iBest]))) {
                /**
                 * the reflection point is better than all of the vertices
                 * compute the expansion point
                 */
                VECTOR expansion(D);
                for (int i = 0; i < D; i++)
                    expansion[i] = centroid[i] + GAMMA * (reflection[i] - centroid[i]);

                /**
                 * accept the expansion point if it is better than the reflection point
                 * otherwise accept the reflection point
                 */
                if (Comparator::isLess(f(expansion), f(reflection))) reflection = std::move(expansion);
            }

            simplex[iWorst] = std::move(reflection);
        } else {
            /**
             * the reflection point is as good as or worse than the second worst point
             * check if it is better than the worst point
             */
            VECTOR contraction(D);
            bool isContracted = false;

            if (Comparator::isLess(f(reflection), f(simplex[iWorst]))) {
                /**
                 * the reflection point is better than the worst point
                 * compute the outsideContraction point
                 */
                for (int i = 0; i < D; i++)
                    contraction[i] = centroid[i] + BETA * (reflection[i] - centroid[i]);

                /**
                 * accept the outsideContraction point if it is better or as good as the reflection point
                 * otherwise perform a shrink transformation by computing n new vertices
                 */
                if (Comparator::isLessEqual(f(contraction), f(reflection))) isContracted = true;
            } else {
                /**
                 * the reflection point is worse than all of the points
                 * compute the insideContraction point
                 */
                for (int i = 0; i < D; i++)
                    contraction[i] = centroid[i] + BETA * (simplex[iWorst][i] - centroid[i]);

                /**
                 * accept the insideContraction point if it is better than the worst point
                 * otherwise perform a shrink transformation by computing n new vertices
                 */
                if (Comparator::isLess(f(contraction), f(simplex[iWorst]))) isContracted = true;
            }

            /**
             * perform contraction or shrinkage
             */
            if (isContracted) simplex[iWorst] = std::move(contraction);
            else
                for (int i = 1; i < N; i++)
                    for (int j = 0; j < D; j++)
                        simplex[i][j] = simplex[iBest][j] + DELTA * (simplex[i][j] - simplex[iBest][j]);
        }
    }

    return {maxIterations != 0, simplex[iBest]};
}

#endif //NELDER_MEAD_CPP_OPTIMIZATION_H
