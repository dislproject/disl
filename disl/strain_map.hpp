#ifndef STRAIN_MAP_HPP
#define STRAIN_MAP_HPP

class Strain_map
{

public:
    Strain_map();
    void calculate_strainmap(int option);
    void set_up_grid();
    void strains(int opt);
    void stresses(int opt);
    void colour_it();
};

#endif // STRAIN_MAP_HPP
