#include "externVar.h"

Energy::Energy()
{
    En = 0.;
}

void Energy::Calc_energy()
{
    // Calculate self energies
    double En_self = 0.; // zero self energy
    for (int k = 0; (k < D.size()); k++)
    {
        if (D[k].image == false)
        {
            En_self += Iso_self_en(k);
            //qDebug() << "1" << En_self;
            //if (isnan(En_self) != 0)
            //{
            //    qDebug() << En_self << "not a number 1";
            //    exit(0);
            //}
        }
    }
    for (int l = 0; l < ImgD.size(); l++)
    {
        if (ImgD[l].image == false)
        {
            En_self += Iso_self_en_im(l);
            //qDebug() << "2" << En_self;
            //if (isnan(En_self) != 0)
            //{
            //    qDebug() << En_self << "not a number 2";
            //    exit(0);
            //}
        }
    }

    En = 0.; // reset energy to zero
    for (int k = 0; (k < D.size() - 1); k++)   // calc En
    {
        for (int l = k + 1; (l < D.size()); l++)
        {
            if (D[k].image == false && D[l].image == false)
                En += (Iso_en(k, l)); //*double(1.00e-11));
            //qDebug() << "3" << En.En;
            //if (isnan(En.En) != 0)
            //{
            //    qDebug() << En.En << k << l << D.size() << D[k].x << D[l].x << D[k].y << D[l].y << "not a number 3";
            //    exit(0);
            //}
        }
    }
    for (int k = 0; (k < D.size()); k++)
    {
        for (int l = 0; l < ImgD.size(); l++)
        {
            if (D[k].image == false && ImgD[l].image == true)
            {
                En += (Iso_en_img(k, l));//*double(1.00e-11));
                //qDebug() << "4" << En.En;
                //if (isnan(En.En) != 0)
                //{
                //    qDebug() << En.En << "not a number 4";
                //    exit(0);
                //}
            }
            else if (D[k].image == true && ImgD[l].image == false)
            {

                En += (Iso_en_img(k, l));//*double(1.00e-11));
                //qDebug() << "5" << En.En;
                //if (isnan(En.En) != 0)
                //{
                //    qDebug() << En.En << "not a number 5";
                //    exit(0);
                //}
            }
        }
    }
    for (int k = 0; (k < ImgD.size()); k++)
    {
        for (int l = 0; l < ImgD.size(); l++)
        {
            if ((ImgD[k].image == false && ImgD[l].image == false) && (k != l))
            {
                En += (Iso_en_img_img(l, k));//*double(1.00e-11));
                //qDebug() << "6" << En.En;
                //if (isnan(En.En) != 0)
                //{
                //    qDebug() << En.En << k << l << ImgD[k].x << ImgD[l].x << ImgD[k].y << ImgD[l].y << "not a number 6";
                //    exit(0);
                //}
            }
        }
    }


    //qDebug() << En << En_self << En_self * double(1.00e-11) << En + En_self * double(1.00e-11);
    En += (En_self);//*double(1.00e-11);
    //qDebug() << En;
    En = En / (material_height * material_width * 1E-10 * 1E-10);

    if (fabs(En) > fabs(max_energy))
    {
        max_energy = En;
    }
}

double Energy::Iso_en(int d1, int d2)
{
    // H&L p.117 (127) Energy of interation per unit length
    vectorDim burg1, burg2, xi, vectR; // vectors for disl1, disl2 & line vector (xi)
    burg1 << D[d1].bx, D[d1].by, 0; // B prismatic
    burg2 << D[d2].bx, D[d2].by, 0;
    xi << 0, 0, 1; // line vector
    vectR << (D[d2].x - D[d1].x) * 1E-10, (D[d2].y - D[d1].y) * 1E-10, 0; // vector from x1 to x2

    // Energy for Wij/L:
    R = vectR.norm(); // sep of disl1, disl2

    if (D[d1].B == 1 || D[d1].B == 2)
    {

        res = -Kex * (1 - nu) / (2. * M_PI) * burg1.dot(xi) * burg2.dot(xi) * log(R / Ra);
        res += -Kex / (2 * M_PI) * (burg1.cross(xi)).dot(burg2.cross(xi)) * log(R / Ra) \
            - Kex / (2 * M_PI * R * R) * (burg1.cross(xi)).dot(vectR) * \
            (burg2.cross(xi)).dot(vectR);
    }
    if (D[d1].B == 3 || D[d1].B == 4)
    {
        res = -Key * (1 - nu) / (2. * M_PI) * burg1.dot(xi) * burg2.dot(xi) * log(R / Ra);
        res += -Key / (2 * M_PI) * (burg1.cross(xi)).dot(burg2.cross(xi)) * log(R / Ra) \
            - Key / (2 * M_PI * R * R) * (burg1.cross(xi)).dot(vectR) * \
            (burg2.cross(xi)).dot(vectR);
    }


    return res;
}

double Energy::Iso_en_img(int d1, int d2)
{

    // H&L p.117 (127) Energy of interation per unit length
    vectorDim burg1, burg2, xi, vectR; // vectors for disl1, disl2 & line vector (xi)
    burg1 << D[d1].bx, D[d1].by, 0; // B prismatic
    burg2 << ImgD[d2].bx, ImgD[d2].by, 0;
    xi << 0, 0, 1; // line vector
    vectR << (ImgD[d2].x - D[d1].x) * 1E-10, (ImgD[d2].y - D[d1].y) * 1E-10, 0; // vector from x1 to x2

    // Energy for Wij/L:
    R = vectR.norm(); // sep of disl1, disl2

    if (D[d1].B == 1 || D[d1].B == 2)
    {

        res = -Kex * (1 - nu) / (2. * M_PI) * burg1.dot(xi) * burg2.dot(xi) * log(R / Ra);
        res += -Kex / (2 * M_PI) * (burg1.cross(xi)).dot(burg2.cross(xi)) * log(R / Ra) \
            - Kex / (2 * M_PI * R * R) * (burg1.cross(xi)).dot(vectR) * \
            (burg2.cross(xi)).dot(vectR);
    }
    if (D[d1].B == 3 || D[d1].B == 4)
    {
        res = -Key * (1 - nu) / (2. * M_PI) * burg1.dot(xi) * burg2.dot(xi) * log(R / Ra);
        res += -Key / (2 * M_PI) * (burg1.cross(xi)).dot(burg2.cross(xi)) * log(R / Ra) \
            - Key / (2 * M_PI * R * R) * (burg1.cross(xi)).dot(vectR) * \
            (burg2.cross(xi)).dot(vectR);
    }


    return res;
}


double Energy::Iso_en_img_img(int d1, int d2)
{

    // H&L p.117 (127) Energy of interation per unit length
    vectorDim burg1, burg2, xi, vectR; // vectors for disl1, disl2 & line vector (xi)
    burg1 << ImgD[d1].bx, ImgD[d1].by, 0; // B prismatic
    burg2 << ImgD[d2].bx, ImgD[d2].by, 0;
    xi << 0, 0, 1; // line vector
    vectR << (ImgD[d2].x - ImgD[d1].x) * 1E-10, (ImgD[d2].y - ImgD[d1].y) * 1E-10, 0; // vector from x1 to x2

    // Energy for Wij/L:
    R = vectR.norm(); // sep of disl1, disl2

    if (ImgD[d1].B == 1 || ImgD[d1].B == 2)
    {

        res = -Kex * (1 - nu) / (2. * M_PI) * burg1.dot(xi) * burg2.dot(xi) * log(R / Ra);
        res += -Kex / (2 * M_PI) * (burg1.cross(xi)).dot(burg2.cross(xi)) * log(R / Ra) \
            - Kex / (2 * M_PI * R * R) * (burg1.cross(xi)).dot(vectR) * \
            (burg2.cross(xi)).dot(vectR);
    }
    if (ImgD[d1].B == 3 || ImgD[d1].B == 4)
    {
        res = -Key * (1 - nu) / (2. * M_PI) * burg1.dot(xi) * burg2.dot(xi) * log(R / Ra);
        res += -Key / (2 * M_PI) * (burg1.cross(xi)).dot(burg2.cross(xi)) * log(R / Ra) \
            - Key / (2 * M_PI * R * R) * (burg1.cross(xi)).dot(vectR) * \
            (burg2.cross(xi)).dot(vectR);
    }


    return res;
}


double Energy::Iso_self_en(int i)
{

    if (D[i].by == 0)
    {
        return (Kex * pow(D[i].bx, 2)) / (4 * M_PI) * (log(Ra / Rc) - 1);
    }
    else if (D[i].bx == 0)
    {
        return (Key * pow(D[i].by, 2)) / (4 * M_PI) * (log(Ra / Rc) - 1);
    }
    else
        return 0;

}

double Energy::Iso_self_en_im(int i)
{

    if (ImgD[i].by == 0)
    {
        double selfn = (Kex * pow(ImgD[i].bx, 2)) / (4 * M_PI) * (log(Ra / Rc) - 1);

        return selfn;
    }
    else if (ImgD[i].bx == 0)
    {
        double selfn = (Key * pow(ImgD[i].by, 2)) / (4 * M_PI) * (log(Ra / Rc) - 1);

        return selfn;
    }
    else
        return 0;

}

