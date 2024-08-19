#include "cellmode.h"
//主函数
//////////// 细胞组织部分
// 抓取
#define MAX_LINE_LENGTH 12

unsigned long long allcellnum;

void get(long long *celllong, int **cellgroup) {
    printf("GET FORM set.txt\n");  
    FILE *fp = fopen("set.txt", "r");  
    if (fp == NULL) {  
        perror("Error opening file");  
        exit(1); // 使用非零状态码表示错误  
    }  
    char line[MAX_LINE_LENGTH];  
    if (fgets(line, MAX_LINE_LENGTH, fp) == NULL) {  
        if (feof(fp)) {  
            printf("Reached end of file without reading any data.\n");  
        } else {  
            perror("Error reading from file");  
        }  
        fclose(fp);  
        exit(1);  
    }  
    *celllong = atoi(line);  
    printf("to -> *celllong :%lld\n", *celllong);  
    *cellgroup = (int*)calloc(*celllong, sizeof(int));  
    if (*cellgroup == NULL) {  
        perror("Memory allocation failed");  
        fclose(fp);  
        exit(1);  
    }  
    for (long long i = 0; i < *celllong; i++) {  
        if (fgets(line, MAX_LINE_LENGTH, fp) == NULL) {  
            break;  
        }  
        (*cellgroup)[i] = atoi(line); // 正确访问和赋值  
    }  
    fclose(fp);  
}  

void checknum(long long *celllong, int **cellgroup) {
    allcellnum = 0;
    for(int i = 0;i < *celllong;i++) {
        allcellnum += (*cellgroup)[i];
    }
    printf("There are %llu Cells\n",allcellnum);
}


void createcellgroup(struct cell ***array, int rows[], int numRows) {  
    *array = (struct cell **)malloc(numRows * sizeof(struct cell *));  
    if (!*array) {
        // 处理内存分配失败的情况!*array 
        printf("Error memory creating:!*array");
        exit(-1);
    }  
    printf("Creating hidden rows...\n");
    for (int i = 0; i < numRows; i++) {  
        (*array)[i] = (struct cell *)malloc(rows[i] * sizeof(struct cell));  
        if (!(*array)[i]) {  
            // 处理内存分配失败的情况  
            // 这里需要释放之前分配的内存  
            printf("Error memory creating:!(*array)[%d]",i);
            exit(-1);  
        }  
        for (int j = 0; j < rows[i]; j++) {  
            if (i == 0) {
                // 初始化输入层  
                (*array)[i][j].wlong = 1;  
                (*array)[i][j].inlong = 1;  
                (*array)[i][j].tdlong = 1;
            } else if (i == numRows - 1) {  
                // 初始化输出层 
                (*array)[i][j].wlong = rows[i-1];  
                (*array)[i][j].inlong = rows[i-1];
                (*array)[i][j].tdlong = rows[i-1]; 
            } else {  
                // 初始化隐藏层
                (*array)[i][j].wlong = rows[i-1];  
                (*array)[i][j].inlong = rows[i-1];
                (*array)[i][j].tdlong = rows[i-1];  
            }  
            cellcj(&(*array)[i][j]);  
        }
        //为输入层W赋值
        for(int j = 0;j < rows[0];j++) {
            (*array)[0][j].w_ptr[0] = 1.0;
        }
        
    }  
    printf("----The Cell group made!!!\n");  
}

void checkcellgroup(struct cell **array, int rows[], int numRows) {  
    for (int i = 0; i < numRows; i++) {  
        printf("Row %d: ", i);  
        for (int j = 0; j < rows[i]; j++) {  
            printf("(%d,%d, %.2f, %.2f, %.2f, %d) ",  
                   array[i][j].tdlong,array[i][j].wlong, array[i][j].b, array[i][j].out,  
                   (array[i][j].w_ptr ? array[i][j].w_ptr[0] : 0.0), // 假设w_ptr至少有一个元素  
                   array[i][j].inlong);  
        }  
        printf("\n");  
    }
    //k
    printf("----Cellgroupchecked\n");  
} 



double * runcellgroup(struct cell **array, int rows[], int numRows,double *input) {  
    printf("----Cellgrouprun:\n");
    for(int i = 0;i < rows[0];i++) {
        array[0][i].in_ptr[0] = input[i];
        printf("INPUT -> cell[0][%d].in:%.2f\n",i,input[i]);
    }
    //for (int i = 0; i < numRows - 1; i++) {  // 减去1以避免访问array[numRows]  
    for (int i = 0; i < numRows; i++) {  // 减去1以避免访问array[numRows]  
        printf("Row %d: \n", i);
        for (int j = 0; j < rows[i]; j++) { 
            printf("cell[%d][%d] run,", i, j);
            celljs(&array[i][j]);  // 假设celljs函数正确实现了其逻辑  
            printf("out: %.2f\n",array[i][j].out);
            // 注意：这里假设array[i+1]存在且rows[i+1]有效  
            if (i + 1 < numRows) {  
                for (int a = 0; a < rows[i + 1] && a < array[i][j].inlong; a++) {  // 确保不越界  
                    // 注意：这里假设in_ptr[j]已经指向了一个有效的double数组  
                    // 这里假设in_ptr[j]是指向double的指针，而不是double数组  
                    // 但通常in_ptr[j]会是一个double* 
                    // 如果in_ptr[j]是一个double数组，则使用索引而不是指针算术
                    array[i + 1][a].in_ptr[j] += array[i][j].out;  // 直接修改值  
                    printf("out: %.2f -> cell[%d][%d].in_ptr[%d]\n", array[i][j].out, i + 1, a, j);  
                }  
            }  
        }  
    }  
    printf("Cell group ran successfully\n");  
    double *out = malloc((rows[numRows - 1]) * sizeof(double));  
    if (out == NULL) {
        printf("Out mem Error!!!\n");  
        exit(-1); // 或处理错误  
    }
    for (int i = 0; i < rows[numRows - 1]; i++) {  
        printf("%.2f\n", array[numRows - 1][i].out);  
        out[i] = array[numRows - 1][i].out;  
    }
    return out; // 调用函数需要负责释放这个内存  
}

void freecellgroup(struct cell **array, int *rows, int numRows) { // 注意添加了 rows 参数  
    printf("----Cellkilling:\n");      
    for (int i = 0; i < numRows; i++) {    
        for (int j = 0; j < rows[i]; j++) {  // 使用 rows[i] 而不是 numRows  
            cellkill(&array[i][j]);  // 如果 cellkill 需要 struct cell * 的地址，则保持这样  
            // 如果 cellkill 只需要 struct cell *，则调用为 cellkill(array[i][j]);  
        }  
        printf("([%d]killed,", i);   
        free(array[i]);  
        printf("[%d]freed)\n", i);     
    }    
    free(array);  
    printf("array freed\n");  
    printf("----The Cellgroup has killed");    
}


#include <math.h>  
  
// 均方误差损失函数  
double mse_loss(double target, double output) {  
    return 0.5 * pow(target - output, 2);  
}  
  
// 神经元输出函数（使用ReLU激活函数）  
double neuron_output(double input, double weight, double bias) {  
    return fmax(0, input * weight + bias);  
}  
  
// 权重的梯度（基于MSE损失和ReLU激活）  
double gradient_weight(double input, double target, double output, double /* unused_weight */) {  
    // 对于ReLU激活，如果output为0（即输入*权重+偏置<=0），则梯度为0；否则为误差乘以输入  
    double error = target - output;  
    return (output > 0) ? error * input : 0.0;  
}  
  
// 偏置的梯度（基于MSE损失和ReLU激活）  
double gradient_bias(double target, double output) {  
    // 对于ReLU激活，偏置的梯度与权重的梯度类似，但不受输入影响  
    double error = target - output;  
    return (output > 0) ? error : 0.0;  
}  
  

  
// 假设的梯度计算和学习率  
double learning_rate = 0.01;  

// 反向传播和权重更新函数  
void cellgroupupdate(struct cell **array, int rows[], int numRows, double *true_out, double *outputs) {  
    // 假设 outputs 是 runcellgroup 函数的输出  
    int total_neurons = 0;  
    for (int i = 0; i < numRows; i++) {  
        total_neurons += rows[i];  
    }  
  
    // 初始化梯度数组（假设每个神经元的梯度数组已经足够大）  
    double *deltas = (double *)malloc(total_neurons * sizeof(double));  
  
    // 计算输出层的梯度  
    for (int i = 0; i < rows[numRows - 1]; i++) {  
        struct cell *cell = &(array[numRows - 1][i]);  
        double error = mse_loss(true_out[i], cell->out);  
        double delta = gradient_bias(true_out[i], cell->out);  
        cell->td_ptr[0] = delta; // 假设输出层只有一个梯度  
        deltas[i + (numRows - 1) * rows[numRows - 1]] = delta;  
    }  
  
    // 反向传播到隐藏层  
    for (int layer = numRows - 2; layer >= 0; layer--) {  
        for (int j = 0; j < rows[layer]; j++) {  
            struct cell *cell = &(array[layer][j]);  
            double delta = 0.0;  
            for (int k = 0; k < cell->tdlong; k++) {  
                int next_neuron_idx = j * rows[layer + 1] + k;  
                delta += cell->w_ptr[k] * deltas[next_neuron_idx + (layer + 1) * rows[layer + 1]];  
            }  
            cell->td_ptr[0] = delta * (cell->out > 0 ? 1.0 : 0.0); // ReLU导数  
            deltas[j + layer * rows[layer]] = delta;  
  
            // 更新权重和偏置  
            for (int k = 0; k < cell->wlong; k++) {  
                int prev_neuron_idx = j * rows[layer - 1] + k;  
                if (layer > 0) {  
                    double prev_output = array[layer - 1][k].out;  
                    cell->w_ptr[k] += learning_rate * cell->td_ptr[0] * prev_output;  
                }  
            }  
            cell->b += learning_rate * cell->td_ptr[0];  
        }  
    }  
    free(deltas);  
}  