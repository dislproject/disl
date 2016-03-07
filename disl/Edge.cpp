#include "externVar.h"


///////////////////EDGE-CONSTRUCTOR///////////////
Edge::Edge()
{
    x = y = 0.; // positions for perfect crystal
    ux = uy = 0.; // displacement from perfect
    type = 5; // initialise type
    count = 0;
    alive = true;
}

////////////////////EDGE STRAIN////////////////////
// isotropic strain to be applied to edge of crystal

// which Burger's vector
void Edge::Edge_strain(double B1, double x_in, double y_in)
{

    ry = y_in - y;
    rx = x_in - x;
    r_sq = pow(rx, 2) + pow(ry, 2);

    //if (rx >= 0.)
    //    theta = 0.;                // correction for atan2
    //else
    //    theta = 2 * M_PI;

    // calculate strain - isotropic
    ux += (B1 / (2 * M_PI)) * ((atan2(ry, rx) + theta)      \
                               + (rx * ry) / (2 * (1 - nu) * r_sq));
    uy += -(B1 / (2 * M_PI)) * (((1 - 2 * nu) * log(r_sq)) / (4 * (1 - nu)) +        \
                                (pow(rx, 2) - pow(ry, 2)) / (r_sq * 4 * (1 - nu)));


    // nabarro displacements

    /*
    ux += B1 / (4 * M_PI * (1 - nu)) * (rx * ry) / (r_sq) - B1 / (2 * M_PI) * atan2(rx, ry) + theta;
    uy += -((1 - 2 * nu) * B1) / (8 * (1 - nu) * M_PI) * log(r_sq / pow(B1, 2)) \
        + (B1 / ((1 - nu) * 4 * M_PI)) * pow(ry, 2) / (pow(rx, 2) + pow(ry, 2));
     */
}


void Edge::Strain_aniso(double bx, double by, double x_in, double y_in, int type)
{

    // anisotropic displacement field from H&L p451
    theta1 = theta2 = 0.; // set thetas to zero
    //
    rxa = (x - x_in) * (1E-10); // basal -M_PI, M_PI, prismatic M_PI, M_PI
    rya = (y - y_in) * (1E-10);


    if (type == 3 || type == 4) //prismatics
    {
        // theta 1 - one arg
        if (rxa >= 0. && rya <= 0.) //1
        {
            theta1 = 3.5 * M_PI;
            theta2 = -2. * M_PI;
        }
        if (rxa < 0. && rya <= 0.)  //2
        {
            theta1 = -0.5 * M_PI;
            //theta2 = 2. * M_PI;
        }
        if (rxa < 0. && rya > 0.)   //3
        {
            theta1 = -0.5 * M_PI;
            theta2 = 0.;
        }
        if (rxa >= 0. && rya > 0.)  //4
        {
            theta1 = -4.5 * M_PI;
            theta2 = 2. * M_PI;
        }
    }


    if (type == 1 || type == 2) //basals
    {
        // theta 1 - one arg
        if (rxa >= 0. && rya <= 0.)
        {
            theta1 = -1. * M_PI;
            theta2 = 0. * M_PI;
        }
        if (rxa < 0. && rya <= 0.)
        {
            theta1 = 1. * M_PI;
            //theta2 = 2. * M_PI;
        }
        if (rxa < 0. && rya > 0.)
        {
            theta1 = 1. * M_PI;
            theta2 = 0.;
        }
        if (rxa >= 0. && rya > 0.)
        {
            theta1 = -1. * M_PI;
            theta2 = 0. * M_PI;
        }
    }

    /*
    if (rxa < 0. && rya > 0. && pow(pow(rya, 2), 1 / 2) < rxa)
    {
        theta1 = theta2 = -2 * M_PI;
    }
    if (rxa < 0. && rya > 0. && pow(pow(rya, 2), 1 / 2) > rxa)
    {
        theta1 = theta2 = 2 * M_PI;
    }
    else if (rxa < 0. && rya > 0.)
    {
        theta1 = 0.;//2*M_PI;
    }
     */

    // theta2 - second arg
    if ((pow(rxa, 2) + lambda * lambda * pow(rya, 2)) < 0.)
    {
        //if (pow(rxa, 2) + lambda * lambda * pow(rya, 2) < 0.)
        //{
        theta2 = 0.; // 2.*M_PI;
        //}
    }
    ux1 = -bx / (4 * M_PI) * (atan2((2 * (rxa) * (rya) * lambda * chd), (pow(rxa, 2) - pow(lambda, 2) * pow(rya, 2))) + theta1 \
                              + (C11 * C33 - C13 * C13) / (2 * pow(C11 * C33, .5) * C44 * sh2d) * atan2(2 * (rxa) * (rya) * lambda * shd    \
                                                                                                        , (pow(rxa, 2) + lambda * lambda * pow(rya, 2))) + theta2);

    ux2 = -by / (4 * M_PI * lambda * pow(C11 * C33, 0.5) * sh2d) * \
        ((pow(C11 * C33, 0.5) - C13) * shd * \
         log(pow(pow(pow(rxa, 2) + pow(rya, 2) \
                     * lambda * lambda, 2) + pow(2 * (rxa) * (rya) * lambda * shd, 2), .5)));

    ux3 = -by / (4 * M_PI * lambda * pow(C11 * C33, 0.5) * sh2d) * (pow(C11 * C33, 0.5) + C13) * chd * \
        1 / 2 * log((1 - (pow(rxa, 2) * sinh(2 * delta))               \
                     / (lambda * lambda * pow(rya, 2) + pow(rxa, 2) * cosh(2 * delta))) \
                    / (1 + (pow(rxa, 2) * sinh(2 * delta)) / (lambda * lambda * pow(rya, 2) + \
                                                              pow(rxa, 2) * cosh(2 * delta))));
    //qDebug() << "ux" << ux1 << ux2 << ux3;
    ux += (ux1 + ux2 + ux3) * (1E10);
    //ux = 0;


    // uy
    //
    uy1 = lambda * bx / (4 * M_PI * pow(C11 * C33, 0.5) * sh2d) * ((pow(C11 * C33, 0.5) - C13) * \
                                                                   shd * log(pow(pow(pow(rxa, 2) + pow(rya, 2) \
                                                                                     * lambda * lambda, 2) + pow(2 * (rxa) * (rya) * lambda * shd, 2), .5)));

    uy2 = lambda * bx / (4 * M_PI * pow(C11 * C33, 0.5) * sh2d) * ((pow(C11 * C33, 0.5) + C13) * chd * \
                                                                   1 / 2 * log((1 - pow(rya, 2) * sh2d * lambda * lambda              \
                                                                                / (pow(rxa, 2) + lambda * lambda * pow(rya, 2) * ch2d))     \
                                                                               / (1 + lambda * lambda * pow(rya, 2) * sh2d         \
                                                                                  / (pow(rxa, 2) + lambda * lambda * pow(rya, 2) * ch2d))));

    uy3 = -by / (4 * M_PI) * (atan2((2 * (rxa) * (rya) * lambda * chd) \
                                    , (pow(rxa, 2) - pow(lambda, 2) * pow(rya, 2))) + theta1 - (C11 * C33 - C13 * C13) \
                              / (2 * pow(C11 * C33, .5) * C44 * sh2d) * atan2(2 * (rxa) * (rya) * lambda * shd \
                                                                              , (pow(rxa, 2) + lambda * lambda * pow(rya, 2))) + theta2);

    //qDebug() << "uy" << uy1 << uy2 << uy3;
    uy += (/*uy1 + uy2 + */ uy3) * (1E10);


}


