/**
 * \file ReportGenerator.h
 */

#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "System.h"
#include "Cell.h"
#include "CAPattern.h"

#include <list>

/**
 * A structure that contains information of a single ring.
 * The pattern is the standard Cellular Automata pattern 
 * based on which the ring is created.
 * noOfPatterns, variable contains the number of pattern
 * which are at a close hamming distance with the standard
 * CAPattern. The other two variables contain count of the
 * number of palatable species and unpalatable species.
 */
struct Ring
{
	CAPattern pattern;
	int noOfPatterns;
	int palatable;
	int unpalatable;
};

/**
 * An instance of \a ReportGenerator.
 * This class is intended to store chunks of memory for Predator species. Each 
 * predator contains one instance of this class, with a list of pattern in its memory.
 * After the initial training period to store memory, this class is used to recognize
 * similarity between patterns that the predator encounter.
 */
class ReportGenerator
{
private:
	/**
	 * The list of rings which has similarity with the standard pattern.
	 */
	std::list<Ring> rings;

	/**
	 * This structure is used to sort the list of mimicry rings according to their
	 * number of patterns.
	 */
	struct SortRingFunctor
	{
	  bool operator()( const Ring a, const Ring b )
	  { return a.noOfPatterns > b.noOfPatterns; }
	};

	int calculateHammingDistance(CAPattern pattern1, CAPattern pattern2);
public:
	void generateMimicryRingReport(Cell cells[][ISIZE][ISIZE]);
	void printMimicryRingReport();
};

#endif