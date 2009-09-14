/**
 * \file HopfieldMemory.cpp
 */

#include <iostream>
#include "HopfieldMemory.h"
#include "Utility.h"

/**
 * Construct a \a HopfieldMemory with width and length of each pattern.
 */
HopfieldMemory::HopfieldMemory(const int width, 
							   const int length)
{
	patternWidth = width;
	patternLength = length;
}

/**
 * \return void
 * Calculate the weight matrix \a weights of Hopfield Netowrk using the patterns stored \a memoryList.
 * This function is called each time a memory is added to the list. 
 */
void HopfieldMemory::calculateWeights()
{
	weights = new matrix<int>(patternWidth*patternLength, patternWidth*patternLength);
	
    //Initialize the weight matrix with all zero.
	for (unsigned i = 0; i < weights->size1 (); ++ i)
        for (unsigned j = 0; j < weights->size2 (); ++ j)
			(*weights)(i, j) = 0;

    //Calculte the outer product of each pattern with its transpose 
	//and add it to the weight matrix.
	for (std::list<Memory>::iterator memoryIter = memoryList.begin(); 
		memoryIter != memoryList.end(); memoryIter++)
	{
		vector<int> pattern = *(*memoryIter).pattern;
		*weights += outer_prod(pattern, trans(pattern));
	}

	//Setting diagonal values to zero
    for (unsigned i = 0; i < weights->size1 (); ++ i)
        for (unsigned j = 0; j < weights->size2 (); ++ j)
			if (i == j)
				(*weights)(i, j) = 0;
}

/**
 * Add a new \a Memory chunck to existing list of memory, \a memoryList. 
 * Also calculate weight matrix for Hopfield Network.
 * \return void
 * \param pattern pointer to a newly found pattern \a CAPattern.
 * \param palatability Information regarding palatability gained after attacking the \a Prey.
 */
void HopfieldMemory::addPattern(CAPattern* pattern, bool palatability)
{
	vector<int> newPattern = *pattern->serialize();
	//Check if new pattern exists in memory
	if(!searchMemory(&newPattern))
	{
		Memory memory(PATTERN_SIZE, PATTERN_SIZE);
		*memory.pattern = newPattern;
		memory.palatability = palatability;
		memoryList.push_back(memory);
		calculateWeights();
	}
}


/**
 * Search list of \a Memory for existing pattern. 
 * \return Pointer instance of the structure \a Memory.
 * \param serializedPattern pointer to a serialized pattern of the prey species recently encountered.
 */
Memory *HopfieldMemory::searchMemory(vector<int>* serializedPattern)
{
	Memory *foundMemory = 0;
	for (std::list<Memory>::iterator memoryIter = memoryList.begin();
		memoryIter != memoryList.end(); memoryIter++)
		if(Utility::checkEquals(*serializedPattern, *(*memoryIter).pattern))
			foundMemory = &(*memoryIter);
	//if(foundMemory != 0)
	//	std::cout << "palatability: " << foundMemory->palatability << std::endl;
	return foundMemory;
}

/**
 * Recognize a pattern using the Hopfield Netowork memory.
 * \return Pointer instance of the structure \a Memory.
 * \param pattern Pointer to a \a CAPattern object. Pattern generated by Cellular Automata.
 */
Memory *HopfieldMemory::recognize(CAPattern* pattern)
{
	vector<int> Y = *pattern->serialize();
	//std::cout << "Y: " << std::endl;
	//printInputPattern(Y);
	int itr = 0;
	while(true)
	{
		vector<int> Y1 = Y;
		for (unsigned index = 0; index < Y1.size(); index++)
		{
			int sum = 0;			
			for (unsigned j = 0; j < weights->size1(); j++)
				sum += Y1(j)*(*weights)(j, index);

			if (sum > 0)
				Y(index) = 1;
			else
				Y(index) = -1;
		}
		if (Utility::checkEquals(Y, Y1) || itr > System::HOPFIELD_MAX_ITERATION)
			break;
		itr++;
	}
	//std::cout << "Later Y: " << std::endl;
	//printInputPattern(Y);
	return searchMemory(&Y);
}

/**
 * Print a serialied pattern into console in pretty format. Function used for debug purpose.
 * \return void
 * \param inputPattern Serialized pattern to be printed.
 */
void HopfieldMemory::printInputPattern(vector<int> inputPattern)
{
	for(int l = 0; l < patternLength; l++)
	{
		for(int w = 0; w < patternWidth; w++)
			if(inputPattern(l*patternWidth + w) == -1)
				std::cout << " 0";
			else
				std::cout << " 1";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
