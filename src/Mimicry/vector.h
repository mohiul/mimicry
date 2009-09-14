/**
 * \file vector.h
 * \author Peter Grogono
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

namespace formal
{

/**
 * An instance of \a Vector is a 3D vector with real-valued components.
 * This class is not intended to be a complete implementation of 3D vectors:
 * it provides only the functions currently needed by the FormAL simulation.
 */
class Vector
{
public:

   Vector(double x = 0, double y = 0, double z = 0);
   //Vector(char code);
   double & operator[](int i);
   const double operator[](int i) const;
   void operator+=(const Vector & rhs);
   Vector operator-(const Vector & rhs);
   Vector operator+(const Vector & rhs);
   Vector operator*(double factor);
   Vector operator/(double factor);
   bool operator==(const Vector & rhs);
   bool operator!=(const Vector & rhs);
   void operator*=(double factor);
   double mag();
   Vector normalize();

private:

   /**
    * The \a X, \a Y, and \a Z components of the \a Vector.
    */
   double cpts[3];
};

// Functions associated with class Vector

/**
 * \return the dot product of the vectors \a u and \a v.
 */
inline double dot(const Vector & u, const Vector & v)
{
   return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

/**
 * \return the cross (outer) product of the vectors \a u and \a v.
 */
inline Vector cross(const Vector & u, const Vector & v)
{
   return Vector
   (
      u[1] * v[2] - u[2] * v[1],
      u[2] * v[0] - u[0] * v[2],
      u[0] * v[1] - u[1] * v[0]
   );
}

// Inlined member functions for class Vector.

/**
 * \param i = 0, 1, or 2 chooses a component (\a X, \a Y, or \a Z).
 * \return a reference to a component of the \a Vector that
 * can be used as an lvalue or an rvalue.
 */
inline double & Vector::operator[](int i)
{
   return cpts[i];
}

/**
 * \param i = 0, 1, or 2 chooses a component (\a X, \a Y, or \a Z).
 * \return a \b const reference to a component of the \a Vector that
 * can be used as an rvalue only.
 */
inline const double Vector::operator[](int i) const
{
   return cpts[i];
}

/**
 * Add \a rhs to this \a Vector.
 * \return the vector sum of this \a Vector and \a rhs.
 */
inline void Vector::operator+=(const Vector & rhs)
{
   cpts[0] += rhs[0];
   cpts[1] += rhs[1];
   cpts[2] += rhs[2];
}

/**
 * \return the vector sum of this \a Vector and \a rhs.
 */
inline Vector Vector::operator+(const Vector & rhs)
{
   return Vector(
      cpts[0] + rhs.cpts[0],
      cpts[1] + rhs.cpts[1],
      cpts[2] + rhs.cpts[2] );
}

/**
 * \return the vector difference of this \a Vector and \a rhs.
 */
inline Vector Vector::operator-(const Vector & rhs)
{
   return Vector(
      cpts[0] - rhs.cpts[0],
      cpts[1] - rhs.cpts[1],
      cpts[2] - rhs.cpts[2] );
}

/**
 * \return the \a Vector obtained by multiplying this \a Vector
 by the scalar \a factor.
 */
inline Vector Vector::operator*(double factor)
{
   return Vector(
      cpts[0] * factor,
      cpts[1] * factor,
      cpts[2] * factor );
}

/**
 * \return the \a Vector obtained by dividing this \a Vector
 by the scalar \a factor.
 */
inline Vector Vector::operator/(double factor)
{
   return Vector(
      cpts[0] / factor,
      cpts[1] / factor,
      cpts[2] / factor );
}

/**
 * \return \a true if this \a Vector is equal to \a rhs,
 * otherwise \a false.
 */
inline bool Vector::operator==(const Vector & rhs)
{
   return
      cpts[0] == rhs.cpts[0] &&
      cpts[1] == rhs.cpts[1] &&
      cpts[2] == rhs.cpts[2];
}

/**
 * \return \a false if this \a Vector is equal to \a rhs,
 * otherwise \a true.
 */
inline bool Vector::operator!=(const Vector & rhs)
{
   return
      cpts[0] != rhs.cpts[0] ||
      cpts[1] != rhs.cpts[1] ||
      cpts[2] != rhs.cpts[2];
}

/**
 * \return the product of the scalar \a factor and the \a Vector \a vec.
 */
inline Vector operator*(double factor, Vector vec)
{
   return Vector(
      factor * vec[0],
      factor * vec[1],
      factor * vec[2] );
}

/**
 * Multiply each of the components of this \a Vector
 * by the scalar \a factor.
 * \return the resulting \a Vector.
 */
inline void Vector::operator*=(double factor)
{
   cpts[0] *= factor;
   cpts[1] *= factor;
   cpts[2] *= factor;
}

/**
 * Return the magnitude (sum of squared components) of a \a Vector.
 */
inline double Vector::mag()
{
   return cpts[0] * cpts[0] + cpts[1] * cpts[1] + cpts[2] * cpts[2];
}
} // End of namespace

/**
 * Write a description of the vector with format "(a,b,c)".
 * \param os is the output stream for the description.
 * \param v is the vector to be written.
 */
std::ostream & operator<<(std::ostream & os, formal::Vector v);

#endif
