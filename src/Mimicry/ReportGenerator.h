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
	int palatable;
	int unpalatable;
};

class ReportGenerator
{
public:
	void generateMimicryRingReport(Cell cells[][ISIZE][ISIZE]);
	void printMimicryRingReport();
private:

	struct SortRingFunctor
	{
	  bool operator()( const Ring a, const Ring b )
	  { return a.noOfPatterns > b.noOfPatterns; }
	};

	//void sortRingsBySize();
	int calculateHammingDistance(CAPattern pattern1, CAPattern pattern2);

	std::list<Ring> rings;
};

#endif