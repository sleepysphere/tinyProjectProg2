#include "PosSymLinSystem.h"
#include <cmath>
#include <stdexcept>

PosSymLinSystem::PosSymLinSystem(const Matrix& A, const Vector& b)
    : LinearSystem(A, b) {
    if (!isSymmetric(*mpA))
        throw std::invalid_argument("Matrix must be symmetric");
}

bool PosSymLinSystem::isSymmetric(const Matrix& A) const {
    for (int i = 1; i <= A.numRows(); ++i)
        for (int j = 1; j <= A.numCols(); ++j)
            if (std::fabs(A(i, j) - A(j, i)) > 1e-12)
                return false;
    return true;
}

Vector PosSymLinSystem::Solve() const {
    const double tol = 1e-10;
    const int maxIter = 1000;
    int n = mSize;

    Vector x(n); // Initial guess: zero vector
    Vector r = *mpb - (*mpA * x); // Residual r = b - Ax
    Vector p = r; // Initial search direction
    double rs_old = r * r;

    for (int i = 0; i < maxIter; ++i) {
        Vector Ap = (*mpA) * p;
        double alpha = rs_old / (p * Ap);
        x = x + p * alpha;
        r = r - Ap * alpha;
        double rs_new = r * r;

        if (std::sqrt(rs_new) < tol)
            break;

        p = r + p * (rs_new / rs_old);
        rs_old = rs_new;
    }

    return x;
}
