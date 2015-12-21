#ifndef INTERACTION_H
#define INTERACTION_H


class Interaction
{

typedef Eigen::Matrix<double,3,1> vectorDim;

public:
    Interaction();
    bool check_position(double x, double y);
    void check_new_interactions();
    void check_all_interactions();
    int Calc_interaction(int, int);
    int Calc_interaction(int, int, int, int);
    int reRun_interaction();
    void check_edges();
    double calc_force(int, int);
    double calc_image_force(int, int);
    double calc_image_image_force(int, int);
    double calc_disl_image_force(int, int);
    double R;
    void check_for_cracks();
    void check_for_ruckandtuck();
    void climb_dislocation(int);
    void deposit_material(int,int);
    void check_for_ruck_tuck_growth();
    int move_dislocation(int);
    double calc_netburg();
    bool lone_dislocations();
    bool net_burg_cons();
    int check_dipole_size(int d1, int d2, int multi);
    void print_group_stats(QList<int> disl_index);
    void print_group_stats(int g);
    void Re_group(int,int);
    int restructure_group(int g);
    void Find_min(QList<int> sorted_list);
    QList<int> sort_list(QList<int> list);
    void cons_vol(int d1, int d2);
};

#endif // INTERACTION_H
