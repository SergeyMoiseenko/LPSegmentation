#pragma once

#include "stdafx.h"
#include "TestedAlgorithm.h"
#include "SegmentationEvaluation.h"
#include "LPSUtils.h"

class AlgorithmTester
{
public:
	AlgorithmTester(TestedAlgorithm & _alg);
	~AlgorithmTester();

	vector<Rect> test(Mat & img, vector<Rect> correctSegmentation, SegmentationEvaluation & outEval);


private:
	TestedAlgorithm & alg;
	vector<SegmentationEvaluation> results;

	SegmentationEvaluation evaluateSegmentationResult(vector<Rect> algSegm, vector<Rect> correctSegm);

	

};

