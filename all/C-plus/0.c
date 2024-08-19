#include <stdio.h>  
  
// 假设的ReLU激活函数  
double relu(double x) {  
    return x > 0 ? x : 0;  
}  
  
// 计算神经元输出  
double neuron_output(double input, double weight, double bias) {  
    double linear_output = input * weight + bias;  
    return relu(linear_output);  
}  
  
// 计算均方误差损失函数  
double mse_loss(double target, double output) {  
    return 0.5 * (target - output) * (target - output);  
}  
  
// 计算损失函数关于权重的梯度（假设只有一个输入）  
double gradient_weight(double input, double target, double output, double weight) {  
    // 对于ReLU，如果输出非零（即输入为正），则梯度为输入；否则为0  
    // 但这里我们需要考虑目标值对权重的整体影响，因此使用链式法则  
    // 假设输出层之后没有更多层，直接连接到损失函数  
    double derivative_relu = (output > 0) ? 1.0 : 0.0;  
    return derivative_relu * (target - output) * input;  
}  
  
// 计算损失函数关于偏置的梯度  
double gradient_bias(double target, double output) {  
    // 对于偏置，其梯度计算与权重类似，但不需要乘以输入  
    double derivative_relu = (output > 0) ? 1.0 : 0.0;  
    return derivative_relu * (target - output);  
}  
  
// 使用梯度下降法更新权重和偏置  
void update_parameters(double *weight, double *bias, double input, double target, double learning_rate) {  
    double output = neuron_output(input, *weight, *bias);  
    double grad_w = gradient_weight(input, target, output, *weight);  
    double grad_b = gradient_bias(target, output);  
  
    *weight += learning_rate * grad_w;  
    *bias += learning_rate * grad_b;  
}  

int main() {  
    double weight = 0.1;  
    double bias = 0.1;  
    double input = 2.0; // 假设的输入值  
    double target = 1.0; // 我们想要神经元输出的目标值  
    double learning_rate = 0.001; // 学习率  
    int iterations = 1000; // 迭代次数  
  
    for (int i = 0; i < iterations; i++) {  
        update_parameters(&weight, &bias, input, target, learning_rate);  
  
        double output = neuron_output(input, weight, bias);  
        double loss = mse_loss(target, output);  
  
        printf("Iteration %d: Weight = %f, Bias = %f, Output = %f, Loss = %f\n",  
               i, weight, bias, output, loss);  
    }  
  
    return 0;  
}