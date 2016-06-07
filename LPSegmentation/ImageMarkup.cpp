#include "ImageMarkup.h"

ImageMarkup::ImageMarkup(Mat img, string _name) : name(_name)
{
	this->image = img.clone();
}

ImageMarkup::~ImageMarkup()
{
}

int ImageMarkup::mark()
{
	namedWindow(this->name, CV_WINDOW_NORMAL);
	resizeWindow(this->name, 1280, 720);
	setMouseCallback(this->name, mouseActionCallback, (void *) this);

	draw();

	int answer = RESULT_CLOSE;
	bool answered = false;
	while (!answered) {
		char key = waitKey(250);
		switch (key)
		{
		case KEY_NEXT_ARROW:
			answer = RESULT_NEXT_IMG;
			answered = true;
			break;
		case KEY_PREV_ARROW:
			answer = RESULT_PREV_IMG;
			answered = true;
			break;
		case KEY_ESC:
			answered = true;
			break;
		}

		if (!(cvGetWindowHandle(this->name.data()))) {
			break;	
		}
	}
	
	destroyWindow(this->name);
	
	return answer;    
}

void ImageMarkup::save(string path)
{   
	writeRectsToXML(this->rectangles, path, this->name);
}

vector<Rect> ImageMarkup::getRectangles()
{
	return this->rectangles;
}

void ImageMarkup::mouseActionProcessor(int evnt, int x, int y)
{
	switch (evnt)
	{
	case CV_EVENT_LBUTTONDOWN :
		curr = Rect(x, y, 0, 0);
		break;
	case CV_EVENT_LBUTTONUP :
		addNewRectangle();
		draw();
		break;
	case CV_EVENT_MOUSEMOVE :
		curr.width = x - curr.x;
		curr.height = y - curr.y;
		draw();
		break;
	case CV_EVENT_RBUTTONDOWN :
		deleteRectByPoint(x, y);
		draw();
		break;
	}

}

void ImageMarkup::draw()
{
	Mat drawingImg = this->image.clone();
	
	for (int i = 0; i < rectangles.size(); i++) {
		addInRect(drawingImg, rectangles[i],CV_RGB(0, 60, 0));
	}

	if (curr.width != 0 && curr.height != 0) {
		addInRect(drawingImg, normRect(curr), CV_RGB(0, 60, 0));
	}

	imshow(this->name, drawingImg);
}

void ImageMarkup::deleteRectByPoint(int x, int y)
{
	int initValue = rectangles.size() - 1;
	for (int i = initValue; i >= 0; i--) {
		if ( pointInRect(rectangles[i], x, y) ) {
			rectangles.erase(rectangles.begin() + i);
			break;
		}
	}
}

void ImageMarkup::addNewRectangle()
{
	Rect newRect = normRect(curr);
	if (curr.width == 0 || curr.height == 0) { return; }
	curr = Rect(0, 0, 0, 0);
	this->rectangles.push_back(newRect);
}

void mouseActionCallback(int evnt, int x, int y, int flags, void * param)
{
	ImageMarkup * imgMark = (ImageMarkup *) param;
	if (evnt == CV_EVENT_MOUSEMOVE && flags & 1 ||
		evnt == CV_EVENT_LBUTTONDOWN            ||
		evnt == CV_EVENT_LBUTTONUP              ||
		evnt == CV_EVENT_RBUTTONDOWN) 
	{
		imgMark->mouseActionProcessor(evnt, x, y);
	}
}