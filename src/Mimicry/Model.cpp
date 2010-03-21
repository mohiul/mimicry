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
 * Construct \a Model.
 */
Model::Model()
{

}

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
							Event *p = getEvent(&eventList);
							p->setMove(agent, &cells[i][j][k], cb);
						}
					} else if (agent->getState() == Agent::DEAD) {
						Event *p = getEvent(&eventList);
						p->setDeath(agent, &cells[i][j][k]);
					}
					agentIter++;
				}
				preyPopulation += cells[i][j][k].getPop(Agent::PREY);
				predatorPop += cells[i][j][k].getPop(Agent::PREDATOR);
			}
			
	// Process event lists.
	processEvents(&eventList);

	if(simTime % 100 == 0)
	{
		std::cout << "Sim time: " << simTime 
				  << ", Population Prey: " << preyPopulation
				  << " Predator: " << predatorPop << std::endl;
	}

	if(simTime % 10 == 0)
		report.generateMimicryRingReport(cells, simTime);

	if(simTime % 500 == 0)
		report.printMimicryRingReport();

	simTime++;
}

void Model::ringReport()
{
	report.writeMimicryRingReport();
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