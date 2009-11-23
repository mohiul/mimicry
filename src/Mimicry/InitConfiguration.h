/**
 * \file InitConfiguration.h
 */

#ifndef INIT_CONFIGURATION_H
#define INIT_CONFIGURATION_H

#include "System.h"
#include "Agent.h"

#include <string>
#include <vector>

/**
 * Structure contains configuration over the 
 * number of \a Prey species to create.
 */
struct PreyConfig
{
	PreyConfig()
	{
		rule = 0;
		population = 0;
	}
	int rule;
	int population;
	bool palatability;
};

/**
 * Structure contains configuration over the 
 * number of \a Predator species to create.
 */
struct PredatorConfig
{
	PredatorConfig()
	{
		population = 0;
	}
	int population;
};

/**
 * Responsibility of this class is to setup the initial 
 * configuration of the simulation. It copies the configuration
 * details from an input XML file and setup the number of \a Prey
 * and \a Predator species.
 */
class InitConfiguration
{
private:
	/**
	 * Create an array of \a Prey configuration structure.
	 * Size is the total number of cells in the model.
	 */
	PreyConfig* preyConfig[ISIZE*ISIZE*ISIZE];

	/**
	 * Create an array of \a Predator configuration structure.
	 * Size is the total number of cells in the model.
	 */
	PredatorConfig* predatorConfig[ISIZE*ISIZE*ISIZE];
public:
	InitConfiguration();
	bool readConfigFile(std::string configFile);
	PreyConfig* getPreyConfig(int cellIndx);
	PredatorConfig* getPredatorConfig(int cellIndx);
	void printInitConfig();
};

/**
 * return \a Prey configuration of a specific \a Cell.
 */
inline PreyConfig* InitConfiguration::getPreyConfig(int cellIndx)
{
	return preyConfig[cellIndx];
}

/**
 * return \a Predator configuration of a specific \a Cell.
 */
inline PredatorConfig* InitConfiguration::getPredatorConfig(int cellIndx)
{
	return predatorConfig[cellIndx];
}

#endif