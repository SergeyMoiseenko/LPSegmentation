#include "SegmentationEvaluation.h"

SegmentationEvaluation::SegmentationEvaluation()
{
	this->numberOfFoundSegments = 0;
	this->trueNumberOfSegments = 0;
}

SegmentationEvaluation::SegmentationEvaluation(vector<SegmentMetric> metrics, vector<Rect> segments, int foundSegments, int _trueNumberOfSegments)
{
	this->metricsForCorrectFoundSegments = metrics;
	this->correctFoundSegments = segments;
	this->numberOfFoundSegments = foundSegments;
	this->trueNumberOfSegments = _trueNumberOfSegments;
}

SegmentationEvaluation::~SegmentationEvaluation()
{
}

vector<SegmentMetric> SegmentationEvaluation::getMetrics()
{
	return this->metricsForCorrectFoundSegments;
}

vector<Rect> SegmentationEvaluation::getCorrectFoundsegments()
{
	return this->correctFoundSegments;
}

int SegmentationEvaluation::getNumberOfFoundSegments()
{
	return this->numberOfFoundSegments;
}

int SegmentationEvaluation::getTrueNumberOfSegments()
{
	return this->trueNumberOfSegments;
}

int SegmentationEvaluation::getNumberOfCorrectFoundSegments()
{
	return this->correctFoundSegments.size();
}
