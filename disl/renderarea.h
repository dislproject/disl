#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "externVar.h"

//! [0]
class RenderArea : public QWidget
{
    Q_OBJECT

public:
    enum Shape {Text, Pixmap, Graph };

    RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void plotPixmaps();
    void plotGraph();
    void annotate_image(char imagename[64]);

public slots:
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);


protected:
    void paintEvent(QPaintEvent *event);

private:
    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;

    QPixmap pixmap;
    QPixmap pixmapLT;
    QPixmap pixmapRT;
    QPixmap pixmapU;
    QPixmap pixmapD;
    QPixmap pixmapT;
    QPixmap pixmapR;
    QPixmap pixmapOT;
    QPixmap pixmapOR;
    QPixmap pixmapC;
    QPixmap pixmapA;
    QPixmap pixmapE;
    QPixmap pixmapArea;

    QPixmap pixmapDLT;
    QPixmap pixmapDRT;
    QPixmap pixmapDU;
    QPixmap pixmapDD;


    QPixmap pixmapTLT;
    QPixmap pixmapTRT;
    QPixmap pixmapTU;
    QPixmap pixmapTD;

    QPixmap pixmapIL;
    QPixmap pixmapIR;
    QPixmap pixmapIU;
    QPixmap pixmapID;

    QPixmap pixmaptestu;
    QPixmap pixmaptestd;

    QPixmap pixmapD5;
    QPixmap pixmapD6;
    QPixmap pixmapD7;
    QPixmap pixmapD8;
    QPixmap pixmapD9;
    QPixmap pixmapD10;
    QPixmap pixmapD11;
    QPixmap pixmapD12;
    QPixmap pixmapD13;
    QPixmap pixmapD14;
    QPixmap pixmapD15;
    QPixmap pixmapD16;

    QPixmap pixmap1;
    QPixmap pixmap2;
    QPixmap pixmap3;
    QPixmap pixmap4;
    QPixmap pixmap5;
    QPixmap pixmap6;
    QPixmap pixmap7;
    QPixmap pixmap8;
    QPixmap pixmap9;
    QPixmap pixmap10;
    QPixmap pixmap0;
    QPixmap pixmap_1;
    QPixmap pixmap_2;
    QPixmap pixmap_3;
    QPixmap pixmap_4;
    QPixmap pixmap_5;
    QPixmap pixmap_6;
    QPixmap pixmap_7;
    QPixmap pixmap_8;
    QPixmap pixmap_9;
    QPixmap pixmap_10;


};
//! [0]

#endif // RENDERAREA_H
