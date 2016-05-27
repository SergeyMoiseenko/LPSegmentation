#include "LPSUtils.h"

Rect rectsIntersection(Rect a, Rect b)
{
	Rect intersection(0, 0, 0, 0);
	int a_x = a.x,
		a_y = a.y,
		a_x2 = a.x + a.width,
		a_y2 = a.y + a.height,
		b_x = b.x,
		b_y = b.y,
		b_x2 = b.x + b.width,
		b_y2 = b.y + b.height;

	if (a_x < b_x2 && a_x2 > b_x && a_y < b_y2 && a_y2 > b_y) {
		intersection.x = max(a_x, b_x);
		intersection.y = max(a_y, b_y);
		intersection.width = min(a_x2, b_x2) - intersection.x;
		intersection.height = min(a_y2, b_y2) - intersection.y;
	}

	return intersection;
}

SegmentMetric getMetricForTwoSegments(Rect correctSegment, Rect algSegment)
{
	SegmentMetric result;
	Rect intersection = rectsIntersection(correctSegment, algSegment);

	if (intersection.width != 0 && intersection.height != 0) {
		result.tp = intersection.area();
		result.fp = algSegment.area() - intersection.area();
		result.fn = correctSegment.area() - intersection.area();
	}

	return result;
}

list<string> getFileList(string path, string mask, bool recursive)
{
	struct _finddata_t fd;
	string findPath = path + "\\" + mask;
	string dirPath = path + "\\" + "*.*";
	list<string> res;

	intptr_t hFile = _findfirst(findPath.data(), &fd);
	if (hFile != -1L)
		do
		{
			if (fd.attrib != 0x00000010)
				res.push_back(path + "\\" + fd.name);
		} while (_findnext(hFile, &fd) == 0);
		hFile = _findfirst(dirPath.data(), &fd);
		if (hFile != -1L && recursive)
			do
			{
				if (fd.attrib == 0x00000010 && fd.name != string(".") && fd.name != string(".."))
				{
					list<string> dir_files = getFileList(path + "\\" + fd.name, mask, true);
					res.merge(dir_files);
				}

			} while (_findnext(hFile, &fd) == 0);
			return res;
}

string getFileName(string *path)
{
	size_t start = path->find_last_of("\\/") + 1;
	size_t finish = path->find_last_of(".");
	if (finish == string::npos)
		finish = path->length();
	return path->substr(start, finish - start);
}

string getFileDirrectory(string *path)
{
	return path->substr(0, path->find_last_of("\\/") + 1);
}

vector<Rect> readRectsFromXML(string path)
{
	FileStorage f(path, FileStorage::READ);
	vector<Rect> result;

	int size = 0;
	f["size"] >> size;
	for (int i = 0; i < size; i++) {
		string id = "rect_" + to_string(i);
		Rect r;
		f[id] >> r;
		result.push_back(r);
	}

	return result;
}

void writeRectsToXML(vector<Rect> rects, string path, string name)
{
	string fullName = path + "\\" + name + ".xml";
	FileStorage file(fullName, FileStorage::WRITE);
	file << "size" << (int) rects.size();
	for (int i = 0; i < rects.size(); i++) {
		Rect r = rects[i];
		file << ("rect_" + to_string(i)) << r;
	}

	file.release();
}

bool pointInRect(Rect rect, int x, int y)
{
	return (x >= rect.x && x < rect.x + rect.width && rect.y <= y && rect.y + rect.height > y);
}

void addInRect(Mat & img, Rect rect, Scalar vec)
{
	Mat area = img(rect);
	add(area, vec, area);
}

Rect normRect(Rect rect)
{
	int x, y, w, h;
	if (rect.width < 0)
	{
		x = rect.x + rect.width;
		w = -rect.width;
	}
	else
	{
		x = rect.x;
		w = rect.width;
	}
	if (rect.height < 0)
	{
		y = rect.y + rect.height;
		h = -rect.height;
	}
	else
	{
		y = rect.y;
		h = rect.height;
	}
	return Rect(x, y, w, h);
}

