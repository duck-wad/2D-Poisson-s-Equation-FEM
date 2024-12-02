#include <iostream>

#include "Element.h"

Element::Element(double coeff, std::vector<int>& indices, std::vector<std::vector<double>>& coordinates, std::vector<std::pair<int, double>>& flux) :
	k(coeff), nodeIndices(indices), nodeCoordinates(coordinates) {
	if (flux.empty()) {
		isFlux = false;
	}
	else {
		isFlux = true;
		elementFlux = flux;
	}


}