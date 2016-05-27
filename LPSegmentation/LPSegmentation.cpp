// LPSegmentation.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "LPSUtils.h"
#include "TestFastMarching.h"
#include "ImageMarkup.h"
#include "AlgorithmTester.h"

using namespace std;
using namespace cv;

void runSegmentationAlgTest(string path );
void markupImages(string path);
bool readImage(string path, Mat & out);



int main()
{
	string path = "D:\\LPImages\\testMarkup";
	//markupImages(path);
	runSegmentationAlgTest(path);
    return 0;
}

void runSegmentationAlgTest(string path)
{
	list<string> imageFiles = getFileList(path, "*.jpg", false);

	if (imageFiles.size() == 0) {
		cout << "There are no images in this folder" << endl;
		return;
	}

	TestFastMarching tfm;
	AlgorithmTester at(tfm);
	
	for (auto it = imageFiles.begin(); it != imageFiles.end(); it++) {
		Mat img;
		readImage(*it, img);
		string name = getFileName(&(*it));
		string xmlFileName = path + "\\" + name + ".xml";
		vector<Rect> correctRects = readRectsFromXML(xmlFileName);
		SegmentationEvaluation eval;
		vector<Rect> algRes = at.test(img, correctRects, eval);

		for (int i = 0; i < algRes.size(); i++) {
			rectangle(img, algRes.at(i), Scalar(0, 255, 0));
		}

		for (int i = 0; i < correctRects.size(); i++) {
			rectangle(img, correctRects.at(i), Scalar(0, 0, 255));
		}

		string segmentedImageName = path + "\\" + name + "_segm" + ".jpg";
		imwrite(segmentedImageName, img);

		cout << "name: " << name << endl;
		cout  << "    " << "all segments: " << eval.getNumberOfFoundSegments() << endl;
		cout << "    " << "correct found segments: " << eval.getNumberOfCorrectFoundSegments() << endl;
		cout << "    " << "true number of segments: " << eval.getTrueNumberOfSegments() << endl;
		cout << endl;
	}


}

void markupImages(string path)
{
	list<string> imageFiles = getFileList(path, "*.jpg", false);

	if (imageFiles.size() == 0) {
		cout << "There are no images in this folder" << endl;
		return;
	}

	auto filesIterator  = imageFiles.begin();
	int nextAction = RESULT_NEXT_IMG;
	do {
		nextAction = RESULT_NEXT_IMG;
		Mat img;

		if (filesIterator == imageFiles.end()) {
			cout << "Done!" << endl;
			break;
		}

		if (readImage(*filesIterator, img)) {
			string name = getFileName(&(*filesIterator));
			ImageMarkup im(img, name);
			nextAction = im.mark();
			im.save(path);
		}
		
		switch (nextAction)
		{
		case RESULT_NEXT_IMG:
				filesIterator++;
			break;
		case RESULT_PREV_IMG:
			if (filesIterator != imageFiles.begin()) {
				filesIterator--;
			}
			break;
		case RESULT_CLOSE:
			break;
		}

	} while (nextAction != RESULT_CLOSE);
}

bool readImage(string path, Mat & out) {
	Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		cout << "Can't load image" << endl;
		return false;
	}
	out = img;
	return true;
}