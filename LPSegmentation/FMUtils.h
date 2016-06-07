#pragma once
#include "stdafx.h"
#include "QuadraticCoefficients.h"

using namespace std;
using namespace cv;

double solveQuadratic(const QuadraticCoefficients &coeff);
void contrastCorrection(Mat & img);

