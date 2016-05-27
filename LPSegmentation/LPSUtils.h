#pragma once
#include "stdafx.h"

#include <io.h>
#include <vector>
#include <string>
#include <list>

#include "SegmentMetric.h"

using namespace std;
using namespace cv;

Rect rectsIntersection(Rect a, Rect b);
SegmentMetric getMetricForTwoSegments(Rect correctSegment, Rect algSegment);
list<string> getFileList(string path, string mask, bool recursive);
string getFileName(string* path);
string getFileDirrectory(string* path);

vector<Rect> readRectsFromXML(string path);
void writeRectsToXML(vector<Rect> rects, string path, string name);
bool pointInRect(Rect rect, int x, int y);

void addInRect(Mat & img, Rect rect, Scalar vec);
Rect normRect(Rect rect);
