/**
 * \file Cell.cpp
 */

#include "Cell.h"
#include <cassert>
#include <iostream>

/**
 * Construct a \a Cell object.
 */
Cell::Cell(): population(0), preyPopulation(0), predatorPopulation(0)
{
}

/**
 * Set the current position of this Cell.
 * \param _position position of this Cell.
 */
void Cell::setPos(formal::Vector _position)
{
	position = _position;
}

/**
 * Insert an agent into this cell.
 * \param pa is a pointer to the new agent.
 */
void Cell::insert(Agent *pa)
{
	agentList.push_back(pa);
	population++;
	if (pa->getAgentType() == Agent::PREY)
		preyPopulation++;
	if (pa->getAgentType() == Agent::PREDATOR)
		predatorPopulation++;
}

/**
 * Delete an agent from this cell.
 * \param pa is a pointer to the agent to be deleted.
 */
void Cell::remove(Agent *pa)
{
	std::list<Agent*>::iterator agentIter = agentList.begin();
	bool found = false;
	while( agentIter != agentList.end() ) {
		std::list<Agent*>::iterator iter = agentIter;
		agentIter++;
		if(*iter == pa)
		{
			agentList.erase(iter);
			reducePop(pa->getAgentType());
			break;
		}
	}
}

/**
 * Outline this cell in the model.
 */
void Cell::draw()
{
	glColor3f(255, 255, 255);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutWireCube(System::CELL_SIZE);
	glPopMatrix();
}

/**
 * Get current population of a specific agent type.
 * \param agentType Type of the Agent.
 */

//int Cell::getPop(Agent::AGENT_TYPE agentType)
//{
//	int pop = 0;
//	for(std::list<Agent*>::iterator agentIter = agentList.begin(); 
//		agentIter != agentList.end(); agentIter++)
//		if((*agentIter)->getState() == Agent::ALIVE 
//			&& (*agentIter)->getAgentType() == agentType)
//			pop++;
//	return pop;
//}

int Cell::getPop(Agent::AGENT_TYPE agentType)
{
	unsigned int pop = 0;
	if(agentType == Agent::PREY)
		pop = preyPopulation;
	else if(agentType == Agent::PREDATOR)
		pop = predatorPopulation;
	return pop;
}

/**
 * Reduce population count of a specific type of agent.
 * \param agentType Type of the Agent.
 */
void Cell::reducePop(Agent::AGENT_TYPE agentType)
{
	if(agentType == Agent::PREDATOR)
	{
		assert((predatorPopulation -1) < 0);
		predatorPopulation--;
	}
	else if(agentType == Agent::PREY)
	{
		assert((preyPopulation -1) < 0);
		preyPopulation--;
		if (preyPopulation < 0)
			std::cout << "Alert!!! 1 Prey Population: " << preyPopulation << std::endl;
	}
	assert((population -1) < 0);
	population--;

}

std::list<Cell*> Cell::getSortedNeighbours(Agent::AGENT_TYPE agentType)
{
	std::list<Cell*> sortedCells;
	NeighbourhoodScanner ns(this);

	for (ns.start(); ns.more(); ns.next())
		sortedCells.push_back(ns.get());

	if (agentType == Agent::PREY)
		sortedCells.sort(SortByPreyFunctor());
	else if(agentType == Agent::PREDATOR)
		sortedCells.sort(SortByPredatorFunctor());
	return sortedCells;
}