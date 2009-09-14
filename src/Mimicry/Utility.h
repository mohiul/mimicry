/**
 * \file Utility.h
 */

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

using boost::numeric::ublas::vector;

/**
 * \a Utility class contains mostly static functions which 
 * are required for utility pruposes.
 */
class Utility
{
public:
	static bool checkEquals(const vector<int> vec1, const vector<int> vec2);
};