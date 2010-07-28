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

	//Initialize predator population.
	int totalPredatorPop = 0;

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
				totalPredatorPop += cells[i][j][k].getPop(Agent::PREDATOR);
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

	//Store Prey history
	ringHistoryMap[simTime] = rings;
	//Store Ring size history
	ringSizeHistoryMap[simTime] = rings.size();

	//Store Predator history
	predatorPopHistoryMap[simTime] = totalPredatorPop;
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
 * Write information about Mimicry rings in a log file to generate reports.
 */
void ReportGenerator::writeMimicryRingReport()
{	
	std::map<long, std::map<int, Ring>> tempRingHistoryMap;
	std::map<int, int> caRuleTotalPopMap;
	std::multimap<int, int> totalPopCARuleMap;
	std::set<int> caRuleSet;
	std::set<int> caRuleToRemoveSet;

	std::map<long, std::list<Ring>>::iterator ringMapIter;

	//Create the tempRingHistoryMap from the existing ringHistoryMap.
	//In the prcess also create a Cellular Automata Rule set.
	for(ringMapIter = ringHistoryMap.begin(); 
		ringMapIter != ringHistoryMap.end(); 
		ringMapIter++)
	{
		long simTime = ringMapIter->first;
		std::list<Ring> ringList = ringMapIter->second;
		std::map<int, Ring> ringPopCAMap;

		std::list<Ring>::iterator ringListIter;
		for(ringListIter = ringList.begin(); 
			ringListIter != ringList.end(); 
			ringListIter++)
		{
			int caRule = ringListIter->pattern.getCARule();
			ringPopCAMap[caRule] = *ringListIter;
			caRuleSet.insert(caRule);
		}
		tempRingHistoryMap[simTime] = ringPopCAMap;
	}

	//Insert zero values to all the caRule which are not present at 
	// all instance of time.
	std::map<long, std::map<int, Ring>>::iterator tempMapIter;
	for(tempMapIter = tempRingHistoryMap.begin(); 
		tempMapIter != tempRingHistoryMap.end(); 
		++tempMapIter )
	{
		long simTime = tempMapIter->first;
		std::map<int, Ring> ringMap = tempMapIter->second;

		std::set<int>::iterator setItr;
		for(setItr = caRuleSet.begin();
			setItr != caRuleSet.end();
			setItr++)
		{
			int caRule = *setItr;
			if( ringMap.find(caRule) == ringMap.end())
				ringMap[caRule] = Ring();
		}

		//Create the caRuleTotalPopMap, which is used to find the 
		// first few sorted list of rings, and remove the list of rings
		// which are too small.
		std::map<int, Ring>::iterator mapItr;
		for(mapItr = ringMap.begin();
			mapItr != ringMap.end();
			mapItr++)
		{
			int caRule = mapItr->first;
			if(caRuleTotalPopMap.find(caRule) != caRuleTotalPopMap.end())
				caRuleTotalPopMap[caRule] += mapItr->second.noOfPatterns;
			else
				caRuleTotalPopMap[caRule] = 0;
		}

		tempRingHistoryMap[simTime] = ringMap;
	}

	//If total number of rings is more than the number of rings to report
	// then reduce the list of rings size. 
	if( caRuleTotalPopMap.size() > System::NUMBER_OF_RINGS_TO_REPORT)
	{
		//Create the totalPopCARuleMap, so we can select only the 
		// most populated rings.
		std::map<int, int>::iterator mapItr;
		for(mapItr = caRuleTotalPopMap.begin();
			mapItr != caRuleTotalPopMap.end();
			mapItr++)
			totalPopCARuleMap.insert(std::make_pair(mapItr->second, mapItr->first));

		int ringsToRemove = caRuleTotalPopMap.size() - System::NUMBER_OF_RINGS_TO_REPORT;
		
		//Create caRuleToRemoveSet, which contains the rings which are list populated.
		std::multimap<int, int>::iterator multiMapItr = totalPopCARuleMap.begin();
		for(int r = 0; r < ringsToRemove; r++)
		{
			caRuleToRemoveSet.insert(multiMapItr->second);
			multiMapItr++;
		}
	}

	//Create log file to write.
	createFile("..\\..\\report\\logs", &logfile);

	//Create log file to write.
	createFile("..\\..\\report\\rings", &ringfile);

	//Write in the log file.
	//std::map<long, std::map<int, Ring>>::iterator tempMapIter;
	for(tempMapIter = tempRingHistoryMap.begin(); 
		tempMapIter != tempRingHistoryMap.end(); 
		++tempMapIter )
	{
		//Write simulation time
		logfile << std::setw(5) << tempMapIter->first << " ";
		//Write predatory population
		logfile << std::setw(4) << predatorPopHistoryMap[tempMapIter->first] << " ";

		//Write simulation time in ring size log
		ringfile << std::setw(5) << tempMapIter->first << " ";
		//Write ring size in ring size log
		ringfile << std::setw(3) << ringSizeHistoryMap[tempMapIter->first] << std::endl;

		std::map<int, Ring> ringMap = tempMapIter->second;

		std::map<int, Ring>::iterator ringMapIter;
		for(ringMapIter = ringMap.begin(); 
			ringMapIter != ringMap.end(); 
			++ringMapIter)
		{
			//Check if ring is one of the least populated ring.
			if( caRuleToRemoveSet.find(ringMapIter->first) == caRuleToRemoveSet.end() )
			{
				logfile << std::setw(3) << ringMapIter->first << " " 
					//<< std::setw(4) << ringMapIter->second.noOfPatterns << " "
					<< std::setw(4) << ringMapIter->second.palatable << " " 
					<< std::setw(4) << ringMapIter->second.unpalatable << " ";
			}
		}
		logfile << std::endl;
	}
	std::cout << "Log file generated " << std::endl;
	logfile.close();
	std::cout << "Ring file generated " << std::endl;
	ringfile.close();
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

void ReportGenerator::createFile(std::string fileName, std::ofstream *ofile)
{
	time_t lt = time(0);
	struct tm *ptr = localtime(&lt);
	int h = ptr->tm_hour;
	if (ptr->tm_isdst)
	  h++;

	std::ostringstream os;
	os.fill('0');
	os << fileName << "." <<
	  std::setw(4) << ptr->tm_year+1900 <<
	  std::setw(2) << ptr->tm_mon <<
	  std::setw(2) << ptr->tm_mday << '.' <<
	  std::setw(2) << ptr->tm_hour <<
	  std::setw(2) << ptr->tm_min <<
	  std::setw(2) << ptr->tm_sec << ".dat";

	ofile->open(os.str().c_str());
}