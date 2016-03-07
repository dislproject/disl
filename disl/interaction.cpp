#include "externVar.h"

Interaction::Interaction()
{
}

int Interaction::Calc_interaction(int d1, int d2, int x1, int x2)
{
    if (d1 < 0 || d2 < 0)
    {
        qDebug() << "referencing negative number... exiting";
        exit(0);
    }
    if (d1 == D.size() || d2 == D.size())
    {
        qDebug() << "referencing item outside range of D list" << d1 << d2 << D.size();
        exit(0);
    }
    typedef Eigen::Matrix<double, 3, 1> vectorDim;
    vectorDim vectR;
    vectR << x2 - x1, D[d2].y - D[d1].y, 0;
    double R = vectR.norm();

    double burg1_mag = fabs(D[d1].B1);
    double burg2_mag = fabs(D[d2].B1);

    bool interact = false;

    if ((R <= 2E10 * burg1_mag) && (D[d1].y == D[d2].y))
    {
        interact = true;
    }
    if ((D[d1].B == 1 || D[d1].B == 2 || D[d2].B == 1 || D[d2].B == 2) && (R <= 20E10 * burg1_mag))
    {
        interact = true;
    }
    if ((D[d1].B == 1 && D[d2].B == 3) || (D[d2].B == 1 && D[d1].B == 3) || (D[d1].B == 1 && D[d2].B == 4) || (D[d2].B == 1 && D[d1].B == 4))
    {
        interact = false;
    }
    if ((D[d1].B == 2 && D[d2].B == 3) || (D[d2].B == 2 && D[d1].B == 3) || (D[d1].B == 2 && D[d2].B == 4) || (D[d2].B == 2 && D[d1].B == 4))
    {
        interact = false;
    }
    if (D[d1].y != D[d2].y)
    {
        interact = false;
    }


    if (interact == true)
    {
        //if prismatic with prismatic
        //1.) if equal magnitudes and opposite signs then eliminate
        //2.) if not equal magnitudes and opposite signs then subtract smaller Burgers vector from the larger one
        //3.) if same signs then add the Burgers vectors together to create super dislocation

        //qDebug() << its << d1 << d2 << D[d1].group << D[d2].group << D[d1].B1 << D[d2].B1 << burg1_mag << burg2_mag << R << D[d1].x << D[d1].xd << D[d2].x << D[d2].xd << x1 << x2 << D.size() << net_burg_cons();

        //Prismatic interactions
        if (((D[d1].B == 4) && D[d2].B == 3) || ((D[d1].B == 3) && (D[d2].B == 4)))
        {

            if (burg1_mag == burg2_mag)
            {
                ///////////1.) if equal magnitudes and opposite signs then eliminate

                //qDebug() << "interaction type 1" << its;

                cons_vol(d1, d2);
                Re_group(d1, d2);
                int g = D[d1].group;
                //qDebug() << "(1)";
                net_burg_cons();
                int multi = abs(burg1_mag / basal_B);
                int ch = check_dipole_size(d1, d2, multi);
                //qDebug() << "(2)" << ch;
                net_burg_cons();
                if (ch == 0)
                {

                    //qDebug() << "removing" << d1 << d2;
                    //ensure dislocations are removed in the right order
                    //dislocation of the highest index is removed first.
                    if (d1 > d2)
                    {
                        D.removeAt(d1);
                        D.removeAt(d2);
                    }
                    if (d1 < d2)
                    {
                        D.removeAt(d2);
                        D.removeAt(d1);
                    }
                }

                else
                {
                    //qDebug() << "calculating net burg (1.0)";
                    net_burg_cons();
                    //qDebug() << "restructuring group" << g;
                    int r = restructure_group(g);
                    if (r == 1)
                    {
                        //qDebug() << "returning:" << -ch;
                        return -2;
                    }
                    // print_group_stats(g);
                    //qDebug() << "returning" << -ch;
                    return -ch;
                }
                //qDebug() << "calculating net burg (1)";
                net_burg_cons();
                //qDebug() << "restructuring group";
                int r = restructure_group(g);
                if (r == 1)
                {
                    //qDebug() << "returning-" << -ch;
                    return -2;
                }
                //qDebug() << "done restructure";
                //print_group_stats(g);
                //qDebug() << "returning -2";
                return -2;
            }
            else if (burg1_mag != burg2_mag)
            {
                ///////////2.) if not equal magnitudes and opposite signs then subtract smaller Burgers vector from the larger one
                //qDebug() << "interaction type 2" << its;

                if (burg1_mag > burg2_mag)
                {
                    //qDebug() << "interaction type 2.1";
                    cons_vol(d2, d1);
                    double new_burg = D[d1].B1 + D[d2].B1;
                    Re_group(d1, d2);
                    int g = D[d1].group;
                    int multi = abs(burg2_mag / basal_B);

                    int ch = check_dipole_size(d1, d2, multi);

                    if (ch == 0)
                    {
                        D[d1].B1 = D[d1].by = new_burg;
                        D[d1].re_run = true;
                        if (D[d1].B1 > 0)
                        {
                            D[d1].B = 3;
                        }
                        else
                        {
                            D[d1].B = 4;
                        }
                        D.removeAt(d2);
                    }
                    else
                    {
                        //qDebug() << "calculating net burg (2.0)" << ch;
                        net_burg_cons();
                        //qDebug() << "restructuring group";
                        restructure_group(g);
                        //qDebug() << "done restructure";
                        //print_group_stats(g);
                        //qDebug() << "returning" << -ch;
                        return -ch;
                    }

                    //qDebug() << "calculating net burg (2)";
                    net_burg_cons();
                    //qDebug() << "restructuring group";
                    restructure_group(g);
                    //qDebug() << "done restructure";
                    //print_group_stats(g);
                    //qDebug() << "returning -1";
                    return -1;
                }
                else if (burg1_mag < burg2_mag)
                {
                    //qDebug() << "interaction type 2.2";
                    cons_vol(d1, d2);
                    double new_burg = D[d2].B1 + D[d1].B1;
                    Re_group(d1, d2);
                    int g = D[d1].group;
                    int multi = abs(burg1_mag / basal_B);
                    int ch = check_dipole_size(d1, d2, multi);

                    if (ch == 0)
                    {
                        //qDebug() << "2.2 ch=0";
                        D[d2].B1 = D[d2].by = new_burg;
                        D[d2].re_run = true;
                        if (D[d2].B1 > 0)
                        {
                            D[d2].B = 3;
                        }
                        else
                        {
                            D[d2].B = 4;
                        }
                        D.removeAt(d1);
                    }
                    else
                    {
                        //qDebug() << "2.2 ch=" << ch;
                        net_burg_cons();
                        //qDebug() << "restructuring group";
                        restructure_group(g);
                        //qDebug() << "done restructuring group";
                        //print_group_stats(g);
                        //qDebug() << "returning" << -ch;
                        return -ch;
                    }
                    //qDebug() << "calculating net burg (3)";
                    net_burg_cons();
                    restructure_group(g);
                    //print_group_stats(g);
                    //qDebug() << "returning -1";
                    return -1;
                }
            }
        }
        else if (((D[d1].B == 3) && (D[d2].B == 3)) || ((D[d1].B == 4) && (D[d2].B == 4)))
        {

            ///////////3.) if same signs then add the Burgers vectors together to create super dislocation
            //qDebug() << "interaction type 3" << its;
            cons_vol(d1, d2);
            double new_burg = D[d2].B1 + D[d1].B1;
            Re_group(d1, d2);

            int g = D[d1].group;
            D[d2].B1 = new_burg;
            D[d2].by = new_burg;
            D[d2].re_run = true;
            D.removeAt(d1);
            //qDebug() << "calculating net burg (4)";
            net_burg_cons();
            //qDebug() << "restructuring group";
            restructure_group(g);
            //qDebug() << "done restructuring group";
            //print_group_stats(g);
            //qDebug() << "returning -1";
            return -1;
        }

        //Basal interactions
        else if (((D[d1].B == 1) && (D[d2].B == 1)) || ((D[d1].B == 2) && (D[d2].B == 2)))
        {
            //qDebug() << "interaction type 4";
            //same signed dislocations - pileup
            double newburg = D[d1].B1 + D[d2].B1;
            D[d1].B1 = newburg;
            D[d1].bx = D[d1].B1;
            D.removeAt(d2);
            //qDebug() << "returning -1";
            return -1;
        }
        else if (((D[d1].B == 1) && (D[d2].B == 2)) || ((D[d1].B == 2) && (D[d2].B == 1)))
        {
            //qDebug() << "interaction type 5";
            //oppositely signed dislocations - annihillate
            if (burg1_mag == burg2_mag)
            {
                if (d1 > d2)
                {
                    D.removeAt(d1);
                    D.removeAt(d2);
                }
                else if (d2 > d1)
                {
                    D.removeAt(d2);
                    D.removeAt(d1);
                }
                //qDebug() << "returning -2";
                return -2;
            }
            else if (burg1_mag != burg2_mag)
            {
                double newburg = D[d1].B1 + D[d2].B1;
                D[d1].B1 = newburg;
                D[d1].bx = D[d1].B1;
                if (newburg > 0)
                {
                    D[d1].B = 1;
                }
                else if (newburg < 0)
                {
                    D[d1].B = 2;
                }
                D.removeAt(d2);
                //qDebug() << "returning -1";
                return -1;
            }
        }
        qDebug() << "interaction none of the above - should never see this!";
    }

    return 0;
}

void Interaction::cons_vol(int d1, int d2)
{
    //function to conserve extra material which is lost when two dislocations interact.
    // in this case d1 is the dislocation which will be removed.
    int c1, c2;
    c1 = (D[d1].xd - 300) / material_width;
    c2 = (D[d2].xd - 300) / material_width;
    double dx;
    if (c1 == c2)
    {
        dx = D[d2].xd - D[d1].xd;
    }
    else if (c1 != c2)
    {
        dx = D[d2].x - D[d1].x;
    }
    double m = ((dx * (D[d1].B1 / basal_B)) / material_width);
    //qDebug() << "cons vol" << d1 << d2 << D.size() << D[d1].B << D[d2].B << dx << m << D[d1].x << D[d1].xd << c1 << D[d2].x << D[d2].xd << c2;
    if (fabs(dx) > 0.8 * material_width)
    {
    }
    else
    {
        structure.Add_material_height(m);
    }
}


void Interaction::check_new_interactions()
{
    for (int l = D.size() - 2; l < D.size(); l++)
    {
        for (int m = 0; m < l; m++)
        {
            if (D[l].y == D[m].y)
            {
                // check if they interact
                //               qDebug()<<m<<l<<D[m].xd<<D[l].xd;
                int ch = Calc_interaction(m, l, D[m].xd, D[l].xd);
                //calc_interaction returns 0 for no interaction, -1 if the current dislocation is removed or -2 if the current and comparison dislocations are removed.
                if (ch == 0)
                {
                    //qDebug() << m << l << D[m].xd + material_width << D[l].xd;
                    ch = Calc_interaction(m, l, (D[m].xd + material_width), D[l].xd);
                    if (ch == 0)
                    {
                        //qDebug() << m << l << D[m].xd - material_width << D[l].xd;
                        ch = Calc_interaction(m, l, (D[m].xd - material_width), D[l].xd);
                    }
                }
                if (ch != 0) //i.e if the current dislocation /both dislocations is/are annihillated
                {
                    l += ch;
                    if (l < 0)
                    {
                        l = 0;
                    }
                    m = l;
                }
            }
        }
    }
    for (int r = 0; r < D.size(); r++)
    {
        if (D[r].re_run == true)
        {
            int flagged = 1;
            while (!flagged == 0) {
                flagged = reRun_interaction();
            }
        }
    }
}

void Interaction::check_all_interactions()
{

    for (int l = 1; l < D.size(); l++)
    {
        for (int m = 0; m < l; m++)
        {
            if (D[l].y == D[m].y)
            {
                // check if they interact
                //qDebug() << m << l << D[m].xd << D[l].xd;
                int ch = Calc_interaction(m, l, D[m].xd, D[l].xd);
                //calc_interaction returns 0 for no interaction, -1 if the current dislocation is removed or -2 if the current and comparison dislocations are removed.
                if (ch == 0)
                {
                    //no interaction check the cell to each side of the origin
                    //qDebug() << m << l << D[m].xd + material_width << D[l].xd;
                    ch = Calc_interaction(m, l, D[m].xd - material_width, D[l].xd);
                    if (ch == 0)
                    {
                        //qDebug() << m << l << D[m].xd - material_width << D[l].xd;
                        ch = Calc_interaction(m, l, D[m].xd + material_width, D[l].xd);
                    }
                }
                if (ch != 0) //i.e if the current dislocation /both dislocations is/are annihillated
                {
                    l += ch;
                    if (l < 0)
                    {
                        l = 0;
                    }
                    m = l;
                }
            }
        }
    }

    for (int r = 0; r < D.size(); r++)
    {
        if (D[r].re_run == true)
        {
            int flagged = 1;
            while (!flagged == 0) {
                flagged = reRun_interaction();
            }
        }
    }
}

double Interaction::calc_netburg()
{
    double burg_tot = 0;
    double burg_mag;
    for (int d = 0; d < D.size(); d++)
    {
        if (D[d].B1 < 0)
        {
            burg_mag = ((-1) * D[d].B1);
        }
        else
        {
            burg_mag = D[d].B1;
        }
        burg_tot += burg_mag;
    }


    return burg_tot;
}

bool Interaction::net_burg_cons()
{
    double net_burg = 0;
    for (int d = 0; d < D.size(); d++)
    {
        net_burg += D[d].B1;
    }
    net_burg = fabs(net_burg);
    if (net_burg >= 1E-10)
    {
        //qDebug() << "burg mag not conserved" << net_burg;
        //exit(0);
        return false;
    }
    else
    {
        //qDebug() << "pass" << its + 100 << "_" << force_iteration;
        return true;
    }
}

bool Interaction::lone_dislocations()
{
    for (int g = 0; g <= group_number; g++)
    {
        int num = 0;

        for (int d = 0; d < D.size(); d++)
        {
            if (D[d].group == g)
            {
                num++;
            }
        }
        if (num == 1)
        {
            //qDebug() << g << num;
            return true;
        }
    }

    return false;
}

bool Interaction::check_position(double x, double y)
{
    //typedef Eigen::Matrix<double, 3, 1> vectorDim;
    for (int i = 0; i < D.size(); i++)
    {
        double ysep = fabs(D[i].y - y);
        if (ysep == 0)
        {
            double R = fabs(x - D[i].xd);
            if (R <= fabs(1E10 * basal_B))
            {
                return false;
            }
            R = fabs(x - (D[i].xd + material_width));
            if (R <= fabs(1E10 * basal_B))
            {
                return false;
            }
            R = fabs(x - (D[i].xd - material_width));
            if (R <= fabs(1E10 * basal_B))
            {
                return false;
            }
        }
    }
    return true;
}

void Interaction::Re_group(int d1, int d2)
{
    int g = D[d2].group;
    for (int i = 0; i < D.size(); i++) // Reassign dislocations in the same group as group 2
    {                                  // to group of dislocation 1.
        if (D[i].group == g)
        {
            //qDebug() << "regrouping" << i << d2 << D[i].B1 << D[i].group << D[d2].group << D[i].xd << D[i].x << D[i].y;
            D[i].group = D[d1].group;
        }
    }

    double net = 0.0;
    int numb = 0;
    for (int i = 0; i < D.size(); i++)
    {
        if (D[i].group == D[d1].group)
        {
            net += D[i].B1;
            //qDebug() << i << D[i].B1 << D[i].xd;
            numb++;
        }
    }
    //qDebug() << "net burg for group" << g << "=" << net << numb;
    //if (fabs(net) > 1E-10)
    //{
    //    exit(0);
    //}
}

int Interaction::restructure_group(int g)
{
    //qDebug() << "starting restructure group" << g;
    //find all dislocations in this group
    QList<int> disl_list, sorted_disl_list, temp_sorted;
    disl_list.clear();
    sorted_disl_list.clear();
    temp_sorted.clear();
    for (int i = 0; i < D.size(); i++)
    {
        if (D[i].group == g)
        {
            disl_list << i;
        }
    }
    //qDebug() << "created list" << disl_list.size();
    int number = disl_list.size();
    //sort list into accending x
    for (int s = 0; s < number; s++)
    {
        int min = disl_list.value(0);
        int min_ind;
        for (int i = 0; i < disl_list.size(); i++)
        {
            int disl = disl_list.value(i);
            if (D[disl].x <= D[min].x)
            {
                min = disl;
                min_ind = i;
            }
        }
        sorted_disl_list << min;
        temp_sorted << min;
        //qDebug() << min << D[min].B << D[min].xd << D[min].y << D[min].B1;
        disl_list.removeAt(min_ind);
    }

    //qDebug() << "initial group" << sorted_disl_list.size();

    if (disl_list.size() == 0 || sorted_disl_list.size() == 0)
    {
        for (int d = 0; d < D.size(); d++)
        {
            if (D[d].B == 3 || D[d].B == 4)
            {
                //qDebug() << d << D[d].B << D[d].B1 << D[d].group;
            }
        }
        return 1;
    }

    //print_group_stats(sorted_disl_list);
    //qDebug() << "finding minimum";
    Find_min(sorted_disl_list);
    //qDebug() << "final group";
    //print_group_stats(sorted_disl_list);

    //now if x<xmin add material width onto the dislocation so that xmin is always smallest.
    bool finished = false;
    for (int i = 0; i < sorted_disl_list.size(); i++)
    {
        if (finished == false)
        {
            int m = sorted_disl_list.value(i);
            finished = false;
            if (D[m].min == true)
            {
                finished = true;
            }
            else
            {
                D[m].x += material_width;
                //qDebug() << "adding to D[m].x" << m;
            }
        }
    }
    //qDebug() << "printing stats";
    //print_group_stats(sorted_disl_list);

    return 0;
}


void Interaction::Find_min(QList<int> sorted_list)
{
    //qDebug() << "finding minimum 1";
    for (int i = 0; i < sorted_list.size(); i++)
    {
        //qDebug() << "finding minimum 1.5";
        int d = sorted_list.value(i);
        D[d].min = false;
    }
    //qDebug() << "finding minimum 2";

    //qDebug() << "all false";
    print_group_stats(sorted_list);

    int num = sorted_list.size();
    //qDebug() << "finding minimum 3";
    for (int i = 0; i < sorted_list.size(); i++)
    {
        //qDebug() << "finding minimum 3.1";
        int d = sorted_list.value(i);
        if (D[d].B == 3)
        {
            //qDebug() << "finding minimum 3.2";
            double a = D[d].B1;
            //       qDebug()<<i<<a;
            for (int j = 1; j <= num; j++)
            {
                //qDebug()<<"finding minimum 3.2";
                int k = j + i;
                //qDebug() << i << j << k << a;
                if (k >= sorted_list.size())
                {
                    k -= sorted_list.size();
                }
                int d2 = sorted_list.value(k);
                a += D[d2].B1;
                //qDebug() << "added to a" << D[d2].B1 << i << j << k << a;
                if (a < -0.1E-10)
                {
                    //qDebug() << "not minimum";
                    //dislocation i is not min
                    j = sorted_list.size() + 1;
                }
                //qDebug() << sorted_list.size() << j << a;
                if ((j == num - 1) && ((a < 0.1E-10) && (a > -0.1E-10)))
                {
                    D[d].min = true;
                    i = sorted_list.size();
                }
            }
        }
        //qDebug() << "finding minimum 3.3";
    }
    //qDebug() << "found min - ";
    print_group_stats(sorted_list);
    //qDebug() << "finding minimum done";

}


int Interaction::Calc_interaction(int d1, int d2)
{
    qDebug() << "WARNING: calling old interaction function, use new function";

    if (d1 < 0 || d2 < 0)
    {
        qDebug() << "referencing negative number... exiting";
        exit(0);
    }
    if (d1 == D.size() || d2 == D.size())
    {
        qDebug() << "refernecing item outside range of D list" << D.size();
        exit(0);
    }
    typedef Eigen::Matrix<double, 3, 1> vectorDim;
    vectorDim vectR;
    vectR << D[d2].xd - D[d1].xd, D[d2].y - D[d1].y, 0;
    double R = vectR.norm();

    double burg1_mag, burg2_mag;
    if (D[d1].B1 < 0)
    {
        burg1_mag = (-1 * D[d1].B1);
    }
    else
    {
        burg1_mag = D[d1].B1;
    }
    if (D[d2].B1 < 0)
    {
        burg2_mag = (-1 * D[d2].B1);
    }
    else
    {
        burg2_mag = D[d2].B1;
    }
    bool interact = false;

    if (((D[d1].B == 1 || D[d1].B == 2) && R <= 1 * 1E10 * burg1_mag) && (D[d1].y == D[d2].y))//check if dislocation is within rc
    {
        interact = true;
    }

    if (((D[d1].B == 3 || D[d1].B == 4) && R <= 1 * 1E10 * burg1_mag) && (D[d1].y == D[d2].y))//check if dislocation is within rc
    {
        interact = true;
    }

    if ((D[d1].B == 1 && D[d2].B == 3) || (D[d2].B == 1 && D[d1].B == 3) || (D[d1].B == 1 && D[d2].B == 4) || (D[d2].B == 1 && D[d1].B == 4))
    {
        interact = false;
    }
    if ((D[d1].B == 2 && D[d2].B == 3) || (D[d2].B == 2 && D[d1].B == 3) || (D[d1].B == 2 && D[d2].B == 4) || (D[d2].B == 2 && D[d1].B == 4))
    {
        interact = false;
    }
    if (D[d1].y != D[d2].y)
    {
        interact = false;
    }
    if (d1 < 0 || d2 < 0)
    {
        interact = false;
    }
    if (interact == true)

    {
        //qDebug() << "interaction" << d1 << d2 << D[d1].x << D[d2].x << R << D.size();
        if (D[d1].B == D[d2].B) //check if they are of the same sign
        {
            //if same sign add burgers vectors
            D[d1].B1 += D[d2].B1;
            D[d1].re_run = true;

            if (basals == 1)
            {
                if (D[d1].B1 > 0)
                {
                    D[d1].B = 1;
                }
                else
                {
                    D[d1].B = 2;
                }
                D[d1].bx = D[d1].B1;
                D[d1].by = 0;
            }
            else if (basals == 0)
            {
                if (D[d1].B1 < 0)
                {
                    D[d1].B = 4;
                }
                else
                {
                    D[d1].B = 3;
                }
                D[d1].by = D[d1].B1;
                D[d1].bx = 0;

                int group = D[d2].group;
                for (int d = 0; d < D.size(); d++)
                {

                    if (D[d].group == group)
                    {
                        D[d].group = D[d1].group;

                    }
                }

            }
            //remove dislocation 2
            D.removeAt(d2);

            //qDebug() << "checking interaction 1 done";
            //iteration loop starts on same number as currently as list will shift once.
            return -1;

        }
        else if (D[d1].B != D[d2].B)
        {
            //annihillation
            if (burg1_mag == burg2_mag)
            {
                //if equal remove dislocations 1 and 2

                if (D[d1].B == 1 || D[d2].B == 1)
                {
                    //qDebug() << "basal annihillation";
                    if (d1 > d2)
                    {
                        D.removeAt(d1);
                        D.removeAt(d2);
                    }

                    else if (d2 > d1)
                    {
                        D.removeAt(d2);
                        D.removeAt(d1);
                    }

                    //structure.add_edge_material();
                    //structure.add_edge_material();

                    return -2;
                }
                else if (D[d1].B == 3 || D[d2].B == 3)
                {

                    //int d3 = d2 + 1;

                    QList<int> disl_group;
                    for (int i = 0; i < D.size(); i++)
                    {
                        if (D[i].group == D[d1].group)
                        {
                            disl_group << i;
                        }
                    }
                    //new test piece ///////////////////////////////////////////////////////////////////////////////////////////////////
                    // if ((D[d1].group == D[d2].group) && ((D[d1].B == 3 && (D[d1].x > D[d2].x)) || (D[d2].B == 3 && D[d2].x > D[d2].x)))
                    // {
                    //     int multi = fabs(D[d1].B1 / basal_B);
                    //     for (int i = 0; i < multi; i++)
                    //     {
                    //         structure.Add_material_height();
                    //     }
                    // }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if (disl_group.size() == 2 && (D[d1].group == D[d2].group)) //if next to each other then annihillate normally
                    {

                        if (d1 > d2)
                        {

                            D.removeAt(d1);
                            D.removeAt(d2);
                        }

                        else if (d2 > d1)
                        {
                            D.removeAt(d2);
                            D.removeAt(d1);
                        }
                        //if (net_burg_cons() == false)
                        //{
                        //    qDebug() << "net burg not conserved at point 3";
                        //    exit(0);
                        //}
                        //qDebug() << "checking interaction 1 done";
                        return -2;
                    }
                    else
                    {

                        int group = D[d2].group;
                        for (int d = 0; d < D.size(); d++)
                        {
                            if (D[d].group == group)
                            {
                                D[d].group = D[d1].group;
                                //qDebug() << "reassigned group to" << group << " for dislocation " << d;
                            }
                        }

                        //new test piece ///////////////////////////////////////////////////////////////////////////////////////////////////
                        // if ((D[d1].group == D[d2].group) && ((D[d1].B == 3 && (D[d1].x > D[d2].x)) || (D[d2].B == 3 && D[d2].x > D[d2].x)))
                        // {
                        //     int multi = fabs(D[d1].B1 / basal_B);
                        //     for (int i = 0; i < multi; i++)
                        //     {
                        //         structure.Add_material_height();
                        //     }
                        // }
                        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


                        if (d1 > d2)
                        {
                            D.removeAt(d1);
                            D.removeAt(d2);
                        }
                        else if (d2 > d1)
                        {
                            D.removeAt(d2);
                            D.removeAt(d1);
                        }
                        //qDebug() << "checking interaction 1 done";

                        return -2;

                    }
                }
                else
                {
                    qDebug() << "not prismatic or basal..." << D[d1].B << D[d2].B;
                }


            }

            else
            {
                //if not equal add burg2 to burg1 remove dislocation 2

                double newburg = D[d1].B1 + D[d2].B1;
                D[d1].re_run = true;

                if (basals == 1 && (D[d1].B == 1 || D[d2].B == 1))
                {
                    double burg_lost = D[d1].B1 - newburg;
                    for (int l = 0; l < (burg_lost / atom_sep); l++)
                    {
                        //structure.add_edge_material();
                    }
                    D[d1].B1 += D[d2].B1;
                    if (D[d1].B1 > 0)
                    {
                        D[d1].B = 1;
                    }
                    else
                    {
                        D[d1].B = 2;
                    }
                    D[d1].bx = D[d1].B1;
                    D[d1].by = 0;
                }
                else if (basals == 1 && (D[d1].B == 3 || D[d2].B == 3))
                {
                    D[d1].B1 = newburg;
                    if (D[d1].B1 < 0)
                    {
                        D[d1].B = 4;
                    }
                    else
                    {
                        D[d1].B = 3;
                    }
                    D[d1].by = D[d1].B1;
                    D[d1].bx = 0;
                }
                else if (basals == 0)
                {


                    //new test piece ///////////////////////////////////////////////////////////////////////////////////////////////////
                    // if ((D[d1].group == D[d2].group) && ((D[d1].B == 3 && (D[d1].x > D[d2].x)) || D[d2].B == 3 && D[d2].x > D[d2].x))
                    // {
                    //     int multi = 0;
                    //     if (fabs(D[d1].B1) > fabs(D[d2].B1))
                    //     {
                    //         multi = int(fabs(D[d2].B1 / basal_B));
                    //     }
                    //     if (fabs(D[d1].B1) < fabs(D[d2].B1))
                    //     {
                    //         multi = int(fabs(D[d1].B1 / basal_B));
                    //     }
                    //     for (int i = 0; i < multi; i++)
                    //     {
                    //         structure.Add_material_height();
                    //     }
                    // }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    D[d1].B1 = newburg;
                    if (D[d1].B1 < 0)
                    {
                        D[d1].B = 4;
                    }
                    else if (D[d1].B1 > 0)
                    {
                        D[d1].B = 3;
                    }
                    D[d1].by = D[d1].B1;
                    D[d1].bx = 0;

                    int group = D[d2].group;
                    for (int d = 0; d < D.size(); d++)
                    {

                        if (D[d].group == group)
                        {
                            D[d].group = D[d1].group;
                        }
                    }

                }


                D.removeAt(d2);


                //if (net_burg_cons() == false)
                //{
                //    qDebug() << "net burg not conserved at point 6";
                //    exit(0);
                //}
                //iteration loop starts on same number as currently as list will shift once.
                //qDebug() << "checking interaction 1 done";
                return -1;
            }
        }
        else
        {

        }
    }


    return 0;
}

int Interaction::reRun_interaction()
{
    int ret = 0; //return value
    for (int d1 = 0; d1 < D.size(); d1++)
    {
        if (D[d1].re_run == true)
        {
            if (d1 == D.size() - 1)
            {
                D[d1].re_run = false;

                //check the possible positions of opposite pair of the dislocation first.
                for (int m = d1 - 1; m <= d1; m = m + 2)
                {

                    if (D[d1].y == D[m].y)
                    {
                        int ch = Calc_interaction(d1, m, D[d1].xd, D[m].xd);
                        if (ch != 0)
                        {
                            d1 += ch;
                            if (d1 < 0)
                            {
                                d1 = 0;
                            }
                            m = d1 + 2;
                        }
                        else if (ch == 0)
                        {
                            D[d1].re_run = true;
                        }
                    }
                }
            }
            else
            {
                D[d1].re_run = false;

                //check the possible positions of opposite pair of the dislocation first.
                for (int m = d1 - 1; m <= d1 + 1; m = m + 2)
                {
                    if (m > 0)
                    {
                        if (D[d1].y == D[m].y)
                        {

                            int ch = Calc_interaction(d1, m, D[d1].xd, D[m].xd);
                            if (ch != 0)
                            {
                                d1 += ch;
                                if (d1 < 0)
                                {
                                    d1 = 0;
                                }
                                m = d1 + 2;
                            }
                            else if (ch == 0)
                            {
                                D[d1].re_run = true;
                            }
                        }
                    }
                }
            }

            if (D[d1].re_run == true)
            {
                D[d1].re_run = false;
                for (int m = 0; m < D.size(); m++)   //for each dislocation...

                {
                    if (m != d1) //other than itself..


                    {
                        if (m < d1) //if the dislocation is before this one in the list
                        {   // check if they interact
                            d1 += Calc_interaction(d1, m, D[d1].xd, D[m].xd);
                        }
                        else   //if the dislocation is after this one in the list
                        {
                            Calc_interaction(d1, m, D[d1].xd, D[m].xd);
                        }
                    }
                }

                if (D[d1].re_run == true)
                {
                    ret++;
                }
            }
        }
    }
    return ret;
}

//function to add material onto the edges when a basal dislocation hits the edge and leaves the material.
void Interaction::check_edges()
{
/*
    for (int d = 0; d < D.size(); d++)
    {

        //position of the edge at this y co-ord
        int k, l;

        for (int i = 0; i < 120; i++)
        {
            if (E_new[i].y <= D[d].y)
            {
                k = i;
            }
        }
        for (int i = 240; i < 360; i++)
        {
            if (E_new[i].y <= D[d].y)
            {
                l = i;
            }
        }

        double min = E_new[k].x;
        double max = E_new[l].x;

        // if (D[d].B == 1 || D[d].B == 2)
        // {
        if (D[d].x <= min)
        {
            for (int j = 0; j < 120; j++)
            {
                if (E[j].y > D[d].y && D[d].B1 > 0.)
                {
                    E_new[j].x -= D[d].B1;
                    int size = D[d].B1 / atom_sep;
                    E_new[j].count = E_new[j].count - size;

                }
                else if (E[j].y < D[d].y && D[d].B1 < 0.)
                {
                    E_new[j].x += D[d].B1;
                    int size = D[d].B1 / atom_sep;
                    E_new[j].count = E_new[j].count + size;

                }

            }
            D.removeAt(d);
            d -= 1;
        }
        else if (D[d].x >= max)
        {
            for (int j = 240; j < 360; j++)
            {
                if (E[j].y > D[d].y && D[d].B1 > 0.)
                {
                    E_new[j].x += D[d].B1;
                    int size = D[d].B1 / atom_sep;
                    E_new[j].count = E_new[j].count + size;

                }
                else if (E[j].y < D[d].y && D[d].B1 < 0.)
                {
                    E_new[j].x -= D[d].B1;
                    int size = D[d].B1 / atom_sep;
                    E_new[j].count = E_new[j].count - size;

                }

            }

            D.removeAt(d);
            d -= 1;
        }
        //  }

        //   else if (D[d].B == 3 || D[d].B == 4)
        //   {
        min = 300;
        max = 300 + material_width;

        if (D[d].x >= max || D[d].x < min)
        {
            D[d].image = true;

        }
        else
        {
            D[d].image = false;
        }


        // }
    }

    for (int d = 0; d < ImgD.size(); d++)
    {
        int k, l;

        for (int i = 0; i < 120; i++)
        {
            if (E_new[i].y <= ImgD[d].y)
            {
                k = i;
            }
        }
        for (int i = 240; i < 360; i++)
        {
            if (E_new[i].y <= ImgD[d].y)
            {
                l = i;
            }
        }

        double min = E_new[k].x;
        double max = E_new[l].x;

        min = 300;
        max = 300 + material_width;

        if ((ImgD[d].x > min) && (ImgD[d].x <= max))
        {
            //D[d].x += material_width;
            ImgD[d].image = false;

        }


    }
 */
}

double Interaction::calc_force(int d1, int d2)
{
    // ref p.452 H&L
    double forcex = 0.0;
    double forcey = 0.0;
    double x = D[d2].x - D[d1].x;
    double y = D[d2].y - D[d1].y;
    //double force = -Kex / (2 * M_PI) * (-D[d1].B1 * D[d2].B1 * ((x * (x * x - y * y)) / (pow(x * x + y * y, 2))));
    if ((D[d1].B == 1 || D[d1].B == 2) && (D[d2].B == 1 || D[d2].B == 2))
    {
        // for b=bx
        forcex = A1 * A7 * D[d1].B1 * D[d2].B1 * (-x * x * x + A2 * x * y * y) / (pow(x * x + A2 * y * y, 2) + A3 * x * x * y * y);
    }
    if ((D[d1].B == 1 || D[d1].B == 2) && (D[d2].B == 3 || D[d2].B == 4))
    {
        // for b=by
        forcey = A1 * A7 * D[d1].B1 * D[d2].B1 * (-x * x * y + A2 * y * y * y) / (pow(x * x + A2 * y * y, 2) + A3 * x * x * y * y);
    }
    if ((D[d1].B == 3 || D[d1].B == 4) && (D[d2].B == 1 || D[d2].B == 2))
    {
        // for b=by
        forcey = A1 * A7 * D[d1].B1 * D[d2].B1 * (-x * x * y + A2 * y * y * y) / (pow(x * x + A2 * y * y, 2) + A3 * x * x * y * y);
    }
    if ((D[d1].B == 3 || D[d1].B == 4) && (D[d2].B == 3 || D[d2].B == 4))
    {
        forcex = -(A1 * D[d1].B1 * D[d2].B1 * (A4 * x * y * y + A5 * x * x * x)) / (A6 * (pow(x * x + A2 * y * y, 2) + A3 * x * x * y * y));
    }

    if (D[d1].y == D[d2].y)
    {
        forcex = (2 / M_PI) * atan(pow(x * x + y * y, 0.5) - 30) * atan(pow(x * x + y * y, 0.5) + 30) * forcex;
    }
    //qDebug() << "force of dislocation " << d1 << "on" << d2 << "=" << forcex + forcey;
    return forcex + forcey;


}

void Interaction::check_for_cracks()
{
    for (int d = 0; d < D.size(); d++)
    {
        if (D[d].B == 2 && D[d].B1 <= (4 * atom_sep))
        {
            //crack present
            D[d].crack = true;
        }
        else if (D[d].B == 1 && D[d].B1 >= (4 * atom_sep))
        {
            //crack present
            D[d].crack = true;
        }

    }
}

void Interaction::check_for_ruckandtuck()
{
    double min_pileup = 3 * 1E-10; //minimum number of dislocations to class as a pileup
    double x_tol = 10; //x separation to be classed as a ruck and tuck
    double y_tol = 0.0; //tolerance on the y value in this case 10%
    //for each dislocation with each other dislocation with no duplicates
    for (int d1 = 0; d1 < D.size(); d1++)
    {

        for (int d2 = d1 + 1; d2 < D.size(); d2++)
        {

            //if burgers vector is bigger than 3 times single burgers vector
            if ((D[d1].B == 2 && D[d2].B == 1 && D[d1].B1 <= (-min_pileup * atom_sep)) && D[d2].B1 >= (min_pileup * atom_sep))
            {


                //check if the dislocations are on adjacent planes
                if ((D[d1].y >= (1 - y_tol) * (D[d2].y + int_layer_sep)) && (D[d1].y <= (1 + y_tol) * (D[d2].y + int_layer_sep)))
                {
                    double xsep = D[d1].xd - D[d2].xd;
                    if (fabs(xsep) < x_tol)
                    {
                        //qDebug() << min_pileup * atom_sep << D[d1].B1 << D[d2].B1;
                        //two large dislocations on adjacent planes opposite signs.
                        //swap for a ruck and tuck.
                        double size = (fabs(D[d1].B1) + fabs(D[d2].B1)) / atom_sep;
                        structure.Ruck_and_tuck(size * 1E10, D[d1].xd, (D[d1].y + D[d2].y) / 2);
                        //qDebug() << "ruck and tuck" << size;
                        //remove dislocations
                        D.removeAt(d2);
                        D.removeAt(d1);
                        d1--;
                        for (int l = 0; l < size; l++)
                        {
                            //structure.add_edge_material();
                        }

                    }
                }
            }
            else if ((D[d1].B == 1 && D[d2].B == 2 && D[d1].B1 >= (min_pileup * atom_sep)) && D[d2].B1 <= (-min_pileup * atom_sep))
            {
                //check if the dislocations are on adjacent planes
                if ((D[d1].y <= (1 - y_tol) * (D[d2].y - int_layer_sep)) && (D[d1].y >= (1 + y_tol) * (D[d2].y - int_layer_sep)))
                {
                    double xsep = D[d1].xd - D[d2].xd;
                    if (abs(xsep) < x_tol)
                    {
                        //qDebug() << min_pileup * atom_sep << D[d1].B1 << D[d2].B1;
                        //two large dislocations on adjacent planes opposite signs.
                        //swap for a ruck and tuck.
                        int size = (fabs(D[d1].B1) + fabs(D[d2].B1)) / atom_sep;
                        structure.Ruck_and_tuck(size * 1E10, D[d1].x, (D[d1].y + D[d2].y) / 2);
                        //qDebug() << "ruck and tuck" << size;
                        //remove dislocations
                        D.removeAt(d2);
                        D.removeAt(d1);
                        d1--;
                        for (int l = 0; l < size; l++)
                        {
                            //structure.add_edge_material();
                        }
                    }
                }


            }
        }
    }

}

void Interaction::check_for_ruck_tuck_growth() //checks if any basals are gliding over ruck and tuck.
{

    for (int d1 = 0; d1 < D.size(); d1++)
    {
        for (int d2 = 0; d2 < D.size(); d2++)
        {
            if (d1 != d2)
            {

                //qDebug() << D[d1].B << D[d2].B;
                if ((D[d1].B == 3 || D[d1].B == 4) && D[d2].B == 2)
                {
                    double xsep = D[d1].xd - D[d2].xd;
                    if (xsep < 0)
                    {
                        xsep = xsep * -1;
                    }

                    if (xsep < 100)
                    {
                        double ysep = D[d1].y - D[d2].y;
                        //if (ysep < 0)
                        //{
                        //    ysep = ysep * -1;
                        //}
                        //qDebug() << "d1.y" << D[d1].y << "d2.y" << D[d2].y << ysep << (0.5 * int_layer_sep);
                        if (ysep >= -0.6 * int_layer_sep && ysep <= -0.4 * int_layer_sep)
                        {
                            //qDebug() << " loop 3 1";
                            deposit_material(d1, d2);
                            climb_dislocation(d2);
                        }

                    }
                }
                else if ((D[d1].B == 3 || D[d1].B == 4) && D[d2].B == 1)
                {
                    double xsep = D[d1].xd - D[d2].xd;
                    if (xsep < 0)
                    {
                        xsep = xsep * -1;
                    }
                    //qDebug() << xsep;
                    if (xsep < 100)
                    {
                        double ysep = D[d1].y - D[d2].y;
                        //if (ysep < 0)
                        //{
                        //    ysep = ysep * -1;
                        //}
                        //qDebug() << "d1.y" << D[d1].y << "d2.y" << D[d2].y << ysep << (0.5 * int_layer_sep);
                        if (ysep <= 0.6 * int_layer_sep && ysep >= 0.4 * int_layer_sep)
                        {
                            //qDebug() << "loop 3 2";
                            deposit_material(d1, d2);
                            climb_dislocation(d2);
                        }

                    }
                }

            }
        }
    }
}

void Interaction::deposit_material(int d1, int d2)
{

    //if basal dislocation glides into the "ruck and tuck" and deposits material increase separation of prismatic
    int size = abs(D[d2].B1 / atom_sep);
    //qDebug() << "deposit" << size;
    if (D[d1].B == 3)
    {
        D[d1].x = D[d1].x - double(size * 0.5 * int_layer_sep);
        D[d1].xd = D[d1].x;
        //qDebug() << "delta x " << D[d1].x << double(size * 0.5 * atom_sep);
    }
    else if (D[d1].B == 4)
    {
        D[d1].x = D[d1].x + double(size * 0.5 * int_layer_sep);
        D[d1].xd = D[d1].x;
        //qDebug() << "delta x" << D[d1].x << double(size * 0.5 * atom_sep);
    }

}

void Interaction::climb_dislocation(int d)
{
    //qDebug() << "climb";
    if (D[d].B1 > 0)
    {
        D[d].y += int_layer_sep;
    }
    else if (D[d].B1 < 0)
    {
        D[d].y -= int_layer_sep;
    }

}

int Interaction::move_dislocation(int d1)
{
    for (int d2 = 0; d2 < D.size(); d2++)
    {
        if (D[d1].y == D[d2].y)
        {
            if ((D[d1].f > 0) && ((D[d2].x > D[d1].x) && (D[d2].x < (D[d1].x + D[d1].f))))
            {
                //qDebug() << "stopped jump";
                D[d1].x = D[d2].x - 0.5;
                Calc_interaction(d1, d2);
                return 0;
            }
            else if ((D[d1].f < 0) && ((D[d2].x < D[d1].x) && (D[d2].x > (D[d1].x + D[d1].f))))
            {
                //qDebug() << "stopped jump";
                D[d1].x = D[d2].x + 0.5;
                Calc_interaction(d1, d2);
                return 0;

            }
        }

    }
    D[d1].x += D[d1].f;
    return 0;
}


QList<int> Interaction::sort_list(QList<int> list)
{
    for (int j = 0; j < list.size() - 1; j++)
    {

        for (int i = j + 1; i < list.size(); i++)
        {
            int d1 = list.value(j);
            int d2 = list.value(i);
            double x1 = D[d1].x;
            double x2 = D[d2].x;
            if (x1 > x2)
            {
                list.removeAt(j);
                list << d1;
                j--;
                i = list.size();
            }
        }

    }

    return list;
}


int Interaction::check_dipole_size(int d1, int d2, int multi)
{
    UNUSED(multi);
    double tolerance = fabs(2 * D[d1].B1 * 1E10);
    QList<int> disl_list, sorted_disl_list;
    //qDebug() << D.size() << d1 << d2;
    for (int d = 0; d < D.size(); d++)
    {
        if (D[d].group == D[d1].group)
        {
            disl_list << d;
        }
    }


    //disl_list = sort_list(disl_list);
    for (int j = 0; j < disl_list.size() - 1; j++)
    {

        for (int i = j + 1; i < disl_list.size(); i++)
        {
            int dd1 = disl_list.value(j);
            int dd2 = disl_list.value(i);
            double x1 = D[dd1].x;
            double x2 = D[dd2].x;
            if (x1 > x2)
            {
                disl_list.removeAt(j);
                disl_list << dd1;
                j--;
                i = disl_list.size();
            }
        }

    }


    Find_min(disl_list);
    bool done = false;
    for (int d = 0; d < disl_list.size(); d++)
    {
        int i = disl_list.value(0);
        if (done == false)
        {
            if (D[i].min == false)
            {
                disl_list.removeFirst();
                disl_list << i;
            }
            else if (D[i].min == true)
            {
                done = true;
            }
        }
    }

    int min_3 = disl_list.value(0);
    int max_4 = disl_list.value(disl_list.size() - 1);

    //print_group_stats(disl_list);
    //qDebug() << min_3 << max_4;

    //any type 4's lower add width to their x pos.
    //from x of first type 3 to x+material width add one for a type 3, subtract 1 for a type 4.

    int size = int(material_width);
    int x_count[size];
    for (int x = 0; x < size; x++)
    {
        x_count[x] = 0;
    }

    //int h = 0;
    for (int i = 0; i < disl_list.size(); i++)
    {
        int disl = disl_list.value(i);
        int c = fabs(D[disl].B1 / basal_B);
        //h = c;
        //qDebug() << D[disl].B1 << D[disl].B1 / basal_B << "c=" << c;
        if (D[disl].B == 4 && D[disl].xd < D[min_3].xd)
        {
            for (int x = D[min_3].xd - material_width; x < D[min_3].xd; x++)
            {
                //subtract one
                if (D[disl].xd > x)
                {
                }
                else if (D[disl].xd <= x)
                {
                    int j = x - (D[min_3].xd - material_width);

                    x_count[j] -= c;
                }
            }
        }
        if (D[disl].B == 4 && D[disl].xd > D[min_3].xd)
        {
            for (int x = D[min_3].xd - material_width; x < D[min_3].xd; x++)
            {
                //subtract one
                if (D[disl].xd - material_width > x)
                {
                }
                else if (D[disl].xd - material_width <= x)
                {
                    int j = x - (D[min_3].xd - material_width);
                    x_count[j] -= c;
                }
            }
        }
        if (D[disl].B == 3)
        {
            for (int x = D[min_3].xd - material_width; x < D[min_3].xd; x++)
            {
                //add one
                if (D[disl].xd - material_width > x)
                {
                }
                else if (D[disl].xd - material_width <= x)
                {
                    int j = x - (D[min_3].xd - material_width);

                    x_count[j] += c;
                }
            }
        }
    }
    //count how many x positions have 0
    int countx = 0;
    int totalx = 0;
    for (int j = 0; j < size; j++)
    {
        //qDebug() << j << x_count[j];
        if (x_count[j] == 0)
        {
            countx++;
        }
        totalx += x_count[j];

    }
    //double average_x = totalx / double(size);
    //qDebug() << d1 << d2 << min_3 << max_4;
    //qDebug() << countx << tolerance;
    print_group_stats(disl_list);
    //if count<tolerance add material
    if (countx < tolerance)

    {
        //qDebug() << d1 << d2 << min_3 << max_4;
        //qDebug() << countx << tolerance;
        //print_group_stats(disl_list);


        if (((min_3 == d1) && (max_4 == d2)) || ((min_3 == d2) && (max_4 == d1)))
        {

            double min_3B, max_4B;
            min_3B = fabs(D[min_3].B1);
            max_4B = fabs(D[max_4].B1);

            if (min_3B == max_4B)
            {
                structure.Add_material_height(min_3B / basal_B);
                if (min_3 > max_4)
                {
                    D.removeAt(min_3);
                    D.removeAt(max_4);
                    return 2;
                }
                if (max_4 > min_3)
                {
                    D.removeAt(max_4);
                    D.removeAt(min_3);
                    return 2;
                }

            }
            else
            {
                if (min_3B > max_4B)
                {
                    structure.Add_material_height(max_4B / basal_B);
                    D[min_3].B1 = D[min_3].by = (D[min_3].B1 + D[max_4].B1);
                    D.removeAt(max_4);
                    return 1;
                }
                else if (min_3B < max_4B)
                {
                    structure.Add_material_height(min_3B / basal_B);
                    D[max_4].B1 = D[max_4].by = (D[min_3].B1 + D[max_4].B1);
                    D.removeAt(min_3);
                    return 1;
                }
            }
        }
    }

    return 0;
}

void Interaction::print_group_stats(QList<int> disl_index)
{
    UNUSED(disl_index);
    /*
    int i = disl_index[0];
    qDebug() << "group:" << D[i].group << "iteration:" << its << "size" << disl_index.size();
    for (int i = 0; i < disl_index.size(); i++)
    {
        int j = disl_index[i];
        qDebug() << j << D[j].x << D[j].xd << D[j].y << D[j].B << D[j].B1 << D[j].group << D[j].min;
    }
     */
}

void Interaction::print_group_stats(int g)
{
    UNUSED(g);
    /*
    QList<int> list;
    list.clear();
    for (int i = 0; i < D.size(); i++)
    {
        if (D[i].group == g)
        {
            list << i;
        }
    }
    print_group_stats(list);
     */
}


