#include <iostream>

#include "Element.h"

Element::Element(double coeff, std::vector<int>& indices, std::vector<std::vector<double>>& coordinates) :
	k(coeff), nodeIndices(indices), nodeCoordinates(coordinates) {
	std::cout << "Hello" << std::endl;
}