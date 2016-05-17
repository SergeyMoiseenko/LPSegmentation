#include "stdafx.h"
#include "FunctionOfSpeed.cpp"
#include "QuadraticCoefficients.h"
#include "FMPointComparator.h"
#include "FMUtils.h"


#include <set>
#include <queue>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include <opencv2/highgui.hpp>


using namespace cv;
using namespace std;

template<typename T>
class FastMarchingAlgorithm
{
public:
	FastMarchingAlgorithm();
	~FastMarchingAlgorithm();

	
	void initialize(Mat _gradient, double alpha, double beta);
	void doFastMarching();
	Mat getTimeMat();
	vector<Rect> getBoundaryRectangles();

private:
	Mat binContours;
	Mat gradient;
	Mat  time;

	set<FMPoint*, FMPointComparator> frontPoints;
	vector<Rect> boundaryRectangles;
	FMPoint *points;

	FunctionOfSpeed<T> speed;
	double maxTime;
	int maxRectArea;
	int minRectArea;
	int rows, cols;

	bool isInitialized;

	void setRectangleAsKnown(Rect rect);
	void setRectangleAsFinding(Rect rect);
		
	void rectanglesExtraction();
	FMPoint* getPoint(const int x, const int y);
	void increaseCoefficients(QuadraticCoefficients &coeff, double time);
	void initializePoints();
	void initializeSpeed(double alpha, double beta);
	void computeTimeForPoint(FMPoint * point);
	FMPoint * getLeftPoint(FMPoint * point);
	FMPoint * getRightPoint(FMPoint * point);
	FMPoint * getUpPoint(FMPoint * point);
	FMPoint * getDownPoint(FMPoint * point);


};


template<typename T>
FastMarchingAlgorithm<T>::FastMarchingAlgorithm()
{
}

template<typename T>
FastMarchingAlgorithm<T>::~FastMarchingAlgorithm()
{
	if (isInitialized) {
		delete [] points;
	}
	
}

template<typename T>
void FastMarchingAlgorithm<T>::initialize(Mat _gradient, double alpha, double beta)
{

	this->maxTime = 0;
	this->gradient = _gradient;
	this->rows = gradient.rows;
	this->cols = gradient.cols;
	this->binContours = Mat(rows, cols, CV_8U, Scalar(255));
	initializeSpeed(alpha, beta);
	initializePoints();
	this->maxRectArea = (rows*cols) / 20;
	this->minRectArea = (rows*cols) / 100;
	this->isInitialized = true;

}

template<typename T>
void FastMarchingAlgorithm<T>::doFastMarching()
{

	int drCont = 1;
	if (!isInitialized) { return; }
	int probably_number_of_iterations = rows*cols;
	int step_for_contour_finding = probably_number_of_iterations / 1000;
	long counter = 0;
	while (!frontPoints.empty()) {

		counter++;
		FMPoint *p_min = *(frontPoints.begin());;

		if (p_min->known) { 
			frontPoints.erase(frontPoints.begin());
			continue; 
		}

		p_min->known = true;
		binContours.at<uchar>(p_min->y, p_min->x) = 0;
		frontPoints.erase(frontPoints.begin());

		FMPoint *left = getLeftPoint(p_min),
		        *right = getRightPoint(p_min),
		        *up = getUpPoint(p_min),
		        *down = getDownPoint(p_min);
		
		computeTimeForPoint(left);
		computeTimeForPoint(right);
		computeTimeForPoint(up);
		computeTimeForPoint(down);
		if ((counter % step_for_contour_finding) == 0) {
			rectanglesExtraction();
		}
		
	}
}

template<typename T>
Mat FastMarchingAlgorithm<T>::getTimeMat()
{
	Mat time_mat(rows, cols, CV_64F);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			time_mat.at<double>(r, c) = (getPoint(c, r)->time);
		}
	}
	return time_mat;
}

template<typename T>
vector<Rect> FastMarchingAlgorithm<T>::getBoundaryRectangles()
{

	return this->boundaryRectangles;
}

template<typename T>
void FastMarchingAlgorithm<T>::setRectangleAsKnown(Rect rect)
{
	int right_edge = rect.x + rect.width;
	int down_edge = rect.y + rect.height;

	for (int x = rect.x; x < right_edge; x++) {
		for (int y = rect.y; y < down_edge; y++) {
			FMPoint * p = getPoint(x, y);
			p->known = true;
		}
	}
}

template<typename T>
void FastMarchingAlgorithm<T>::setRectangleAsFinding(Rect rect)
{
	int right_edge = rect.x + rect.width;
	int down_edge = rect.y + rect.height;

	for (int x = rect.x; x < right_edge; x++) {
		for (int y = rect.y; y < down_edge; y++) {
			binContours.at<uchar>(y, x) = 0;
		}
	}
}

template<typename T>
void FastMarchingAlgorithm<T>::rectanglesExtraction()
{
	vector< vector<Point> > contours;
	Mat temp = binContours.clone();

	findContours(temp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		Rect  rect = boundingRect(contours.at(i));
		bool acceptCondition;
		acceptCondition = (rect.height > rect.width) && (((rect.height*1.) / rect.width) < 2.3) && (rect.area() < maxRectArea) && (rect.area() > minRectArea);
		if (acceptCondition) {
			this->boundaryRectangles.push_back(rect);
			
			setRectangleAsKnown(rect);
			setRectangleAsFinding(rect);
		}

	}
}

template<typename T>
FMPoint * FastMarchingAlgorithm<T>::getPoint(const int x, const int y)
{
	return &(points[y*cols+x]);
}

template<typename T>
void FastMarchingAlgorithm<T>::increaseCoefficients(QuadraticCoefficients & coeff, double time)
{
	coeff.a++;
	coeff.b -= (2 * time);
	coeff.c += (time*time);
}

template<typename T>
void FastMarchingAlgorithm<T>::initializePoints()
{
	this->frontPoints = set<FMPoint*, FMPointComparator >(FMPointComparator());

	int points_length = rows*cols;
	this->points = new FMPoint[points_length];

	for (int i = 0; i < points_length; i++) {
		int x = i % cols;
		int y = i / cols;
		points[i].x = x;
		points[i].y = y;

		if ((x == 0) || (x == cols-1) ||(y == 0) || (y == rows-1) ) {
			points[i].time = 0;
			points[i].known = true;
			binContours.at<uchar>(y, x) = 0;
		}
		else if ((x == 1) || (y == 1) || (x == cols - 2) || (y == rows - 2)) {
			points[i].time = 1. / speed(x, y);
			points[i].known = false;
			frontPoints.insert(&points[i]);
		}
		else {
			points[i].time = DBL_MAX;
			points[i].known = false;
		}

	}
}

template<typename T>
void FastMarchingAlgorithm<T>::initializeSpeed(double alpha, double beta)
{
	this->speed = FunctionOfSpeed<T>(&gradient);
	speed.setAlpha(alpha);
	speed.setBeta(beta);
}

template<typename T>
void FastMarchingAlgorithm<T>::computeTimeForPoint(FMPoint * point)
{
	if (point->time != DBL_MAX) { return; }
	FMPoint *left = getLeftPoint(point),
		    *right = getRightPoint(point),
		    *up = getUpPoint(point),
		    *down = getDownPoint(point);
	QuadraticCoefficients coeff;

	if (left->known) { increaseCoefficients(coeff, left->time); }
	if (right->known) { increaseCoefficients(coeff, right->time); }
	if (up->known) { increaseCoefficients(coeff, up->time); }
	if (down->known) { increaseCoefficients(coeff, down->time); }

	double point_speed = speed(point->x, point->y);

	coeff.c -= ((1.) / (point_speed*point_speed));
	point->time = solveQuadratic(coeff);
	if (point->time > maxTime) { maxTime = point->time; }
	frontPoints.insert(point);
}

template<typename T>
FMPoint * FastMarchingAlgorithm<T>::getLeftPoint(FMPoint * point)
{
	if (point->x == 0) { return nullptr; }
	return getPoint(point->x-1, point->y);
}

template<typename T>
FMPoint * FastMarchingAlgorithm<T>::getRightPoint(FMPoint * point)
{
	if (point->x == (cols - 1)) { return nullptr; }
	return getPoint(point->x+1, point->y);
}

template<typename T>
FMPoint * FastMarchingAlgorithm<T>::getUpPoint(FMPoint * point)
{
	if (point->y == 0) { return nullptr; }
	return getPoint(point->x, point->y-1);
}

template<typename T>
FMPoint * FastMarchingAlgorithm<T>::getDownPoint(FMPoint * point)
{
	if(point->y == rows-1) { return nullptr; }
	return getPoint(point->x, point->y + 1);
}



