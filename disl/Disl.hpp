#ifndef Ra // en calc from Ra(outer limit) to R (disl pos)
#define Ra 1E-4
#endif
#ifndef Rc // Core radius
#define Rc 3.32E-10
#endif
#ifndef mu
#define mu 1.
#endif
//#ifndef x_max // x limit of area for disls
//#define x_max 1190.
//#endif
//#ifndef y_max // y limit of area for disls
//#define y_max 1640.
//#endif
#ifndef DISL_HPP
#define DISL_HPP

#ifndef nu
#define nu 0.31 // poisson ratio
#endif
#ifndef pris_B
#define pris_B 3.32 // absolute value of Burger's vector
// changed for test output
#endif
#ifndef AC_B
#define AC_B 2.46E-10
#endif
#ifndef ZZ_B
#define ZZ_B 4.26E-10
#endif
#ifndef speed_limit
#define speed_limit 10.0 // (was 2) limit for movement of dislocation
#endif
#ifndef coeff
#define coeff 8. // coefficient to multiply force for posn change
#endif
#ifndef int_layer_sep
#define int_layer_sep 3.32 // interlayer speparation (Angs)
#endif
#ifndef atom_sep
#define atom_sep 1.42 // atom speparation (Angs)
#endif
#define delta  1.82896658   // 1/2*acos(arg)
#define ch2d   19.40344566  // cosh(2delta)
#define sh2d   19.37765991  // sinh(2delta)
#define shd    3.033434164  // sinh(delta)
#define chd    3.194013592  // cosh(delta)
#define lambda 2.321417480  // lambda=(C11/C33)^1/4
#define C11 1060E9 // Elastic constants
#define C12 180E9
#define C13 7.9E9
#define C33 36.5E9
#define C44 5.05E9

//#ifndef edge_segs
//#define edge_segs 4*157
//#endif
#ifndef edge_length
#define edge_length 4 // arbitrary edge length
#endif
//#define disl_lays 8 // # of disl layers of dipoles
// #define layers
/*
   #ifndef A1
   #define A1 14.1539081
   #endif
   #ifndef A2
   #define A2 5.388979114//(C11*C33)^0.5/C33
   #endif
   #ifndef A3
   #define A3 198.98//200
   #endif
   #ifndef A4
   #define A4 3.966165E22//39500
   #endif
   #ifndef A5
   #define A5 1.84325E20//183
   #endif
   #ifndef A6
   #define A6 1.237787506E12//1240
   #endif
   #ifndef A7
   #define A7 803732462//0.798
   #endif
 */

class Edge;

class Disl
{

private:
    int i;

public:

    ////// Parameters //////

    // Dislocation properties
    // current posns and previous posns
    double x, y, xd;
    int B;
    // 1=up,2=down,3=left,4=right,
    // for in plane 5=AC30_1, 6=AC30_2, 7=AC90_1, 8=AC90_2, 9=AC150_1, 10=AC150_2,
    //              11=ZZ0_1, 12=ZZ0_2, 13=ZZ60_1, 14=ZZ60_2, 15=ZZ120_1, 16=ZZ120_2.
    //


    double B1; // magnitude of B
    double bx, by;
    double f, df, fx, fy;
    bool re_run;
    bool crack;
    bool fixed;
    int group;
    bool flagged;
    int number;
    bool min;

    // properties of edge of crystal
    //double ux, uy; // displacement vector entries for edge

    // Forces
    // double Fx; // x-Force between dislocations

    // number of plane. for adding half plane at edge
    //int plane;
    // edge positions relative to dislocation glide plane
    // int edge1, edge2;

    ////// Functions //////
    Disl(); // Constructor (make random dipole posns)
    //void setConstants(int, int, double, int, int);

    // values for dipole pair of disl up to disl_num-1
    void Dipole_pair(int&, int&, double&, double&, int&);
    double Calc_Fx(double&, double&, double&); // calc x-force wrt other disls
    void Move(double&);
    void Check(int&, int&, Disl D[], Edge E[]);
    void Edge_strain();

    void Echo(int&, double&); // print to screen for testing
    void Echo(int&); // final print to screen for testing

    friend class Edge;

};

#endif // DISL_HPP
