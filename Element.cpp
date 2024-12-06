#include <iostream>

#include "Element.h"
#include "Utils.h"

Element::Element(double coeff, std::vector<int>& indices, std::vector<std::vector<double>>& coordinates, std::vector<std::pair<int, double>>& flux, double Q, bool isQ) :
	k(coeff), nodeIndices(indices), nodeCoordinates(coordinates), pointSource(Q), isPointSource(isQ) {
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
	//TODO: pass flux into the gausspoint constructor
	for (size_t i = 0; i < 4; i++) {
		gaussPoints.emplace_back(gaussCoordinates[i][0], gaussCoordinates[i][1], gaussWeights[i], coordinates);
	}

	//sum each gausspoint contribution to get the stiffness matrix
	stiffnessMatrix.resize(4, std::vector<double>(4));
	for (size_t i = 0; i < 4; i++) {
		stiffnessMatrix += gaussPoints[i].getKProduct();
	}
	stiffnessMatrix *= k;

	forceVector.resize(4, 0.0);
	//do something with the force vector
	if (isPointSource) {
		//loop over gausspoints and add contribution of each to force vector
		for (size_t i = 0; i < 4; i++) {
			forceVector += (gaussPoints[i].getFVector() * pointSource);
		}
	}

	gaussPoints.clear();
}