/**
 * \file vector.cpp
 * \author Peter Grogono
 */

#include "vector.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

namespace formal
{

/**
 * Construct a \a Vector from its components.
 */
Vector::Vector(double x, double y, double z)
{
   cpts[0] = x;
   cpts[1] = y;
   cpts[2] = z;
}

/**
 * \return a normalized version of this \a Vector.
 * If this vector is the zero vector, return the zero vector.
 */
Vector Vector::normalize()
{
   double len = sqrt(cpts[0] * cpts[0] + cpts[1] * cpts[1] + cpts[2] * cpts[2]);
   if (len == 0)
      return Vector();
   else
      return Vector
      (
         cpts[0] /= len,
         cpts[1] /= len,
         cpts[2] /= len
      );
}

}// End of namespace formal

/**
 * Write a string representation of a vector to an output stream.
 * \param os is the output stream.
 * \param v is the vector to be written.
 */
std::ostream & operator<<(std::ostream & os, formal::Vector v)
{
   int width = os.width();
   os << std::setprecision(3) <<
	  std::setw(0) << '(' <<
      std::setw(width) << v[0] << ',' <<
      std::setw(width) << v[1] << ',' <<
      std::setw(width) << v[2] << ')';
   return os;
}
