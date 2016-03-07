//////////////////////////////////////////////////
// write disl positions to dat files
//////////////////////////////////////////////////
#include "externVar.h"
double cexpansion, aexpansion;
double total_area;
class Write
{

private:

    int i;

public:


    double sumyt;
    double sumyb;
    double sumxl;
    double sumxr;
    int cnt;

    Write();
    void Write_to_file();
    void Write_to_file(int, Disl D[], Edge E_new[], Edge E[]);
    //void Write_to_file(Edge E[], double, int);
    void Expansion();
    void Area();
};


Write::Write()
{
    sumyt = sumyb = sumxl = sumxr = aexpansion = cexpansion = 0.;
}

/*
void Write::Write_to_file(Edge E[], double En, int it)
{

    // write to log file

    time_t currenttime;
    time(&currenttime);
    std::string s1 = "logs/logfile";
    std::string s2 = ctime(&currenttime);
    s1.append(s2);
    std::ofstream a_file;
    a_file.open(s1.c_str());
    if (T == 1)
    {
        a_file << "Anisotropic" << std::endl;
    }
    if (T == 2)
    {
        a_file << "Isotropic" << std::endl;
    }
    if (S == 2)
    {
        a_file << "Circle" << std::endl;
    }
    if (S == 1)
    {
        a_file << "Square" << std::endl;
    }
    a_file << "Number of dislocations: " << 2 * disl_lays * disl_num << std::endl;
    a_file << "C axis expansion: " << cexpansion << std::endl;
    a_file << "A axis expansion: " << aexpansion << std::endl;
    //a_file<<"Energy:"<<En<<std::endl;
    a_file.close();

    std::string c1 = "Animate/Dat_files/comment.dat";
    std::ofstream c_file;
    c_file.open(c1.c_str());
    if (T == 1)
    {
        c_file << "Anisotropic ";
    }
    if (T == 2)
    {
        c_file << "Isotropic ";
    }
    c_file << 2 * disl_lays * disl_num << " " << cexpansion << " " << aexpansion;
    c_file.close();
}
 */

void Write::Write_to_file()
{

    time_t currenttime;
    time(&currenttime);
    std::string s5 = ctime(&currenttime);

    std::string s1 = "Animate/Dat_files/output_s";
    std::stringstream stress;
    stress << applied_stress;
    std::string s2 = stress.str();
    std::string s3 = "_t";
    std::stringstream timestep;
    timestep << timesteps;
    std::string s4 = timestep.str();
    std::string s6 = ".dat";

    s1.append(s2);
    s1.append(s3);
    s1.append(s4);
    s1.append(s5);
    s1.append(s6);

    std::ofstream a_file;
    a_file.open(s1.c_str());

    a_file << "Number of dislocations " << D.size() << std::endl;
    a_file << "iterations " << num_its << std::endl;
    a_file << "increment " << inc << std::endl;
    a_file << "time steps " << timesteps << std::endl;
    a_file << "applied stress " << applied_stress << std::endl;
    a_file << "c expansion " << cexpansion << std::endl;
    if (e_calc == 1)
    {
        a_file << "Energy " << En.En << std::endl;
    }
    a_file.close();
}


void Write::Write_to_file(int its, Disl D[], Edge E_new[], Edge E[])
{
    // strings for file naming
    std::string s1 = "Animate/Dat_files/pos_";
    std::string s3 = ".dat";
    std::string s2;


    // convert int i to string for file numbering
    std::stringstream out;
    out << its;
    s2 = out.str();

    s1.append(s2);
    s1.append(s3);

    std::ofstream a_file;
    ////////////////////OPEN FILE TO WRITE//////
    a_file.open(s1.c_str());

    ////////////////////EDGE POSITIONS//////////
    for (int i = 0; i < edge_segments; i++)
    {
        if (E_new[i].type == 5)
        {
            a_file << "   " << int( E_new[i].x ) << "  " << \
                int( E_new[i].y ) << " T" << std::endl;
        }
        if (E_new[i].type == 6)
        {
            a_file << "   " << int( E_new[i].x ) << "  " <<
                int( E_new[i].y ) << " R" << std::endl;
        }
    }

    for (int i = 0; i < edge_segments; i++)
    {
        if (E_new[i].type == 5)
        {
            a_file << "   " << int( E[i].x ) << "  " << \
                int( E[i].y ) << " OT" << std::endl;
        }
        if (E_new[i].type == 6)
        {
            a_file << "   " << int( E[i].x ) << "  " <<
                int( E[i].y ) << " OR" << std::endl;
        }
    }

    ////////////////////DISL POSITIONS///////////
    for (int i = 0; i < 2 * disl_lays * disl_num; i++)
    {
        if (D[i].B == 1 /* && D[i].alive */)
        {
            a_file << "   " << int( D[i].x ) << "  " << \
                int( D[i].y ) << " U" << std::endl;
        }
        else if (D[i].B == 2 /* && D[i].alive*/)
        {
            a_file << "   " << int( D[i].x ) << "  " <<
                int( D[i].y ) << " D" << std::endl;
        }
        else if (D[i].B == 3 /* && D[i].alive*/)
        {
            a_file << "   " << int( D[i].x ) << "  " <<
                int( D[i].y ) << " LT" << std::endl;
        }
        else if (D[i].B == 4 /* && D[i].alive */)
        {
            a_file << "   " << int( D[i].x ) << "  " <<
                int( D[i].y ) << " RT" << std::endl;
        }
    }

    ////////////////////CLOSE FILE////////////////////
    a_file.close();
}

void Write::Expansion()
{
    double top_edges = 0.;
    double bottom_edges = 0.;
    sumyt = sumyb = sumxr = sumxl = 0.;
    for (cnt = 120; cnt < 240; cnt++)
    {
        if ((E_new[cnt].x > E_new[119].x) && (E_new[cnt].x < E_new[359].x))
        {
            sumyt += E_new[cnt].y;
            E_new[cnt].alive = true;
            top_edges++;
        }
        else
        {
            E_new[cnt].alive = false;
        }
    }
    for (cnt = 360; cnt < 480; cnt++)
    {
        if ((E_new[cnt].x > E_new[0].x) && (E_new[cnt].x < E_new[240].x))
        {
            sumyb += E_new[cnt].y;
            E_new[cnt].alive = true;
            bottom_edges++;
        }
        else
        {
            E_new[cnt].alive = false;
        }
    }
    for (cnt = 0; cnt < 120; cnt++)
    {
        sumxr += E_new[cnt].x;
    }
    for (cnt = 240; cnt < 360; cnt++)
    {
        sumxl += E_new[cnt].x;
    }
    aexpansion = (sumxl / 120. - sumxr / 120.) - material_width;
    cexpansion = (sumyt / top_edges) - (sumyb / bottom_edges) - material_height;
    if (isnan(cexpansion) != 0)
    {
        qDebug() << "c-expansion is NAN";
        for (int d = 0; d < D.size(); d++)
        {
            qDebug() << "d=" << d << "x,y =" << D[d].x << "," << D[d].y << "xd=" << D[d].xd << "f, df=" << D[d].f << "," << D[d].df;
        }
        exit(0);
    }

}

void Write::Area()
{
    total_area = 0;
    int j = 360;
    for (i = 0; i < 240; i++)
    {
        total_area += (E_new[j].x + E_new[i].x) * (E_new[j].y - E_new[i].y);
        j = i;
    }
    j = 239;
    for (i = 359; i >= 240; i--)
    {
        total_area += (E_new[j].x + E_new[i].x) * (E_new[j].y - E_new[i].y);
        j = i;
    }
    j = 240;
    for (i = 479; i >= 360; i--)
    {
        total_area += (E_new[j].x + E_new[i].x) * (E_new[j].y - E_new[i].y);
        j = i;
    }


    total_area = (total_area / (material_height * material_width)) * 100 / 2;


    /*
    total_area = 0;
    for (int j = 120; j < 240; j++)
    {
        double y_sep = E_new[j].y - E_new[j + 240].y;
        double width = material_width / 120.;
        total_area += y_sep * width;
    }
    for (int j = 0; j < 120; j++)
    {
        double x_sep = E_new[j].x - E_new[j + 240].x;
        double height = material_height / 120.;
        total_area -= x_sep * height;
    }
    total_area = (total_area / (material_height * material_width)) * 100;
     */
}

