#include "InitConfiguration.h"
#include "tinyxml.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

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

void InitConfiguration::readConfigFile(std::string configFile)
{
	std::ifstream xmlFile(configFile.c_str(), std::ios_base::in);
	std::string xmlStr = "";
	std::string temp = "";

	while (xmlFile >> temp)
		xmlStr.append(temp);

	std::cout << "XML->" << xmlStr << "<-XML" << std::endl;
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

			std::string palatibilityInfo = std::string(child->FirstChildElement("palatability")->GetText());
			bool palatability = false;
			if(palatibilityInfo.compare("true"))
				palatability = true;

			TiXmlElement* location = child->FirstChildElement( "location" );
			int from = atoi(location->FirstChildElement("from")->GetText());
			int to = atoi(location->FirstChildElement("to")->GetText());

			int cellIndx = from;
			for(int i = 1; i <= pop; i++)
			{
				preyConfig[cellIndx]->rule = rule;
				preyConfig[cellIndx]->palatibility = palatability;
				preyConfig[cellIndx]->population++;

				if(cellIndx == to)
					cellIndx = from;
				cellIndx++;
			}
		}
		while( child = agents->IterateChildren( "predator", child ) ) {
			int pop = atoi(child->FirstChildElement("population")->GetText());
			std::cout << "pop: " << pop << std::endl;

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
}

void InitConfiguration::printInitConfig()
{
	int cellIndx = 0;
	for(int i = 0; i < ISIZE; i++)
		for(int j = 0; j < ISIZE; j++)
			for(int k = 0; k < ISIZE; k++)
			{
				std::cout << "cellIndx: " << cellIndx 
					<< " rule:" << preyConfig[cellIndx]->rule
					<< " palatibility:" << preyConfig[cellIndx]->palatibility
					<< " pop:"  << preyConfig[cellIndx]->population
					<< " predator pop:"  << predatorConfig[cellIndx]->population
					<< std::endl;
				cellIndx++;
			}
}