#pragma once
#include "TestedAlgorithm.h"
#include "FastMarchingAlgorithm.cpp"
class TestFastMarching :
	public TestedAlgorithm
{
public:
	TestFastMarching();
	~TestFastMarching();

	vector<Rect> doSegmentation(Mat &image);

private:
	void preProcessing(Mat &img);
	
};

