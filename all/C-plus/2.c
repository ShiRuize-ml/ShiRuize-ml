#include <stdio.h>  
#include <stdlib.h>  
#define INPUT_SIZE 1 
#define HIDDEN1_SIZE 2  
#define HIDDEN2_SIZE 2  
#define OUTPUT_SIZE 1  
typedef struct {  
    double *weights;  
    double *bias;  
    int size;  
} Layer;  
Layer create_layer(int size) {  
    Layer l;  
    l.size = size;  
    l.weights = malloc(size * sizeof(double));  
    l.bias = malloc(size * sizeof(double));  
    for (int i = 0; i < size; i++) {  
        l.weights[i] = 0.1;  
        l.bias[i] = 0.1;  
    }  
    return l;  
}  
void free_layer(Layer *l) {  
    free(l->weights);  
    free(l->bias);  
}  
double relu(double x) { return x > 0 ? x : 0; }  
double relu_derivative(double x) { return x > 0 ? 1.0 : 0.0; }  
void forward_pass(double input, Layer h1, Layer h2, Layer out, double *h1_out, double *h2_out, double *final_out) {  
    for (int i = 0; i < HIDDEN1_SIZE; i++) h1_out[i] = relu(input * h1.weights[i] + h1.bias[i]);  
    for (int i = 0; i < HIDDEN2_SIZE; i++) h2_out[i] = relu(h1_out[0] * h2.weights[i] + h2.bias[i]); // Simple for example  
    *final_out = relu(h2_out[0] * out.weights[0] + out.bias[0]);  
}

void backpropagation(double input, double target, double *h1_out, double *h2_out, double final_out, Layer *h1, Layer *h2, Layer *out, double lr) {  
    double out_err = target - final_out;  
    double h2_err = out_err * out->weights[0] * relu_derivative(h2_out[0]);  
    double h1_err = h2_err * h2->weights[0] * relu_derivative(h1_out[0]);  
    out->weights[0] += lr * out_err * h2_out[0];  
    out->bias[0] += lr * out_err;  
    h2->weights[0] += lr * h2_err * h1_out[0];  
    h2->bias[0] += lr * h2_err;  
    h1->weights[0] += lr * h1_err * input;  
    h1->bias[0] += lr * h1_err;  
    // 忽略其他权重和偏置更新，假设简单网络  
}  

int main() {  
    Layer h1 = create_layer(HIDDEN1_SIZE), h2 = create_layer(HIDDEN2_SIZE), out = create_layer(OUTPUT_SIZE);  
    double input = 1.0, target = 2.0, lr = 0.0001;  
    int iter = 2000;  
    double *h1_out = malloc(HIDDEN1_SIZE * sizeof(double)), *h2_out = malloc(HIDDEN2_SIZE * sizeof(double));  
    double final_out;  
    for (int i = 0; i < iter; i++) {  
        forward_pass(input, h1, h2, out, h1_out, h2_out, &final_out);  
        backpropagation(input, target, h1_out, h2_out, final_out, &h1, &h2, &out, lr);  
        printf("Iter %d: Output=%f\n", i, final_out);  
    }  
    free(h1_out); free(h2_out);  
    free_layer(&h1); free_layer(&h2); free_layer(&out);
}