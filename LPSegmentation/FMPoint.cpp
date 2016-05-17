#include "FMPoint.h"



FMPoint::FMPoint()
{
}


FMPoint::~FMPoint()
{
}

cv::Point FMPoint::getCVPoint()
{
	return cv::Point(x, y);
}