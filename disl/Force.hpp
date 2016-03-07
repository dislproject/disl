#ifndef FORCE_HPP
#define FORCE_HPP

class Force
{


public:
    void move_basals();
    void calc_total_diff_x(int);
    double calc_diff_d1_d2(int, int, double);
    void move_pris();
    void move_pris2();
    Force();
    double calc_f_d1_d2(int d1, int d2, double x2);
    void confirm_diff(int);
    double appStress(int);
    void inplane_forces();

};


#endif // FORCE_HPP
