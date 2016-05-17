#pragma once
#include "FMPoint.h"
#include <functional>

using namespace std;

class FMPointComparator : public binary_function<const FMPoint *, const FMPoint *, bool>
{
public:
	FMPointComparator();
	~FMPointComparator();
	bool operator()(const FMPoint *first, const FMPoint *second);
};

