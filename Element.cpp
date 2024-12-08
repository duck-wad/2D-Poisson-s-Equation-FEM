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
	if (isFlux) {
		for (size_t i = 0; i < elementFlux.size(); i++) {
			int edge = elementFlux[i].first;
			int startNode, endNode = 0;
			//first number is x coordinate, second is y coordinate
			std::vector<double> startPoint(2), endPoint(2);
			if (edge == 1) {
				startNode = 0;
				endNode = 1;
			}
			else if (edge == 2) {
				startNode = 1;
				endNode = 2;
			}
			else if (edge == 3) {
				startNode = 2;
				endNode = 3;
			}
			else if (edge == 4) {
				startNode = 3;
				endNode = 0;
			}
			else
				throw std::invalid_argument("Edge must be from 1-4");

			startPoint[0] = nodeCoordinates[0][startNode];
			startPoint[1] = nodeCoordinates[1][startNode];
			endPoint[0] = nodeCoordinates[0][endNode];
			endPoint[1] = nodeCoordinates[1][endNode];

			double L = std::sqrt(std::pow(endPoint[0] - startPoint[0], 2) + std::pow(endPoint[1] - startPoint[1], 2));
			forceVector[startNode] -= elementFlux[i].second * L / 2;
			forceVector[endNode] -= elementFlux[i].second * L / 2;
		}
	}

	gaussPoints.clear();
}