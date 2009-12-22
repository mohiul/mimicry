/**
 * \file ReportGenerator.cpp
 */

#include "ReportGenerator.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <stdlib.h>

/**
 * The purpose of this function is to calculate the number 
 * of Mimicry ring present in the simulation and find 
 * count of the number of species present in each ring.
 * \param cells, 3D Array containing all the cells in the \a Model
 */
void ReportGenerator::generateMimicryRingReport(Cell cells[][ISIZE][ISIZE], long simTime)
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
								ring.unpalatable = 1;
								ring.palatable = 0;
							}
							rings.push_back(ring);
						}
					}
					agentIter++;
				}
			}
	//Sort ring count according to number of agents in each ring. Descending order.
	rings.sort(SortRingFunctorByPopulation());

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
	storeHistory(simTime);
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
void ReportGenerator::storeHistory(long simTime)
{
	ringHistoryMap[simTime] = rings;
}

/**
 * Write information about Mimicry rings in a log file to generate reports.
 */
void ReportGenerator::writeMimicryRingReport()
{	
	std::map<long, std::map<int, Ring>> tempRingHistoryMap;
	std::set<int> allPatterns;

	std::map<long, std::list<Ring>>::iterator iter;

	for( iter = ringHistoryMap.begin(); iter != ringHistoryMap.end(); ++iter )
	{
		std::list<Ring> ringList = iter->second;
		std::list<Ring>::iterator ringIter;
		for (ringIter = ringList.begin(); ringIter != ringList.end(); ringIter++)
			allPatterns.insert(ringIter->pattern.getCARule());
	}

	for( iter = ringHistoryMap.begin(); iter != ringHistoryMap.end(); ++iter )
	{
		long simTime = iter->first;
		std::list<Ring> ringList = iter->second;
		
		std::map<int, Ring> newRingMap;

		std::set<int>::iterator patternIter;
		for (patternIter = allPatterns.begin(); patternIter != allPatterns.end(); patternIter++)
			newRingMap[*patternIter] = Ring();

		std::list<Ring>::iterator ringIter;
		for (ringIter = ringList.begin(); ringIter != ringList.end(); ringIter++)
			newRingMap[ringIter->pattern.getCARule()] = *ringIter;

		tempRingHistoryMap[simTime] = newRingMap;
	}

	std::map<long, std::map<int, Ring>>::iterator tempMapIter;

	createFile();
	
	for( tempMapIter = tempRingHistoryMap.begin(); tempMapIter != tempRingHistoryMap.end(); ++tempMapIter )
	{
		logfile << std::setw(5) << tempMapIter->first << " ";
		std::map<int, Ring> ringMap = tempMapIter->second;

		std::map<int, Ring>::iterator ringMapIter;
		for(ringMapIter = ringMap.begin(); ringMapIter != ringMap.end(); ++ringMapIter)
		{
			logfile << std::setw(3) << ringMapIter->first << " " 
				//<< std::setw(3) << ringMapIter->second.noOfPatterns << " "
				<< std::setw(3) << ringMapIter->second.palatable << " " 
				<< std::setw(3) << ringMapIter->second.unpalatable << " ";
		}
		logfile << std::endl;
	}
	logfile.close();
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

void ReportGenerator::createFile()
{
	time_t lt = time(0);
	struct tm *ptr = localtime(&lt);
	int h = ptr->tm_hour;
	if (ptr->tm_isdst)
	  h++;

	std::ostringstream os;
	os.fill('0');
	os << "log." <<
	  std::setw(4) << ptr->tm_year+1900 <<
	  std::setw(2) << ptr->tm_mon <<
	  std::setw(2) << ptr->tm_mday << '.' <<
	  std::setw(2) << ptr->tm_hour <<
	  std::setw(2) << ptr->tm_min <<
	  std::setw(2) << ptr->tm_sec << ".dat";

	logfile.open(os.str().c_str());
}