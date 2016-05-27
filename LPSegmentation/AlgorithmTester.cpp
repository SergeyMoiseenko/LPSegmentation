#include "AlgorithmTester.h"

AlgorithmTester::AlgorithmTester(TestedAlgorithm & _alg) : alg(_alg)
{
}

AlgorithmTester::~AlgorithmTester()
{
}

vector<Rect> AlgorithmTester::test(Mat & img, vector<Rect> correctSegmentation, SegmentationEvaluation & outEval)
{
	vector<Rect> algorithmSegmentation = alg.doSegmentation(img);
	SegmentationEvaluation se = evaluateSegmentationResult(algorithmSegmentation, correctSegmentation);
	this->results.push_back(se);
	outEval = se;
	return algorithmSegmentation;
}

SegmentationEvaluation AlgorithmTester::evaluateSegmentationResult(vector<Rect> algSegm, vector<Rect> correctSegm)
{
	vector<SegmentMetric> correctFoundSegmentMetrics;
	vector<Rect> correctFoundSegments;

	for (int i = 0; i < correctSegm.size(); i++) {
		for (int j = 0; j < algSegm.size(); j++) {
			SegmentMetric sr = getMetricForTwoSegments(correctSegm[i], algSegm[j]);
			if (sr.tp != 0) {
				double tpAlgPercent = (sr.tp * 1.0) / (sr.tp + sr.fp);
				double tpCorrectPercent = (sr.tp * 1.0) / (sr.tp + sr.fn);
				if (tpAlgPercent > 0.75 && tpCorrectPercent > 0.85) {
					correctFoundSegmentMetrics.push_back(sr);
					correctFoundSegments.push_back(algSegm[j]);
					break;
				}
			}
		}
	}

	SegmentationEvaluation result(correctFoundSegmentMetrics, correctFoundSegments, 
		                          algSegm.size(), correctSegm.size());
	return result;
}
