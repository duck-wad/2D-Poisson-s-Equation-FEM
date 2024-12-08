#include <fstream>

#include "Mesh.h"
#include "Utils.h"

Mesh::Mesh(std::string fileName) {
	ReadFile(fileName);

	Discretize();

	AssembleGlobalStiffness();

	AssembleGlobalForce();
	ApplyBC();
	//Solve();
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

	//read flux on boundary
	int numflux;
	infile >> junk >> numflux;

	fluxLocation.resize(numflux, std::vector<int>(2, 0));
	fluxValue.resize(numflux, 0.0);

	for (size_t i = 0; i < numflux; i++) {
		infile >> junk >> junk;
		for (size_t j = 0; j < 2; j++) {
			infile >> fluxLocation[i][j];
		}
		infile >> junk >> fluxValue[i];
	}

	//read point sources
	int numpointsource;
	infile >> junk >> numpointsource;
	for (size_t i = 0; i < numpointsource; i++) {
		int tempLoc;
		double tempVal;
		infile >> junk >> junk >> tempLoc >> junk >> tempVal;
		pointSource.push_back(std::make_pair(tempLoc, tempVal));
	}

	//read boundary conditions
	int numBC;
	infile >> junk >> numBC;

	bcLocation.resize(numBC);
	bcValue.resize(numBC, 0.0);

	for (size_t i = 0; i < numBC; i++) {
		infile >> junk >> junk >> bcLocation[i] >> junk >> bcValue[i];
	}
}

void Mesh::Discretize() {
	//assign each element their node indices (vector of 4), global coordinates for each node (4x2), and flux if it exists. how to assign the flux though 
	//need to get a 4x2 vector from the globalCoordinates to have the global coord for each elements nodes

	for (size_t elem = 0; elem < numelem; elem++) {

		std::vector<std::vector<double>> elemCoord(2, std::vector<double> (4));
		//populate temporary container for element global coordinates
		for (size_t i = 0; i < 2; i++) {
			for (size_t j = 0; j < 4; j++) {
				elemCoord[i][j] = globalCoordinates[i][connectivity[elem][j]-1];
			}
		}

		//need to determine if any of the applied flux boundary conditions are applied on an edge of the element
		//there can be multiple flux conditions per element, store them in a vector of std::pairs
		//first is the local edge ex) edge 1 is between local node 1 and 2
		std::vector<std::pair<int, double>> elemFlux;
		ApplyFluxToElement(connectivity[elem], elemFlux);

		double Q = 0.0;
		bool isQ = false;
		for (size_t i = 0; i < pointSource.size(); i++) {
			if (pointSource[i].first - 1 == elem) {
				Q = pointSource[i].second;
				isQ = true;
				break;
			}
		}

		elements.emplace_back(k, connectivity[elem], elemCoord, elemFlux, Q, isQ);
	}
}

void Mesh::ApplyFluxToElement(const std::vector<int>& indices, 
	std::vector<std::pair<int, double>>& elementFlux) {

	//loop over every flux condition to see if the condition applies to the element
	for (std::size_t i = 0; i < fluxLocation.size(); i++) {
		int startNode = fluxLocation[i][0];
		int endNode = fluxLocation[i][1];
		double fluxVal = fluxValue[i];

		//check if both start and end nodes are in element node indices
		std::vector<int>::const_iterator it1 = std::find(indices.begin(),
			indices.end(), startNode);
		std::vector<int>::const_iterator it2 = std::find(indices.begin(),
			indices.end(), endNode);
		//if both iterators are not equal to the end indice (which indicates start and end node both exist)
		if (it1 != indices.end() && it2 != indices.end()) {
			int edgeIndex = -1;
			//iterators are pointers to location within the indices vector. doing the subtraction is actually determining the distance between the iterator and the start of the vector, thereby determining which local node it corresponds to
	
			// Check for edge 1 (bottom edge: from BL to BR)
			if ((it1 - indices.begin() == 0 && it2 - indices.begin() == 1) || 
				(it1 - indices.begin() == 1 && it2 - indices.begin() == 0)) {
				edgeIndex = 1;
			}
			// Check for edge 2 (right edge: from BR to TR)
			else if ((it1 - indices.begin() == 1 && it2 - indices.begin() == 2) || 
				(it1 - indices.begin() == 2 && it2 - indices.begin() == 1)) {
				edgeIndex = 2;
			}
			// Check for edge 3 (top edge: from TR to TL)
			else if ((it1 - indices.begin() == 2 && it2 - indices.begin() == 3) || 
				(it1 - indices.begin() == 3 && it2 - indices.begin() == 2)) {
				edgeIndex = 3;
			}
			// Check for edge 4 (left edge: from TL to BL)
			else if ((it1 - indices.begin() == 3 && it2 - indices.begin() == 0) || 
				(it1 - indices.begin() == 0 && it2 - indices.begin() == 3)) {
				edgeIndex = 4;
			}

			// If an edge was found, store the flux for this element
			if (edgeIndex != -1) {
				elementFlux.push_back(std::make_pair(edgeIndex, fluxVal));
			}
		}
	}
}

void Mesh::AssembleGlobalStiffness() {
	globalStiffness.resize(maxnode, std::vector<double>(maxnode, 0.0));

	//loop over every element, use connectivity to map the element stiffness entries to the correct location
	for (size_t elem = 0; elem < numelem; elem++) {
		const std::vector<std::vector<double>>& elementStiffness = elements[elem].getStiffness();

		const std::vector<int>& elementNodes = connectivity[elem];

		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				globalStiffness[elementNodes[i]-1][elementNodes[j]-1] += elementStiffness[i][j];
			}
		}
	}

	//debug
	//writeMatrixToCSV(globalStiffness, "GLOBAL_STIFFNESS.csv");
}

void Mesh::AssembleGlobalForce() {
	globalForce.resize(maxnode);

	for (size_t elem = 0; elem < numelem; elem++) {
		const std::vector<double>& elementForce = elements[elem].getForce();
		const std::vector<int>& elementNodes = connectivity[elem];

		for (size_t i = 0; i < 4; i++) {
			globalForce[elementNodes[i] - 1] += elementForce[i];
		}
	}
}

void Mesh::ApplyBC() {

	//each node that has dirichlet BC, replace the whole row with zero except the column corresponding to the node number
	//replace that with 1. in force vector, set the row value to the bcValue
	for (size_t i = 0; i < bcLocation.size(); i++) {
		int loc = bcLocation[i] - 1;
		for (size_t j = 0; j < maxnode; j++) {
			if (j == loc) {
				globalStiffness[j][loc] = 1.0;
			}
			else globalStiffness[j][loc] = 0.0;
		}
	}

	//debug
	//writeMatrixToCSV(globalStiffness, "GLOBAL_STIFFNESS_BC.csv");
}