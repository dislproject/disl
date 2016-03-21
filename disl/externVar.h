#ifndef EXTERNVAR_H
#define EXTERNVAR_H

// Define a macro to avoid gcc "-Wunused-parameter/variable" warnings
// on variables which are not used (especially function parameters)
#define UNUSED(...) (void)(__VA_ARGS__)

#include <Eigen/Dense>

#include <QtWidgets>

#include <stdio.h>
#include <stdlib.h>

extern double total_area;
extern double material_height;
extern double material_width;
extern int x_max;
extern int y_max;
extern int disl_num;
extern int T;
extern int S;
extern int e_calc;
extern int a_calc;
extern int inplane;
extern int num_its;
extern double basal_B;
extern double cexpansion;
extern double aexpansion;
extern double res;
extern int its;
extern int force_iteration;
extern int timesteps;
extern int edge_segments;
extern int disl_lays;
extern int disl_columns;
extern int initial_disl_num;
extern int inc;
extern double max_cexp;
extern double max_aexp;
extern double max_energy;
extern double max_area_change;
extern int group_number;
extern int dislocation_number;
extern int num_cells;
extern double applied_stress;
extern double sxx;
extern double sxy;
extern double sxz;
extern double syx;
extern double syy;
extern double syz;
extern double szx;
extern double szy;
extern double szz;


#include "Edge.hpp"
#include "Disl.hpp"
#include "Energy.hpp"
#include "interaction.h"
#include "test.hpp"

extern int basals;
extern Edge E[];
extern Edge E_new[];
extern Energy En;
extern double En_self;
extern QList<Disl> D;
extern QList<Disl> ImgD;

#include "Structure.hpp"
extern Structure structure;
extern Interaction interaction;
extern Test test;
extern double Key;
extern double Kex;
extern double A1, A2, A3, A4, A5, A6, A7;


extern QList<QPoint> listOfAPoints;
extern QList<double> listOfEXPos;
extern QList<double> listOfEYPos;
extern QList<double> listOfCXPos;
extern QList<double> listOfCYPos;
extern QList<double> listOfAYPos;
extern QList<double> listOfAreaYPos;
extern QList<double> listOfAreaXPos;

#include "Force.hpp"

#include "inplane.hpp"
extern Inplane P;
extern int inplane_h_planes; //number of horizontal glide planes
extern int inplane_a_planes; // number of angled glide planes

#include "strain_map.hpp"
extern Strain_map MP;

#include "grid.hpp"
extern Grid grid[][500];

#endif // EXTERNVAR_H
