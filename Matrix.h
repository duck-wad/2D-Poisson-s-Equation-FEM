#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

// Function to invert a matrix using Gauss-Jordan elimination
std::vector<std::vector<double>> invertMatrix(const std::vector<std::vector<double>>& A) {
    int n = A.size();
    if (n == 0 || A[0].size() != n) {
        throw std::invalid_argument("Matrix must be square for inversion.");
    }

    // Create an augmented matrix with the identity matrix
    std::vector<std::vector<double>> augmented(n, std::vector<double>(2 * n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented[i][j] = A[i][j];
        }
        augmented[i][n + i] = 1.0; // Append identity matrix
    }

    // Perform Gaussian elimination
    for (int i = 0; i < n; ++i) {
        // Find the pivot element
        double pivot = augmented[i][i];
        if (pivot == 0.0) {
            throw std::runtime_error("Matrix is singular and cannot be inverted.");
        }

        // Normalize the pivot row
        for (int j = 0; j < 2 * n; ++j) {
            augmented[i][j] /= pivot;
        }

        // Eliminate the current column in other rows
        for (int k = 0; k < n; ++k) {
            if (k == i) continue;
            double factor = augmented[k][i];
            for (int j = 0; j < 2 * n; ++j) {
                augmented[k][j] -= factor * augmented[i][j];
            }
        }
    }

    // Extract the inverted matrix
    std::vector<std::vector<double>> inverted(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverted[i][j] = augmented[i][n + j];
        }
    }

    return inverted;
}