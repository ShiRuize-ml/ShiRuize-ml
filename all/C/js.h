#include <stdio.h>
#include <stdlib.h>
/**********暂停使用*******
double fc(double *data, int length) {  
    if (length <= 1) {  
        return 0.0;  
    }  
    double sum = 0.0;  
    double mean = 0.0;  
    for (int i = 0; i < length; i++) {  
        sum += data[i];  
    }  
    mean = sum / length;  
    double sumOfSquares = 0.0;  
    for (int i = 0; i < length; i++) {  
        sumOfSquares += (data[i] - mean) * (data[i] - mean);  
    }   
    return sumOfSquares / (length - 1);  
}  
*/

/*Ols
double hs(int inlong, double *in_ptr, int wlong, double *w_ptr, double b){
    double *in_ptr_mem = in_ptr;
    double *w_ptr_mem = w_ptr;
    double in[inlong];
    double w[wlong];
    for (int i = 0; i < inlong; i++){
        in[i] = *in_ptr_mem;
        in_ptr_mem++;
        w[i] = *w_ptr_mem;
        w_ptr_mem++;
    }
    double out = 0.0;
    for (int i = 0; i < inlong; i++){
        out = out + (in[i] * w[i]);
    }
    out = out + b;
    if (out > 0)
    return out;
    else
    return 0;
}
*/

double hs(int inlong, double *in_ptr, int wlong, double *w_ptr, double b){//RELU 函数
    double out = 0;
    for(int a = 0;a < inlong;a++) {out += in_ptr[a] * w_ptr[a];}
    out += b;
    return out > 0 ? out : 0;
}
//报废
//double loss(double out,double true_out){return 0.5*(true_out-out)*(true_out-out);}