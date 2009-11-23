/**
 * \file InitConfiguration.cpp
 */

#include "InitConfiguration.h"
#include "tinyxml.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

/**
 * Constructor for InitConfiguration class
 */
InitConfiguration::InitConfiguration()
{
	int cellIndx = 0;
	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				preyConfig[cellIndx] = new PreyConfig;
				predatorConfig[cellIndx] = new PredatorConfig;
				cellIndx++;
			}
}

/**
 * Read the configuration XML file which contains the initial configuration
 * of all the species and setup the data structure used to create initial 
 * configuration.
 */
bool InitConfiguration::readConfigFile(std::string configFile)
{
	std::ifstream xmlFile(configFile.c_str(), std::ios_base::in);

	if( !xmlFile ) {
		std::cerr << "Error opening input file: " << configFile.c_str() << std::endl;
		return false;
	}

	std::string xmlStr = "";
	std::string temp = "";

	while (xmlFile >> temp)
		xmlStr.append(temp);

	xmlFile.close();

	TiXmlDocument doc;
	doc.Parse(const_cast<char*>(xmlStr.c_str()));

	TiXmlElement* root = doc.FirstChildElement( "root" );
	if ( root )
	{
		TiXmlElement* agents = root->FirstChildElement( "agents" );
		TiXmlNode* child = 0;
		while( child = agents->IterateChildren( "prey", child ) ) {
			int rule = atoi(child->FirstChildElement("rule")->GetText());
			int pop = atoi(child->FirstChildElement("population")->GetText());

			std::string palatabilityInfo = std::string(child->FirstChildElement("palatability")->GetText());
			bool palatability = false;
			if(palatabilityInfo.compare("true"))
				palatability = true;

			TiXmlElement* location = child->FirstChildElement( "location" );
			int from = atoi(location->FirstChildElement("from")->GetText());
			int to = atoi(location->FirstChildElement("to")->GetText());

			int cellIndx = from;
			for(int i = 1; i <= pop; i++)
			{
				preyConfig[cellIndx]->rule = rule;
				preyConfig[cellIndx]->palatability = palatability;
				preyConfig[cellIndx]->population++;

				if(cellIndx == to)
					cellIndx = from;
				cellIndx++;
			}
		}
		child = 0;
		while( child = agents->IterateChildren( "predator", child ) ) {
			int pop = atoi(child->FirstChildElement("population")->GetText());

			TiXmlElement* location = child->FirstChildElement( "location" );
			int from = atoi(location->FirstChildElement("from")->GetText());
			int to = atoi(location->FirstChildElement("to")->GetText());

			int cellIndx = from;
			for(int i = 1; i <= pop; i++)
			{
				predatorConfig[cellIndx]->population++;

				if(cellIndx == to)
					cellIndx = from;
				cellIndx++;
			}
		}
	} else {
		std::cout << "Initial configuration file unformatted!!" << std::endl;
	}
	return true;
}

/**
 * Print the initial configuration of all the species in console.
 */
void InitConfiguration::printInitConfig()
{
	int cellIndx = 0;
	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				std::cout << "cellIndx: " << cellIndx 
					<< " rule:" << preyConfig[cellIndx]->rule
					<< " palatability:" << preyConfig[cellIndx]->palatability
					<< " pop:"  << preyConfig[cellIndx]->population
					<< " predator pop:"  << predatorConfig[cellIndx]->population
					<< std::endl;
				cellIndx++;
			}
}