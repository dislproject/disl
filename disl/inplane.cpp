#include "externVar.h"


//Class for inplane dislocation dynamics.
//i.e non-basal dislocation dynamics in graphene.
//


Inplane::Inplane()
{
}

int Inplane::run_inplane(int option)
{
    //running inplane dislocation dynamics program.
   // qDebug()<<"Running in plane dislocation mapping";
    //set up material outline and elastic constants. In this program the x direction is in the armchair direction and y is the zig zag direction.
    structure.Build_Square_Ed(E);
    Kex=1;
    Key=1;

    //generate random dislocation positions
    for (int i=0; i<initial_disl_num; i++)
    {
        structure.Add_nonBasals();
    }

    //calculate stress/strains due to dislocations
//    structure.calculate_edge_strains();

    MP.calculate_strainmap(option);

    //return to main thread to plot to screen
return 0;
}
