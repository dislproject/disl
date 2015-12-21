#include "externVar.h"

Energy::Energy()
{
    En=0.;
}

void Energy::Calc_energy()
{
    // Calculate self energies
    double En_self=0.; // zero self energy
    for(int k=0;(k<D.size());k++){

        En_self+=Iso_self_en(k);
    }

    En=0.; // reset energy to zero

    for(int k=0;(k<D.size());k++){
        for(int l=k+1;(l<D.size());l++)
        {
            En+=Iso_en(k,l,D[l].xd);
        }

        for(int l=0;l<D.size();l++){
            for (int j=-num_cells;j<=num_cells;j++)
            {
                if (j!=0)
                {
                    double x=(D[l].xd+(j*material_width));
                    En+=0.5*Iso_en(k,l,x);
                }
            }
        }
    }

    En+=(En_self);
    En=En/(material_height*material_width*1E-10*1E-10);

    if(fabs(En)>fabs(max_energy)){
        max_energy=/*fabs*/(En);
    }
}

void Energy::Calc_energy_test()
{
    En=0;
    int k=D.size()-1;

        for(int l=0;(l<D.size()-1);l++)
        {
            En+=Iso_en(k,l,D[l].xd);
        }

        for(int l=0;l<D.size();l++){
            for (int j=-num_cells;j<=num_cells;j++)
            {
                if (j!=0)
                {
                    double x=(D[l].xd+(j*material_width));
                    En+=0.5*Iso_en(k,l,x);
                }
            }
        }


    En=En/(material_height*material_width*1E-10*1E-10);
    //En-=6E7;
    En=En/basal_B;
//    qDebug()<<En;

    if(fabs(En)>fabs(max_energy)){
        max_energy=/*fabs*/(En);
    }
}

double Energy::Iso_en(int d1, int d2, double x)
{
    // H&L p.117 (127) Energy of interation per unit length
    vectorDim burg1,burg2,xi,vectR; // vectors for disl1, disl2 & line vector (xi)
    burg1<<D[d1].bx,D[d1].by,0; // B prismatic
    burg2<<D[d2].bx,D[d2].by,0;
    xi<<0,0,1; // line vector
    vectR<<(x-D[d1].xd)*1E-10,(D[d2].y-D[d1].y)*1E-10,0; // vector from x1 to x2

    // Energy for Wij/L:
    R=vectR.norm(); // sep of disl1, disl2

    if (D[d1].B==1||D[d1].B==2){

        res=-Kex*(1-nu)/(2.*M_PI)*burg1.dot(xi)*burg2.dot(xi)*log(R/Ra);
        res+=-Kex/(2*M_PI)*(burg1.cross(xi)).dot(burg2.cross(xi))*log(R/Ra)\
                -Kex/(2*M_PI*R*R)*(burg1.cross(xi)).dot(vectR)*\
                (burg2.cross(xi)).dot(vectR);
    }
    if(D[d1].B==3||D[d1].B==4){
        res=-Key*(1-nu)/(2.*M_PI)*burg1.dot(xi)*burg2.dot(xi)*log(R/Ra);
        res+=-Key/(2*M_PI)*(burg1.cross(xi)).dot(burg2.cross(xi))*log(R/Ra)\
                -Key/(2*M_PI*R*R)*(burg1.cross(xi)).dot(vectR)*\
                (burg2.cross(xi)).dot(vectR);
    }

    if(isnan(res)!=0){/*qDebug()<<"D D"<<d1<<d2<<D[d1].x<<D[d2].x<<D[d1].y<<D[d2].y<<D[d1].B<<D[d2].B<<D[d1].group<<D[d2].group<<D[d1].number<<D[d2].number<<R;*/return 0;}

    return res;
}

double Energy::Iso_self_en(int i){


    if(D[i].by==0){
        return (Kex*pow(D[i].bx,2))/(4*M_PI)*(log(Ra/Rc)-1);}
    else if(D[i].bx==0){
        return (Key*pow(D[i].by,2))/(4*M_PI)*(log(Ra/Rc)-1);}
    else return 0;

}


double Energy::Ma_Iso_en(int d1, int d2, double x)
{
    vectorDim vectR; // vectors for disl1, disl2 & line vector (xi)
    vectR<<(x-D[d1].xd)*1E-10,(D[d2].y-D[d1].y)*1E-10,0; // vector from x1 to x2
    R=vectR.norm();
    double o_h=(x-D[d1].xd)/(R*1E10);
    double ang=(asin(o_h));

    if (D[d1].B==3||D[d2].B==4)
    {
        res=((Key*D[d1].B1*D[d2].B1)/(2*M_PI))*(log(R/Ra)+(0.5*cos(2*ang))-1);
    }
    //     qDebug()<<"1"<<x<<D[d1].xd<<D[d2].xd<<x-D[d1].xd<<R<<(D[d2].y-D[d1].y)<<o_h<<ang<<res;
    return res;
}
