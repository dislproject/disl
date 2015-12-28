

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/nondet_random.hpp>
#include "externVar.h"


QList<QPoint> disl_positions;
using namespace boost;
mt19937 gen;
int plane;

Structure::Structure()
{
    gen.seed(static_cast<unsigned int>(time(0)));

}


int Structure::random_plane()
{
    int max_y=y_max/int_layer_sep;
    boost::uniform_int<> dist(1, max_y);
    int y =dist(gen);
    //   qDebug()<<y;
    return y;

}

double Structure::random_x()
{
    boost::uniform_int<> dist(1, x_max);
    int x =dist(gen);
    //   qDebug()<<x;
    return x;
}

double Structure::random_y()
{
    boost::uniform_int<> dist(1, y_max);
    int y =dist(gen);
    //   qDebug()<<x;
    return y;
}

double Structure::random_inplane_h()
{
    //pick a random glide plane
    boost::uniform_int<> dist(1, inplane_h_planes);
    int y =dist(gen);
    //   qDebug()<<x;
    return y;
}
double Structure::random_inplane_a_x()
{
    //pick a random glide plane
    boost::uniform_int<> dist(1, inplane_a_planes);
    int y =dist(gen);
    //   qDebug()<<x;
    return y;
}
double Structure::random_inplane_a_y()
{
    //length of the glide plane is equal to height/cos(30)
    int length_g_plane=material_height/0.8660254038;
    //pick a random point along the glide plane.
    boost::uniform_int<> dist(1, length_g_plane);
    int y =dist(gen);
    //   qDebug()<<x;
    return y;
}

void Structure::Random_position()
{
    bool position_ok=false;
    sep = 80;// + rand()% int(20);
    // int plane = rand() % int( (y_max)/int_layer_sep );

    plane = random_plane();

    pos_y = 20+(double(plane*int_layer_sep));


    pos_x=random_x();
    // pos_x = double( rand() % int(x_max));

    /* position_ok=interaction.check_position(pos_x+300,pos_y+120);
    if (position_ok==false)
    {Random_position();}

    position_ok=interaction.check_position(pos_x+300+sep,pos_y+120+sep);
    if (position_ok==false)
    {Random_position();}*/
}

void Structure::Random_position_inplane(int line_type)
{
    if (line_type==1)
    {

        pos_y = ZZ_B*1E10*random_inplane_h();
        pos_x = random_x();
    }
    if (line_type==2)
    {
        int l=random_inplane_a_y();
        pos_x = (ZZ_B*1E10*random_inplane_a_x())+(l*0.5);
        if (pos_x>material_width){pos_x-=material_width;}
        pos_y = l*0.8660254038;
    }
    if (line_type==3)
    {
        int l=random_inplane_a_y();
        pos_x =(ZZ_B*1E10*random_inplane_a_x())-(l*0.5);
        if (pos_x<0){pos_x+=material_width;}
        pos_y = l*0.8660254038;
    }

   // do some sort of overlap checking here?
}


void Structure::Build_Di()
{
}

void Structure::Set_positions(int i)
{
    sep=80;
    if (i==0){pos_x=  496  ;pos_y= 295.56 ;}
     if (i==1){pos_x=  173  ;pos_y= 361.96 ;}
     if (i==2){pos_x=  337  ;pos_y= 219.2  ;}
     if (i==3){pos_x=  284  ;pos_y= 192.64 ;}
     if (i==4){pos_x=  175  ;pos_y= 454.92 ;}
     if (i==5){pos_x=  96   ;pos_y= 773.64 ;}
     if (i==6){pos_x=  185  ;pos_y= 577.76 ;}
     if (i==7){pos_x=  447  ;pos_y= 620.92 ;}
     if (i==8){pos_x=  249  ;pos_y= 703.92 ;}
     if (i==9){pos_x=  402  ;pos_y= 375.24 ;}
     if (i==10){pos_x= 182  ;pos_y= 733.8  ;}
     if (i==11){pos_x= 346  ;pos_y= 654.12 ;}
     if (i==12){pos_x= 469  ;pos_y= 371.92 ;}
     if (i==13){pos_x= 38   ;pos_y= 76.44  ;}
     if (i==14){pos_x= 484  ;pos_y= 103    ;}
     if (i==15){pos_x= 165  ;pos_y= 464.88 ;}
     if (i==16){pos_x= 593  ;pos_y= 116.28 ;}
     if (i==17){pos_x= 528  ;pos_y= 39.92  ;}
     if (i==18){pos_x= 51   ;pos_y= 604.32 ;}
     if (i==19){pos_x= 129  ;pos_y= 740.44 ;}
     if (i==20){pos_x= 441  ;pos_y= 713.88 ;}
     if (i==21){pos_x= 65   ;pos_y= 245.76 ;}
     if (i==22){pos_x= 516  ;pos_y= 166.08 ;}
     if (i==23){pos_x= 58   ;pos_y= 464.88 ;}
     if (i==24){pos_x= 541  ;pos_y= 26.64  ;}
     if (i==25){pos_x= 102  ;pos_y= 63.16  ;}
     if (i==26){pos_x= 432  ;pos_y= 365.28 ;}
     if (i==27){pos_x= 553  ;pos_y= 740.44 ;}
     if (i==28){pos_x= 589  ;pos_y= 89.72  ;}
     if (i==29){pos_x= 527  ;pos_y= 737.12 ;}
     if (i==30){pos_x= 547  ;pos_y= 255.72 ;}
     if (i==31){pos_x= 497  ;pos_y= 737.12 ;}
     if (i==32){pos_x= 367  ;pos_y= 541.24 ;}
     if (i==33){pos_x= 546  ;pos_y= 737.12 ;}
     if (i==34){pos_x= 333  ;pos_y= 375.24 ;}
     if (i==35){pos_x= 345  ;pos_y= 537.92 ;}
     if (i==36){pos_x= 356  ;pos_y= 262.36 ;}
     if (i==37){pos_x= 544  ;pos_y= 461.56 ;}
     if (i==38){pos_x= 205  ;pos_y= 757.04 ;}
     if (i==39){pos_x= 218  ;pos_y= 302.2  ;}
}

void Structure::Add_new_pair()
{
   Random_position();
//        for(i=0;i<40;i++){
//        Set_positions(i);
    //   qDebug()<<pos_y<<pos_x;

    Disl dis1, dis2;
    //x positions
    dis1.x=pos_x+300;
    dis2.x=pos_x+300+sep;

    dis1.xd=dis1.x;
    dis2.xd=dis2.x;





    //y positions
    dis1.y=pos_y+120;
    dis2.y=pos_y+120;

//    qDebug()<<"disl positions:"<<dis1.x<<dis1.xd<<dis2.x<<dis2.xd<<dis1.y;

    //Burgers vectors & magnitudes
    if (basals==0){
        dis1.B=3;
        dis2.B=4;
        dis1.B1=basal_B;
        dis2.B1=-basal_B;
        dis1.by=basal_B;
        dis2.by=-basal_B;
        dis1.bx=0;
        dis2.bx=0;
        dis1.group=group_number;
        dis2.group=group_number;
        dis1.min=true;
        group_number++;
    }
    if(basals==1)
    {
        int random=rand()%2;
        if(random==0){
            dis1.B=2;
            dis2.B=1;
            dis1.B1=atom_sep*1E-10;
            dis2.B1=-atom_sep*1E-10;
            dis1.bx=atom_sep*1E-10;
            dis2.bx=-atom_sep*1E-10;
            dis1.y+=int_layer_sep;
            dis1.x+=sep;

            dis1.xd=dis1.x;
        }
        else if(random==1)
        {
            dis1.B=1;
            dis2.B=2;
            dis1.B1=-atom_sep*1E-10;
            dis2.B1=atom_sep*1E-10;
            dis1.bx=-atom_sep*1E-10;
            dis2.bx=atom_sep*1E-10;
            dis1.y+=int_layer_sep;
            dis1.x+=sep;
            dis1.xd=dis1.x;
        }
        dis1.by=0;
        dis2.by=0;
    }

    if(dis1.x>=300+material_width)
    {
        dis1.xd=dis1.x-material_width;
    }
    if(dis2.x>=300+material_width)
    {
        dis2.xd=dis2.x-material_width;
    }


    dis1.number=dislocation_number;
    dis2.number=dislocation_number+1;
    dislocation_number=dislocation_number+2;

    // int rucks=rand()%100;
    // if (rucks>95){Ruck_and_tuck(4,pos_x+300, pos_y+120-(0.5*int_layer_sep));}
    // else
    // {

//    if (group_number==1)
 //   {D<<dis1;}
 //   else{D<<dis2;}

   D<<dis1<<dis2;
//        }

  // qDebug()<<"dis1.x="<<dis1.x<<"; dis1.y="<<dis1.y<<"; dis1.xd=dis1.x; if(dis1.x>=300+material_width) {dis1.xd=dis1.x-material_width;} dis1.B="<<dis1.B<<"; dis1.bx="<<dis1.bx<<"; dis1.by="<<dis1.by<<"; dis1.group="<<dis1.group<<"; dis1.number="<<dis1.number<<"; D<<dis1;";
  // qDebug()<<"dis2.x="<<dis2.x<<"; dis2.y="<<dis2.y<<"; dis2.xd=dis2.x; if(dis2.x>=300+material_width) {dis2.xd=dis2.x-material_width;} dis2.B="<<dis2.B<<"; dis2.bx="<<dis2.bx<<"; dis2.by="<<dis2.by<<"; dis2.group="<<dis2.group<<"; dis2.number="<<dis2.number<<";D<<dis2;";
  // qDebug()<<D.size()-2<<dis1.x<<dis1.y;


}


void Structure::prismatic_contraction(int d)
{
    double top=0;
    double bottom=0;
    double height=0;
    for (int i=0;i<120;i++)
    {
        top+=E_new[i+120].y;
        bottom+=E_new[i+360].y;
    }
    height=fabs((top/120)-(bottom/120));

    double contraction=0.05*((basal_B*10E10*sep)/height);
    for (int i=0; i<120;i++)
    {
        E_new[i].count+=contraction/atom_sep;
        E_new[i].x+=contraction;
        E_new[i+240].count-=contraction/atom_sep;
        E_new[i+240].x-=contraction;

    }

    /*double contraction=double(sep*int_layer_sep*120)/double(2.*y_max); //how much to move each edge in by
    int k = ((D[d].y-120)/(y_max+40))*120; //which edge piece corrisponds to dislocation d
    E_new[k].x+=contraction;
    E_new[k].count+=double(contraction/atom_sep);
    E_new[k+240].x-=contraction;
    E_new[k+240].count-=double(contraction/atom_sep);*/


}


//removes half a atom separation from each side of the material.
//called when basals are introduced to conserve material volume.
void Structure::remove_edge_material()
{
    for(int j=0; j<120; j++)
    {
        E_new[j].count+=0.25;
        E_new[j+240].count-=0.25;


    }
}

//adds new material to the edge. Called when basals are annihillated
//to conserve total volume of the material.
void Structure::add_edge_material()
{
    for(int j=0; j<120; j++)
    {
        E_new[j].count-=0.25;
    }
    for(int j=240; j<360; j++)
    {
        E_new[j].count+=0.25;
    }
}

// build dislocation structure
void Structure::Build_Di(double Dip_posn_x[], double Dip_posn_y[])
{
    //for aniso - Dislocations

    for(j = 0; j<disl_lays; j++) {
        for(i = 0; i<disl_columns; i++) {
            QPoint position(Dip_posn_x[i], Dip_posn_y[j]);
            disl_positions<<position;
        }
    }
}


void Structure::Add_new_grid_pair()
{

    int num_left=disl_positions.size();

    int num=rand()%num_left;
    QPoint pos = disl_positions.value(num);

    disl_positions.removeAt(num);

    D.clear();

    int sep = 5*its;

    Disl dis1,dis2,dis3,dis4, dis5, dis6;

    //x positions
    dis1.x=pos.x()+230;
    dis2.x=pos.x()+230+sep;
    //y positions
    dis1.y=pos.y()+140;
    dis2.y=pos.y()+140;

    //Burgers vectors & magnitudes
    if (basals==0){
        dis1.B=3;
        dis2.B=4;
        dis1.B1=basal_B;
        dis2.B1=-basal_B;
        dis1.by=basal_B;
        dis2.by=-basal_B;
    }
    if(basals==1)
    {
        dis1.B=2;
        dis2.B=1;
        dis1.B1=-atom_sep;
        dis2.B1=atom_sep;
        dis1.bx=-atom_sep;
        dis2.bx=atom_sep;

    }
    dis3=dis5=dis1;
    dis4=dis6=dis2;

    dis1.x=800;
    dis1.y=200;
    dis2.x=320;
    dis2.y=200;
    dis3.x=dis1.x-600;
    dis3.y=dis4.y=dis5.y=dis6.y=200;
    dis4.x=dis2.x+600;
    dis5.x=dis1.x+600;
    dis6.x=dis2.x-600;


    D<<dis1;
    D<<dis2;
    //D<<dis3<<dis4<<dis5<<dis6;


}

//adds a prismatic loop in place of a pile up of basals. The separation of
//the prismatic loop is dictated by size (number of basals which pile up to
//create the ruck and tuck)
void Structure::Ruck_and_tuck(int size, double xpos, double ypos)
{
//     qDebug()<<"ruck and tuck"<<size;
    Disl dis1, dis2, dis3, dis4;
    double sep_size=(size*20);
    dis1.x=xpos-(sep_size/2);
    dis1.y=ypos;
    dis1.xd=dis1.x;
    dis2.x=xpos+(sep_size/2);
    dis2.y=ypos;
    dis2.xd=dis2.x;

    if(dis1.x>=300+material_width)
    {
        dis1.xd=dis1.x-material_width;
    }
    if(dis2.x>=300+material_width)
    {
        dis2.xd=dis2.x-material_width;
    }

    dis1.B=3;
    dis2.B=4;


    dis1.B1=basal_B;
    dis2.B1=-basal_B;


    dis1.by=basal_B;
    dis2.by=-basal_B;


    dis1.bx=0;
    dis2.bx=0;

    group_number++;
    dis1.group=group_number;
    dis2.group=group_number;

    D<<dis1;
    D<<dis2;

    // qDebug()<<dis1.x<<dis2.x<<dis1.y;

}


void Structure::Add_test_basal()
{
Disl dis;
dis.B=1;
dis.B1=basal_B;
dis.bx=basal_B;
dis.by=0;
dis.y=250;
dis.x=300;
dis.xd=300;
D<<dis;
}



void Structure::Add_nonBasals()
{
 /*
 This function adds a dislocation dipole to the dislocation list, D, for inplane dislocation dynamics
 These are non basal edge dislocations. i.e. vacancy lines in the sheet of graphene.
 Kinetic Monte Carlos simulation has suggested that zigzag lines will form 90% of the time
 with armchair lines forming the remaining 10% of the time. (Trevethan et al 2014)
 This is accounted for by generating a random number between 0 and 999, numbers less than 900 cause
 addition of a zigzag line and numbers 90 or over cause addition of a zigzag line.
 Both the zigzag and armchair lines have three orientations:
 armchair 30, 90, 150 and zigzag 0, 60, 120 degrees from vertical.
 These orientations are assumed to have equal probability.
  */

    // for in plane Disl.B is:
    //      5=AC30_1, 6=AC30_2, 7=AC90_1,  8=AC90_2,  9=AC150_1, 10=AC150_2,
    //     11=ZZ0_1, 12=ZZ0_2, 13=ZZ60_1, 14=ZZ60_2, 15=ZZ120_1, 16=ZZ120_2.

            Disl dis1, dis2;

            // use random numbers to choose if AC or ZZ line and which orientation.
            // set dislocation type and burgers vectors
            // Burgers vector is perpendicular to the dipole line and resolved as: by=B1*sin(angle), bx=B1*cos(angle)
            boost::uniform_int<> dist(0, 999);
            boost::uniform_int<> dist2(0, 2);
            int random_num =dist(gen);
            int random_num2=dist2(gen);


            //number of vacancies in the line, could be randomly generated, for now set to 10. More vacnacies= bigger separation
            int num_vac=10;

            random_num=895; //temp. forcing of the lines to be zigzag.
            //random_num2=0;

            if (random_num<900)
            {
                //dislocation line is zigzag
                sep=ZZ_B*1E10*(0.5+(num_vac/2));
                dis1.B1=ZZ_B;
                dis2.B1=-ZZ_B;

                if (random_num2==0)  //0 degrees from vertical, sin(0)=1 cos(0)=1
                {
                    Random_position_inplane(1);
                    dis1.x=dis1.xd=pos_x+300;
                    dis1.y=dis2.xd=pos_y+120;

                    dis1.B=11;
                    dis1.by=0;
                    dis1.bx=dis1.B1;
                    dis2.B=12;
                    dis2.by=0;
                    dis2.bx=dis2.B1;

                    dis2.x=dis2.xd=dis1.x;
                    dis2.y=dis1.y-sep;
                }

                if (random_num2==1)//60 degrees from vertical, sin(60)=0.8660254038 cos(60)=0.5
                {
                    Random_position_inplane(2);
                    dis1.x=dis1.xd=pos_x+300;
                    dis1.y=dis2.xd=pos_y+120;


                    dis1.B=13;
                    dis1.by=-dis1.B1*0.8660254038;
                    dis1.bx=dis1.B1*0.5;
                    dis2.B=14;
                    dis2.by=-dis1.bx;
                    dis2.bx=-dis2.by;

                    dis2.x=dis2.xd=dis1.x-sep*0.8660254038;
                    dis2.y=dis1.y-sep*0.5;
                }

                if (random_num2==2)  //120 degrees from vertical, sin(120)=0.8660254038 cos(120)=-0.5
                {

                    Random_position_inplane(3);
                    dis1.x=dis1.xd=pos_x+300;
                    dis1.y=dis2.xd=pos_y+120;

                    dis1.B=15;
                    dis1.by=dis1.B1*0.8660254038;
                    dis1.bx=dis1.B1*0.5;
                    dis2.B=16;
                    dis2.by=-dis1.bx;
                    dis2.bx=-dis2.by;

                    dis2.x=dis2.xd=dis1.x+sep*0.8660254038;
                    dis2.y=dis1.y-sep*0.5;
                }
            }
            else
            {
            //dislocation line is armchair
                sep=AC_B*1E10*(2+((3*num_vac)/2));
                dis1.B1=AC_B;
                dis2.B1=-AC_B;
                if (random_num2==0)  //30 degrees from vertical, sin(30)=0.5 cos(30)=0.8660254038
                {
                    dis1.B=5;
                    dis1.by=dis1.B1*0.5;
                    dis1.bx=-dis1.B1*0.8660254038;
                    dis2.B=6;
                    dis2.by=-dis1.bx;
                    dis2.bx=-dis2.by;

                    dis2.x=dis2.xd=dis1.x+sep*0.5;
                    dis2.y=dis1.y+sep*0.8660254038;
                }

                if (random_num2==1) //90 degrees from vertical, sin(90)=1 cos(90)=0
                {
                    dis1.B=7;
                    dis1.by=dis1.B1;
                    dis1.bx=0;
                    dis2.B=8;
                    dis2.by=-dis1.bx;
                    dis2.bx=-dis2.by;

                    dis2.x=dis2.xd=dis1.x+sep;
                    dis2.y=dis1.y;

                }

                if (random_num2==2) //150 degrees from vertical, sin(150)=0.5 cos(150)=-0.8660254038
                {
                    dis1.B=9;
                    dis1.by=dis1.B1*0.5;
                    dis1.bx=dis1.B1*0.8660254038;
                    dis2.B=10;
                    dis2.by=-dis1.bx;
                    dis2.bx=-dis2.by;

                    dis2.x=dis2.xd=dis1.x+sep*0.5;
                    dis2.y=dis1.y-sep*0.8660254038;
                }
            }

D<<dis1<<dis2;

}




/*
/////////////INITIAL EDGE POSITIONS - CIRCLE//////

   E_new[i].type = E_new[i+157].type = E_new[i+2*157].type \
     = E_new[i+3*157].type = 6; // for animation image
 changed 6 from horizontal straight line to dot for circle
 }*/

// build edge structure
void Structure::Build_Square_Ed(Edge E[])
{

/*
    for( int i = 0; i <  157; i++ ) {
      E[i].x = E[i+157].x = 600+300*cos(double(i)/100.);
      E[i+2*157].x=E[i+3*157].x=600-300*cos(double(i)/100.);
      E[i].y = E[i+2*157].y = 400+300*sin(double(i)/100);
      E[i+157].y=E[i+3*157].y=400-300*sin(double(i)/100);
  }

*/
    /////////////INITIAL EDGE POSITIONS - user input////////
    for( int i = 0; i <  120; i++ ) {
        E[i].x = 300.;
        E[i+120].x = 300.+material_width/120.*i;
        E[i+240].x = 300.+material_width;
        E[i+360].x = 300.+material_width/120.*i;
        E[i].y = 120.+material_height/120.*i;
        E[i+120].y = 120.+material_height;
        E[i+240].y = 120.+material_height/120.*i;
        E[i+360].y = 120.;
        E[i].type=6;
        E[i+240].type=6;
    }


}

void Structure::calculate_edge_strains()
{
    for ( int j = 0; j < edge_segments; j++ ) {
        E[j].ux = E[j].uy = 0.; // initialise before new calc

        for ( int i = 0; i<D.size(); i++ ) {

            if (T==1)
            {
                double xs;
                for (int k=-num_cells;k<=num_cells;k++)
                {
                    xs=D[i].x-300;
                    xs+=(k*material_width);
                  // if (j==120){ qDebug()<<"adding strain due to"<<i<<"in cell"<<k<<xs+300;}
                    E[j].Strain_aniso(D[i].bx, D[i].by, xs+300, D[i].y, D[i].B);

                }
            }
            //   else if (T==2)
            //   {E[j].Edge_strain(D[i].B1,D[i].x+(k*material_width),D[i].y);
            //   }

        }
        //new edge position = original position + change due to strain + additional material deposited by basals in previous iterations.
        E_new[j].x = (E[j].x + E[j].ux + double(E_new[j].count*atom_sep));
        E_new[j].y = E[j].y + E[j].uy + double(E_new[j].counth*(basal_B*1E10/2)); // displacement
        // qDebug()<<E_new[j].counth;
        E_new[j].type=E[j].type;

    }
}


void Structure::Inplane_rounding()
{
   //function to ensure that there are an integer number of glide planes in the horizontal direction and on the 30 degree angle.
material_height=int(material_height/(ZZ_B*1E10))*ZZ_B*1E10;
inplane_h_planes=material_height/(ZZ_B*1E10);
//number of glide planes = width/(ZZ_B/cos 30)
material_width=int(material_width/4.919024293)*4.919024293;
inplane_a_planes=material_width/4.919024293;

//qDebug()<<material_height<<inplane_h_planes<<material_width<<inplane_a_planes;

}

void Structure::Add_material_height()
{
  //  qDebug()<<"add material height function disabled";
    for (int i=0;i<120;i++)
    {
    //    E_new[i+120].counth++;
       // E_new[i+360].counth--;
    }
}

void Structure::Add_material_height(double m)
{
 //   qDebug()<<"add material height function disabled"<<m<<m*basal_B;
    for (int i=0;i<120;i++)
    {
     //   E_new[i+120].counth+=m;
     //   E_new[i+360].counth-=m;
    }
}

