#include "fraction.h"

Fraction::Fraction()
{

}

fract Fraction::sum(fract frst, fract scnd, bool p_m){
    short int flag = check_null(frst, scnd);
    if(p_m){ scnd.u_num *= -1; }
    if(flag == 1){
        return scnd;
    } else if(flag == 2){
        return frst;
    } else if(flag == 0){
        return frst;
    }
    fract answer;
    answer.d_num = frst.d_num * scnd.d_num;

    answer.u_num = frst.u_num * scnd.d_num + scnd.u_num * frst.d_num;
    return reduct(answer);
}
fract Fraction::mult(fract frst, fract scnd, bool d_m){
    short int flag = check_null(frst, scnd);
    if(flag == 1){
        return frst;
    } else if(flag == 2){
        if(!d_m){
            return scnd;
        } else {
            qDebug() << "Division on null!";
            exit(0);
        }

    } else if(flag == 0){
        if(!d_m){
            return frst;
        } else {
            qDebug() << "Division on null!";
            exit(0);
        }
    }
    fract answer;
    if(d_m){ qSwap(scnd.u_num, scnd.d_num); }
    answer.u_num = frst.u_num * scnd.u_num;
    answer.d_num = frst.d_num * scnd.d_num;
    return reduct(answer);
}
fract Fraction::reduct(fract f){
    int a, b;
    if(f.d_num < 0 && f.u_num < 0){
        f.d_num *= -1;
        f.u_num *= -1;
    }
    a = qAbs(f.u_num);
    b = qAbs(f.d_num);
    while (a && b){
        if (a > b) a%=b;
        else b%=a;
    }
    f.u_num /= (a+b);
    f.d_num /= (a+b);
    return f;
}

short int Fraction::check_null(fract frst, fract scnd){
    short int flag;
    if(frst.d_num == 0 || scnd.d_num == 0){
        qDebug() << "Null in the d_num!";
        exit(1);
    }
    if(frst.u_num == 0 && scnd.u_num != 0){
        flag = 1;
    } else if(frst.u_num != 0 && scnd.u_num == 0){
        flag = 2;
    } else if(frst.u_num == 0 && scnd.u_num == 0){
        flag = 0;
    } else {
        flag = -1;
    }
    return flag;
}
