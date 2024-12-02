#pragma once

#include <iostream>
#include <vector>

#include "Element.h"

//manages entire domain, interface for FEM setup and solution
class Mesh
{
public:
	Mesh(std::string fileName);

	//read the input file and fill coordinate, connectivity, and flux vectors
	void ReadFile(std::string fileName);
	//assign coordinates, flux, etc to individual elements
	void Discretize();
protected:
	//columns of globalCoordinates is x and y values
	std::vector<std::vector<double>> globalCoordinates;
	//each vector inside container is per element node indices
	std::vector<std::vector<int>> connectivity;

	int numelem;
	int maxnode;
	std::vector<Element> elements;

	//constitutive coefficient
	double k;

	//flux location is nx2 vector, each column is the start and last node of boundary segment
	std::vector<std::vector<double>> fluxLocation;
	std::vector<double> fluxValue;
};

