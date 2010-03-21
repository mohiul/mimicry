/**
 * \file Model.h
 */

#ifndef MODEL_H
#define MODEL_H

#include "Cell.h"
#include "System.h"
#include "Vector.h"
#include "Event.h"
#include "ReportGenerator.h"

#include <GL/glut.h>

/**
 * An instance controls the simulation of a collection of agents.
 * An instance of \a Model controls the simulation of a collection of agents.
 */
class Model
{
public:
	Model();
	bool init(std::string configurationFile);
	void draw();
	void stats();
	void step();
	void checkPosition(formal::Vector & pos);
	void setStatsWindow(int newWidth, int newHeight, GLdouble newXRange, GLdouble newYRange);
	long getSimTime();
	void ringReport();
private:
   /**
    * The current simulation time.
    */
	long simTime;

   /**
    * Array of \a Cells.
    */
	Cell cells[ISIZE][ISIZE][ISIZE];

	std::list<Event*> eventList;

   /**
    * Statistics window width (pixels).
    */
   int width;

   /**
    * Statistics window height (pixels).
    */
   int height;

   /**
    * Statistics window X-coordinate range (zero at centre).
    */
   GLdouble xRange;

   /**
    * Statistics window Y-coordinate range (zero at centre).
    */
   GLdouble yRange;

   /**
    * Instance of the \a ReportGenerator class which will 
	* generate Mimicry ring report.
    */
   ReportGenerator report;

};

/**
* \return simulation time.
*/
inline long Model::getSimTime()
{ 
	return simTime; 
}

#endif