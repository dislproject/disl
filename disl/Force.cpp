#include "externVar.h"

Force::Force()
{
}

void Force::move_basals()
{
    for(int d1=0; d1<D.size(); d1++)
    {
        if (D[d1].B==1||D[d1].B==2)
        {
            calc_total_diff_x(d1);
        }

    }

    for (int d=0; d<D.size(); d++){
        if (D[d].B==1||D[d].B==2)
        {
            bool loop=false;
            double move=0;
            move=-100*D[d].f;
            if(isnan(move)!=0){move=0;}
            if(move>40){move=40;} //put an upper limit on how far loops can move.
            if(move<-40){move=-40;}



            D[d].x+=move;
            D[d].xd=D[d].x;
            if ((D[d].x>=300+(num_cells*material_width))||(D[d].x<300-(num_cells*material_width)))
            {loop=true;}
            else
            {
                while (D[d].xd>=300+material_width)
                {
                    D[d].xd-=material_width;
                }
                while (D[d].xd<300)
                {
                    D[d].xd+=material_width;
                }
            }
            if (loop==true)
            {
                int c=(D[d].x-300)/material_width;

                D[d].x-=(c*material_width);
                D[d].xd=D[d].x;
                while (D[d].xd>=300+material_width)
                {
                    D[d].xd-=material_width;
                }
                while (D[d].xd<300)
                {
                    D[d].xd+=material_width;
                }


            }

        }
    }
}

void Force::calc_total_diff_x(int d1)
{
    double force=0.;
    double diff=0.;
    for (int d2=0;d2<D.size();d2++)
    {
        for(int i=-num_cells;i<=num_cells;i++)
        {
            if (i!=0)
            {
                double x2=D[d2].xd+(i*material_width);
                double fce=calc_f_d1_d2(d1,d2,x2);
                double dif=calc_diff_d1_d2(d1,d2,x2);
                force+=fce;
                diff+=dif;
            }
            else
            {
                if(d1!=d2)
                {
                    double dif=calc_diff_d1_d2(d1,d2,D[d2].xd);
                    double fce=calc_f_d1_d2(d1,d2,D[d2].xd);
                    force+=fce;
                    diff+=dif;
                }
            }
        }
    }
    D[d1].df=(diff*1E-9)*10;
    D[d1].f=force+(appStress(d1));
//    qDebug()<<D[d1].df<<D[d1].f;
    //  qDebug()<<"force ="<<force<<", 2nd diff ="<<diff;confirm_diff(d1);
    //if (d1==0){qDebug()<<"force ="<<force<<", 2nd diff ="<<diff;confirm_diff(d1);}
    //   if (d1==1){qDebug()<<"force ="<<force<<", 2nd diff ="<<diff;confirm_diff(d1);}
}

void Force::confirm_diff(int d)
{
    double En1, En2, En0;
    En1=En2=En0=0.0;
    double dx=double(0.8*1E-10);
    D[d].xd+=(1E10*dx);
    //calc energy for x+dx
    for (int d2=0;d2<D.size();d2++)
    {
        if (d!=d2)
        {
            En1+=En.Iso_en(d,d2,D[d2].xd);
        }
        for (int j=-num_cells;j<=num_cells;j++)
        {
            if (j!=0)
            {
                double x=D[d2].xd+(j*material_width);
                En1+=0.5*En.Iso_en(d,d2,x);
            }
        }
    }

    D[d].xd-=(2E10*dx);
    //calc energy for x-dx
    for (int d2=0;d2<D.size();d2++)
    {
        if (d!=d2)
        {
            En2+=En.Iso_en(d,d2,D[d2].xd);
        }
        for (int j=-num_cells;j<=num_cells;j++)
        {
            if (j!=0)
            {
                double x=D[d2].xd+(j*material_width);
                En2+=0.5*En.Iso_en(d,d2,x);
            }
        }
    }

    D[d].xd+=(1E10*dx);
    //calc energy for x
    for (int d2=0;d2<D.size();d2++)
    {
        if (d!=d2)
        {
            En0+=En.Iso_en(d,d2,D[d2].xd);
        }
        for (int j=-num_cells;j<=num_cells;j++)
        {
            if (j!=0)
            {
                double x=D[d2].xd+(j*material_width);
                En0+=0.5*En.Iso_en(d,d2,x);
            }
        }
    }

    double dedx=(En2-En1)/(2*dx);
    double d2edx2=(En1+En2-(2*En0))/(dx*dx);
    qDebug()<<"dE/dx ="<<dedx<<", d2E/dx2 ="<<d2edx2;
    qDebug()<<"";

}

double Force::calc_diff_d1_d2(int d1,int d2, double x2)
{
    double second_diff, dx, dy;
    dx=1E-10*(D[d1].xd-x2);
    dy=1E-10*(D[d1].y-D[d2].y);
    //    second_diff=1E-10*(A1*D[d1].B1*D[d2].B1*(A4*A2*pow(dy,4)-3*A4*dx*dy*dy+3*A5*A2*dy*dy*dx*dx-A5*pow(dx,4))/(A6*pow(((A2*dy*dy)+(dx*dx)),3))); //df/dx

    //prismatic with prismatic
    if((D[d1].B==3||D[d1].B==4)&&(D[d2].B==3||D[d2].B==4))
    {
        second_diff=-(A1*D[d1].B1*D[d2].B1*((3*A5*dx*dx)+(A4*dy*dy)))/(A6*(pow((dx*dx+A2*dy*dy),2)+(A3*dy*dy*dx*dx)));
        second_diff+=((A1/A6)*(D[d1].B1*D[d2].B1)*((4*dx*(dx*dx+A2*dy*dy))+(2*A3*dy*dy*dx))*((A4*dy*dy*dx)+(A5*dx*dx*dx)))/(pow((pow((dx*dx+A2*dy*dy),2)+(A3*dy*dy*dx*dx)),2));
    }

    //basal with basal
    if((D[d1].B==1||D[d1].B==2)&&(D[d2].B==1||D[d2].B==2))
    {
        second_diff=((A1*A7*D[d1].B1*D[d2].B1)/(pow((pow(dx*dx+A2*dy*dy,2)+A3*dy*dy*dx*dx),2)));
        second_diff=second_diff*(((A2*dy*dy-3*dx*dx)*(pow(dx*dx+A2*dy*dy,2)+A3*dy*dy*dx*dx))-(((4*dx*(dx*dx+A2*dy*dy))+2*A3*dy*dy*dx)*(A2*dy*dy*dx-dx*dx*dx)));
    }

    //basal with prismatic
    if(((D[d1].B==1||D[d1].B==2)&&(D[d2].B==3||D[d2].B==4))||((D[d1].B==3||D[d1].B==4)&&(D[d2].B==1||D[d2].B==2)))
    {
        second_diff=((A1*A7*D[d1].B1*D[d2].B1)/(pow((pow(dx*dx+A2*dy*dy,2)+A3*dy*dy*dx*dx),2)));
        second_diff=second_diff*((-2*dy*dx*(pow(dx*dx+A2*dy*dy,2)+A3*dy*dy*dx*dx))-((4*dx*(dx*dx+A2*dy*dy)+2*A3*dy*dy*dx)*(A2*dy*dy*dy-dy*dx*dx)));
    }

 //   second_diff=2*dx;
 //   qDebug()<<"dx"<<dx<<second_diff;

    return second_diff;
}

double Force::calc_f_d1_d2(int d1,int d2, double x2)
{
    double force, dx, dy;
    dx=1E-10*(D[d1].xd-x2);
    dy=1E-10*(D[d1].y-D[d2].y);

    //prismatic with prismatic
    if((D[d1].B==3||D[d1].B==4)&&(D[d2].B==3||D[d2].B==4))
    {
        force=A1*D[d1].B1*D[d2].B1*(A4*dy*dy*dx+A5*dx*dx*dx)/(A6*pow((A2*dy*dy+dx*dx),2)); //force
    }

    //basal with basal
    if((D[d1].B==1||D[d1].B==2)&&(D[d2].B==1||D[d2].B==2))
    {
        force=(A1*A7*D[d1].B1*D[d2].B1*(-dx*dx*dx+A2*dx*dy*dy))/(pow(dx*dx+A2*dy*dy,2)+A3*dx*dx*dy*dy);
    }

    //basal with prismatic
    if(((D[d1].B==1||D[d1].B==2)&&(D[d2].B==3||D[d2].B==4))||((D[d1].B==3||D[d1].B==4)&&(D[d2].B==1||D[d2].B==2)))
    {
        force=(A1*A7*D[d1].B1*D[d2].B1*(-dx*dx*dy+A2*dy*dy*dy))/(pow(dx*dx+A2*dy*dy,2)+A3*dx*dx*dy*dy);
    }

    if (isnan(force)!=0){qDebug()<<"force is NAN"<<d1<<d2<<D[d1].B1<<D[d1].x<<D[d1].y<<D[d2].B1<<D[d2].x<<D[d2].y<<dx<<dy<<x2;force=0;}

  //  force=(dx)*(dx); //test force function

    return force;
}

double Force::appStress(int d)
{
double fx;
//double fy;
fx=(D[d].bx*sxy)+(D[d].by*syx);
//fy=(D[d].bx*sxx)+(D[d].by*syy);
return fx;
}


void Force::move_pris()
{
    QList<int> disl_group, disl_group_i;
    for (int g=0;g<group_number;g++) //for each group find the average force on the dislocations
    {
        bool loop=false;
        disl_group.clear();
        double tot_force=0.0;
        double tot_diff=0.0;
        int num=0;
        double move=0.0;
        for (int d=0;d<D.size();d++)
        {
            if (D[d].group==g&&(D[d].B==3||D[d].B==4))
            {
                calc_total_diff_x(d);
               // qDebug()<<"adding force from dislocation"<<d<<D[d].f<<D[d].df;
                tot_force+=D[d].f;
                tot_diff+=D[d].df;

                num++;
                //      qDebug()<<num;
                disl_group<<d;
                //qDebug()<<D[d].f<<D[d].df;
            }
        }

        move=((tot_force/fabs(tot_diff))/num);///(tot_diff/num);

     //   qDebug()<<"total force on dislocation dipole"<<g<<tot_force<<tot_diff<<move<<D[0].xd-D[1].xd;
        if (tot_diff<0.008E-14){move=0.00;}
        if(isnan(move)!=0){move=0;}
        if(move>40){move=40;} //put an upper limit on how far loops can move.
        if(move<-40){move=-40;}

        //           qDebug()<<"group ="<<g<<", move ="<<1E-10*move;

        for (int i=0;i<disl_group.size();i++) //move all dislocations in group by same amount.
        {
            int d=disl_group.value(i);

            if(D[d].B==3||D[d].B==4)
            {
                D[d].x+=move;
                D[d].xd=D[d].x;
                if ((D[d].x>=300+(num_cells*material_width))||(D[d].x<300-(num_cells*material_width)))
                {loop=true;}
                else
                {
                    while (D[d].xd>=300+material_width)
                    {
                        D[d].xd-=material_width;
                    }
                    while (D[d].xd<300)
                    {
                        D[d].xd+=material_width;
                    }
                }
            }
        }
        if (loop==true)
        {
            int d=disl_group.value(0);
            int c=(D[d].x-300)/material_width;
            for (int i=0;i<disl_group.size();i++)
            {
                int d=disl_group.value(i);
                D[d].x-=(c*material_width);
                D[d].xd=D[d].x;
                while (D[d].xd>=300+material_width)
                {
                    D[d].xd-=material_width;
                }
                while (D[d].xd<300)
                {
                    D[d].xd+=material_width;
                }
            }
        }
    }
}


void Force::move_pris2()
{

    bool loop;
    QList<int> disl_group, disl_group_i;
    for (int g=0;g<=group_number;g++) //for each group find the average force on the dislocations
    {
        //      qDebug()<<"group"<<g;
        loop=false;
        disl_group.clear();
        double tot_force=0.0;
        double tot_diff=0.0;
        int num=0;
        double move=0.0;
        for (int d=0;d<D.size();d++)
        {
            if (D[d].group==g&&(D[d].B==3||D[d].B==4))
            {
                calc_total_diff_x(d);
                tot_force+=D[d].f;
                tot_diff+=D[d].df;
                //     qDebug()<<tot_force;
                num++;
                //      qDebug()<<num;
                disl_group<<d;
                //         qDebug()<<"adding"<<d;
            }
        }
        move=tot_force;///tot_diff;
        for (int i=0;i<disl_group.size();i++) //move all dislocations in group by same amount.
        {
            int d=disl_group.value(i);
            D[d].f=move;
            //        qDebug()<<"assigning move to f"<<"group"<<g<<"d"<<i<<D[d].f;
        }
    }
    for (int g=0;g<=group_number;g++){
        disl_group.clear();
        for (int d=0;d<D.size();d++){
            //     qDebug()<<"group ="<<g<<", move ="<<1E-10*move;
            if (D[d].group==g&&(D[d].B==3||D[d].B==4))
            {
                disl_group<<d;
            }
        }
        for (int i=0;i<disl_group.size();i++) //move all dislocations in group by same amount.
        {
            int d=disl_group.value(i);

            if(D[d].B==3||D[d].B==4)
            {
                D[d].x+=D[d].f;
                D[d].xd=D[d].x;
                if ((D[d].x>=300+(num_cells*material_width))||(D[d].x<300-(num_cells*material_width)))
                {loop=true;}
                else
                {
                    while (D[d].xd>=300+material_width)
                    {
                        D[d].xd-=material_width;
                    }
                    while (D[d].xd<300)
                    {
                        D[d].xd+=material_width;
                    }
                }
            }
        }
        if (loop==true)
        {
            int d=disl_group.value(0);
            int c=(D[d].x-300)/material_width;
            for (int i=0;i<disl_group.size();i++)
            {
                int d=disl_group.value(i);
                D[d].x-=(c*material_width);
                D[d].xd=D[d].x;
                while (D[d].xd>=300+material_width)
                {
                    D[d].xd-=material_width;
                }
                while (D[d].xd<300)
                {
                    D[d].xd+=material_width;
                }
            }
        }
    }
}

void Force::inplane_forces()
{
    //calculate the force on each dislocation as a result of each of the other dislocations in the system.
    //Force has an x and y component which is then resolved along the glide plane to calculate the net force on the dislocation.
    //the net force can then be used to move the dislocation by an appropritate amount.
    for (int d1=0;d1<D.size();d1++)
    {
        //Calculate total force on each dislocation
        D[d1].fx=D[d1].fy=0;
        for (int d2=0;d2<D.size();d2++)
        {
            double dx, dy;
            if (d1!=d2)
            {
                dx=D[d1].x-D[d2].x;
                dy=D[d1].y-D[d2].y;
                D[d1].fx+=D[d1].B1*1E10*(mu*D[d2].B1*1E10/(2*M_PI*(1-nu)))*((3*dx*dx+dy*dy)/((dx*dx+dy*dy)*((dx*dx+dy*dy))));
                D[d1].fy+=D[d1].B1*1E10*(mu*D[d2].B1*1E10/(2*M_PI*(1-nu)))*((dx*dx-dy*dy)/((dx*dx+dy*dy)*((dx*dx+dy*dy))));
            }
            for (int r=-num_cells;r<num_cells;r++)
            {
                for (int s=-num_cells;s<num_cells;s++)
                if(r==0&&s==0){}
                else
                {
                    dx=D[d1].x-(D[d2].x+r*material_width);
                    dy=D[d1].y-(D[d2].y+s*material_height);
                    D[d1].fx+=D[d1].B1*1E10*(mu*D[d2].B1*1E10/(2*M_PI*(1-nu)))*((3*dx*dx+dy*dy)/((dx*dx+dy*dy)*((dx*dx+dy*dy))));
                    D[d1].fy+=D[d1].B1*1E10*(mu*D[d2].B1*1E10/(2*M_PI*(1-nu)))*((dx*dx-dy*dy)/((dx*dx+dy*dy)*((dx*dx+dy*dy))));

                }
            }

        }
        //resolve the force along the glide plane

        if (D[d1].B==11||D[d1].B==12)
        {
            D[d1].f=D[d1].fx;
        }
        if (D[d1].B==13||D[d1].B==14)
        {
            D[d1].f=(0.5*D[d1].fx)-((1/0.866025404)*D[d1].fy);
        }
        if (D[d1].B==15||D[d1].B==16)
        {
            D[d1].f=(0.5*D[d1].fy)+((1/0.866025404)*D[d1].fx);
        }

    }

    //move the dislocation by an appropriate amount w.r.t force on it.

    for (int d=0;d<D.size();d++)
    {
        D[d].f=D[d].f*1000;

        //set limit of movement to stop dislocations moving too far too fast.
        if(D[d].f>4){D[d].f=4;}
        if(D[d].f<-4){D[d].f=-4;}

        double dx, dy;
        if (D[d].B==11||D[d].B==12)
        {
            dx=D[d].f;
            dy=0;
        }
        if (D[d].B==13||D[d].B==14)
        {
            dx=-0.5*D[d].f;
            dy=0.866025404*D[d].f;
        }
        if (D[d].B==15||D[d].B==15)
        {
            dx=0.866025404*D[d].f;
            dy=0.5*D[d].f;
        }

      //  qDebug()<<dx<<dy;
        D[d].x+=dx;
        D[d].xd+=dx;
        D[d].y+=dy;

        if (D[d].x>material_width+300){D[d].x-=material_width;D[d].xd-=material_width;}
        if (D[d].x<0+300){D[d].x+=material_width;D[d].xd+=material_width;}
        if(D[d].y+120>material_height){D[d].y-=material_height;}
        if(D[d].y<0+120){D[d].y+=material_height;}
    }

}
