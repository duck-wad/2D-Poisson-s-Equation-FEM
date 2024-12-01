#pragma once

#include <vector>

#include "ShapeFunctions.h"

//everything related to individual gausspoints
class GaussPoint
{
public:
	//constructor should receive the coordinates
	GaussPoint();

protected:
	//local coordinates associated with gauss point ex) -1/sqrt3, 1/sqrt3
	double xsi;
	double eta;
	double weight;
	//2x4 vector containing the B matrix associated with each gauss point
	std::vector<std::vector<double>> BMatrix;
	//2x2 vector for the Jacobian matrix
	std::vector<std::vector<double>> JMatrix;
	//gamma matrix is inverse of JMatrix. do i need to store this through? if not remove
	std::vector<std::vector<double>> GMatrix;
	//shape function derivatives at the gausspoint. do i need to store this though? if not remove
	std::vector<std::vector<double>> NDerivatives;
};

