#include <QCoreApplication>
#include "fraction.h"
#include <iostream>
#include <QFile>
using namespace std;

void read_matrix(QVector<QVector<fract>>& matrix){
    QFile file(":/matrix.txt");
    if(!file.size()){
        qDebug() << "File is empty!";
        exit(0);
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        while(!file.atEnd()){
            QString lineStr = file.readLine();
            QStringList elMat = lineStr.split("\t");
            QVector<fract> lineEl;
            for(int i=0; i<elMat.size(); i++){
                fract oneEl;
                oneEl.u_num = elMat.at(i).toInt();
                oneEl.d_num = 1;
                lineEl.push_back(oneEl);
            }
            matrix.push_back(lineEl);
        }
    }
}

void cout_matrix(QVector<QVector<fract>> matrix){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j <matrix[0].size(); j++){
            if(matrix[i][j].d_num == 1){
                cout<<matrix[i][j].u_num<<"\t";
            }else
                cout<<matrix[i][j].u_num<<"/"<<matrix[i][j].d_num<<"\t";
        }
        cout << endl << endl;
    }
    cout << endl;
}

struct aux_struct{
    fract f;
    int index;
};

bool comparison(fract frst, fract scnd){
    frst.u_num = scnd.d_num * frst.u_num;
    scnd.u_num = frst.d_num * scnd.u_num;
    if(abs(frst.u_num) < abs(scnd.u_num)){ //?
        return true;
    } else {
        return false;
    }
}

void sort_bubble(QVector <aux_struct>& row_num){
    for(int i = 0; i < row_num.size(); i++){
        for(int j = 0; j < row_num.size() - i - 1; j++){
            if(comparison(row_num[j].f, row_num[j + 1].f)){
                qSwap(row_num[j], row_num[j + 1]);
            }
        }
    }
}

void sort_row(QVector<QVector<fract>>& matrix, int yi, int xi){
    int row = matrix.size();
    aux_struct as;
    QVector <aux_struct> row_num;
    QVector<QVector<fract>> dop_mat = matrix;
    for(int i = yi; i < row; i++){
        as.f = matrix[i][xi];
        as.index = i;
        row_num << as;
    }
    sort_bubble(row_num);
    for(int i = yi, j = 0; i < row; i++, j++){
        matrix[i] = dop_mat[row_num[j].index];
    }
}

bool check_line_forward(QVector<fract> row){
    int cnt = 0;
    bool flag = true;
    for(int i = 0; i < row.size(); i++){
        if(row[i].u_num != 0){
            cnt++;
            flag = false;
        }
    }
    if(cnt == 1 && row[row.size() - 1].u_num != 0){
        qDebug() << "There are no solutions!";
        exit(0);
    }
    return flag;
}

bool check_line_reverse(QVector<fract> row){
    int cnt = 0;
    bool flag = true;
    for(int i = row.size() - 1; i >= 0; i--){
        if(row[i].u_num != 0){
            cnt++;
            flag = false;
        }
    }
    if(cnt == 1 && row[0].u_num != 0){
        qDebug() << "There are no solutions!";
        exit(0);
    }
    return flag;
}

void mirror(QVector<QVector<fract>>& matrix){
    int n_row = matrix.size();
    int n_col = matrix[0].size();
    int cnt = 0;
    for(int i = 0; i < n_row; i++){
        for(int j = 0; j < n_col; j++){
            cnt++;
            if(cnt <= (n_col * n_row) / 2){
                qSwap(matrix[i][j], matrix[n_row - i - 1][n_col - j - 1]);
            }
        }
    }
}

void unit_matrix_forward(QVector<QVector<fract>>& matrix, int& f_col){
    Fraction obj;
    int n_row = matrix.size();
    int n_col = matrix[0].size();
    fract f_num, diag_num;
    // forward
    for(int yi = 0, xi = 0; yi < n_row; yi++, xi++){
        sort_row(matrix, yi, xi);
        cout_matrix(matrix);
        diag_num = matrix[yi][xi];
        if(diag_num.u_num == 0){
            if(check_line_forward(matrix[yi])){
                if(xi < n_col - 1 && yi < n_row - 1){
                    xi++;
                    yi++;
                } else {
                    return;
                }
                diag_num = matrix[yi][xi];
                while(check_line_forward(matrix[yi])){
                    xi++;
                    yi++;
                    diag_num = matrix[yi][xi];
                }
            } else {
                while(diag_num.u_num == 0){
                    xi++;
                    sort_row(matrix, yi, xi);
                    cout_matrix(matrix);
                    diag_num = matrix[yi][xi];
                }
            }
        }
        for(int i = yi; i < n_col; i++){
            matrix[yi][i] = obj.mult(matrix[yi][i], diag_num, 1); //str(i) / A(xi)
        }
        for(int i = yi + 1; i < n_row; i++){
            f_num = matrix[i][xi];
            for(int j = xi; j < n_col; j++){
                matrix[i][j] = obj.sum(matrix[i][j], obj.mult(matrix[yi][j],f_num), 1);
            }
        }
        f_col = xi;
    }
}
void unit_matrix_reverse(QVector<QVector<fract>>& matrix){
    Fraction obj;
    int n_row = matrix.size();
    int n_col = matrix[0].size();
    int r_col = n_col - 1;
    fract f_num, diag_num;
    for(int yi = n_row-1-1, xi = r_col-1; yi >= 0; yi--, xi--){
        diag_num = matrix[yi][xi];
        qDebug("1");
        if(diag_num.u_num != 1){
            qDebug("2");
            if(check_line_reverse(matrix[yi])){
                if(xi > 0 && yi < 0){
                    xi--;
                    yi--;
                } else {
                    return;
                }
                diag_num = matrix[yi][xi];
                while(check_line_reverse(matrix[yi])){
                    xi--;
                    yi--;
                    diag_num = matrix[yi][xi];
                }
            } else {
                xi--;
                diag_num = matrix[yi][xi];
                while(diag_num.u_num != 1){
                    xi--;
                    diag_num = matrix[yi][xi];
                }
            }
        }
        qDebug("3");
        int count=1;
        for(int i = yi + 1; i < n_row; i++){
            f_num = matrix[i][xi];
            cout<<"f_num"<<f_num.u_num<<" "<<i<<" "<<xi<<endl;
            cout_matrix(matrix);
            for(int j = xi; j >= 0; j--){
                matrix[i][j] = obj.sum(matrix[i][j], obj.mult(matrix[i - count][j], f_num), 1);
            }
            count++;
        }
        qDebug("4");

    }
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int f_col = 0;
    QVector<QVector<fract>> matrix;
    read_matrix(matrix);
    cout_matrix(matrix);
    unit_matrix_forward(matrix, f_col);
    cout_matrix(matrix);
    mirror(matrix);
    cout_matrix(matrix);
    unit_matrix_reverse(matrix);
    cout_matrix(matrix);
    cout<<"end"<<endl;
    return a.exec();
}
