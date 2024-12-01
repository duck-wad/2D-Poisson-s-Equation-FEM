#pragma once

#include <vector>

void transpose(std::vector<std::vector<double>>& matrix);
void inverse2x2(std::vector<std::vector<double>>& matrix);
double determinant2x2(const std::vector<std::vector<double>>& matrix);

//some function to return the local coordinates of every gauss point for 2x2 quadrature