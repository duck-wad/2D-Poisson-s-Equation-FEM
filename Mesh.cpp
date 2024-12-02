#include <fstream>

#include "Mesh.h"
#include "Utils.h"

Mesh::Mesh(std::string fileName) {
	ReadFile(fileName);

	Discretize();

	//assemble

	//solve
}

void Mesh::ReadFile(std::string fileName) {

	std::string junk;

	std::ifstream infile(fileName);
	if (!infile) {
		std::cerr << "Error: Unable to open file." << std::endl;
	}

	infile >> junk >> k;

	infile >> junk >> numelem;

	connectivity.resize(numelem, std::vector<int>(4));

	maxnode = 0;
	for (size_t i = 0; i < numelem; i++) {
		infile >> junk >> junk;
		for (size_t j = 0; j < 4; j++) {
			infile >> connectivity[i][j];
			if (connectivity[i][j] > maxnode) {
				maxnode = connectivity[i][j];
			}
		}
	}

	//resize global coordinates to nx2, each column is x y coordinate
	globalCoordinates.resize(2, std::vector<double>(maxnode, 0.0));

	for (size_t i = 0; i < maxnode; i++) {
		infile >> junk >> junk;
		for (size_t j = 0; j < 2; j++) {
			infile >> globalCoordinates[j][i];
		}
	}

	int numflux;
	infile >> junk >> numflux;

	fluxLocation.resize(2, std::vector<double>(numflux, 0.0));
	fluxValue.resize(numflux, 0.0);

	for (size_t i = 0; i < numflux; i++) {
		infile >> junk >> junk;
		for (size_t j = 0; j < 2; j++) {
			infile >> fluxLocation[i][j];
		}
		infile >> junk >> fluxValue[i];
	}
}

void Mesh::Discretize() {
	//assign each element their node indices (vector of 4), global coordinates for each node (4x2), and flux if it exists. how to assign the flux though 
	//need to get a 4x2 vector from the globalCoordinates to have the global coord for each elements nodes

	for (size_t elem = 0; elem < numelem; elem++) {

		std::vector<std::vector<double>> elemcoord(2, std::vector<double> (4));
		//populate temporary container for element global coordinates
		for (size_t i = 0; i < 2; i++) {
			for (size_t j = 0; j < 4; j++) {
				elemcoord[i][j] = globalCoordinates[i][connectivity[elem][j]-1];
			}
		}

		elements.emplace_back(k, connectivity[elem], elemcoord);
	}
}