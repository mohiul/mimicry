/**
 * \file Model.cpp
 */

#include "Model.h"
#include "System.h"
#include "Event.h"
#include "randutil.h"
#include "InitConfiguration.h"

#include <iostream>
#include <list>
#include <bitset>
#include <cmath>
#include <string>
#include "GL/glut.h"

// OpenGL colours
const GLfloat WHITE[]  = { 1, 1, 1 };
const GLfloat BLACK[]  = { 0, 0, 0 };

/**
 * Pointer to first 'move' event.
 */
Event *moveFirst = new Event;

/**
 * Pointer to first 'birth' event.
 */
Event *birthFirst = new Event;

/**
 * Pointer to first 'death' event.
 */
Event *deathFirst = new Event;

/**
 * Pointer to last 'move' event.
 */
Event *moveLast;

/**
 * Pointer to last 'birth' event.
 */
Event *birthLast;

/**
 * Pointer to last 'death' event.
 */
Event *deathLast;

/**
 * Construct \a Model.
 */
Model::Model()
{

}

/**
 * Draw a histogram in an OpenGL window.
 * If the data is positive, the origin is at the left.
 * If the data is positive and negative, the origin is in the middle.
 * \param left is minimum X coordinate for enclosing box.
 * \param right is maximum X coordinate for enclosing box.
 * \param bottom is minimum Y coordinate for enclosing box.
 * \param top is maximum Y coordinate for enclosing box.
 * \param xSpacing gives character spacing for 9x15 characters.
 * \param ySpacing gives line spacing for 9x15 characters.
 * \param title describes the data.
 * \param entries is the number of valid entries in the array \a data.
 * \param data contains the data to be displayed.
 * \param intervals is the number of intervals for the histogram.
 */
/*
void drawHistogram(
      double left, double right, double bottom, double top,
      double xSpacing, double ySpacing, char *title, int entries,
      double data[], int intervals)
{
   // Find maximum and set scaling factor for counting frequencies.
   double maxData = data[0];
   double minData = data[0];
   double average = 0;
   int i, h;
   for (i = 0; i < entries; i++)
   {
      average += data[i];
      if (maxData < data[i])
         maxData = data[i];
      if (minData > data[i])
         minData = data[i];
   }
   if (entries > 0)
      average /= entries;
   double scale;
   double offset;
   if (minData < 0)
   {
      double max = -minData > maxData ? -minData : maxData;
      scale = (intervals - 1) / (2 * max);
      offset = (intervals - 1) / 2;
   }
   else
   {
      scale = (intervals - 1) / maxData;
      offset = 0;
   }
   // Count frequencies and note maximum frequency for scaling.
   const int COLUMNS = 100;
   assert(intervals <= COLUMNS);
   int hist[COLUMNS];
   int maxFrequency = 0;
   for (h = 0; h < intervals; h++)
      hist[h] = 0;
   for (i = 0; i < entries; i++)
   {
      int n = int(scale * data[i] + offset);
      if (0 <= n && n < intervals)
      {
         hist[n]++;
         if (maxFrequency < hist[n])
            maxFrequency = hist[n];
      }
   }

   // Find coordinates and draw bounding box.
   double xstep = (right - left) / intervals;
   double colwidth = 0.9 * xstep;
   double yscale = (top - bottom) / maxFrequency;
   double x = left;
   glColor3fv(BLUE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectd(left, bottom, right, top);

   if (entries == 0)
      return;

   // Draw histogram bars.
   glColor3fv(YELLOW);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   for (h = 0, x = left; h < intervals; h++, x += xstep)
      glRectd(x, bottom, x + colwidth, bottom + yscale * hist[h]);

   // Write title and maximum values.
   const int BUFLEN = 600;
   char buffer[BUFLEN];
   ostringstream os;
   os.setf(ios::fixed, ios::floatfield);
   os.precision(0);
   glColor3fv(GREEN);
   os <<
      title << ": " <<
      minData << " < " <<
      average << " < " <<
      maxData;
   double midPoint = 0.5 * (left + right);
   double xPos = midPoint - 0.5 * xSpacing * 20; // os.pcount();
   double yPos = bottom - ySpacing;
   showString(xPos, yPos, ySpacing, os.str().c_str());
}
*/

/**
 * Convert a coordinate to the corresponding \a Cell index.
 */
inline int posToCell(double a)
{
   int index = int((a + System::HALF_WORLD_SIZE) / System::CELL_SIZE);
   if (index < 0) index = 0;
   if (index >= ISIZE) index = ISIZE - 1;
   return index;
}

/**
 * Convert \a Cell index to \a Cell coordinate.
 */
inline double cellToPos(int i)
{
   return System::CELL_SIZE * (i + 0.5) - System::HALF_WORLD_SIZE;
}

/**
 * Convert \a Cell indexes to \a Cell position
 */
formal::Vector cellToPos(int i, int j, int k)
{
   return formal::Vector(cellToPos(i), cellToPos(j), cellToPos(k));
}

/**
 * Check that a cell index is in the range [0,ISIZE).
 * \param index is the index to be checked.
 * \return the corresponding index within range.
 */
int mod(int index)
{
   while (index < 0)
      index += ISIZE;
   while (index >= ISIZE)
      index -= ISIZE;
   return index;
}

/**
 * Initialize model by creating new \n Agents.
 */
bool Model::init(std::string configurationFile)
{	
	InitConfiguration initConfig;
	if( !initConfig.readConfigFile(configurationFile) )
		return false;

	int cellIndx = 0;
	
	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				cells[i][j][k] = Cell();
				cells[i][j][k].setPos(cellToPos(i,j,k));

				// Create neighbour array for this cell.
				int n = 0;
				for (int ii = i - 1; ii <= i + 1; ii++)
				   for (int jj = j - 1; jj <= j + 1; jj++)
					  for (int kk = k - 1; kk <= k + 1; kk++)
						 cells[i][j][k].getNeighbour(n++) = & cells[mod(ii)][mod(jj)][mod(kk)];

				int rule = initConfig.getPreyConfig(cellIndx)->rule;
				bool palatability = initConfig.getPreyConfig(cellIndx)->palatability;
				int preyPop = initConfig.getPreyConfig(cellIndx)->population;

				for(int p = 1; p <= preyPop; p++)
				{
					Genome<PREY_GENE_SIZE> genome(rule, palatability);
					cells[i][j][k].insert(new Prey(this, &cells[i][j][k], cellToPos(i,j,k), genome));
				}

				int predatorPop = initConfig.getPredatorConfig(cellIndx)->population;

				for(int p = 1; p <= predatorPop; p++)
				{
					Genome<PREDATOR_GENE_SIZE> genome;
					std::bitset<PREDATOR_GENE_SIZE> bs(randomInteger(pow(2.0, PREDATOR_GENE_SIZE)));
					for(int g = 0; g < PREDATOR_GENE_SIZE; g++)
						genome.set(g, bs[PREDATOR_GENE_SIZE - 1 - g]);

					cells[i][j][k].insert(new Predator(this, &cells[i][j][k], cellToPos(i,j,k), genome));
				}
				cellIndx++;
			}
	simTime = 0;
	return true;
}

/**
 * Draw the \a Model in the OpenGL model window.
 */
void Model::draw()
{
	if(System::showOutline)
	{
		glColor3fv(WHITE);
		glutWireCube(System::WORLD_SIZE);
	}

	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				//Draw the outline of the cells
				if(System::showCells)
					cells[i][j][k].draw();

				//Draw the agents
				std::list<Agent*> agentList = cells[i][j][k].getAgentList();
				std::list<Agent*>::iterator agentIter = agentList.begin();
				while( agentIter != agentList.end() ) {
					Agent* agent = *agentIter;
					if (agent->getState() == Agent::ALIVE)
						agent->draw();
					agentIter++;
				}
			}
}

void Model::stats()
{
	//ReportGenerator report;
	//report.generateMimicryRingReport(cells);
}

/**
 * Execute one step for the \a Model.
 */
void Model::step()
{
	int preyPopulation = 0;
	int predatorPop = 0;

	// Process each agent.
	moveLast = moveFirst;
	birthLast = birthFirst;
	deathLast = deathFirst;

	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				std::list<Agent*> agentList = cells[i][j][k].getAgentList();
				std::list<Agent*>::iterator agentIter = agentList.begin();
				while( agentIter != agentList.end() ) {
					Agent* agent = *agentIter;
					if (agent->getState() == Agent::ALIVE)
					{
						agent->step();

						// Check for agents who have moved to another cell.
						formal::Vector pos = agent->getPosition();
						int ni = posToCell(pos[0]);
						int nj = posToCell(pos[1]);
						int nk = posToCell(pos[2]);
						if (ni != i || nj != j || nk != k)
						{
							Cell *cb = &cells[ni][nj][nk];
							Event *p = getEvent(moveFirst, moveLast);
							p->setMove(agent, &cells[i][j][k], cb);
						}
					} else if (agent->getState() == Agent::DEAD) {
						Event *p = getEvent(deathFirst, deathLast);
						p->setDeath(agent, &cells[i][j][k]);
					}
					agentIter++;
				}
				preyPopulation += cells[i][j][k].getPop(Agent::PREY);
				predatorPop += cells[i][j][k].getPop(Agent::PREDATOR);
			}
			
	// Process event lists.
	processEvents(moveFirst, moveLast);
	processEvents(birthFirst, birthLast);
	processEvents(deathFirst, deathLast);

	if(simTime % 100 == 0)
	{
		std::cout << "Population Prey: " << preyPopulation;
		std::cout << " Predator: " << predatorPop << std::endl;
	}

	if(simTime % 500 == 0)
	{		
		report.generateMimicryRingReport(cells);
		report.printMimicryRingReport();
		std::cout << "Sim time: " << simTime << std::endl;
	}

	simTime++;
	//std::cout << std::endl << "simTime: " << simTime << std::endl;
}

/**
 * Coordinate checker utility function.
 */
void CheckCoordinate(double & x)
{
   while (x < - System::HALF_WORLD_SIZE)
      x += System::WORLD_SIZE;
   while (x > System::HALF_WORLD_SIZE)
      x -= System::WORLD_SIZE;
}

/**
 * Check each coordinate of an agent position given as a \a Vector.
 * \param pos is a \a Vector giving the position of an \a Agent.
 */
void Model::checkPosition(formal::Vector & pos)
{
   CheckCoordinate(pos[0]);
   CheckCoordinate(pos[1]);
   CheckCoordinate(pos[2]);
}

/**
 * Set size parameters for Statistics window.
 */
void Model::setStatsWindow(int newWidth, int newHeight, GLdouble newXRange, GLdouble newYRange)
{
   width = newWidth;
   height = newHeight;
   xRange = newXRange;
   yRange = newYRange;
}