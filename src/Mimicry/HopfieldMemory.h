/**
 * \file HopfieldMemory.h
 */

#ifndef HOPFIELD_MEMORY_H
#define HOPFIELD_MEMORY_H

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "CAPattern.h"

#include <list>

using namespace boost::numeric::ublas;

/**
  * A structure that contains a single chunk of memory. 
  * It stores one pattern in serialized form and associate that pattern with palatability.
  * \a HopfieldMemory class contains a list of these memory chunks.
  */
struct Memory
{
	Memory(int _width, int _length)
	{
		pattern = new vector<int>(_width*_length);
	}

	~Memory()
	{
		delete pattern;
	}

	vector<int> *pattern;
	bool palatability;
};

/**
 * An instance of \a HopfieldMemory.
 * This class is intended to store chunks of memory for Predator species. Each 
 * predator contains one instance of this class, with a list of pattern in its memory.
 * After the initial training period to store memory, this class is used to recognize
 * similarity between patterns that the predator encounter.
 */
class HopfieldMemory
{
private:
   /**
     * Width of each pattern stored in memory.
     */
	int patternWidth;

   /**
     * Length of each pattern stored in memory.
     */
	int patternLength;

	/**
     * Hopefield Network weight matrix.
     */
	matrix<int>* weights;

	/**
     * List of \a Memory chuck of the containing pattern and palatability.
     */
	std::list<Memory> memoryList;

public:
	HopfieldMemory(const int width, const int length);
	~HopfieldMemory();
	void calculateWeights();
	void addPattern(CAPattern* pattern, bool palatability);
	Memory *searchMemory(vector<int>* pattern);
	Memory *recognize(CAPattern* pattern);
	int getMemorySize() {return (unsigned) memoryList.size();}
	void printInputPattern(vector<int> inputPattern);
};

#endif