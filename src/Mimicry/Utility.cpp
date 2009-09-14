/**
 * \file Utility.cpp
 */

#include "Utility.h"

/**
 * Check's equality between two vectors. 
 * \return True if equal, or else false.
 * \param vec1 Vector object to check equality
 * \param vec2 Another vector object to compare for equality.
 */
bool Utility::checkEquals(const vector<int> vec1, const vector<int> vec2)
{
	bool isEqual = true;
	if (vec1.size() != vec2.size())
		isEqual = false;
	else
		for(unsigned int i = 0; i < vec1.size(); i++)
			if (vec1(i) != vec2(i))
				isEqual = false;
	return isEqual;
}