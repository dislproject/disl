#ifndef ENERGY_HPP
#define ENERGY_HPP

class Energy
{

    typedef Eigen::Matrix<double, 3, 1> vectorDim;

private:

    double R; // separation between dislocations

public:

    double En; // energy
    double res; // result out

    Energy();

    // isotropic energy
    double Iso_en(int, int, double);

    // Self energy
    double Iso_self_en(int);


    void Calc_energy();
    void Calc_energy_test();

    double Ma_Iso_en(int d1, int d2, double x);


};

#endif // ENERGY_HPP
