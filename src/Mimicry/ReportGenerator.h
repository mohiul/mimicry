/**
 * \file ReportGenerator.h
 */

#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "System.h"
#include "Cell.h"
#include "CAPattern.h"

#include <list>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <string>

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
	Ring()
	{
		noOfPatterns = 0;
		palatable = 0;
		unpalatable = 0;
	}
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
	 * Simulation time vs. prey ring population map.
	 */
	std::map<long, std::list<Ring>> ringHistoryMap;

	/**
	 * Simulation time vs. predator population map.
	 */
	std::map<long, int> predatorPopHistoryMap;

	std::ofstream logfile;

	/**
	 * This structure is used to sort the list of mimicry rings according to their
	 * number of patterns.
	 */
	struct SortRingFunctorByPopulation
	{
	  bool operator()( const Ring a, const Ring b )
	  { return a.noOfPatterns > b.noOfPatterns; }
	};

	/**
	 * This structure is used to sort the list of mimicry rings according to their
	 * number of patterns.
	 */
	//struct SortRingFunctorByPattern
	//{
	//  bool operator()( const Ring a, const Ring b )
	//  { return a.pattern.getCARule() > b.pattern.getCARule(); }
	//};

	int calculateHammingDistance(CAPattern pattern1, CAPattern pattern2);
public:
	void generateMimicryRingReport(Cell cells[][ISIZE][ISIZE], long simTime);
	void printMimicryRingReport();
	void writeMimicryRingReport();
	void createFile();
};

#endif