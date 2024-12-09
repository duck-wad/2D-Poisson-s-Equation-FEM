#include <cmath>

#include "Utils.h"

//return 4 gauss points and their weights assuming 2x2 gauss quadrature
void GaussCoordinates(std::vector<std::vector<double>>& coords, std::vector<double>& weights) {
	coords.resize(4);
	for (auto& it1 : coords) {
		it1.resize(2);
	}
	weights.resize(4);

	double temp = 1 / sqrt(3);

	coords[0] = { -1 * temp, -1 * temp };
	coords[1] = { temp, -1 * temp };
	coords[2] = { temp, temp };
	coords[3] = { -1 * temp, temp };

	weights[0] = weights[1] = weights[2] = weights[3] = 1;
}