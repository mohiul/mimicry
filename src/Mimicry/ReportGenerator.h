#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "System.h"
#include "Cell.h"
#include "CAPattern.h"

#include <list>

struct Ring
{
	CAPattern pattern;
	int noOfPatterns;
};

class ReportGenerator
{
public:
	void generateMimicryRingReport(Cell ***cells);
private:
	int calculateHammingDistance(CAPattern pattern1, CAPattern pattern2);

	std::list<Ring> rings;
};

#endif