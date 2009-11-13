#include "ReportGenerator.h"

#include <iostream>
#include <iomanip>

void ReportGenerator::generateMimicryRingReport(Cell cells[][ISIZE][ISIZE])
{
	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				std::list<Agent*> agentList = cells[i][j][k].getAgentList();
				std::list<Agent*>::iterator agentIter = agentList.begin();
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
							rings.sort(Functor());
						}
					}
					agentIter++;
				}
			}
}

int ReportGenerator::calculateHammingDistance(CAPattern pattern1, CAPattern pattern2)
{
	int distance = 0;
	for (int w = 0; w < PATTERN_SIZE; w++)
		for (int l = 0; l < PATTERN_SIZE; l++)
			if(pattern1.get(w, l) != pattern2.get(w, l))
				distance++;
	return distance;
}

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