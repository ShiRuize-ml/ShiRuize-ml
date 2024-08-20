#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> // 包含 math.h 以使用 fmax 函数

#define INPUT_SIZE 3
#define HIDDEN1_SIZE 4
#define HIDDEN2_SIZE 3
#define OUTPUT_SIZE 2

typedef struct {
    double **weights; // 权重矩阵，二维数组
    double *bias;     // 偏置向量，一维数组
    int input_size;   // 输入尺寸
    int output_size;  // 输出尺寸
} Layer;

// 激活函数ReLU
double relu(double x) {
    return fmax(x, 0);
}

// ReLU函数的导数
double relu_derivative(double x) {
    return x > 0 ? 1.0 : 0.0;
}

// 矩阵乘法
void mat_mul(double **a, double *b, double *c, int m, int n) {
    for (int i = 0; i < m; i++) {
        c[i] = 0;
        for (int k = 0; k < n; k++) { // 注意这里应该是 n，因为b是列向量
            c[i] += a[i][k] * b[k];
        }
    }
}

// 均方误差损失函数
double mse_loss(double predicted, double actual) {
    return (predicted - actual) * (predicted - actual);
}

// 创建层
Layer create_layer(int input_size, int output_size) {
    Layer l;
    l.input_size = input_size;
    l.output_size = output_size;
    l.weights = (double **)malloc(output_size * sizeof(double *));
    for (int i = 0; i < output_size; i++) {
        l.weights[i] = (double *)malloc(input_size * sizeof(double));
        for (int j = 0; j < input_size; j++) {
            l.weights[i][j] = (double)(rand() / RAND_MAX) * 0.2 - 0.1; // 初始化权重
        }
    }
    l.bias = (double *)malloc(output_size * sizeof(double));
    for (int i = 0; i < output_size; i++) {
        l.bias[i] = 0; // 初始化偏置
    }
    return l;
}

// 释放层资源
void free_layer(Layer *l) {
    for (int i = 0; i < l->output_size; i++) {
        free(l->weights[i]);
    }
    free(l->weights);
    free(l->bias);
}

// 前向传播
void forward_pass(Layer *layers, int num_layers, double *input, double ***activations) {
    *activations = (double **)malloc((num_layers + 1) * sizeof(double *));
    (*activations)[0] = input;
    for (int i = 1; i <= num_layers; i++) {
        (*activations)[i] = (double *)malloc(layers[i - 1].output_size * sizeof(double));
    }

    double *prev_activations = input;
    for (int i = 0; i < num_layers; i++) {
        double *current_activations = (*activations)[i + 1];
        mat_mul(layers[i].weights, prev_activations, current_activations, layers[i].output_size, layers[i].input_size);
        for (int j = 0; j < layers[i].output_size; j++) {
            current_activations[j] = relu(current_activations[j] + layers[i].bias[j]);
        }
        prev_activations = current_activations;
    }
}

// 反向传播
void backpropagation(Layer *layers, int num_layers, double **activations, double *target, double learning_rate) {
    // 内存分配给delta和d_weights
    double **delta = (double **)malloc(num_layers * sizeof(double *));
    double ***d_weights = (double ***)malloc(num_layers * sizeof(double **));
    double *d_biases = (double *)malloc(num_layers * sizeof(double));

    for (int i = 0; i < num_layers; i++) {
        delta[i] = (double *)malloc(layers[i].output_size * sizeof(double));
        d_weights[i] = (double **)malloc(layers[i].output_size * sizeof(double *));
        for (int j = 0; j < layers[i].output_size; j++) {
            d_weights[i][j] = (double *)calloc(layers[i].input_size, sizeof(double));
        }
        d_biases[i] = 0;
    }

    // 计算输出层的delta和d_weights
    for (int i = 0; i < layers[num_layers - 1].output_size; i++) {
        delta[num_layers - 1][i] = relu_derivative(activations[num_layers][i]) * (activations[num_layers][i] - target[i]);
        d_biases[num_layers - 1] += delta[num_layers - 1][i];
        for (int k = 0; k < layers[num_layers - 1].input_size; k++) {
            d_weights[num_layers - 1][i][k] += delta[num_layers - 1][i] * activations[num_layers - 1][k];
        }
    }

    // 反向传播计算delta和d_weights
    for (int i = num_layers - 2; i >= 0; i--) {
        for (int j = 0; j < layers[i].output_size; j++) {
            double error = 0;
            for (int k = 0; k < layers[i + 1].output_size; k++) {
                error += delta[i + 1][k] * layers[i + 1].weights[k][j];
            }
            delta[i][j] = relu_derivative(activations[i + 1][j]) * error; // activations索引需要调整
            d_biases[i] += delta[i][j];
            for (int k = 0; k < layers[i].input_size; k++) {
                d_weights[i][j][k] += delta[i][j] * activations[i][k];
            }
        }
    }

    // 更新权重和偏置
    for (int i = 0; i < num_layers; i++) {
        for (int j = 0; j < layers[i].output_size; j++) {
            for (int k = 0; k < layers[i].input_size; k++) {
                layers[i].weights[j][k] -= learning_rate * d_weights[i][j][k];
            }
            layers[i].bias[j] -= learning_rate * d_biases[i];
        }
    }

    // 清理分配的内存
    for (int i = 0; i < num_layers; i++) {
        free(delta[i]);
        for (int j = 0; j < layers[i].output_size; j++) {
            free(d_weights[i][j]);
        }
        free(d_weights[i]);
    }
    free(delta);
    free(d_weights);
    free(d_biases);
}

int main() {
    srand(time(NULL));

    Layer layers[3];
    layers[0] = create_layer(INPUT_SIZE, HIDDEN1_SIZE);
    layers[1] = create_layer(HIDDEN1_SIZE, HIDDEN2_SIZE);
    layers[2] = create_layer(HIDDEN2_SIZE, OUTPUT_SIZE);

    double input[INPUT_SIZE] = {0.5, -0.2, 0.1};
    double target[OUTPUT_SIZE] = {0.4, -0.6};
    double **activations = NULL;
    double learning_rate = 0.01;

    // 训练模型
    for (int epoch = 0; epoch < 1000; epoch++) {
        forward_pass(layers, 3, input, &activations);
        backpropagation(layers, 3, activations, target, learning_rate);
        if (epoch % 100 == 0) { // 每100个epoch打印一次结果
            printf("Epoch %d, Output: %f, %f\n", epoch, activations[3][0], activations[3][1]);
        }
        // 清理activations内存
        for (int i = 1; i <= 3; i++) {
            if (activations[i] != NULL) {
                free(activations[i]);
            }
        }
        free(activations);
    }

    // 清理层资源
    for (int i = 0; i < 3; i++) {
        free_layer(&layers[i]);
    }

    return 0;
}
