#include "externVar.h"

Test::Test()
{

}

int Test::line_test()
{
    int sep=28;
    int max_its=(material_width/(sep+1.5E10*basal_B))+1;

    if (its>max_its){return 2;}

    double column=(material_width-double((its+1)*sep))/double(2*(its+1));
    D.clear();
    group_number=0;
    for(int i=0;i<=its;i++){
        double x_pos=double(i*sep)+(double(2*i+1)*column);
        double y_pos=material_height/2;
        add_dipole(x_pos,y_pos,sep);
        // qDebug()<<its<<x_pos<<y_pos<<sep<<column;
    }
    return 1;
}

void Test::add_dipole(int pos_x, int pos_y, int sep)
{

    Disl dis1, dis2;
    //x positions
    dis1.x=pos_x+300;
    dis2.x=pos_x+300+sep;

    //y positions
    dis1.y=pos_y+120;
    dis2.y=pos_y+120;
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
        group_number++;
    }
    //flagged for re-run?
    dis1.re_run=false;
    dis2.re_run=false;
    dis1.number=dislocation_number;
    dis2.number=dislocation_number+1;
    dislocation_number=dislocation_number+2;

    D<<dis1<<dis2;

}



void Test::convergence_test(int density)
{
    int x=10*density;
    for (int i=0;i<x;i++)
    {
        structure.Add_new_pair();
        interaction.check_all_interactions();
    }

}


void Test::negative_test_dist()
{
    int dist=(its+1)*5;
    D.clear();
    for (int i =1; i<(material_height/int_layer_sep);i++)
    {
        add_dipole(dist,(i*int_layer_sep),(material_width-2*dist));
    }
}

void Test::negative_test_dense()
{
    int dist=30;

    for (int j=0; j<=inc;j++){
        int plane = rand() % int( (y_max)/int_layer_sep );
        double y= 20+double( plane ) * int_layer_sep;
        add_dipole(dist,y,(material_width-2*dist));
    }
     interaction.check_all_interactions();

}

/* else if(test_run==4)
 {
 test.negative_test();
 structure.Build_Di_cells(num_cells);
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
 ui.label_10->setNum(cexpansion);
 ui.label_11->setNum(aexpansion);
 if(e_calc==1){
     ui.label_13->setNum(En.En);}
 }
 else if (test_run==5)
 {
 test.negative_test_dense();
 structure.Build_Di_cells(num_cells);
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
 ui.label_10->setNum(cexpansion);
 ui.label_11->setNum(aexpansion);
 if(e_calc==1){
     ui.label_13->setNum(En.En);}
 }*/
