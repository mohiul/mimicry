#ifndef RAND_H
#define RAND_H

#include <cassert>
#include <cstdlib>
#include <cmath>

/**
 * Return a random integer >= 0.
 * \param max is one more than the largest number that will be returned.
 */
inline int randomInteger(int max)
{
   return rand() % max;
}

#endif
