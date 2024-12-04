#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>

//tranpose an nxm matrix
template<typename T>
std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>>& matrix) {
	
	size_t rows = matrix.size();
	assert(rows != 0 && "Matrix must be populated to be transposed");
	size_t cols = matrix[0].size();

	std::vector<std::vector<T>> output(cols, std::vector<T>(rows));

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			output[j][i] = matrix[i][j];
		}
	}
	return output;
}

template<typename T>
double determinant2x2(const std::vector<std::vector<T>>& matrix) {
	assert(matrix.size() == 2 && matrix[0].size() == 2 && "Matrix must be 2x2");
	return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

//invert 2x2 matrix and return new matrix
template<typename T>
std::vector<std::vector<T>> inverse2x2(std::vector<std::vector<T>>& matrix) {
	assert(matrix.size() == 2 && matrix[0].size() == 2 && "Matrix must be 2x2");
	
	std::vector<std::vector<T>> output(2, std::vector<T>(2));

	double det = determinant2x2(matrix);

	output[0][0] = matrix[1][1] / det;
	output[1][1] = matrix[0][0] / det;
	output[0][1] = -1 * matrix[0][1] / det;
	output[1][0] = -1 * matrix[1][0] / det;
	return output;
}

template<typename T>
void printMatrix(const std::vector<std::vector<T>>& matrix) {
	size_t rows = matrix[0].size();
	size_t cols = matrix.size();    

	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			std::cout << matrix[j][i] << " "; 
		}
		std::cout << std::endl;
	}
}

template<typename T>
std::vector<std::vector<T>> operator*(const std::vector<std::vector<T>>& mat1, const std::vector<std::vector<T>>& mat2) {

	if (mat1.empty() || mat2.empty()) {
		throw std::invalid_argument("Matrices must not be empty");
	}

	size_t mat1col = mat1.size();
	size_t mat1row = mat1[0].size();
	size_t mat2col = mat2.size();
	size_t mat2row = mat2[0].size();

	if (mat1col != mat2row || mat1row != mat2col) {
		throw std::invalid_argument("Matrix dimensions are not compatible for multiplication");
	}

	std::vector<std::vector<T>> output(mat2col, std::vector<T>(mat1row, T()));

	for (size_t i = 0; i < mat2col; i++) {
		for (size_t j = 0; j < mat1row; j++) {
			for (size_t k = 0; k < mat1col; k++) {
				output[j][i] += mat1[k][i] * mat2[j][k];
			}
		}
	}
	return output;
}

template<typename T>
std::vector<std::vector<T>> operator+(const std::vector<std::vector<T>>& mat1, const std::vector<std::vector<T>>& mat2) {

	if (mat1.empty() || mat2.empty()) {
		throw std::invalid_argument("Matrices must not be empty");
	}
	if (mat1.size() != mat2.size() || mat1[0].size() != mat2[0].size()) {
		throw std::invalid_argument("Matrices must be the same size for addition");
	}

	std::vector<std::vector<T>> output(mat1.size(), std::vector<T>(mat1[0].size(), T()));

	for (size_t i = 0; i < mat1.size(); i++) {
		for (size_t j = 0; j < mat1[0].size(); j++) {
			output[i][j] = mat1[i][j] + mat2[i][j];
		}
	}
	return output;
}

template<typename T>
std::vector<std::vector<T>>& operator+=(std::vector<std::vector<T>>& mat1, const std::vector<std::vector<T>>& mat2) {

	if (mat1.empty() || mat2.empty()) {
		throw std::invalid_argument("Matrices must not be empty");
	}
	if (mat1.size() != mat2.size() || mat1[0].size() != mat2[0].size()) {
		throw std::invalid_argument("Matrices must be the same size for addition");
	}

	for (size_t i = 0; i < mat1.size(); i++) {
		for (size_t j = 0; j < mat1[0].size(); j++) {
			mat1[i][j] += mat2[i][j];
		}
	}
	return mat1;
}


template<typename T>
std::vector<std::vector<T>>& operator*= (std::vector<std::vector<T>>& matrix, T scalar) {
	for (auto& row : matrix) {
		for (auto& value : row) {
			value *= scalar;
		}
	}
	return matrix;
}

//some function to return the local coordinates of every gauss point for 2x2 quadrature
void GaussCoordinates(std::vector<std::vector<double>>& coords, std::vector<double>& weights);

template<typename T>
void writeMatrixToCSV(const std::vector<std::vector<T>>& matrix, const std::string& filename) {
	// Open the file stream
	std::ofstream file(filename);

	if (!file.is_open()) {
		throw std::ios_base::failure("Failed to open file for writing.");
	}

	size_t rows = matrix[0].size();
	size_t cols = matrix.size();

	// Write the matrix to the file in row-major order
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			file << matrix[j][i];
			if (j < cols - 1) { // Add a comma unless it's the last column
				file << ",";
			}
		}
		file << "\n"; // Newline after each row
	}

	file.close();

	std::cout << "Matrix written to " << filename << " successfully." << std::endl;

	// Open the file in the default application (Excel on Windows)
	std::string command = "start " + filename;
	if (system(command.c_str()) != 0) {
		std::cerr << "Failed to open the file." << std::endl;
	}
}