/**
 * \file Model.h
 */

#ifndef MODEL_H
#define MODEL_H

#include "Cell.h"
#include "System.h"
#include "Vector.h"

/**
 * An instance controls the simulation of a collection of agents.
 * An instance of \a Model controls the simulation of a collection of agents.
 */
class Model
{
public:
	Model();
	bool init();
	void draw();
	void step();
	void checkPosition(formal::Vector & pos);
	long getSimTime();
private:
   /**
    * The current simulation time.
    */
	long simTime;

   /**
    * Array of \a Cells.
    */
	Cell cells[ISIZE][ISIZE][ISIZE];
};

/**
* \return simulation time.
*/
inline long Model::getSimTime()
{ 
	return simTime; 
}

#endif