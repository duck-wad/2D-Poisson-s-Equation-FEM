#pragma once

#include <vector>

void transpose(std::vector<std::vector<double>>& matrix);
void inverse2x2(std::vector<std::vector<double>>& matrix);
double determinant2x2(const std::vector<std::vector<double>>& matrix);

//some function to return the local coordinates of every gauss point for 2x2 quadrature

template<typename T>
void PrintMatrix(const std::vector<std::vector<T>>& matrix) {
	for (const auto& row : matrix) {
		for (const auto& value : row) {
			std::cout << value << "\t";
		}
		std::cout << std::endl;
	}
}