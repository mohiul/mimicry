#include "ConfigReader.h"
#include "tinyxml.h"

#include <iostream>
#include "stdio.h"

ConfigReader::ConfigReader(std::string _file): configFile(_file)
{
}

void ConfigReader::readConfigFile()
{
	TiXmlDocument doc;
	doc.Parse(const_cast<char*>(configFile.c_str()));
	std::cout << "printing file: " << configFile << std::endl;
	doc.Print();

	TiXmlElement* root = doc.FirstChildElement( "agents" );
	if ( root )
	{
		TiXmlNode* child = 0;
		while( child = root->IterateChildren( "prey", child ) ) {
			std::cout << child->FirstChildElement("rule")->GetText() << std::endl;
		}
	} else {
		std::cout << "bad luck!!" << std::endl;
	}
}