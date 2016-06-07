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

void contrastCorrection(Mat & img)
{
	Scalar m, stdDev;
	meanStdDev(img, m, stdDev);
	if (stdDev[0] <= 40) {
		for (int r = 0; r < img.rows; r++) {
			for (int c = 0; c < img.cols; c++) {
				uchar p = img.at<uchar>(r, c);
				double delta = (p*1.) - m[0];
				double scale = abs(delta / stdDev[0]);
				scale = (scale > 1) ? 1 : scale;
				uchar res;
				if (delta < 0) {
					scale = 1 - scale;
					res = scale * 127;
				}
				else {
					res = 127 + scale * 127;
				}

				img.at<uchar>(r, c) = res;
			}
		}
	}
}
