#include "GaussPoint.h"
#include "Utils.h"

//when gausspoint is constructed automatically calculate the product of BT, B, J
GaussPoint::GaussPoint(double x, double e, double w, const std::vector<std::vector<double>>& coordinates) :
	xsi(x), eta(e), weight(w) {

	ShapeDerivative(NDerivatives, xsi, eta);
	NDerivatives = transpose(NDerivatives);
	ShapeFunction(N, xsi, eta);
	ConstructJ(coordinates);
	GMatrix = inverse2x2(JMatrix);
	Jacobian = determinant2x2(JMatrix);
	ConstructB();

	//for stiffness
	KProduct = transpose(BMatrix) * BMatrix;
	KProduct *= (Jacobian * weight);

	FVector = N * (Jacobian*weight);

	//TODO: receive flux and use it to make the force vector. but how? 
}

void GaussPoint::ConstructJ(const std::vector<std::vector<double>>& coordinates) {

	JMatrix = NDerivatives * coordinates;
}

void GaussPoint::ConstructB() {

	BMatrix.resize(4, std::vector<double>(2));

	for (size_t i = 0; i < 4; i++) {
		BMatrix[i][0] = GMatrix[0][0] * NDerivatives[i][0] + GMatrix[0][1] * NDerivatives[i][1];
		BMatrix[i][1] = GMatrix[1][0] * NDerivatives[i][0] + GMatrix[1][1] * NDerivatives[i][1];
	}
}
