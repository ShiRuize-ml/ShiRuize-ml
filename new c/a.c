#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
            l.weights[i][j] = fabs((double)(rand() / (double)RAND_MAX) * 0.2 - 0.1); // 初始化权重为正数
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
    double **d_weights = (double **)malloc(num_layers * sizeof(double *)); // 修正这里
    double *d_biases = (double *)malloc(num_layers * sizeof(double));

    for (int i = 0; i < num_layers; i++) {
        delta[i] = (double *)malloc(layers[i].output_size * sizeof(double));
        d_weights[i] = (double *)malloc(layers[i].input_size * layers[i].output_size * sizeof(double)); // 修正这里
        d_biases[i] = 0;
    }

    // 计算输出层的delta和d_weights
    for (int i = 0; i < layers[num_layers - 1].output_size; i++) {
        delta[num_layers - 1][i] = relu_derivative(activations[num_layers][i]) * (activations[num_layers][i] - target[i]);
        for (int k = 0; k < layers[num_layers - 1].input_size; k++) {
            int index = i * layers[num_layers - 1].input_size + k; // 计算d_weights的正确索引
            d_weights[num_layers - 1][index] += delta[num_layers - 1][i] * activations[num_layers - 1][k];
        }
        d_biases[num_layers - 1] += delta[num_layers - 1][i];
    }

    // 反向传播计算delta和d_weights
    for (int i = num_layers - 2; i >= 0; i--) {
        for (int j = 0; j < layers[i].output_size; j++) {
            double error = 0;
            for (int k = 0; k < layers[i + 1].output_size; k++) {
                error += delta[i + 1][k] * layers[i + 1].weights[k][j];
            }
            delta[i][j] = relu_derivative(activations[i + 1][j]) * error; // activations索引需要调整
            for (int k = 0; k < layers[i].input_size; k++) {
                int index = j * layers[i].input_size + k; // 计算d_weights的正确索引
                d_weights[i][index] += delta[i][j] * activations[i][k];
            }
            d_biases[i] += delta[i][j];
        }
    }

    // 更新权重和偏置
    for (int i = 0; i < num_layers; i++) {
        for (int j = 0; j < layers[i].output_size; j++) {
            for (int k = 0; k < layers[i].input_size; k++) {
                int index = j * layers[i].input_size + k; // 计算d_weights的正确索引
                layers[i].weights[j][k] = fmax(0, layers[i].weights[j][k] - learning_rate * d_weights[i][index]); // 确保权重不为负数
            }
            layers[i].bias[j] -= learning_rate * d_biases[i];
        }
    }

    // 清理分配的内存
    for (int i = 0; i < num_layers; i++) {
        free(delta[i]);
        free(d_weights[i]); // 修正这里
    }
    free(delta);
    free(d_weights); // 修正这里
    free(d_biases);
}

// 保存模型
void save_model(Layer *layers, int num_layers, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "cannor open file %s for writing\n", filename);
        return;
    }

    // 写入层数
    fwrite(&num_layers, sizeof(int), 1, file);

    // 写入每一层的参数
    for (int i = 0; i < num_layers; i++) {
        fwrite(&layers[i].input_size, sizeof(int), 1, file);
        fwrite(&layers[i].output_size, sizeof(int), 1, file);
        for (int j = 0; j < layers[i].output_size; j++) {
            fwrite(layers[i].weights[j], sizeof(double), layers[i].input_size, file);
        }
        fwrite(layers[i].bias, sizeof(double), layers[i].output_size, file);
    }

    fclose(file);
}

// 加载模型
void load_model(Layer **layers, int *num_layers, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "cannot open file %s for reading\n", filename);
        return;
    }

    // 读取层数
    fread(num_layers, sizeof(int), 1, file);

    *layers = (Layer *)malloc(*num_layers * sizeof(Layer));

    // 读取每一层的参数
    for (int i = 0; i < *num_layers; i++) {
        fread(&(*layers)[i].input_size, sizeof(int), 1, file);
        fread(&(*layers)[i].output_size, sizeof(int), 1, file);
        (*layers)[i].weights = (double **)malloc((*layers)[i].output_size * sizeof(double *));
        for (int j = 0; j < (*layers)[i].output_size; j++) {
            (*layers)[i].weights[j] = (double *)malloc((*layers)[i].input_size * sizeof(double));
            fread((*layers)[i].weights[j], sizeof(double), (*layers)[i].input_size, file);
        }
        (*layers)[i].bias = (double *)malloc((*layers)[i].output_size * sizeof(double));
        fread((*layers)[i].bias, sizeof(double), (*layers)[i].output_size, file);
    }

    fclose(file);
}

// 从文件读取数据集
void read_datasets(const char *input_file, const char *target_file, double ***inputs, double ***targets, int *num_datasets) {
    FILE *input_fp = fopen(input_file, "r");
    FILE *target_fp = fopen(target_file, "r");
    if (!input_fp || !target_fp) {
        fprintf(stderr, "Cannot open input or target file\n");
        exit(1);
    }

    // 读取输入数据
    fscanf(input_fp, "%d", num_datasets);
    *inputs = (double **)malloc(*num_datasets * sizeof(double *));
    for (int i = 0; i < *num_datasets; i++) {
        int input_size;
        fscanf(input_fp, "%d", &input_size);
        (*inputs)[i] = (double *)malloc(input_size * sizeof(double));
        for (int j = 0; j < input_size; j++) {
            fscanf(input_fp, "%lf", &(*inputs)[i][j]);
        }
    }

    // 读取目标数据
    fscanf(target_fp, "%d", num_datasets);
    *targets = (double **)malloc(*num_datasets * sizeof(double *));
    for (int i = 0; i < *num_datasets; i++) {
        int target_size;
        fscanf(target_fp, "%d", &target_size);
        (*targets)[i] = (double *)malloc(target_size * sizeof(double));
        for (int j = 0; j < target_size; j++) {
            fscanf(target_fp, "%lf", &(*targets)[i][j]);
        }
    }

    fclose(input_fp);
    fclose(target_fp);
}

int main() {
    srand(time(NULL));

    int input_size = 1;
    int output_size = 3;
    int hidden_layers_sizes[] = {4, 3};
    int num_hidden_layers = sizeof(hidden_layers_sizes) / sizeof(hidden_layers_sizes[0]);
    int num_layers = num_hidden_layers + 1;

    Layer *layers = NULL;
    int loaded_num_layers = 0;  // 初始化为0

    // 尝试加载模型
    load_model(&layers, &loaded_num_layers, "model.bin");

    // 如果模型加载失败，则创建新的模型
    if (layers == NULL) {
        printf("Cannot load model, creating a new model\n");
        layers = (Layer *)malloc(num_layers * sizeof(Layer));
        layers[0] = create_layer(input_size, hidden_layers_sizes[0]);
        for (int i = 1; i < num_hidden_layers; i++) {
            layers[i] = create_layer(hidden_layers_sizes[i - 1], hidden_layers_sizes[i]);
        }
        layers[num_layers - 1] = create_layer(hidden_layers_sizes[num_hidden_layers - 1], output_size);
        loaded_num_layers = num_layers;
    }

    // 定义多个训练数据集
    double **inputs = NULL;
    double **targets = NULL;
    int num_datasets = 0;

    // 从文件读取输入和目标数据
    read_datasets("inputs.txt", "targets.txt", &inputs, &targets, &num_datasets);

    double **activations = NULL;
    double learning_rate = 0.00001; // 调整学习率

    // 训练模型
    printf("Model train epoch\n> ");
    int epoch_num = 0;
    scanf("%d", &epoch_num);
    for (int epoch = 0; epoch < epoch_num; epoch++) {
        for (int dataset_index = 0; dataset_index < num_datasets; dataset_index++) {
            forward_pass(layers, loaded_num_layers, inputs[dataset_index], &activations);
            backpropagation(layers, loaded_num_layers, activations, targets[dataset_index], learning_rate);
            // 清理activations内存
            for (int i = 1; i <= loaded_num_layers; i++) { // 修正循环的开始和结束条件
                free(activations[i]);
            }
            free(activations);
        }
        if (epoch % 500 == 0) {
            printf("Epoch %d, Output: ", epoch);
            for (int i = 0; i < layers[loaded_num_layers - 1].output_size; i++) {
                printf("%f ", activations[loaded_num_layers][i]);
            }
            printf("\n");
        }
    }

    // 保存模型
    printf("Model saved?[y/n]\n> ");
    char answer = 'y';
    answer = getchar();
    getchar();
    if (answer == 'y' || answer == 'Y') {
        save_model(layers, loaded_num_layers, "model.bin");
    }

    // 清理层资源
    for (int i = 0; i < loaded_num_layers; i++) {
        free_layer(&layers[i]);
    }
    free(layers);

    // 清理输入和目标数据内存
    for (int i = 0; i < num_datasets; i++) {
        free(inputs[i]);
        free(targets[i]);
    }
    free(inputs);
    free(targets);

    return 0;
}
