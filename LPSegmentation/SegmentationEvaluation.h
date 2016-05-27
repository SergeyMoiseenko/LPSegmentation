#pragma once
#include "stdafx.h"
#include "SegmentMetric.h"
#include <vector>

using namespace std;
using namespace cv;

class SegmentationEvaluation
{
public:
	SegmentationEvaluation();
	SegmentationEvaluation(vector<SegmentMetric> metrics, vector<Rect> segments, int foundSegments, int _trueNumberOfSegments);
	~SegmentationEvaluation();

	vector<SegmentMetric> getMetrics();
	vector<Rect> getCorrectFoundsegments();
	int getNumberOfFoundSegments();
	int getTrueNumberOfSegments();
	int getNumberOfCorrectFoundSegments();


private:
	vector<SegmentMetric> metricsForCorrectFoundSegments;
	vector<Rect> correctFoundSegments;

	int numberOfFoundSegments;
	int trueNumberOfSegments;
};

