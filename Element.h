#pragma once

#include <vector>

#include "Utils.h"
#include "GaussPoint.h"

//class containing information for individual elements (element stiffness and force)
class Element
{
public:
	Element();
protected:
	//global indices of the element nodes. order is CCW from bottom left corner
	std::vector<int> nodeIndices;
	//global coordinates of the element nodes. 4x2 where each column is x y 
	std::vector<std::vector<double>> nodeCoordinates;
	std::vector<std::vector<double>> stiffnessMatrix;
	std::vector<double> forceVector;
	
};

