/**
 * \file Predator.cpp
 */

#include "Agent.h"
#include "Model.h"
#include "randutil.h"
#include <list>
#include <iostream>

/**
 * Construct a Predator species. 
 * Also initializes the Hopfield Memory.
 * \param _model The \a Model this Predator is part of.
 * \param _cell The \a Cell this Predator currently resides.
 * \param _position The current position of this Agent.
 * \param _genome \a Genome of this species. 
 */
Predator::Predator(Model* _model, 
				   Cell* _cell, 
				   formal::Vector _position, 
				   Genome<PREDATOR_GENE_SIZE> _genome)
{
	model = _model;
	cell = _cell;
	position = _position;
	genome = _genome;
	velocity = 0.0;
	force = 0.0;
	age = 0;
	state = Agent::ALIVE;
	memory = new HopfieldMemory(PATTERN_SIZE, PATTERN_SIZE);
	lastAttackTime = 0;
}

/**
 * Draw the Predator as a RED solid cube in the model.
 */
void Predator::draw()
{
	//Predators are drawn in RED.
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutSolidCube(0.20);
	glPopMatrix();
}

/**
 * Operations preformed in every step for by a \a Predator.
 * Age update. Add force for mobility based on genome. 
 * Find \a Prey species to attack and perform movement 
 * towards crowds of \a Prey.
 */
void Predator::step()
{
	age++;
	force = formal::Vector(0, 0, 0);
	mobilityBhvrGeneIndx0to5();
	if(age >= System::PREDATOR_ATTACK_AGE
		&& ( lastAttackTime + System::PREDATOR_ATTACK_INTERVAL ) < model->getSimTime())
		findPreyToAttack();

	move();
}

/**
 * Move an \a Agent.
 * This function is called once during each step by the \a Model.
 * It uses the force component of the Agent to compute the Agent's acceleration.
 * If the force and velocity are both zero, it has no effect.
 * Otherwise, Newton's law is used to obtain the acceleration, which is
 * integrated to obtain the new velocity and new position.
 */
void Predator::move()
{
	//std::cout << "force: " << force << std::endl;
	//std::cout << "velocity: " << velocity << std::endl;
    formal::Vector acceleration = System::FORCE_FACTOR * force - System::FRICTION * velocity;
	//std::cout << "acceleration: " << acceleration << std::endl;
    velocity += System::DT * acceleration;
	//std::cout << "new velocity: " << velocity << std::endl;
    formal::Vector deltaPos = System::DT * velocity;
	//std::cout << "deltaPos: " << deltaPos << std::endl;
	//std::cout << "position: " << position << std::endl;
	position += deltaPos;
	//std::cout << "new position: " << position << std::endl;
	model->checkPosition(position);
}

/**
 * Movement behavior of a \a Predator calculated from its \a Genome.
 * The genome for this species is 4 bits which is converted from binary 
 * to decimal to determine the magnitude of force at which it
 * will move towards the maximum crowd of \a Prey present within its
 * neighbourhood. If no prey is present in the nieghbourhood then 
 * this force is active in trying to keep predators distributed
 * all over the cells. A predator chooses the neighbourhood cell which 
 * contains the least number of predators and when the neighbourhood
 * contains zero predators, it would select any one of them randomly.
 */

void Predator::mobilityBhvrGeneIndx0to5()
{
	int forceMagnitude = genome.get(0)*1 + genome.get(1)*2 + genome.get(2)*3
		+ genome.get(3)*4;
	
	Cell *best = cell;

	std::list<Cell*> sortedNeighbours = cell->getSortedNeighbours(Agent::PREY);

	//if(model->getSimTime() % 500 == 0)
	//{
	//	std::cout << "Cell: ";
	//	for (std::list<Cell*>::iterator cellIter = sortedNeighbours.begin();
	//		cellIter != sortedNeighbours.end(); cellIter++)
	//		std::cout << " " << (*cellIter)->getPop(Agent::PREY);
	//	std::cout << std::endl;
	//}

	if((*sortedNeighbours.begin())->getPop(Agent::PREY) > 0)
	{
		best = *sortedNeighbours.begin();
	} else {
		sortedNeighbours = cell->getSortedNeighbours(Agent::PREDATOR);

		std::list<Cell*> zeroPopNeighbours;
		for (std::list<Cell*>::iterator cellIter = sortedNeighbours.begin();
			cellIter != sortedNeighbours.end(); cellIter++)
			if((*cellIter)->getPop(Agent::PREDATOR) == 0)
				zeroPopNeighbours.push_back(*cellIter);

		int randCell = randomInteger(zeroPopNeighbours.size());

		int indx = 0;
		for (std::list<Cell*>::iterator cellIter = zeroPopNeighbours.begin();
			cellIter != zeroPopNeighbours.end(); cellIter++)
			if(indx++ == randCell)
			{
				best = *cellIter;
				break;
			}
	}
	if (best != cell)
		addForce(forceMagnitude * offset(best, cell));
}

/**
 * Find a random \a Prey species in the current cell and attack.
 * Predator has to be above a certain age to start attacking.
 */
void Predator::findPreyToAttack()
{
	int preyPop = cell->getPop(Agent::PREY);
	if(preyPop >= 1)
	{
		int rand = randomInteger(preyPop);
		std::list<Agent*> agentList = cell->getAgentList();
		int preyCount = 0;
		for(std::list<Agent*>::iterator agentIter = agentList.begin();
			agentIter != agentList.end(); agentIter++)
			if((*agentIter)->getState() == Agent::ALIVE 
				&& (*agentIter)->getAgentType() == Agent::PREY
				&& preyCount++ == rand)
			{
					attack(static_cast<Prey*>(*agentIter));
					this->lastAttackTime = model->getSimTime();
			}
	}
}

/**
 * Attack a the randomly selected a\ Prey.
 * If memory of this Predator is lower than the \a MIN_MEMORY_SIZE
 * then Predator attacks anyway. If memory size is above threshold
 * then predator first tries to recall the Prey by its pattern. If Prey
 * found to be palatable then only it attacks, otherwise it ignores. If 
 * Prey cannot be recalled then also the Predator attacks.
 * \param prey \a Prey species to attack.
 */
void Predator::attack(Prey* prey)
{
	if(memory->getMemorySize() <= System::MIN_MEMORY_SIZE)
		attemptToKill(prey);
	else
	{
		Memory *identifiedMemory = memory->recognize(&prey->pattern);
		if(identifiedMemory == 0 || identifiedMemory->palatability)
			attemptToKill(prey);
	}
}

/**
 * Attempt to kill a\ Prey.
 * Irrespective of the Prey's palatability, whenever Predator 
 * attempts to kill a Prey, it dies in that process. 
 * If memory size hasn't reached the threshold
 * all Prey pattern which are attempted to kill are memorized.
 * \param prey \a Prey species to kill.
 */
void Predator::attemptToKill(Prey* prey)
{
	prey->kill();

	if(memory->getMemorySize() < System::MAX_MEMORY_SIZE)
		memory->addPattern(&prey->pattern, prey->isPalatable());
}

/**
 * Reproduce another \a Predator.
 * \todo Reproduction requires to be implemented and used. 
 * Currently no Predators are reproduced.
 * \param agent with which reproduction process will continue.
 */
void Predator::reproduce(Agent* agent)
{
	Predator* predator = static_cast<Predator*>(agent);
}