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
	std::map<int, int> caRuleTotalPopMap;
	std::multimap<int, int> totalPopCARuleMap;
	std::set<int> caRuleSet;
	std::set<int> caRuleToRemoveSet;

	std::map<long, std::list<Ring>>::iterator ringMapIter;

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

	if( caRuleTotalPopMap.size() > System::NUMBER_OF_RINGS_TO_REPORT)
	{
		std::map<int, int>::iterator mapItr;
		for(mapItr = caRuleTotalPopMap.begin();
			mapItr != caRuleTotalPopMap.end();
			mapItr++)
			totalPopCARuleMap.insert(std::make_pair(mapItr->second, mapItr->first));

		int ringsToRemove = caRuleTotalPopMap.size() - System::NUMBER_OF_RINGS_TO_REPORT;
		
		std::multimap<int, int>::iterator multiMapItr = totalPopCARuleMap.begin();
		for(int r = 0; r < ringsToRemove; r++)
		{
			caRuleToRemoveSet.insert(multiMapItr->second);
			multiMapItr++;
		}
	}

	//Create log file to write.
	createFile();

	//Write in the log file.
	//std::map<long, std::map<int, Ring>>::iterator tempMapIter;
	for(tempMapIter = tempRingHistoryMap.begin(); 
		tempMapIter != tempRingHistoryMap.end(); 
		++tempMapIter )
	{
		logfile << std::setw(5) << tempMapIter->first << " ";
		std::map<int, Ring> ringMap = tempMapIter->second;

		std::map<int, Ring>::iterator ringMapIter;
		for(ringMapIter = ringMap.begin(); 
			ringMapIter != ringMap.end(); 
			++ringMapIter)
		{
			if( caRuleToRemoveSet.find(ringMapIter->first) == caRuleToRemoveSet.end() )
			{
				logfile << std::setw(3) << ringMapIter->first << " " 
					//<< std::setw(3) << ringMapIter->second.noOfPatterns << " "
					<< std::setw(3) << ringMapIter->second.palatable << " " 
					<< std::setw(3) << ringMapIter->second.unpalatable << " ";
			}
		}
		logfile << std::endl;
	}
	logfile.close();
}

/**
 * Write information about Mimicry rings in a log file to generate reports.
 */
//void ReportGenerator::writeMimicryRingReport()
//{	
//	std::map<long, std::map<int, Ring>> tempRingHistoryMap;
//	std::set<int> allPatterns;
//	std::map<int, int> ringTotalPopMap;
//	std::set<int> totalPopSet;
//
//	std::map<long, std::list<Ring>>::iterator iter;
//
//	//Iterate over existing Ring History Map to find a unique Set of all patterns.
//	//Also calculate total ring population to find most populated rings.
//	for( iter = ringHistoryMap.begin(); iter != ringHistoryMap.end(); ++iter )
//	{
//		std::list<Ring> ringList = iter->second;
//		std::list<Ring>::iterator ringIter;
//		//Iterate over ring list for every instance of time
//		for (ringIter = ringList.begin(); ringIter != ringList.end(); ringIter++)
//		{
//			int rule = ringIter->pattern.getCARule();
//			allPatterns.insert(ringIter->pattern.getCARule());
//			
//			//Calculate total population of each ring
//			if(ringTotalPopMap.find(rule) != ringTotalPopMap.end())
//				ringTotalPopMap[rule] += ringIter->noOfPatterns;
//			else
//				ringTotalPopMap[rule] = ringIter->noOfPatterns;
//		}
//	}
//
//	std::cout << "allPatterns.size(): " << allPatterns.size() << std::endl;
//
//	//Sort ring according to total population
//	std::map<int, int>::iterator totPopItr;
//
//	for( totPopItr = ringTotalPopMap.begin(); totPopItr != ringTotalPopMap.end(); ++totPopItr )
//		totalPopSet.insert( totPopItr->second );
//
//	int noOfRingsToRemove = totalPopSet.size() - System::NUMBER_OF_RINGS_TO_REPORT;
//	
//	std::cout << "noOfRingsToRemove: " << noOfRingsToRemove << std::endl;
//
//	if(noOfRingsToRemove > 0)
//	{
//		std::set<int>::iterator totPopSetItr = totalPopSet.begin();
//		for(int i = 0; i < noOfRingsToRemove; i++)
//		{
//			for( totPopItr = ringTotalPopMap.begin(); totPopItr != ringTotalPopMap.end(); ++totPopItr )
//				if(totPopItr->second == *totPopSetItr)
//				{
//					allPatterns.erase(totPopItr->first);
//					break;
//				}
//			totPopSetItr++;
//		}
//	}
//
//	std::cout << "allPatterns.size(): " << allPatterns.size() << std::endl;
//
//	//Iterate over existing Ring History Map to create another map which is 
//	// suitable for writting in the log file.
//	for( iter = ringHistoryMap.begin(); iter != ringHistoryMap.end(); ++iter )
//	{
//		long simTime = iter->first;
//		std::list<Ring> ringList = iter->second;
//		
//		std::map<int, Ring> newRingMap;
//
//		std::set<int>::iterator patternIter;
//		for (patternIter = allPatterns.begin(); patternIter != allPatterns.end(); patternIter++)
//			newRingMap[*patternIter] = Ring();
//
//		std::list<Ring>::iterator ringIter;
//		for (ringIter = ringList.begin(); ringIter != ringList.end(); ringIter++)
//			newRingMap[ringIter->pattern.getCARule()] = *ringIter;
//
//		tempRingHistoryMap[simTime] = newRingMap;
//	}
//
//	//Create log file to write.
//	createFile();
//
//	//Write in the log file.
//	std::map<long, std::map<int, Ring>>::iterator tempMapIter;
//	for( tempMapIter = tempRingHistoryMap.begin(); tempMapIter != tempRingHistoryMap.end(); ++tempMapIter )
//	{
//		logfile << std::setw(5) << tempMapIter->first << " ";
//		std::map<int, Ring> ringMap = tempMapIter->second;
//
//		std::map<int, Ring>::iterator ringMapIter;
//		for(ringMapIter = ringMap.begin(); ringMapIter != ringMap.end(); ++ringMapIter)
//		{
//			logfile << std::setw(3) << ringMapIter->first << " " 
//				//<< std::setw(3) << ringMapIter->second.noOfPatterns << " "
//				<< std::setw(3) << ringMapIter->second.palatable << " " 
//				<< std::setw(3) << ringMapIter->second.unpalatable << " ";
//		}
//		logfile << std::endl;
//	}
//	logfile.close();
//}

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