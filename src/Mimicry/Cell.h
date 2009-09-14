/**
 * \file Cell.h
 */

#ifndef CELL_H
#define CELL_H

#include <list>
#include "Agent.h"
#include "System.h"

/**
 * An instance describes the information in each discrete cell of the universe.
 * \a Cells are rectangular (strictly: cuboidal) regions that are used to determine locality.
 * Each cell has a \a Bag of atoms associated with it.
 *
 * At any time, an \a Agent is inside a particular cell.  *
 * The \a Cell maintains a list of \a Agents that are currently inside it.
 */
class Cell
{
public:
	Cell();
	Cell(int i_pos, int j_pos, int k_pos);
	void insert(Agent* agent);
	void remove(Agent* agent);
	int getPop() {return population;}
	int getPop(Agent::AGENT_TYPE agentType);
	void reducePop(Agent::AGENT_TYPE agentType);
	std::list<Agent*> getAgentList() {return agentList;}
	void setPos(formal::Vector _position);
	formal::Vector getPos(){return position;}
	Cell * & getNeighbour(int i);
	void draw();
private:
	/**
	 * Population of \a Agents inside this \a Cell.
	 */
	int population;

	/**
	 * Population of \a Prey species inside this \a Cell.
	 */
	int preyPopulation;

	/**
	 * Population of \a Predator species inside this \a Cell.
	 */
	int predatorPopulation;

	/**
	 * List of \a Agents inside this cell.
	 */
	std::list<Agent*> agentList;

	/**
	 * Position of this Cell.
	 */
	formal::Vector position;

	/**
	 * Array containing pointer to all neighbouring cells.
	 */
	Cell* neighbours[NUM_CELL_NEIGHBOURS];
};

/**
 * Provide iteration capability for searching for a \a Cell
 * within the neighbourhood of a particular \a Cell.
 */
class NeighbourhoodScanner
{
public:
   NeighbourhoodScanner(Cell *pc);
   void start();
   bool more();
   void next();
   Cell *get();
private:

   /**
    * Pointer to the cell at the centre of the neighbourhood being scanned.
    */
   Cell *pc;

   /**
    * index into the neighbourhood array for the cell.
    */
   int index;
};


/**
 * Return a normalized vector giving the offset of \a Cell \a py with respect to \a Cell \a px.
 */
inline formal::Vector offset(Cell *px, Cell *py)
{
   return (px->getPos() - py->getPos()).normalize();
}

/**
 * \return a pointer to a \a Cell neighbouring this one.
 * \param i is an index for a neighbour: 0 <= i < 27.
 */
inline Cell * & Cell::getNeighbour(int i)
{
   return neighbours[i];
}

/**
 * Construct a neighbourhood scanner for a given cell.
 * \param pc points to the cell at the centre of the 'neighbourhood'.
 */
inline NeighbourhoodScanner::NeighbourhoodScanner(Cell *pc) 
   : pc(pc), index(0)
{ }

/**
 * Initialize the neighbourhood scanner.
 */
inline void NeighbourhoodScanner::start()
{
   index = 0;
}

/**
 * \return \a true if there are more neighbours.
 */
inline bool NeighbourhoodScanner::more()
{
   return index < NUM_CELL_NEIGHBOURS;
}

/**
 * Set the iterator to the next cell.
 */
inline void NeighbourhoodScanner::next()
{
   index++;
}

/**
 * Return a pointer to the current cell in the neighbourhood.
 */
inline Cell *NeighbourhoodScanner::get()
{
   return pc->getNeighbour(index);
}

#endif CELL_H