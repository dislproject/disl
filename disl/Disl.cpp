#include "externVar.h"

/////////////////////////////DISL-CONSTRUCTOR//////////////////
Disl::Disl()
{
    crack = false;
    re_run = false;
    f = 0;
    df = 0;
    fixed = false;
    flagged = false;
    min = false;


    //set alive to true
    //alive = true;

    // create random x,y positions
    // keep within limits of x_max, y_max
    //x = separation + 1. + double( rand() % int( x_max - 2 * int( separation ) - 1));

    //plane = rand() % int( y_max / int_layer_sep );

    //y = double( plane ) * int_layer_sep;

    // if x even -> B up (positive)
    // ( int( x ) % 2 != 0 ) ? B = 3 : B = 4;

    //if (B == 3)
    //    B1 = -basal_B;        // set magnitude of B
    //else
    //    B1 = basal_B;
    // override random Burger's sign for circle
    //B1 = basal_B; // initialise prev variables
    //prev_x = prev_y = 0.;
}

