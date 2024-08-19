#include <stdio.h>  
#include <stdlib.h>  
  
#define INPUT_SIZE 1  
#define HIDDEN_SIZE 2  
#define OUTPUT_SIZE 1  
  
typedef struct {  
    double *weights;  
    double *bias;  
    int size;  
} Layer;  
  
Layer create_layer(int size) {  
    Layer layer;  
    layer.size = size;  
    layer.weights = malloc(size * sizeof(double));  
    layer.bias = malloc(size * sizeof(double));  
    for (int i = 0; i < size; i++) {  
        layer.weights[i] = 0.1; // 简单的初始化  
        layer.bias[i] = 0.1;  
    }  
    return layer;  
}  
  
void free_layer(Layer *layer) {  
    free(layer->weights);  
    free(layer->bias);  
}  
  
// 假设的激活函数和导数  
double relu(double x) { return x > 0 ? x : 0; }  
double relu_derivative(double x) { return x > 0 ? 1.0 : 0.0; }  
  
// 前向传播  
void forward_pass(double input, Layer hidden, Layer output, double *hidden_output, double *final_output) {  
    // 隐藏层  
    for (int i = 0; i < HIDDEN_SIZE; i++) {  
        hidden_output[i] = relu(input * hidden.weights[i] + hidden.bias[i]);  
    }  
  
    // 输出层（简单示例，只取隐藏层的一个输出）  
    *final_output = relu(hidden_output[0] * output.weights[0] + output.bias[0]);  
}  
  
// 反向传播（简化）  
void backpropagation(double input, double target, double *hidden_output, double final_output, Layer *hidden, Layer *output, double learning_rate) {  
    double output_error = target - final_output;  
    double hidden_error = output_error * output->weights[0] * relu_derivative(hidden_output[0]);  
  
    // 更新输出层  
    output->weights[0] += learning_rate * output_error * hidden_output[0];  
    output->bias[0] += learning_rate * output_error;  
  
    // 更新隐藏层（这里只考虑了一个输出神经元对隐藏层的影响）  
    for (int i = 0; i < HIDDEN_SIZE; i++) {  
        if (i == 0) { // 假设我们只使用隐藏层的第一个输出  
            hidden->weights[i] += learning_rate * hidden_error * input;  
            hidden->bias[i] += learning_rate * hidden_error;  
        }  
    }  
}  
  
int main() {  
    Layer hidden = create_layer(HIDDEN_SIZE);  
    Layer output = create_layer(OUTPUT_SIZE);  
  
    double input = 1.0;  
    double target = 2.0;  
    double learning_rate = 0.0001;  
    int iterations = 2000;  
  
    double *hidden_output = malloc(HIDDEN_SIZE * sizeof(double));  
    double final_output;  
  
    for (int i = 0; i < iterations; i++) {
        input = 0.9;
        target = 1.0; 
        forward_pass(input, hidden, output, hidden_output, &final_output);  
        backpropagation(input, target, hidden_output, final_output, &hidden, &output, learning_rate);
        printf("Iteration %d: Final Output = %f\n", i, final_output); 
        input = 1.0;
        target = 2.0;
        forward_pass(input, hidden, output, hidden_output, &final_output);  
        backpropagation(input, target, hidden_output, final_output, &hidden, &output, learning_rate);
        printf("  Iteration %d: Final Output = %f\n", i, final_output);  
    }  
  
    free(hidden_output);  
    free_layer(&hidden);  
    free_layer(&output);  
  
    return 0;  
}