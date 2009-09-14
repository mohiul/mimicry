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
	mobilityBehavior();
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
    formal::Vector acceleration = System::FORCE_FACTOR * force - System::FRICTION * velocity;
    velocity += System::DT * acceleration;
    formal::Vector deltaPos = System::DT * velocity;
	position += deltaPos;
	model->checkPosition(position);
}

/**
 * Movement behavior of a \a Predator calculated from its \a Genome.
 * The genome for this species is 4 bits which is calculated in decimal 
 * format from binary to determine the magnitude of force at which it
 * will move towards the maximum crowd of \a Prey present within its
 * neighbourhood. 
 */
void Predator::mobilityBehavior()
{
	int forceMagnitude = genome.get(3)*4 + genome.get(2)*3 
		+ genome.get(1)*2 + genome.get(0)*1;
	int maxAgents = 0;
	Cell *best = cell;
	NeighbourhoodScanner ns(cell);
	for (ns.start(); ns.more(); ns.next())
	{
		Cell *neighbour = ns.get();
		int pop = neighbour->getPop(Agent::PREY);
		if (maxAgents < pop)
		{
			maxAgents = pop;
			best = neighbour;
		}
	}
	if (best != cell)
		addForce(forceMagnitude * offset(cell, best));
	/**
	 * \todo The following condition is for continuous predator movement, 
	 * If no \a Prey is present in its neighbourhood \a Predator 
	 * will choose a random neighbour cell and move towards it.
	 * Currently under test. 
	 */
	//else if(best == cell && best->getPop(Agent::PREY) == 0)
	//	addForce((forceMagnitude + 5) * offset(cell, cell->getNeighbour(randomInteger(NUM_CELL_NEIGHBOURS))));
}

/**
 * Find a random \a Prey species in the current cell and attack.
 * Predator has to be above a certain age to start attacking.
 */
void Predator::findPreyToAttack()
{
	if(age >= System::PREDATOR_ATTACK_AGE)
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
						attack(static_cast<Prey*>(*agentIter));
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