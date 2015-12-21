#include <QtGui>
#include <QtWidgets>//VERYTEMPORARY
#include <QMovie>
#include <QDebug>
#include "ImageMagick/Magick++.h"
#include <math.h>
#include "renderarea.h"
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include "Write_to_file.hpp"
#include "externVar.h"
#include "interaction.h"
#include <list>
#include <time.h>
#include <sys/time.h>



using namespace std;
using namespace Magick;

//declare external variables (see also externVar.h)
class RenderArea;
int T;
double material_height;
double material_width;
int x_max;
int y_max;
int num_its;
int S;
int e_calc;
int a_calc;
int inplane;
int basals;
int disl_num;
int initial_disl_num;
double applied_stress;
int inc;
int force_iteration;
int timesteps;
double max_cexp;
double max_aexp;
double max_energy;
double max_area_change;
double basal_B;
int its;
int disl_lays;
int disl_columns;
int edge_segments;
int num_cells;
Edge E[480];
Edge E_new[480];
QList<Disl> D;
QList<Disl> ImgD;
QMovie *animation;
QMovie *graphanimation;
Energy En;
Structure structure;
Test test;
double Key, Kex, A1, A2, A3, A4, A5, A6, A7;
Interaction interaction; //interaction between dislocations
int group_number;
int dislocation_number;
bool stop;
double sxx;
double sxy;
double sxz;
double syx;
double syy;
double syz;
double szx;
double szy;
double szz;
Inplane P;
int inplane_a_planes;
int inplane_h_planes;
Strain_map MP;
Grid grid[500][500];

//setup signal and slots for GUI.
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)//,ui(new Ui::MainWindow)
{
    ui.setupUi(this);
    connect(ui.reset_button, SIGNAL(clicked()), this, SLOT(resetValues()));
    connect(ui.run_button, SIGNAL(clicked()), this, SLOT(run_main_program()));
    connect(ui.replay_button, SIGNAL(clicked()), this, SLOT(animateImages()));
    connect(ui.actionQuit, SIGNAL(triggered()),this, SLOT(quit()));
    connect(ui.actionReadme, SIGNAL(triggered()),this, SLOT(showHelp()));
    connect(ui.actionAbout, SIGNAL(triggered()),this, SLOT(showAbout()));
    connect(ui.stop_button, SIGNAL(clicked()),this, SLOT(stopRun()));
    connect(ui.continue_button, SIGNAL(clicked()),this, SLOT(continueRun()));
}

void MainWindow::stopRun()
{
stop=true;
}

void MainWindow::quit()
{exit(0);}

void MainWindow::showHelp()
{system("gedit README.txt");}

void MainWindow::showAbout()
{
    QString title="About the Dislocation Program.";
    QString text = "\n\n\n The dislocation program models the elastic strains of \n dislocations in graphite. \n\n \n A graphical representation of the bulk material is shown \n along with numerical values for the expansions \n and energies of the system due to the dislocations \n\n\n For more information refer to 'Help' \n \n \nVersion Date: 3rd October 2012 \n \n Authors \n G. Sheehan and P. Young";
    QMessageBox::about(this, title, text);
}



//function which is called when replay button is clicked
void MainWindow::animateImages()
//disable replay button and hide cexp, aexp and energy display labels
{   ui.replay_button->setDisabled(true);
//    ui.label_3->setHidden(true);
//    ui.label_4->setHidden(true);
//    ui.label_10->setHidden(true);
//    ui.label_11->setHidden(true);
//    ui.label_12->setHidden(true);
//    ui.label_13->setHidden(true);

    char filename[64];
    char filename2[64];
    list<Magick::Image> imageList;
    list<Magick::Image> graphList;
    for (int t=100; t<100+num_its; t++){
        for(int f=0; f<=timesteps; f++){
            if (timesteps*num_its<=1000)
            {
                snprintf(filename, sizeof(char) * 64, "Animate/images/bmp%d_%d.bmp", t, f);
            }
            else if (timesteps*num_its>1000)
            {
                snprintf(filename, sizeof(char) * 64, "Animate/images/bmp%d.bmp", t);
            }

            readImages(&imageList, filename);
            snprintf(filename2, sizeof(char) * 64, "Animate/images/graph%d.bmp",t);
            //read image files into a list of images

            readImages(&graphList, filename2);
            // remove(filename);
            // remove(filename2);
            //QCoreApplication::processEvents(); //stop GUI being reported as unresponsive
        }}
    //write images from list to animated gif
    writeImages(imageList.begin(), imageList.end(), "Animate/animations/animation.gif", true );
    writeImages(graphList.begin(), graphList.end(), "Animate/animations/graph.gif", true );



    //create Qmovie object to hold the animation, hide renderarea and show movie
    animation = new QMovie("Animate/animations/animation.gif");
    graphanimation = new QMovie("Animate/animations/graph.gif");
    ui.graphicalRenderArea->close();
    ui.renderArea->close();
//    ui.label_9->setMovie(animation);
//    ui.label_14->setMovie(graphanimation);
    //adjust speed of movie (percent of original speed)
    animation->setSpeed(30);
    animation->start();
    graphanimation->setSpeed(30);
    graphanimation->start();

}

void MainWindow::continueRun()
{
    ui.disl_spinbox->setValue(0);
    ui.its_spinbox->setValue(0);
    ui.inc_spinbox->setValue(1);
    ui.burgers_spinbox->setValue(6.64);
    ui.comboBox_4->setCurrentIndex(0);

    ui.run_button->setDisabled(false);
    ui.replay_button->setDisabled(true);
    ui.stop_button->setDisabled(true);
    ui.continue_button->setDisabled(true);
    ui.reset_button->setDisabled(true);
    ui.disl_spinbox->setDisabled(false);
    ui.its_spinbox->setDisabled(false);
    ui.timesteps->setDisabled(false);
    ui.inc_spinbox->setDisabled(false);
    ui.burgers_spinbox->setDisabled(false);
    ui.energy_checkBox->setDisabled(false);
    ui.randomise_checkBox->setDisabled(false);
    ui.basals_checkBox->setDisabled(false);

    ui.energy_checkBox->setChecked(false);
    ui.inplane_checkBox->setChecked(false);
    e_calc=0;
    basals=0;
    a_calc=0;
    inplane=0;

    ui.comboBox_4->setDisabled(false);

    ui.randomise_checkBox->setChecked(true);
    ui.basals_checkBox->setChecked(true);
    ui.width_spinbox->setDisabled(false);
    ui.width_spinbox->setValue(material_width);
    ui.height_spinbox->setDisabled(false);
    ui.height_spinbox->setValue(material_height);
    ui.cells_spinbox->setDisabled(false);
    ui.cells_spinbox->setValue(10);
    ui.timesteps->setValue(0);
    ui.inc_spinbox->setValue(0);

    ui.test_combobox->setDisabled(false);
    ui.mapping_combobox->setDisabled(false);
    ui.s_xx->setDisabled(false);
    ui.s_xy->setDisabled(false);
//    ui.s_xz->setDisabled(false);
    ui.s_yx->setDisabled(false);
    ui.s_yy->setDisabled(false);
//    ui.s_yz->setDisabled(false);
//    ui.s_zx->setDisabled(false);
//    ui.s_zy->setDisabled(false);
//    ui.s_zz->setDisabled(false);
    ui.inplane_checkBox->setDisabled(false);


}

void MainWindow::resetValues()
{
    //set values for each parameter back to default
    ui.disl_spinbox->setValue(0);
    ui.its_spinbox->setValue(0);
    ui.inc_spinbox->setValue(1);
    ui.burgers_spinbox->setValue(6.64);
    ui.comboBox_4->setCurrentIndex(0);

    ui.run_button->setDisabled(false);
    ui.replay_button->setDisabled(true);
    ui.stop_button->setDisabled(true);
    ui.continue_button->setDisabled(true);
    //close render area and setup a new one.
    ui.renderArea->close();
    ui.renderArea = new RenderArea;
    ui.graphicalRenderArea->close();
    ui.graphicalRenderArea = new RenderArea;

    ui.horizontalLayout_4->addWidget(ui.renderArea);
    ui.horizontalLayout_4->addWidget(ui.graphicalRenderArea);



    ui.disl_spinbox->setDisabled(false);
    ui.its_spinbox->setDisabled(false);
    ui.timesteps->setDisabled(false);
    ui.inc_spinbox->setDisabled(false);
    ui.burgers_spinbox->setDisabled(false);
    ui.energy_checkBox->setDisabled(false);
    ui.randomise_checkBox->setDisabled(false);
    ui.basals_checkBox->setDisabled(false);

    ui.energy_checkBox->setChecked(false);
    ui.inplane_checkBox->setChecked(false);
    e_calc=0;
    basals=0;
    a_calc=0;
    inplane=0;
    // ui.comboBox->setDisabled(false);
    ui.comboBox_4->setDisabled(false);
    listOfAPoints.clear();
    listOfAYPos.clear();
    listOfCYPos.clear();
    listOfCXPos.clear();
    listOfEYPos.clear();
    listOfEXPos.clear();
    listOfAreaYPos.clear();
    listOfAreaXPos.clear();
    ui.randomise_checkBox->setChecked(true);
    ui.basals_checkBox->setChecked(true);
    ui.width_spinbox->setDisabled(false);
    ui.width_spinbox->setValue(1200);
    ui.height_spinbox->setDisabled(false);
    ui.height_spinbox->setValue(1680);
    ui.cells_spinbox->setDisabled(false);
    ui.cells_spinbox->setValue(10);
    ui.timesteps->setValue(0);
    ui.inc_spinbox->setValue(0);

    ui.test_combobox->setDisabled(false);
    ui.mapping_combobox->setDisabled(false);
    ui.s_xx->setDisabled(false);
    ui.s_xy->setDisabled(false);
  //  ui.s_xz->setDisabled(false);
    ui.s_yx->setDisabled(false);
    ui.s_yy->setDisabled(false);
 //   ui.s_yz->setDisabled(false);
 //   ui.s_zx->setDisabled(false);
 //   ui.s_zy->setDisabled(false);
 //   ui.s_zz->setDisabled(false);
    ui.inplane_checkBox->setDisabled(false);

    D.clear();
    max_cexp=0;
    max_area_change=0;



    //remove saved image files
//    remove_Image_Files();

}

//main program which is run when the Run button is clicked
int MainWindow::run_main_program()
{
    stop=false;
    int test_run=ui.test_combobox->currentIndex()+1;
    //disable push buttons while program is running.
    ui.run_button->setDisabled(true);
    ui.reset_button->setDisabled(true);
    ui.replay_button->setDisabled(true);
    ui.stop_button->setDisabled(false);
    ui.continue_button->setDisabled(false);
    //read in values from input parameters and assign to external variables.
    disl_num=ui.disl_spinbox->value();
    initial_disl_num=ui.disl_spinbox->value();
    num_its=ui.its_spinbox->value();
    basal_B=ui.burgers_spinbox->value()*(1E-10);
    inc=ui.inc_spinbox->value();
    timesteps=ui.timesteps->value();
    material_height=ui.height_spinbox->value();
    material_width=ui.width_spinbox->value();
    x_max=(material_width);
    y_max=(material_height-40);
    num_cells=ui.cells_spinbox->value();
    if(ui.energy_checkBox->isChecked()){e_calc=1;}
    if(ui.basals_checkBox->isChecked()){basals=1;}
    if(ui.inplane_checkBox->isChecked()){inplane=1;}
    S=1;//ui.comboBox->currentIndex()+1; //if 1 then circle, if 0 then square
    T=ui.comboBox_4->currentIndex()+1; //if 0 then Anisotropic if 1 then Isotropic
    //call function to setup display boxes for energy if isotropic strain is selected.
    if (e_calc==1){include_energy();}

    sxx=ui.s_xx->value();
    sxy=ui.s_xy->value();
    sxz=ui.s_xz->value();
    syx=ui.s_yx->value();
    syy=ui.s_yy->value();
    syz=ui.s_yz->value();
    szx=ui.s_zx->value();
    szy=ui.s_zy->value();
    szz=ui.s_zz->value();


    edge_segments=480;//4*157;
    //disable spin boxes while program is running
    ui.disl_spinbox->setDisabled(true);
    ui.its_spinbox->setDisabled(true);
    ui.inc_spinbox->setDisabled(true);
    ui.burgers_spinbox->setDisabled(true);
    ui.timesteps->setDisabled(true);
    ui.comboBox_4->setDisabled(true);
    ui.energy_checkBox->setDisabled(true);
    ui.randomise_checkBox->setDisabled(true);
    ui.basals_checkBox->setDisabled(true);
    ui.width_spinbox->setDisabled(true);
    ui.height_spinbox->setDisabled(true);
    ui.cells_spinbox->setDisabled(true);
    ui.test_combobox->setDisabled(true);
    ui.mapping_combobox->setDisabled(true);
    ui.s_xx->setDisabled(true);
    ui.s_xy->setDisabled(true);
    ui.s_xz->setDisabled(true);
    ui.s_yx->setDisabled(true);
    ui.s_yy->setDisabled(true);
    ui.s_yz->setDisabled(true);
    ui.s_zx->setDisabled(true);
    ui.s_zy->setDisabled(true);
    ui.s_zz->setDisabled(true);
    ui.inplane_checkBox->setDisabled(true);



    Write write;  // to write info to file


    srand(time(NULL)); // initialise rand()

    Structure A;// A: Class structure to build disl arrangements
    Force F;
    max_energy=0.0;

    Kex=((pow(C11*C33,0.5)+C13)*(pow(C44*(pow(C11*C33,0.5))/(C33*(pow(C11*C33,0.5)+2*C44)),0.5)));
    Key=((pow(C11*C33,0.5)+C13)*(pow(C44*(pow(C11*C33,0.5))/(C11*(pow(C11*C33,0.5)+2*C44)),0.5)));
    A1=((pow(C11*C33,0.5))+C13)*(pow(((pow(C11*C33,0.5))-C13)/(C33*C44*((pow(C11*C33,0.5))+C13+(2*C44))),0.5));
    A2=(pow(C11*C33,0.5))/C33;
    A3=(((pow(C11*C33,0.5))+C13)*((pow(C11*C33,0.5))-C13+(2*C44)))/(C33*C44);
    A4=(((pow(C11*C33,0.5))-C13)*((pow(C11*C33,0.5))+C13-(2*C44)))-((pow(C11*C33,0.5))*C44);
    A5=C33*C44;
    A6=2*M_PI*(pow(C11*C33,0.5));
    A7=C44/(2*M_PI);


    if (inplane==1)
    {
        A.Inplane_rounding();
        int m=ui.mapping_combobox->currentIndex();
        Inplane P;
        int ret=P.run_inplane(m);

        for(int t=0;t<timesteps;t++)
        {
            F.inplane_forces();
            MP.calculate_strainmap(m);
            ui.renderArea->setShape(RenderArea::Pixmap);
        }

        //ui.renderArea->setShape(RenderArea::Pixmap);
        ui.reset_button->setDisabled(false);
        ui.replay_button->setDisabled(false);
        if (ret==0){return 0;}
        else{qDebug()<<"Something wrong, Inplane is not returning zero...";}
    }

    ////////////////////////////////////////////////////////////
    // array of dislocations
    // Disl x posns for plane of dipoles (1 of each dipole)
    if (test_run==1){
        if (!ui.randomise_checkBox->isChecked()){
            double Dip_posn_x[disl_columns];
            for(int l=0; l<disl_columns; l++){
                Dip_posn_x[l]=90+(l*(x_max/disl_columns));}

            // y posns for planes of dipoles
            double Dip_posn_y[disl_lays];
            for(int l=0; l<disl_lays; l++){
                Dip_posn_y[l]=l*(y_max/disl_lays);}

            A.Build_Di(Dip_posn_x, Dip_posn_y); // make disl posns
            for (int k=0; k<(initial_disl_num); k++)
            {
                structure.Add_new_grid_pair();
            }

        }
        else {

            for (int j=0;j<edge_segments;j++)
            {E_new[j].count=0;
                E_new[j].counth=0;
                E_new[j].x=E[j].x;
                E_new[j].y=E[j].y;}
            group_number=0;
            dislocation_number=0;
            for (int k=0; k<(initial_disl_num); k++)
            {
                structure.Add_new_pair();
                interaction.check_new_interactions();

            }

        }
    }

    ////////////////////////////////////////////////////////////
    // array of edge segments
    // x,y remain as unstrained crystal edge positions
    //Edge E[ int( edge_segments ) ];
    // array of edge positions with added strains
    // use E.x,y as reference to add strains to
    // Edge E_new[ int( edge_segments ) ]; // new edge positions
    A.Build_Square_Ed(E);  //INITIAL EDGE POSITIONS - SQUARE


    //////////////////////Main Program///////////////////////////////////////////

    for(its=0; its<=num_its; its++){
   //     qDebug()<<"iteration:"<<its;
        if (test_run==2)
        {
            int done=test.line_test();

            interaction.check_all_interactions();
            structure.calculate_edge_strains();
            ui.renderArea->setShape(RenderArea::Pixmap);
            write.Expansion(); // calc expansion
            write.Area(); //calc area
            if (e_calc==1){
                En.Calc_energy();} //calc energy

            if((cexpansion/(material_height/100))>max_cexp){
                max_cexp=cexpansion/(material_height/100);}
            if(fabs(aexpansion/(material_width/100))>max_cexp){
                max_cexp=fabs(aexpansion/(material_width/100));}
            if(fabs(100-total_area)>max_area_change){
                max_area_change=fabs(100-total_area);}

            ui.graphicalRenderArea->setShape(RenderArea::Graph);
            //display the expansions and energies in the display labels
            ui.label_17->setNum(cexpansion);
            ui.label_15->setNum(aexpansion);
            if(e_calc==1){
     //           ui.label_13->setNum(En.En);
            }

            if (done==2)
            {

                ui.reset_button->setDisabled(false);
                ui.replay_button->setDisabled(false);

                return 0;
            }
        }
        else if (test_run==3){
            double clo;
            int density=40;
            test.convergence_test(density);
            for (int j=1;j<num_its;j++)
            {
                clo=clock();
                num_cells=j;
                its=j;

                structure.calculate_edge_strains();

                ui.renderArea->setShape(RenderArea::Pixmap);
                write.Expansion(); // calc expansion
                write.Area(); //calc area
                if (e_calc==1){
                    En.Calc_energy();} //calc energy

                if((cexpansion/(material_height/100))>max_cexp){
                    max_cexp=cexpansion/(material_height/100);}
                if(fabs(aexpansion/(material_width/100))>max_cexp){
                    max_cexp=fabs(aexpansion/(material_width/100));}
                if(fabs(100-total_area)>max_area_change){
                    max_area_change=fabs(100-total_area);}

                ui.graphicalRenderArea->setShape(RenderArea::Graph);
                //display the expansions and energies in the display labels
                ui.label_17->setNum(cexpansion);
                ui.label_15->setNum(aexpansion);
                if(e_calc==1){
    //                ui.label_13->setNum(En.En);
                }

                double dif = double(clock()-clo)/1000000;

                double lehto_oberg = density*10*basal_B*1E+10*80*100/(material_height*material_width);
                double program = (cexpansion*100/material_height);
                double agreement = program/lehto_oberg*100;

                qDebug()<<"number of  repeat cells: "<<num_cells<<"  agreement with Lehto-Oberg: "<<agreement<<"%"<<"Time:"<<dif<<"seconds  "<<cexpansion<<"  "<<En.En;

            }
            ui.reset_button->setDisabled(false);
            ui.replay_button->setDisabled(false);
            return 0;

        }

        else if (test_run==4) //insert a test dislocation and move it across the material and map the energy profile.
        {
  //          structure.Add_new_pair();
            structure.Add_test_basal();
            for(int ystep=1;ystep<(material_height/200);ystep++){
             ui.renderArea->setShape(RenderArea::Pixmap);

            int jmax=material_width;
            num_its=jmax;
            int pos=D.size()-1;
            for (int j=0;j<jmax;j++)
            {
                its=j;
                En.Calc_energy_test();
                F.move_pris();
                ui.label_13->setNum(En.En);
                D[pos].x+=1;
                D[pos].xd+=1;
                D[pos].f=0.0;
                for(int i=0;i<D.size()-2;i++){
                    for (int k=-num_cells;k<=num_cells;k++){
                        D[pos].f+=F.calc_f_d1_d2(pos,i,D[i].xd+(k*300));
                    }
                }
                qDebug()<<D[pos].f<<","<<D[pos].xd-300;

                ui.graphicalRenderArea->setShape(RenderArea::Graph);
                ui.renderArea->setShape(RenderArea::Pixmap);
            }
            D[pos].y+=200;
            D[pos].x-=material_width;
            D[pos].xd-=material_width;
        }
            return 0;
        }

        else if (test_run==1){

            force_iteration=0;
            structure.calculate_edge_strains();
             ui.renderArea->setShape(RenderArea::Pixmap);

            for(int i=1; i<=timesteps; i++)
            {
                if (checkStop()==true){return 0;}
                force_iteration=i;
                //calculate the force on each dislocation due to the others
                if(group_number!=0){F.move_pris();}
                F.move_basals();
                interaction.check_all_interactions();
                interaction.check_for_ruckandtuck();
                interaction.check_for_ruck_tuck_growth();
          //      structure.calculate_edge_strains();  //very slow to recalculate after each time step but useful for debugging
          // if(i==timesteps-1){      ui.renderArea->setShape(RenderArea::Pixmap);}
          // if ((timesteps>100)&&(i%10==0)){ ui.renderArea->setShape(RenderArea::Pixmap);}
            ui.renderArea->setShape(RenderArea::Pixmap);
            }
            write.Expansion(); // calc expansion
            write.Area(); //calc area
            if (e_calc==1){
                En.Calc_energy();} //calc energy


   //         qDebug()<<"";
   //         qDebug()<<"New iteration -"<<its;
   //         for (int i=0;i<D.size();i++)
   //         {qDebug()<<i<<D[i].B<<D[i].B1<<D[i].by<<D[i].bx;}

            bool ex=false;

            if((cexpansion/(material_height/100))>max_cexp){
             //   if (fabs(cexpansion-max_cexp)/((max_cexp/(its-1)))>2.5){qDebug()<<its<<"c-expansion jump"<<max_cexp<<cexpansion<<(max_cexp/(its-1))<<cexpansion-max_cexp;ex=true;}
                max_cexp=cexpansion/(material_height/100);}
            if(fabs(aexpansion/(material_width/100))>max_cexp){
                max_cexp=fabs(aexpansion/(material_width/100));}
            if(fabs(100-total_area)>max_area_change){
                max_area_change=fabs(100-total_area);}

            ui.graphicalRenderArea->setShape(RenderArea::Graph);
            //display the expansions and energies in the display labels
            ui.label_17->setNum(cexpansion);
            ui.label_15->setNum(aexpansion);

            if(e_calc==1){

                ui.label_13->setNum(En.En);
            }

            qDebug()<<its<<"    "<<cexpansion<<"    "<<En.En;
           // if (ex==true){exit(0);}


            //increase number of dislocations by one for each iteration
            disl_num+=inc;
            for (int d=1; d<=inc; d++)
            {   if (!ui.randomise_checkBox->isChecked())
                {
                      structure.Add_new_grid_pair();
                }
                else{
                    structure.Add_new_pair();
                   interaction.check_new_interactions();
                   interaction.check_for_ruckandtuck();
                   interaction.check_for_ruck_tuck_growth();

                }

            }

   //         qDebug()<<"iteration"<<its<<"done";
        }

    }

    write.Write_to_file();
    //once program has finished re-enable replay and reset buttons.
    ui.reset_button->setDisabled(false);
    ui.replay_button->setDisabled(false);

    return 0;
}


void MainWindow::include_energy()
//if isotropic strains are selected then display labels for Energy display.
{
    ui.label_12->setText(QApplication::translate("MainWindow", "Energy:", 0));
    ui.label_13->setText(QApplication::translate("MainWindow", "", 0));
}



//function to remove image files when the reset button is pressed
void MainWindow::remove_Image_Files()
{
    char filename[64];
    char filename2[64];
    for (int t=initial_disl_num; t<=(initial_disl_num+(inc*num_its)); (t+=inc)){
        for(int f=0; f<=timesteps; f++){
            snprintf(filename, sizeof(char) * 64, "Animate/images/bmp%d_%d.bmp", t, f);
            snprintf(filename2, sizeof(char) * 64, "Animate/images/graph%d.bmp",t);
            remove(filename);
            remove(filename2);
        }
    }
}



void MainWindow::netburg()
{
    double net_burg=0;
    for (int d=0; d<D.size(); d++)
    {net_burg+=D[d].B1;}
    net_burg=fabs(net_burg);
    if (net_burg>=1E-10)
    {
        qDebug()<<"burg mag not conserved"<<net_burg<<its+100<<"_"<<force_iteration;
        qDebug()<<"exiting program";
       // exit(0);
    }
    else {qDebug()<<"pass"<<its+100<<"_"<<force_iteration;}
}

bool MainWindow::checkStop()
{
QCoreApplication::processEvents();
if (stop==false){return false;}
else
{
    ui.reset_button->setDisabled(false);
    return true;
}
}
