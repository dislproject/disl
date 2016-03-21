#ifndef TEST_HPP
#define TEST_HPP

class Test
{
public:
    int line_test();
    void convergence_test(int density);
    void add_dipole(int pos_x, int pos_y, int sep);
    void negative_test_dist();
    void negative_test_dense();
    Test();
};

#endif // TEST_HPP
