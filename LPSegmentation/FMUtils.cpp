#include "FMUtils.h"
#include <iostream>
#include <algorithm>

double solveQuadratic(const QuadraticCoefficients &coeff)
{
	double discriminant = coeff.b*coeff.b - 4*coeff.a*coeff.c;

	if (discriminant < 0) {
		cerr << "Negative discriminant value: " << discriminant << endl;
	}

	double x1 = (std::sqrt(discriminant) - coeff.b) / (2 * coeff.a);
	double x2 = (-(std::sqrt(discriminant)) - coeff.b) / (2 * coeff.a);

	return max(x1, x2);;
}
