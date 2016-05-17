#pragma once
#include "stdafx.h"
class FMPoint
{
public:
	FMPoint();
	~FMPoint();

	int x, y;
	bool known;
	double time;
	cv::Point getCVPoint();
};


