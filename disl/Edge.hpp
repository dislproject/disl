#include <math.h>
#ifndef edge_length
#define edge_length 3.32 // arbitrary edge length
#endif  

class Disl;

class Edge {

private:

  // Burger's vector in Edge_strain

  double theta;  // correction for atan2
  double r_sq;   // distance between segment_i and disl_j
  double rx, ry; // x and y components of r
  // ******* for anisotropic **********
  double theta1, theta2;
  double rxa,rya;
  double ux1, ux2, ux3;
  double uy1, uy2, uy3;


public:
  double count, counth;
  bool alive;
  ////// Parameters //////
  double x, y, ux, uy, B1;
  int type; // (top/bottom = 5) or (left/right = 6)

  ////// Functions //////
  Edge(); // constructor
  // isotropic function
  void Edge_strain( double, double, double ); // strain field (isotropic)
  // anisotropic function
  void Strain_aniso(double bx, double by, double x_in, double y_in, int type);

};



