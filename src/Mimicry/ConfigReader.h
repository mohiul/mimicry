/**
 * \file ConfigReader.h
 */

#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>

class ConfigReader
{
private:
	std::string configFile;
public:
	ConfigReader(std::string _file);
	void readConfigFile();
};

#endif