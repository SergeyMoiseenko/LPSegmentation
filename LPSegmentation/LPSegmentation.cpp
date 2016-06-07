// LPSegmentation.cpp: ���������� ����� ����� ��� ����������� ����������.
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
void segmentImages(string path);
bool readImage(string path, Mat & out);
int help();



int main(int argc, char** argv)
{
	string command = argv[1];
	if (argc == 2 && command == "help") { return help(); }
	string path = argv[2];
	if (command == "markup") {
		markupImages(path);
		return 0;
	}
	if (command == "test") {
		runSegmentationAlgTest(path);
		return 0;
	}
	if (command == "segment") {
		segmentImages(path);
		return 0;
	}
	cout << "�������� �������";
	return 0;
	//string path = "D:\\LPImages\\testMarkup";
	//markupImages(path);
	//runSegmentationAlgTest(path);
}

int help()
{
	cout << "1. ������� 'markup <path>' - ������ �������� ���� ����������� ����������� � <path>" << endl;
	cout << "2. ������� 'test <path>' - ������ ������������ ��������� ����������� �� ���� ������������, ����������� � ���� <path>" << endl;
	cout << "3. ������� 'segment <path>' - ������ ����������� ���� ����������� ����������� � <path>" << endl;
	return 0;
}

void runSegmentationAlgTest(string path)
{
	list<string> imageFiles = getFileList(path, "*.jpg", false);

	if (imageFiles.size() == 0) {
		cout << "��� ����������� � ���� �����." << endl;
		return;
	}

	TestFastMarching tfm;
	AlgorithmTester at(tfm);
	
	for (auto it = imageFiles.begin(); it != imageFiles.end(); it++) {
		Mat img;
		if (!readImage(*it, img)) {
			continue;
		}
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

		cout << "���: " << name << endl;
		cout  << "    " << "��� ��������: " << eval.getNumberOfFoundSegments() << endl;
		cout << "    " << "��������� ��������� ��������: " << eval.getNumberOfCorrectFoundSegments() << endl;
		cout << "    " << "���������� ���������� ���������: " << eval.getTrueNumberOfSegments() << endl;
		cout << endl;
	}

	cout << "���������!" << endl;
}

void markupImages(string path)
{
	list<string> imageFiles = getFileList(path, "*.jpg", false);

	if (imageFiles.size() == 0) {
		cout << "��� ����������� � ���� �����." << endl;
		return;
	}

	auto filesIterator  = imageFiles.begin();
	int nextAction = RESULT_NEXT_IMG;
	do {
		nextAction = RESULT_NEXT_IMG;
		Mat img;

		if (filesIterator == imageFiles.end()) {
			cout << "���������!" << endl;
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

void segmentImages(string path)
{
	list<string> imageFiles = getFileList(path, "*.jpg", false);

	if (imageFiles.size() == 0) {
		cout << "��� ����������� � ���� �����." << endl;
		return;
	}

	TestFastMarching tfm;	

	for (auto it = imageFiles.begin(); it != imageFiles.end(); it++) {
		Mat img;

		if (!readImage(*it, img)) {
			continue;
		}

		string name = getFileName(&(*it));

		vector<Rect> rectangles = tfm.doSegmentation(img);

		for (int i = 0; i < rectangles.size(); i++) {
			Mat segment = img(rectangles.at(i));
			string segmentedImageName = path + "\\" + name + "_segm_" + to_string(i) + ".jpg";
			imwrite(segmentedImageName, segment);
		}

		for (int i = 0; i < rectangles.size(); i++) {
			rectangle(img, rectangles.at(i), Scalar(0, 255, 0));
		}

		imwrite(path + "\\" + name + "_segmented.jpg", img);
	}

	cout << "���������!" << endl;
}

bool readImage(string path, Mat & out) {
	Mat img = imread(path, CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		cout << "�� ���� ��������� �����������." << endl;
		return false;
	}
	out = img;
	return true;
}