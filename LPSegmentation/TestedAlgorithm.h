#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;
using namespace cv;

class TestedAlgorithm
{
public:
	virtual vector<Rect> doSegmentation(Mat &image) = 0;
};

