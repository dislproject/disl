#include "externVar.h"

Strain_map::Strain_map()
{
}

void Strain_map::calculate_strainmap(int option)
{

    set_up_grid();

    if (option == 4 || option == 5 || option == 6)
    {
        stresses(option);
    }
    else if (option == 7 || option == 8 || option == 9)
    {
        strains(option);
    }
    colour_it();
}


void Strain_map::set_up_grid()
{
    //Grid is a 2 dimensional array of 1000x1000 elements, each with an x and y position and stress and strain values in x, y and net.

    //Loop over each element and calculate the x and y position of the element for rendering.
    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            grid[i][j].x = i * (material_width / 500) + 300;
            grid[i][j].y = j * (material_height / 500) + 120;
        }
    }
}


void Strain_map::strains(int opt)
{
    //for each element in the grid:
    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            //cumulatively add the strain due to each dislocation in the system
            for (int d = 0; d < D.size(); d++)
            {
                //Calculate the strain in x and add to existing
                grid[i][j].strain_x += 0;
                //calculate the strain in y and add to existing
                grid[i][j].strain_y += 0;
            }

            //calculate net strain
            grid[i][j].strain_net = pow(0.5, ((grid[i][j].strain_x * grid[i][j].strain_x) + (grid[i][j].strain_y * grid[i][j].strain_y)));

            //store in grid for colouring
            if (opt == 7)
            {
                grid[i][j].val = grid[i][j].strain_net;
            }
            if (opt == 8)
            {
                grid[i][j].val = grid[i][j].strain_x;
            }
            if (opt == 9)
            {
                grid[i][j].val = grid[i][j].strain_y;
            }
        }
    }
}


void Strain_map::stresses(int opt)
{

    //for each element in the grid:
    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            grid[i][j].stress_x = grid[i][j].stress_y = 0;
            //cumulatively add the stress due to each dislocation in the system
            for (int d = 0; d < D.size(); d++)
            {
                double dy = (grid[i][j].y - D[d].y) * (1E-10);
                double dx = (grid[i][j].x - D[d].x) * (1E-10);

                //Calculate the stress in x and add to existing
                grid[i][j].stress_x += (A1 * A7 * D[d].bx * (-dx * dx * dy + A2 * dy * dy * dy)) / (pow(dx * dx + A2 * dy * dy, 2) + A3 * dx * dx * dy * dy) + A1 * D[d].by * (A4 * dy * dy * dx + A5 * dx * dx * dx) / (A6 * pow((A2 * dy * dy + dx * dx), 2));
                //calculate the stress in y and add to existing
                grid[i][j].stress_y += (A1 * A7 * D[d].by * (-dx * dx * dx + A2 * dx * dy * dy)) / (pow(dx * dx + A2 * dy * dy, 2) + A3 * dx * dx * dy * dy) + A1 * D[d].bx * (A4 * dy * dx * dx + A2 * A6 * A7 * dy * dy * dy) / (A6 * pow((A2 * dy * dy + dx * dx), 2));


            }

            grid[i][j].stress_x = grid[i][j].stress_x / 1E9;
            grid[i][j].stress_y = grid[i][j].stress_y / 1E9;

            //calculate net stress
            grid[i][j].stress_net = sqrt((grid[i][j].stress_x * grid[i][j].stress_x) + (grid[i][j].stress_y * grid[i][j].stress_y));

            //store in grid for colouring

            if (opt == 4)
            {
                grid[i][j].val = grid[i][j].stress_net;
            }
            if (opt == 5)
            {
                grid[i][j].val = grid[i][j].stress_x;
            }
            if (opt == 6)
            {
                grid[i][j].val = grid[i][j].stress_y;
            }


            //grid[i][j].val = rand() % 200 - 100;
        }
    }
}

void Strain_map::colour_it()
{
    //once all values of the stress or strain have been calculated they need to be applied to
    //a scale. There are 21 different colours ranging from -10 to +10 (orange through white to blue)

    //first find the largest value in the grid to set as the darkest colour.
    double max_s = 0;
    double old_max[250000];
    int k = 0;
    double average = 0;
    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            if (fabs(grid[i][j].val) > max_s)
            {
                k++;
                for (int l = k; l > 0; l--)
                {
                    old_max[l] = old_max[l - 1];
                }
                old_max[0] = max_s;
                max_s = fabs(grid[i][j].val);

            }
            average += grid[i][j].val;
        }
    }


    average = average / (500 * 500);
    double sd = 0;
    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            sd += (grid[i][j].val - average) * (grid[i][j].val - average);
        }
    }
    sd = sqrt(sd / 250000);

    for (int m = 0; m < k; m++)
    {
        if (old_max[m] > average + 2 * sd)
        {
            max_s = old_max[m + 1];
        }
    }


    double bin_width = max_s / 10.5;
    //qDebug() << max_s << bin_width;

    for (int i = 0; i < 500; i++)
    {
        for (int j = 0; j < 500; j++)
        {
            //qDebug() << i << j;
            //qDebug() << grid[i][j].val;
            grid[i][j].colour = grid[i][j].val / bin_width;
            if (grid[i][j].colour > 10)
            {
                grid[i][j].colour = 10;
            }
            if (grid[i][j].colour < -10)
            {
                grid[i][j].colour = -10;
            }
            //if (grid[i][j].colour != 0)
            //{
            //    qDebug() << grid[i][j].val / bin_width << grid[i][j].val << grid[i][j].colour;
            //}
        }
    }

}
