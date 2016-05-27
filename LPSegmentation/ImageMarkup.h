#pragma once
#include "stdafx.h"
#include "LPSUtils.h"
#include <vector>
#include <iostream>

#define KEY_ESC 27
#define KEY_NEXT_ARROW 100
#define KEY_PREV_ARROW 97
#define KEY_ENTER 13

#define RESULT_NEXT_IMG 1
#define RESULT_CLOSE 0
#define RESULT_PREV_IMG -1

using namespace std;
using namespace cv;

class ImageMarkup
{
public:
	ImageMarkup(Mat img, string _name);
	~ImageMarkup();

	

	int mark();
	void save(string path);
	vector<Rect> getRectangles();
	void mouseActionProcessor(int evnt, int x, int y);

private: 	
	Mat image;
	string name;
	vector<Rect> rectangles;
	Rect curr;

	void draw();
	void deleteRectByPoint(int x, int y);
	void addNewRectangle();
};

void mouseActionCallback(int evnt, int x, int y, int flags, void * param);

