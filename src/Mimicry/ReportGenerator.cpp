/**
 * \file ReportGenerator.cpp
 */

#include "ReportGenerator.h"

#include <iostream>
#include <iomanip>

/**
 * The purpose of this function is to calculate the number 
 * of Mimicry ring present in the simulation and find 
 * count of the number of species present in each ring.
 * \param cells, 3D Array containing all the cells in the \a Model
 */
void ReportGenerator::generateMimicryRingReport(Cell cells[][ISIZE][ISIZE])
{
	//Initially set all count over the number of prey to zero.
	for (std::list<Ring>::iterator ringIter = rings.begin(); 
		ringIter != rings.end(); ringIter++)
	{
		ringIter->noOfPatterns = 0;
		ringIter->palatable = 0;
		ringIter->unpalatable = 0;
	}

	//Iterate over all the cells
	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				std::list<Agent*> agentList = cells[i][j][k].getAgentList();
				std::list<Agent*>::iterator agentIter = agentList.begin();
				//Iterate over all the agents in each \a Cell
				while( agentIter != agentList.end() )
				{
					Agent* agent = *agentIter;
					if (agent->getState() == Agent::ALIVE 
						&& agent->getAgentType() == Agent::PREY)
					{
						Prey* prey = static_cast<Prey*>(agent);
						bool ringFound = false;
						for (std::list<Ring>::iterator ringIter = rings.begin(); 
							ringIter != rings.end(); ringIter++)
						{
							//If \a Prey is within close hamming distance with the standard pattern
							//then incease count.
							if (calculateHammingDistance(ringIter->pattern, prey->pattern) 
								<= System::MIMICRY_RING_HAMMING_DIST)
							{
								ringIter->noOfPatterns++;
								if(prey->isPalatable())
									ringIter->palatable++;
								else
									ringIter->unpalatable++;
								ringFound = true;
							}
						}
						//No ring found for the Prey species. So create new ring.
						if(!ringFound)
						{
							Ring ring;
							ring.pattern = prey->pattern;
							ring.noOfPatterns = 1;
							if(prey->isPalatable())
							{
								ring.palatable = 1;
								ring.unpalatable = 0;
							} else {
								ring.palatable = 0;
								ring.unpalatable = 1;
							}
							rings.push_back(ring);
						}
					}
					agentIter++;
				}
			}
	//Sort ring count according to number of agents in each ring. Descending order.
	rings.sort(SortRingFunctor());

	//Remove rings which has zero number of species.
	if(rings.size() > 0)
	{
		std::list<Ring>::iterator ringIter = rings.end();
		ringIter--;
		while(ringIter->noOfPatterns == 0)
		{
			rings.pop_back();
			if(rings.size() > 0)
			{
				ringIter = rings.end();
				ringIter--;
			}
			else
				break;
		}
	}
}

/**
 * Calculate the hamming distance between two patterns.
 * \param pattern1, CAPattern to calculate distance with.
 * \param pattern2, other CAPattern to compare distance with.
 */
int ReportGenerator::calculateHammingDistance(CAPattern pattern1, CAPattern pattern2)
{
	int distance = 0;
	for (int w = 0; w < PATTERN_SIZE; w++)
		for (int l = 0; l < PATTERN_SIZE; l++)
			if(pattern1.get(w, l) != pattern2.get(w, l))
				distance++;
	return distance;
}

/**
 * Print the list of rings and the number of species in each ring into console
 */
void ReportGenerator::printMimicryRingReport()
{
	std::cout << "Rings.size(): " << rings.size() << std::endl;
	for (std::list<Ring>::iterator ringIter = rings.begin(); 
		ringIter != rings.end(); ringIter++)
	{
		std::cout << "Rule: " << std::setw(4) << (*ringIter).pattern.getCARule() 
			<< " -> " << std::setw(4) << (*ringIter).noOfPatterns
			<< " palatable: " << std::setw(4) << (*ringIter).palatable
			<< " unpalatable: " << std::setw(4) << (*ringIter).unpalatable << std::endl;
	}
}