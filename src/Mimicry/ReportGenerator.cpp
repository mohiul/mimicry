#include "ReportGenerator.h"

void ReportGenerator::generateMimicryRingReport(Cell ***cells)
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
						for (std::list<Ring>::iterator ringIter = rings.begin(); 
							ringIter != rings.end(); ringIter++)
						{
							bool ringFound = false;
							if (calculateHammingDistance((*ringIter).pattern, prey->pattern) 
								<= System::MIMICRY_RING_HAMMING_DIST)
							{
								(*ringIter).noOfPatterns++;
								ringFound = true;
							}
							//No ring found for the Prey species. So create new ring.
							if(!ringFound)
							{
								Ring ring;
								ring.pattern = prey->pattern;
								ring.noOfPatterns = 1;
								rings.push_back(ring);
							}
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
	return 0;
}