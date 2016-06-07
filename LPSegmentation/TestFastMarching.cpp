#include "TestFastMarching.h"




TestFastMarching::TestFastMarching()
{

}


TestFastMarching::~TestFastMarching()
{
}

vector<Rect> TestFastMarching::doSegmentation(Mat &image)
{
	const double alpha = 1;
	const double beta = 1000;

	FastMarchingAlgorithm<uchar> fm;
	Mat gradient = image.clone();

	this->preProcessing(gradient);

	fm.initialize(gradient, alpha, beta);
	fm.doFastMarching();

	vector<Rect> rezult = fm.getBoundaryRectangles();

	return rezult;
}

void TestFastMarching::preProcessing(Mat & img)
{
	int imgSize = img.rows*img.cols;
	cvtColor(img, img, CV_BGR2GRAY);
	
	// contrast correction
	contrastCorrection(img);

	// Медианный фильтр
	medianBlur(img, img, 5);
	// Фильтр Гаусса
	GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
	// Билатеральная фильтрация
	Mat bilateral_input = img.clone();
	bilateralFilter(bilateral_input, img, 5, 9, 9);
	//----------------------------------------------------------------------------
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	//// Gradient X
	Sobel(img, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	///// Gradient Y	
	Sobel(img, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, img);
}
