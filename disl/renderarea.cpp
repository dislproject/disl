#include <QtGui>
#include <QDebug>
#include "ImageMagick/Magick++.h"
#include "externVar.h"
#include "renderarea.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <list>
#include <time.h>
using namespace std;
using namespace Magick;

Image bmpimage;
QList<double> listOfAYPos;
QList<double> listOfCYPos;
QList<double> listOfCXPos;
QList<double> listOfEYPos;
QList<double> listOfEXPos;
QList<double> listOfAreaYPos;
QList<double> listOfAreaXPos;
QList<QPoint> listOfAPoints;
QList<QPoint> scaledEPoints;
QList<QPoint> scaledCPoints;
QList<QPoint> scaledAPoints;




RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    //initial setup: display text first, load pixmaps which will be needed.
    shape = Text;
    antialiased = false;
    pixmapLT.load(":/Animate/T_left.png");
    pixmapRT.load(":/Animate/T_right.png");
    pixmapU.load(":/Animate/T_up.png");
    pixmapD.load(":/Animate/T_down.png");
    pixmapT.load(":/Animate/E_top.png");
    pixmapR.load(":/Animate/E_right.png");
    pixmapOT.load(":/Animate/E_top_o.png");
    pixmapOR.load(":/Animate/E_right_o.png");
    pixmapA.load(":/Animate/a_point.png");
    pixmapC.load(":/Animate/c_point.png");
    pixmapE.load(":/Animate/e_point.png");
    pixmapArea.load(":/Animate/area_point.png");

    pixmapDLT.load(":/Animate/DT_left.png");
    pixmapDRT.load(":/Animate/DT_right.png");
    pixmapDU.load(":/Animate/DT_up.png");
    pixmapDD.load(":/Animate/DT_down.png");

    pixmapTLT.load(":/Animate/TT_left.png");
    pixmapTRT.load(":/Animate/TT_right.png");
    pixmapTU.load(":/Animate/TT_up.png");
    pixmapTD.load(":/Animate/TT_down.png");

    pixmapIL.load(":/Animate/T_left_img.png");
    pixmapIR.load(":/Animate/T_right_img.png");
    pixmapIU.load(":/Animate/T_up_img.png");
    pixmapID.load(":/Animate/T_down_img.png");

    pixmaptestu.load(":/Animate/test_up.png");
    pixmaptestd.load(":/Animate/test_down.png");

    pixmapD5.load(":/Animate/D5.png");
    pixmapD6.load(":/Animate/D6.png");
    pixmapD7.load(":/Animate/D7.png");
    pixmapD8.load(":/Animate/D8.png");
    pixmapD9.load(":/Animate/D9.png");
    pixmapD10.load(":/Animate/D10.png");
    pixmapD11.load(":/Animate/D11.png");
    pixmapD12.load(":/Animate/D12.png");
    pixmapD13.load(":/Animate/D13.png");
    pixmapD14.load(":/Animate/D14.png");
    pixmapD15.load(":/Animate/D15.png");
    pixmapD16.load(":/Animate/D16.png");

    pixmap1.load(":/Animate/1.png");
    pixmap2.load(":/Animate/2.png");
    pixmap3.load(":/Animate/3.png");
    pixmap4.load(":/Animate/4.png");
    pixmap5.load(":/Animate/5.png");
    pixmap6.load(":/Animate/6.png");
    pixmap7.load(":/Animate/7.png");
    pixmap8.load(":/Animate/8.png");
    pixmap9.load(":/Animate/9.png");
    pixmap10.load(":/Animate/10.png");
    pixmap_1.load(":/Animate/-1.png");
    pixmap_2.load(":/Animate/-2.png");
    pixmap_3.load(":/Animate/-3.png");
    pixmap_4.load(":/Animate/-4.png");
    pixmap_5.load(":/Animate/-5.png");
    pixmap_6.load(":/Animate/-6.png");
    pixmap_7.load(":/Animate/-7.png");
    pixmap_8.load(":/Animate/-8.png");
    pixmap_9.load(":/Animate/-9.png");
    pixmap_10.load(":/Animate/-10.png");
    pixmap0.load(":/Animate/0.png");



    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}


//minimum size of the render area in pixels
QSize RenderArea::minimumSizeHint() const
{
    return QSize(400, 600);
}
//default size of render area
QSize RenderArea::sizeHint() const
{
    return QSize(400, 750);
}

void RenderArea::setShape(Shape shape)
{
    this->shape = shape;
    repaint();

}

void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void RenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

void RenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

//function which is called when shape is updated (in this case to pixmap.)
void RenderArea::paintEvent(QPaintEvent * /* event */)
{

    //setup a rectangle to write text in.
    QRect rect(10, 20, 600, 400);

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    //setup painters. first writes to screen, second to Qimage for saving.
    QPainter painter(this);
    // QPainter painter2;


    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);



    switch (shape) {
    case Text:
        painter.drawText(rect, Qt::AlignCenter, tr("Waiting for\nParameters"));
        break;
    case Pixmap:
        plotPixmaps();
        break;
    case Graph:
        plotGraph();
        break;

    }


}

void RenderArea::plotGraph()
{
    QImage image(750,800,QImage::Format_RGB32);
    image.fill(QColor(Qt::white).rgb()); //white background (default is black)
    QPainter graphpainter(this);
    QPainter graphpainter2;
    graphpainter2.begin(&image);
    for (int xaxis=60; xaxis<570; xaxis++)
    {
        graphpainter.drawPixmap(xaxis, 316, pixmapT );
        graphpainter2.drawPixmap(xaxis, 316, pixmapT );
    }
    for (int yaxis=60; yaxis<600;yaxis++)
    {
        graphpainter.drawPixmap(106, yaxis, pixmapR);
        graphpainter2.drawPixmap(106, yaxis, pixmapR);
        if (e_calc==1||a_calc==1){
            graphpainter.drawPixmap(556, yaxis, pixmapR);
            graphpainter2.drawPixmap(556, yaxis, pixmapR);
        }

    }

    graphpainter.drawText(400,316,"Iteration Number");
    graphpainter.drawText(60, 40, "Expansion (%)");
    graphpainter2.drawText(380,316,"Iteration Number");
    graphpainter2.drawText(40, 40, "Expansion (%)");
    graphpainter.drawText(50, 700, "Number of Dislocations            Density                                   disl/cm2");
    graphpainter2.drawText(50, 700, "Number of Dislocations");
    graphpainter.drawText(50, 660, "Sum of Burgers Vector Magnitudes");
    graphpainter2.drawText(50, 660, "Sum of Burgers Vector Magnitudes");
    graphpainter.drawText(50, 620, "Net Burgers Vector");
    graphpainter2.drawText(50, 620, "Net Burgers Vector");
    graphpainter.drawText(50, 740, "Area");
    graphpainter2.drawText(50, 740, "Area");

    if (e_calc==1){
        graphpainter.drawText(580, 40, "Energy (J/m^3)");
        graphpainter2.drawText(580, 40, "Energy (J/m^3)");
    }
    if (a_calc==1){
        graphpainter.drawText(580, 595, "Area Change (%)");
        graphpainter2.drawText(580, 595, "Area Change (%)");
    }


    double net_burg=0;
    for (int d=0; d<D.size(); d++)
    {net_burg+=D[d].B1;}
    QString net;
    net.append(QString("%1").arg(net_burg));
    graphpainter.drawText(220,620,net);
    graphpainter2.drawText(220,620,net);

    QString dis;
    dis.append(QString("%1").arg(D.size()));
    graphpainter.drawText(220,700,dis);
    graphpainter2.drawText(220,700,dis);

    QString density;
    density.append(QString("%1").arg(D.size()/(material_height*1E-8*material_width*1E-8),0,'G',4));
    graphpainter.drawText(310,700,density);

    QString area;
    area.append(QString("%1").arg(int(total_area)));
    area.append(QString("%1").arg("%"));
    graphpainter.drawText(220,740,area);
    graphpainter2.drawText(220,740,area);

    QString burg_sum;
    burg_sum.clear();
    double burg_tot=0;
    double burg_mag;
    for (int d=0;d<D.size();d++)
    {
        if(D[d].B1<0){
            burg_mag=((-1)*D[d].B1);
        }
        else {
            burg_mag=D[d].B1;
        }
        burg_tot+=burg_mag;

    }


    burg_sum.append(QString("%1").arg(burg_tot));
    graphpainter.drawText(280,660,burg_sum);
    graphpainter2.drawText(280,660,burg_sum);




    //axis labelling
    for (int a=1; a<=5; a++){
        graphpainter.drawPixmap( 104+(a*(450/5)) , 315 ,18,20, pixmapR);
        graphpainter2.drawPixmap( 104+(a*(450/5)) , 315 ,18,20, pixmapR);
        QString x_label;
        x_label.append(QString("%1").arg(a*(num_its+1)/5));
        graphpainter.drawText( 100+(a*(450/5)) , 340 , x_label);
        graphpainter2.drawText( 100+(a*(450/5)) , 340 , x_label);

        graphpainter.drawPixmap( 100 , 315-(a*250/5) ,20,18, pixmapT);
        graphpainter2.drawPixmap( 100 , 315-(a*250/5) ,20,18, pixmapT);
        QString y_label;
        y_label.append(QString("%1").arg(a*(max_cexp/5),0,'f',1));
        graphpainter.drawText( 60 , 320-(a*250/5) , y_label);
        graphpainter2.drawText( 60 , 320-(a*250/5) , y_label);
    }
    for(int a=-5; a<0; a++){
        QString y_label;
        y_label.append(QString("%1").arg(a*(max_cexp/5),0,'f',1));
        graphpainter.drawText( 60 , 320-(a*250/5) , y_label);
        graphpainter2.drawText( 60 , 320-(a*250/5) , y_label);
        graphpainter.drawPixmap( 100 , 315-(a*250/5) ,20,18, pixmapT);
        graphpainter2.drawPixmap( 100 , 315-(a*250/5) ,20,18, pixmapT);

    }

    //axis scaling

    double x_axis_factor = double(450./num_its);

    double expy = (cexpansion/(material_height/100));
    double expx =105+double(x_axis_factor*its);
    listOfCXPos<<expx;
    listOfCYPos<<expy;
    expy = (aexpansion/(material_width/100));
    listOfAYPos<<expy;
    listOfAPoints.clear();
    scaledCPoints.clear();
    for(int s=0; s<listOfCYPos.size();s++)
    { double y_axis_factor = -(250./max_cexp);

        QPoint scaledC;
        scaledC.setY(315.+(y_axis_factor*listOfCYPos.value(s)));
        scaledC.setX(listOfCXPos.value(s));
        scaledCPoints<<scaledC;

        QPoint scaledA;
        scaledA.setY(315.+(y_axis_factor*listOfAYPos.value(s)));
        scaledA.setX(listOfCXPos.value(s));
        listOfAPoints<<scaledA;
    }

    if(e_calc==1){
        for (int a=1; a<=5; a++){
            graphpainter.drawPixmap( 555, 314-(a*(250/5)) ,20,18, pixmapT);
            graphpainter2.drawPixmap( 555, 314-(a*(250/5)) ,20,18, pixmapT);

            QString e_label;
            e_label.append(QString("%1").arg(a*(max_energy/5),0,'G',3));
            graphpainter.drawText( 570 , 320-(a*250/5) , e_label);
            graphpainter2.drawText( 570 , 320-(a*250/5) , e_label);
        }

        double energy_y = (En.En);
        double energy_x =105+double(x_axis_factor*its);
        listOfEXPos<<energy_x;
        listOfEYPos<<energy_y;
        scaledEPoints.clear();
        for(int s=0; s<listOfEXPos.size();s++)
        {
            double y_axis_e_factor = -(250.0/max_energy);
            QPoint scaledE;

            scaledE.setX(listOfEXPos.value(s));
            scaledE.setY(315.+(y_axis_e_factor*listOfEYPos.value(s)));
            scaledEPoints<<scaledE;
        }

        foreach(const QPoint &epoint, scaledEPoints){
            graphpainter.drawPixmap(epoint, pixmapE);
            graphpainter2.drawPixmap(epoint, pixmapE);
        }
    }



    if(a_calc==1)
    {
        for(int a=-5;a<0;a++)
        {
            graphpainter.drawPixmap( 555, 314-(a*(250/5)) ,20,18, pixmapT);
            graphpainter2.drawPixmap( 555, 314-(a*(250/5)) ,20,18, pixmapT);

            QString a_label;
            a_label.append(QString("%1").arg(a*(max_area_change/5),0,'f',1));
            graphpainter.drawText( 570 , 320-(a*250/5) , a_label);
            graphpainter2.drawText( 570 , 320-(a*250/5) , a_label);

        }
        double area_y=(100-total_area);
        double area_x=105+double(x_axis_factor*its);
        listOfAreaXPos<<area_x;
        listOfAreaYPos<<area_y;
        scaledAPoints.clear();

        double area_axis_factor=(250/max_area_change);
        QPoint scaledA;
        for (int s=0; s<listOfAreaXPos.size();s++)
        {
            scaledA.setX(listOfAreaXPos.value(s));
            scaledA.setY(315.+(area_axis_factor*listOfAreaYPos.value(s)));
            scaledAPoints<<scaledA;
        }
        foreach(const QPoint &areapoint, scaledAPoints){
            graphpainter.drawPixmap(areapoint, pixmapArea);
            graphpainter2.drawPixmap(areapoint, pixmapArea);
        }
    }

    foreach(const QPoint &cpoint, scaledCPoints){
        graphpainter.drawPixmap(cpoint, pixmapC);
        graphpainter2.drawPixmap(cpoint, pixmapC);
    }

    foreach(const QPoint &apoint, listOfAPoints){
        graphpainter.drawPixmap(apoint, pixmapA);
        graphpainter2.drawPixmap(apoint, pixmapA);
    }



    //draw a legend
    graphpainter.drawPixmap(480,500, pixmapC);
    graphpainter.drawPixmap(480,520, pixmapA);
    if(e_calc==1){ graphpainter.drawPixmap(480,540, pixmapE);
        graphpainter.drawText(500,550,"Energy");}
    graphpainter.drawText(500,510,"C axis");
    graphpainter.drawText(500,530,"A axis");


    graphpainter2.drawPixmap(480,500, pixmapC);
    graphpainter2.drawPixmap(480,520, pixmapA);
    if(e_calc==1){ graphpainter2.drawPixmap(480,540, pixmapE);
        graphpainter2.drawText(500,550,"Energy");}
    graphpainter2.drawText(500,510,"C axis");
    graphpainter2.drawText(500,530,"A axis");



    graphpainter2.end();

    char imagename[64];
    snprintf(imagename, sizeof(char) * 64, "Animate/images/graph%d.bmp", (100+its)); //create filename for image
    image.save(imagename,0,-1); //save image


}

void RenderArea::plotPixmaps()
{
    //set up image to write to for saving bmps (image size 700x800pix)
    QImage image(650,800,QImage::Format_RGB32);
    image.fill(QColor(Qt::white).rgb()); //white background (default is black)
    QPainter painter(this);
    QPainter painter2;

    painter2.begin(&image);//start painting to Qimage

    double factor;
    double offset_factor;

    if(material_height>=material_width)
    {
        factor=height()/((material_height/1680)*2400.);

        if ((max_cexp>25)&&(int(max_cexp)==int(cexpansion/(material_height/100))))
        {
            factor=(height())/((material_height/1680)*2400.+(8*(max_cexp-10)));
        }

        offset_factor=(0.5/(material_height/1680))*(0.625/factor);
    }


    else if(material_width>material_height)
    {
        factor=width()/((material_width/1800)*2400.);
        offset_factor=(0.5/(material_width/1800))*(0.4/factor);
    }


    double dis_factor=5.;//(9*height()/1200.);
    double dis_factor2=5.;//(10*height()/1200.);
    //plot new edge positions on the screen and draw to Qimage.
    //NB factor is a scaling factor to ensure the image fills the screen.
    //x values are scaled by same amount to keep aspect ratio.

    for ( int i = 0; i < edge_segments; i++ ){
        if( E_new[i].type == 5 && E_new[i].alive==true) {
            painter.drawPixmap(100*offset_factor+((E_new[i].x-305)*factor), 140*offset_factor+((E_new[i].y-125)*factor), pixmapT );
            painter2.drawPixmap(100*offset_factor+((E_new[i].x-305)*factor), 140*offset_factor+((E_new[i].y-125)*factor), pixmapT );
        }
        if( E_new[i].type == 6 && E_new[i].alive==true) {
            painter.drawPixmap(100*offset_factor+((E_new[i].x-305)*factor), 140*offset_factor+((E_new[i].y-125)*factor), pixmapR );
            painter2.drawPixmap(100*offset_factor+((E_new[i].x-305)*factor), 140*offset_factor+((E_new[i].y-125)*factor), pixmapR );
        }
    }
    //plot old edge positions and draw to image using red icons.
    for ( int i = 0; i < edge_segments; i++ ){
        if( E[i].type == 5 ) {
            painter.drawPixmap(100*offset_factor+((E[i].x-305)*factor), 140*offset_factor+((E[i].y-125)*factor), pixmapOT );
            painter2.drawPixmap(100*offset_factor+((E[i].x-305)*factor), 140*offset_factor+((E[i].y-125)*factor), pixmapOT );
        }
        if( E[i].type == 6 ) {
            painter.drawPixmap(100*offset_factor+((E[i].x-305)*factor), 140*offset_factor+((E[i].y-125)*factor), pixmapOR );
            painter2.drawPixmap(100*offset_factor+((E[i].x-305)*factor), 140*offset_factor+((E[i].y-125)*factor), pixmapOR );
        }
    }

    for (int i = 0; i<500; i++)
    {
        for (int j=0; j<500; j++)
        {
            if (grid[i][j].colour==-1)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_1);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_1);
            }
            if (grid[i][j].colour==-2)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_2);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_2);
            }
            if (grid[i][j].colour==-3)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_3);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_3);
            }
            if (grid[i][j].colour==-4)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_4);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_4);
            }
            if (grid[i][j].colour==-5)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_5);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_5);
            }
            if (grid[i][j].colour==-6)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_6);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_6);
            }
            if (grid[i][j].colour==-7)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_7);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_7);
            }
            if (grid[i][j].colour==-8)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_8);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_8);
            }
            if (grid[i][j].colour==-9)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_9);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_9);
            }
            if (grid[i][j].colour==-10)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_10);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap_10);
            }
            if (grid[i][j].colour==0)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap0);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap0);
            }
            if (grid[i][j].colour==1)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap1);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap1);
            }
            if (grid[i][j].colour==2)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap2);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap2);
            }
            if (grid[i][j].colour==3)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap3);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap3);
            }
            if (grid[i][j].colour==4)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap4);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap4);
            }
            if (grid[i][j].colour==5)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap5);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap5);
            }
            if (grid[i][j].colour==6)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap6);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap6);
            }
            if (grid[i][j].colour==7)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap7);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap7);
            }
            if (grid[i][j].colour==8)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap8);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap8);
            }
            if (grid[i][j].colour==9)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap9);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap9);
            }
            if (grid[i][j].colour==10)
            {
            painter.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap10);
            painter2.drawPixmap(100*offset_factor+((grid[i][j].x-305)*factor), 140*offset_factor+((grid[i][j].y-125)*factor), pixmap10);
            }
        }
    }



    if (inplane==1)
    {
        dis_factor=dis_factor*-2;
        for( int i = 0; i <D.size(); i++ ) {
            //qDebug()<<"type "<<D[i].B<<" pos (x,y)"<<D[i].xd<<","<<D[i].y;
            if (D[i].B==5)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD5);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD5);
            }
            if (D[i].B==6)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD6);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD6);
            }
            if (D[i].B==7)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD7);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD7);
            }
            if (D[i].B==8)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD8);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD8);
            }
            if (D[i].B==9)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD9);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD9);
            }
            if (D[i].B==10)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD10);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD10);
            }
            if (D[i].B==11)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD11);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD11);
            }
            if (D[i].B==12)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD12);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD12);
            }
            if (D[i].B==13)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD13);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD13);
            }
            if (D[i].B==14)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD14);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD14);
            }
            if (D[i].B==15)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD15);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD15);
            }
            if (D[i].B==16)
            {
                painter.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD16);
                painter2.drawPixmap(100*offset_factor+(D[i].xd-321)*factor,140*offset_factor+(D[i].y-141)*factor, dis_factor, dis_factor, pixmapD16);
            }


        }
    }
    else{
        //plot dislocation pixmaps onto screen and image. The pixmaps themselves are also scaled in this case by same scaling factor.
        for( int i = 0; i <D.size(); i++ ) {
            double burg;
            if(D[i].B1<0){burg=(-1*D[i].B1);}
            else{burg=D[i].B1;}

            if(burg==basal_B||burg<=atom_sep*1E-10){
                if( D[i].B == 1){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapU);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapU);
                }
                else if( D[i].B == 2){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapD);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapD);
                }
                else if( D[i].B == 3) {
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor2, dis_factor2, pixmapLT);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor2, dis_factor2, pixmapLT);
                }
                else if( D[i].B == 4) {
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor2, dis_factor2, pixmapRT);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor2, dis_factor2, pixmapRT);
                }

            }


            if(burg==(2*basal_B)||burg==(2*atom_sep*1E-10))
            {
                if( D[i].B == 1){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDU);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDU);
                }
                else if( D[i].B == 2){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDD);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDD);
                }
                else if( D[i].B == 3){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDLT);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDLT);
                }
                else if( D[i].B == 4){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDRT);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapDRT);
                }
            }
            if(burg>=(3*atom_sep*1E-10)){
                if( D[i].B == 1){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTU);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTU);
                }
                else if( D[i].B == 2){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTD);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTD);
                }
            }
            if (burg>=(3*basal_B)){
                if( D[i].B == 3){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTLT);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTLT);
                }
                else if( D[i].B == 4){
                    painter.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTRT);
                    painter2.drawPixmap(100*offset_factor+(D[i].xd-300)*factor,140*offset_factor+(D[i].y-120)*factor, dis_factor, dis_factor, pixmapTRT);
                }
            }

            if(burg>25.0*1E-10){
                QString burgm;
                burgm.append(QString("%1").arg(burg,0,'G',1));
                //painter.drawText(-107-14*factor+D[i].x*factor , 35+D[i].y*factor, burgm);
                //painter2.drawText(-107-14*factor+D[i].x*factor , 35+D[i].y*factor, burgm);

            }

        }
    }


    painter2.end(); //finish writing to image

    char imagename[64];

    //   if (timesteps*num_its<=2000.){
    snprintf(imagename, sizeof(char) * 64, "Animate/images/bmp%d_%d.bmp",(100+its),force_iteration); //create filename for image
    image.save(imagename,0,-1); //save image
    //    }
    //    else if(timesteps*num_its>2000.)
    //    {
    //        snprintf(imagename, sizeof(char) * 64, "Animate/images/bmp%d.bmp",100+its); //create filename for image
    //        image.save(imagename,0,-1);
    //    }
    //annotate_image(imagename);

}

void RenderArea::annotate_image(char imagename[])
{
    bmpimage.read(imagename);
    std::ostringstream cexp;
    cexp<<cexpansion;
    std::ostringstream aexp;
    aexp<<aexpansion;

    bmpimage.annotate("c Expansion:","+40+5", NorthWestGravity);
    bmpimage.annotate(cexp.str(),"+150+5", NorthWestGravity);
    bmpimage.annotate("a Expansion:","+290+5", NorthWestGravity);
    bmpimage.annotate(aexp.str(),"+400+5", NorthWestGravity);
    if(e_calc==1){
        std::ostringstream energy;
        energy<<En.En;
        bmpimage.annotate("Energy:","+540+5",NorthWestGravity);
        bmpimage.annotate(energy.str(),"+600+5",NorthWestGravity);
    }
    bmpimage.write(imagename);//write annotated image back to file
}

