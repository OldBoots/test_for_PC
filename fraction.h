#ifndef FRACTION_H
#define FRACTION_H
#include <QtMath>
#include <QDebug>

struct fract{
    int u_num;
    int d_num;
};

class Fraction
{
public:
    Fraction();
    fract sum(fract frst, fract scnd, bool p_m = 0);
    fract mult(fract frst, fract scnd, bool d_m = 0);
private:
    fract reduct(fract);
    short int check_null(fract frst, fract scnd);

};

#endif // FRACTION_H
