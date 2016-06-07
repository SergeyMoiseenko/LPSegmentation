#include "stdafx.h"
#include <functional>


template <typename T>
class FunctionOfSpeed : public std::binary_function<const int, const int, double>
{
public:
	FunctionOfSpeed();
	~FunctionOfSpeed();
	FunctionOfSpeed(cv::Mat *_gradient);

	double operator() (const int x, const int y);
	void setBeta(double _beta);
	void setAlpha(double _alpha);
private:
	cv::Mat *gradient;
	double beta;
	double alpha;
};

template<typename T>
FunctionOfSpeed<T>::FunctionOfSpeed()
{
}

template<typename T>
FunctionOfSpeed<T>::~FunctionOfSpeed()
{
}

template<typename T>
FunctionOfSpeed<T>::FunctionOfSpeed( cv::Mat *_gradient) : gradient(_gradient), alpha(1), beta(1)
{
}


template<typename T>
double FunctionOfSpeed<T>::operator() (const int x, const int y) {
	T magnitude = gradient->at<T>(y, x);
	return beta*(1. / (1 + alpha*magnitude));
}

template<typename T>
void FunctionOfSpeed<T>::setBeta(double _beta)
{
	this->beta = _beta;
}

template<typename T>
void FunctionOfSpeed<T>::setAlpha(double _alpha)
{
	this->alpha = _alpha;
}
