/**
 * \file Model.cpp
 */

#include "Model.h"
#include "System.h"
#include "Event.h"
#include "randutil.h"
#include "ReportGenerator.h"

#include <iostream>
#include <list>
#include <bitset>
#include <cmath>
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
bool Model::init()
{
	int noOfAgentsToCreate = 0;
	int rule30 = 0;
	int rule110 = 0;
	
	for(int i = 0; i < ISIZE; i++)
	{
		for(int j = 0; j < ISIZE; j++)
		{
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

				//Creating random genome for \a Prey. One Prey for each \a Cell.
				Genome<PREY_GENE_SIZE> genome;
				std::bitset<PATTERN_GENE_SIZE> *bs;
				if(noOfAgentsToCreate < 110)
				//if(noOfAgentsToCreate < 14)
				{
					bs = new std::bitset<PATTERN_GENE_SIZE>(30);
					genome.set(8, 0);
					genome.set(9, 0);
					rule30++;
				} else {
					bs = new std::bitset<PATTERN_GENE_SIZE>(110);
					genome.set(8, 1);
					genome.set(9, 1);
					rule110++;
				}

				for(int g = 0; g < PATTERN_GENE_SIZE; g++)
					genome.set(g, (*bs)[PATTERN_GENE_SIZE - 1 - g]);

				std::bitset<7> bs1(randomInteger(pow(2.0, 7)));
				for(int g = 0; g < 7; g++)
					genome.set(10 + g, bs1[7 - 1 - g]);

				//std::cout << "Genome: " << genome << std::endl;
				
				//if(noOfAgentsToCreate == 100)
					cells[i][j][k].insert(new Prey(this, &cells[i][j][k], cellToPos(i,j,k), genome));

				//Creating fewer Predators than Prey. One in every 5 Cell (approx...)
				if(noOfAgentsToCreate % 10 == 0)
				//if(noOfAgentsToCreate == 100)
				{
					//Create random genome for Predators.
					Genome<PREDATOR_GENE_SIZE> genome;
					std::bitset<PREDATOR_GENE_SIZE> bs(randomInteger(pow(2.0, PREDATOR_GENE_SIZE)));
					for(int g = 0; g < PREDATOR_GENE_SIZE; g++)
						genome.set(g, bs[PREDATOR_GENE_SIZE - 1 - g]);

					//std::cout << " predator gene: " << genome << std::endl;
					cells[i][j][k].insert(new Predator(this, &cells[i][j][k], cellToPos(i,j,k), genome));
				}
				noOfAgentsToCreate++;
			}
		}
	}
	std::cout << "Prey population by CA rule: " << std::endl;	
	std::cout << "rule30: " << rule30 << " rule110: " << rule110 << std::endl;
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

/**
 * Execute one step for the \a Model.
 */
void Model::step()
{
	int preyPopulation = 0;

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
					}
					agentIter++;
				}
				preyPopulation += cells[i][j][k].getPop(Agent::PREY);
			}
			
	// Process event lists.
	processEvents(moveFirst, moveLast);
	processEvents(birthFirst, birthLast);
	processEvents(deathFirst, deathLast);

	if(simTime % 100 == 0)
		std::cout << "Prey Population: " << preyPopulation << std::endl;

	if(simTime % 500 == 0)
	{
		ReportGenerator report;
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
