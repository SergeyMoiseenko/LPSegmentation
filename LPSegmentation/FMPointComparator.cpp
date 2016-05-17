#include "FMPointComparator.h"

FMPointComparator::FMPointComparator()
{
}

FMPointComparator::~FMPointComparator()
{
}

bool FMPointComparator::operator()(const FMPoint *first, const FMPoint *second)
{
	double first_time = first->time, second_time = second->time;

	if (first_time < second_time) { return true; }

	if (first_time == second_time) {
		int first_X = first->x, first_Y = first->y, second_X = second->x, second_Y = second->y;
		if (first_Y < second_Y) { return true; }
		if (first_Y == second_Y) {
			return (first_X < second_X);
		}
	}
	return false;
}


