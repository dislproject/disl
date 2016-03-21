#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

class Structure
{

private:
    int i, j;

public:

    Structure();
    void Build_Di(double Dip_posn[], double Dip_posn_y[]);
    void Build_Di();
    void Build_Square_Ed(Edge E[]);
    void Random_position();
    void Random_position_inplane(int);
    void Add_new_pair();
    void Build_images();
    void prismatic_contraction(int d);
    void remove_edge_material();
    void add_edge_material();
    void Add_new_grid_pair();
    void Ruck_and_tuck(int, double xpos, double ypos);
    double pos_y, pos_x;
    int sep;
    void Add_material_height();
    void Add_material_height(double);
    void calculate_edge_strains();
    int random_plane();
    double random_x();
    double random_y();
    double random_inplane_h();
    double random_inplane_a_x();
    double random_inplane_a_y();
    void Set_positions(int);
    void Add_test_basal();
    void Add_nonBasals();
    void Inplane_rounding();
};

#endif // STRUCTURE_HPP
