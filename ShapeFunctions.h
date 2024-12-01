#pragma once

#include <vector>

void ShapeFunction(std::vector<double>& Ni,	double xsi, double eta);

void ShapeDerivative(std::vector<std::vector<double>>& DNi, double xsi, double eta);
