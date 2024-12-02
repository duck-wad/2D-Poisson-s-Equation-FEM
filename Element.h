#pragma once

#include <vector>

#include "Utils.h"
#include "GaussPoint.h"

//class containing information for individual elements (element stiffness and force)
class Element
{
public:
	Element(double k, std::vector<int>& indices, std::vector<std::vector<double>>& coordinates, std::vector<std::pair<int, double>>& flux);
protected:
	//constitutive coefficient
	double k;
	//global indices of the element nodes. order is CCW from bottom left corner
	//may not need this
	std::vector<int> nodeIndices;
	//global coordinates of the element nodes. 4x2 where each column is x y 
	std::vector<std::vector<double>> nodeCoordinates;
	//pair container for flux applied. first value is the local edge applied, second is value
	std::vector<std::pair<int, double>> elementFlux;
	bool isFlux;

	std::vector<std::vector<double>> stiffnessMatrix;
	std::vector<double> forceVector;
	
	//4 gauss points computed per element at predefined locations
	//each GaussPoint object stores its own B matrices, Jacobian etc
	std::vector<GaussPoint> gaussPoints;
};

