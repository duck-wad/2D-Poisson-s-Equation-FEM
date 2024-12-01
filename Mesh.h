#pragma once

#include <vector>

#include "Element.h"

//manages entire domain, interface for FEM setup and solution
class Mesh
{
public:
	Mesh();
protected:
	std::vector<std::vector<double>> globalCoordinates;
	std::vector<std::vector<int>> connectivity;
	std::vector<Element> elements;
};

