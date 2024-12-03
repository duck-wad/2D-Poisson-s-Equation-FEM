#include <iostream>

#include "Element.h"
#include "Utils.h"

Element::Element(double coeff, std::vector<int>& indices, std::vector<std::vector<double>>& coordinates, std::vector<std::pair<int, double>>& flux) :
	k(coeff), nodeIndices(indices), nodeCoordinates(coordinates) {
	if (flux.empty()) {
		isFlux = false;
	}
	else {
		isFlux = true;
		elementFlux = flux;
	}

	//obtain the local coordinates for the 4 gauss points assuming 2x2 gauss quadrature
	std::vector<std::vector<double>> gaussCoordinates;
	std::vector<double> gaussWeights;
	GaussCoordinates(gaussCoordinates, gaussWeights);

	//construct the list of gausspoints
	for (size_t i = 0; i < 4; i++) {
		gaussPoints.emplace_back(gaussCoordinates[i][0], gaussCoordinates[i][1], gaussWeights[i], coordinates);
	}
}

//after the construction of stiffness and force matrices, deallocate the gausspoint memory with
//gausspoint.clear()