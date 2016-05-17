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
	FastMarchingAlgorithm<uchar> fm;

	void preProcessing(Mat &img);
	
};

