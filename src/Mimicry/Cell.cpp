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
	while( agentIter != agentList.end() ) {
		if(*agentIter == pa)
		{
			agentList.erase(agentIter);
			break;
		}
		agentIter++;
	}
	population--;
	if (pa->getAgentType() == Agent::PREY)
		preyPopulation--;
	if (pa->getAgentType() == Agent::PREDATOR)
		predatorPopulation--;
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
int Cell::getPop(Agent::AGENT_TYPE agentType)
{
	int pop = 0;
	if(agentType == Agent::PREDATOR)
		pop = predatorPopulation;
	else if(agentType == Agent::PREY)
		pop = preyPopulation;
	return pop;
}

/**
 * Reduce population count of a specific type of agent.
 * \param agentType Type of the Agent.
 */
void Cell::reducePop(Agent::AGENT_TYPE agentType)
{
	if(agentType == Agent::PREDATOR)
		predatorPopulation--;
	else if(agentType == Agent::PREY)
		preyPopulation--;
	population--;

}