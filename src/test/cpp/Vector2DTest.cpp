#include <iostream>
#include "util/Vector2D.hpp"
#include "util/Log.hpp"

using namespace util;
using namespace std;

typedef Vector2D<double> Vector;

void equals(Vector const & a, Vector const & b) 
{
  ASSERT(equals(a.x(), b.x()));
  ASSERT(equals(a.y(), b.y()));
}

void equals(Vector const & a, double x, double y) 
{
  ASSERT(equals(a.x(), x));
  ASSERT(equals(a.y(), y));
}

void testVector()
{
  Vector v = Vector(0, 1);
  ASSERT(equals(v.angleDegrees(), 90));
  ASSERT(equals(v.radius(), 1));
  v.setCartesian(0, 2);
  ASSERT(equals(v.angleDegrees(), 90));
  ASSERT(equals(v.radius(), 2));
  v.setCartesian(1.5, 0);
  ASSERT(equals(v.angleDegrees(), 0));
  ASSERT(equals(v.radius(), 1.5));
  v.setCartesian(0, -1);
  ASSERT(equals(v.angleDegrees(), 270));
  ASSERT(equals(v.radius(), 1));
  v.setCartesian(-3, 0);
  ASSERT(equals(v.angleDegrees(), 180));
  ASSERT(equals(v.radius(), 3));
  
  v.setPolarDegrees(90, 10);
  equals(v, 0, 10);

  v.setPolarDegrees(0, 15);
  equals(v, 15, 0);

  v.setPolarDegrees(180, 5);
  equals(v, -5, 0);
  
  v.setPolarDegrees(270, 4.5);
  equals(v, 0, -4.5);
  
  v.setCartesian(4.6, -11.8);
  ASSERT(equals(v.angleDegrees(), 291.297354));
  
  v.setPolarDegrees(135, 3 * sqrt(2.0));
  equals(v, -3, 3);

  v.setPolarDegrees(135, -3 * sqrt(2.0));
  equals(v, 3, -3);

  v.setPolarDegrees(135 + 360, 3 * sqrt(2.0));
  equals(v, -3, 3);

  v.setPolarDegrees(135 - 360, 3 * sqrt(2.0));
  equals(v, -3, 3);
}

void testMultDiv()
{
  Vector a(2, 2, true);
  equals(a, 2, 2);

  a *= 1.5;
  equals(a, 3, 3);

  Vector b(a * 2);
  equals(a, 3, 3);
  equals(b, 6, 6);
  
  Vector c(0.5 * a);
  equals(a, 3, 3);
  equals(c, 1.5, 1.5);
  
  Vector d(a / 3);
  equals(a, 3, 3);
  equals(d, 1, 1);
  
  Vector i(2, 2, true);
  Vector j(M_PI / 4.0, sqrt(8.0), false);
  equals(i * 2, j * 2);
  equals(i / 3.3, j / 3.3);
  equals(2.6 * i, 2.6 * j);
}

void testDistance()
{
  ASSERT(equals(Vector(2,2).distance(Vector(0,0)), sqrt(8.0)));
  ASSERT(equals(Vector(1,1).distance(Vector(4.5,1)), 3.5));
  
  Vector a(4, 4);
  a = a.scaleRadiusTo(1);
  ASSERT(equals(a.radius(), 1));
  ASSERT(equals(a.angleDegrees(), 45));
  
  equals(a, 1.0/sqrt(2),1.0/sqrt(2));

  a = Vector(-1, 3);
  a = a.scaleRadiusTo(1);
  ASSERT(equals(a.radius(), 1));
  ASSERT(equals(a.angleDegrees(), 108.434949));
  equals(a, -0.316227766, 0.948683298);
}

void testAngle()
{
  Vector t(2, 2);
  equals(t, 2, 2);
  equals(-t, -2, -2);
 
  ASSERT(equals(Vector(0,0).angleTowardsDegrees(Vector(2,2)), 45));
  ASSERT(equals(Vector(2,2).angleTowardsDegrees(Vector(0,0)), 225));
  ASSERT(equals(Vector(0,0).angleTowardsRadians(Vector(2,2)), M_PI/4));
  ASSERT(equals(Vector(2,2).angleTowardsRadians(Vector(0,0)), 5*M_PI/4));

  ASSERT(equals(Vector(-2,1).angleTowardsDegrees(Vector(5,-0.4)), 348.690068));
}

int main(int argc, char ** argv) 
{
  cout << "Testing Vector2D ........" << endl;
  cout << "-------------------------" << endl;  

  cout << "Vector ............... ";
  testVector();
  cout << "ok" << std::endl; 

  cout << "Multipl. & divis. .... ";
  testMultDiv();
  cout << "ok" << std::endl; 

  cout << "Distance ............. ";
  testDistance();
  cout << "ok" << std::endl; 

  cout << "Angle ................ ";
  testAngle();
  cout << "ok" << std::endl; 

  cout << "-------------------------" << endl;
  cout << "All tests passed!" << endl;  
  return 0;
}
