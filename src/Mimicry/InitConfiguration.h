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
		population = 0;
	}
	int rule;
	int population;
	bool palatibility;
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
	void readConfigFile(std::string configFile);
	void printInitConfig();
};

#endif