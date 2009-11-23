/**
 * \file Agent.h
 */

#ifndef AGENT_H
#define AGENT_H

#include <cmath>
#include <iostream>

#include "System.h"
#include "Vector.h"
#include "CAPattern.h"
#include "HopfieldMemory.h"
#include "Genome.h"
#include "GL/glut.h"

class Model;
class Cell;

/**
 * An instance is an \a Agent.
 * This is the base class. The derived classes are a \a Prey and \a Predator.
 */
class Agent
{
public:

	/**
	* The states of an \a Agent: used for determining whether the \a Agent is still active.
	*/
	enum State
	{
		ALIVE,	/**< The state of being alive for an \a Agent. */
		DEAD	/**< The state of being dead for an \a Agent. */
	};

	/**
	* The type of an \a Agent: used for determining whether it's an instance of \a Prey or \a Predator.
	*/
	enum AGENT_TYPE
	{
		PREY,		/**< The Prey type \a Agent. */
		PREDATOR	/**< The Predator type \a Agent. */
	};

	virtual void draw() = 0;
	virtual void step() = 0;
	virtual void move() = 0;
	virtual void reproduce(Agent* agent) = 0;
	virtual AGENT_TYPE getAgentType() = 0;
	virtual void kill() = 0;
	void addForce(formal::Vector v);
	formal::Vector getPosition();
	void setCell(Cell* _cell);
	Agent::State getState();
	bool isCapableToReproduce();
protected:
   /**
    * The position of the \a Agent. 
    */
	formal::Vector position;

	/**
    * Age of the Agent till its birth.
    */
	int age;

	/**
    * \a State of the agent. State could be \a ALIVE or \a DEAD.
    */
	State state;

	/**
    * The force that the \a Agent is currently using to move.
    */
	formal::Vector force;

   /**
    * The current velocity of the \a Agent.
    */
   formal::Vector velocity;

   /**
    * Pointer to the \a Model where the agent exists.
    */
   Model* model;

   /**
    * Pointer to the \a Cell of which the \a Agent is currently a part of.
    */
   Cell* cell;

	/**
	 * Reproduction behavior of the \a Agent.
	 */
	bool capableToReproduce;

	/**
	 * Contains last simulation time when this \a Agent has reproduced. 
	 * Used to create a time interval between two consecutive reproduction by same Agent.
	 */
	long lastReproductionTime;

};

class Prey;

/**
 * An instance is derived class of an \a Agent. 
 * A \a Predator is respondible for hunting \a Prey species existing in the \a Model.
 */
class Predator: public Agent
{
public:
	Predator(Model* _model, Cell* _cell, formal::Vector _position, 
				   Genome<PREDATOR_GENE_SIZE> _genome);
	void draw();
	void step();
	void move();
	void kill();
	void reproduce(Agent* agent);
	Genome<PREDATOR_GENE_SIZE>* getGenome();
	AGENT_TYPE getAgentType();
	void mobilityBhvrGeneIndx0to3();
	void reproductionGeneIndx4();
	void findPreyToAttack();
	void attack(Prey* prey);
	void attemptToKill(Prey* prey);
private:

	/**
	 * \a Genome of a Predator. The genes currently represent only mobility behavior. 
	 * \todo More genes to add involving other behavior of \a Predator.
	 */
	Genome<PREDATOR_GENE_SIZE> genome;

	/**
	 * Pointer to \a HopfieldMemory. Initialized at the construction of a \a Predator.
	 * This memory contains a list of \a Memory structures which associates palatability 
	 * behavior with Cellular Automata patterns representing the \a Prey species, this 
	 * predator has interacted with. 
	 */
	HopfieldMemory* memory;

	/**
	 * Contains last simulation time when this \a Predator attacked. 
	 * Used to create a time interval between two consecutive attack by same Predator.
	 */
	long lastAttackTime;
};

/**
 * An instance is derived class of an \a Agent. 
 * A \a Prey contains a specific Cellular Automata generated pattern 
 * with which predators recognize them.
 */
class Prey: public Agent
{
public:
	/**
	 * An instance of Cellular Automata generated pattern, \a CAPattern.
	 * This pattern is generated from the \a Genome of each \a Prey species. 
	 */
	CAPattern pattern;

	/**
	 * OpenGL textured image generated from the binary Cellular Automata pattern. 
	 */
	GLubyte texturedImage[PATTERN_SIZE][PATTERN_SIZE][4];

	/**
	 * Name of the OpenGL textured image.
	 */
	GLuint texName;

	Prey();
	Prey(Model* _model, Cell* _cell, formal::Vector _position, Genome<PREY_GENE_SIZE> _genome);
	void draw();
	void step();
	void move();
	void kill();
	void reproduce(Agent* agent);
	Genome<PREY_GENE_SIZE>* getGenome();
	AGENT_TYPE getAgentType();
	void makeTexturedImage();
	//Palatability genes
	void geneIndex8to9();
	//Mobility genes attraction towards crowds of prey and repulsion from predator
	void geneIndex10to15();
	//Reproduction gene
	void geneIndex16();
	bool isPalatable();
private:

	/**
	 * \a Genome of the \a Prey species. 
	 */
	Genome<PREY_GENE_SIZE> genome;

	/**
	 * Palatability behavior of the \a Prey. Determined from Gene index 8 and 9.
	 */
	bool palatability;
};

/**
 * \return Current position of the \a Agent.
 */
inline formal::Vector Agent::getPosition()
{
	return position;
}

/**
 * Set pointer to the current cell location.
 * \param _cell Pointer to the current \a Cell where the \a Agent resides.
 */
inline void Agent::setCell(Cell* _cell)
{
	cell = _cell;
}

/**
 * \return The current \a State of the \a Agent. \a ALIVE or \a DEAD.
 */
inline Agent::State Agent::getState()
{
	return state;
}

/**
 * Update the force component of this \a Body.
 * The force is used to compute the acceleration,
 * velocity, and hence the new position of the \a Body.
 * The largest force given to \a addForce() becomes the new force.
 * \param v is a vector giving the direction of the new force that
 * is to be added to the current force vector.
 */
inline void Agent::addForce(formal::Vector v)
{
   if (v.mag() > force.mag())
      force = v;
}

/**
 * \return The type of the \a Agent, in this case a \a Predator.
 */
inline Agent::AGENT_TYPE Predator::getAgentType()
{
	return PREDATOR;
}

/**
 * \return The type of the \a Agent, in this case a \a Prey.
 */
inline Agent::AGENT_TYPE Prey::getAgentType()
{
	return PREY;
}

/**
 * \return The reproduction capability of this \a Prey.
 */
inline bool Agent::isCapableToReproduce()
{ 
	return capableToReproduce; 
}

/**
 * \return Pointer to the \a Genome of this \a Prey.
 */
inline Genome<PREY_GENE_SIZE>* Prey::getGenome()
{ 
	return &genome;
}

/**
 * \return Pointer to the \a Genome of this \a Predator.
 */
inline Genome<PREDATOR_GENE_SIZE>* Predator::getGenome()
{ 
	return &genome;
}

/**
 * \return The palatability property of this \a Prey.
 */
inline bool Prey::isPalatable()
{ 
	return palatability; 
}

#endif