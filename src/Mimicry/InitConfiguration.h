/**
 * \file InitConfiguration.h
 */

#ifndef INIT_CONFIGURATION_H
#define INIT_CONFIGURATION_H

#include "System.h"
#include "Agent.h"

#include <string>
#include <vector>

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

struct PredatorConfig
{
	PredatorConfig()
	{
		population = 0;
	}
	int population;
};

class InitConfiguration
{
private:
	PreyConfig* preyConfig[ISIZE*ISIZE*ISIZE];
	PredatorConfig* predatorConfig[ISIZE*ISIZE*ISIZE];
public:
	InitConfiguration();
	bool readConfigFile(std::string configFile);
	PreyConfig* getPreyConfig(int cellIndx);
	PredatorConfig* getPredatorConfig(int cellIndx);
	void printInitConfig();
};

inline PreyConfig* InitConfiguration::getPreyConfig(int cellIndx)
{
	return preyConfig[cellIndx];
}

inline PredatorConfig* InitConfiguration::getPredatorConfig(int cellIndx)
{
	return predatorConfig[cellIndx];
}

#endif