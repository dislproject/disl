#ifndef GRID_HPP
#define GRID_HPP

class Grid
{

public:
    Grid();
    double x, y, stress_x, stress_y, stress_net, strain_x, strain_y, strain_net, val;
    int colour;
};

#endif // GRID_HPP
