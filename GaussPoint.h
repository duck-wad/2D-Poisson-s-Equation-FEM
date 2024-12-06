#pragma once

#include <vector>

#include "ShapeFunctions.h"

//everything related to individual gausspoints
class GaussPoint
{
public:
	//constructor should receive the coordinates
	GaussPoint(double xsi, double eta, double weight, const std::vector<std::vector<double>>& coordinates);

	void ConstructJ(const std::vector<std::vector<double>>& coordinates);
	void ConstructB();

	std::vector<std::vector<double>> getKProduct() { return KProduct; }
	std::vector<double> getFVector() { return FVector; }

protected:
	//local coordinates associated with gauss point ex) -1/sqrt3, 1/sqrt3
	double xsi;
	double eta;
	double weight;
	double Jacobian;
	//2x4 vector containing the B matrix associated with each gauss point
	std::vector<std::vector<double>> BMatrix;
	//2x2 vector for the Jacobian matrix
	std::vector<std::vector<double>> JMatrix;
	//gamma matrix is inverse of JMatrix
	std::vector<std::vector<double>> GMatrix;
	std::vector<double> N;
	//shape function derivatives at the gausspoint
	std::vector<std::vector<double>> NDerivatives;
	//product of B, BTranspose, and Jacobian
	std::vector<std::vector<double>> KProduct;
	//vector for contribution to force vector from point source
	std::vector<double> FVector;
};

