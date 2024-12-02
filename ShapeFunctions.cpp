#include <iostream>
#include <vector>

#include "ShapeFunctions.h"

//shape functions for a 4x4 quad. in the future can implement for triangles too, take shape parameter
void ShapeFunction(std::vector<double>& Ni, double xsi, double eta) {

	//temporary variables to simplify calculations
	double mxs, pxs, met, pet;

	Ni.resize(4);
	mxs = 1.0 - xsi;
	pxs = 1.0 + xsi;
	met = 1.0 - eta;
	pet = 1.0 + eta;

	Ni[0] = 0.5 * mxs * 0.5 * met;
	Ni[1] = 0.5 * pxs * 0.5 * met;
	Ni[2] = 0.5 * pxs * 0.5 * pet;
	Ni[3] = 0.5 * mxs * 0.5 * pet;
}

//shape function derivatives for 4x4 quad. in future implement triangles
void ShapeDerivative(std::vector<std::vector<double>>& DNi, double xsi, double eta) {

	DNi.resize(2, std::vector<double>(4, 0.0));

	//temporary variables to simplify calculations
	double mxs, pxs, met, pet;

	mxs = 1. - xsi;
	pxs = 1. + xsi;
	met = 1. - eta;
	pet = 1. + eta;

	DNi[0][0] = -0.25 * met;
	DNi[1][0] = -0.25 * mxs;
	DNi[0][1] = 0.25 * met;
	DNi[1][1] = -0.25 * pxs;
	DNi[0][2] = 0.25 * pet;
	DNi[1][2] = 0.25 * pxs;
	DNi[0][3] = -0.25 * pet;
	DNi[1][3] = 0.25 * mxs;
}