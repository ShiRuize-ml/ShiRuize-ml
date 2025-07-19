#include <stdio.h>
#include <stdlib.h>

struct num{//不规则数组
    int length;
    double *num_ptr_double;
    int *num_ptr_int;
};

struct cell{//单一神经元
    struct num w;//权重
    double in;//所有输入
    double out;//输出
};

struct group {//神经网络
    struct num cellnum;
    int num_all;//细胞总数
    struct cell *cell_ptr;//细胞指针
};

/*文件格式：
1.(int）纵向长度
2.纵向长度个（int）横向长度
3.（double)所有W
*/
unsigned long long read(struct group *group0) {
    FILE *fp;
    fp = fopen("data.bin", "r");
    if (fp == NULL) {
        fprintf(stderr, "File open failed\n");
        return 1;
    }

    fread(&group0->cellnum.length, sizeof(int), 1, fp); // 读取纵向长度
    unsigned long long cellnum_all = 0;
    group0->cellnum.num_ptr_int = (int *)malloc(group0->cellnum.length * sizeof(int)); // 创建纵向长度数组
    if (group0->cellnum.num_ptr_int == NULL) {
        fprintf(stderr, "Memory allocation failed[cellnum]\n");
        fclose(fp);
        return 1;
    }

    for (int a = 0; a < group0->cellnum.length; a++) { // 修正循环条件
        fread(&group0->cellnum.num_ptr_int[a], sizeof(int), 1, fp); // 读取每一纵列的长度
        cellnum_all += group0->cellnum.num_ptr_int[a]; // 计算细胞总数
        printf("%d\n", group0->cellnum.num_ptr_int[a]); // 回显纵向长度数组的每一项
    }
    printf("cellnum_all [%llu]\n", cellnum_all); // 显示细胞总数

    group0->cell_ptr = (struct cell *)malloc(cellnum_all * sizeof(struct cell)); // 分配细胞数组
    if (group0->cell_ptr == NULL) { // 判断细胞数组是否分配成功
        fprintf(stderr, "Memory allocation failed[cellgroup]\n");
        free(group0->cellnum.num_ptr_int);
        fclose(fp);
        return 1;
    }

    // 将输入层细胞权重初始化为1
    for (int a = 0; a < group0->cellnum.num_ptr_int[0]; a++) {
        group0->cell_ptr[a].w.length = 1; // 输入层细胞权重数等于1
    }

    // 初始化隐藏层和输出层细胞权重数
    for (unsigned long long a = group0->cellnum.num_ptr_int[0]; a < cellnum_all; a++) {
        group0->cell_ptr[a].w.length = group0->cellnum.num_ptr_int[0]; // 修正权重数赋值逻辑
    }

    fclose(fp);
    return cellnum_all;
}

void free_cellgroup(struct group *group0, unsigned long long cellnum_all) { // 释放神经网络内存
    if (group0->cell_ptr != NULL) { // 检查细胞数组是否已分配
        for (unsigned long long a = 0; a < cellnum_all; a++) { // 修正循环条件
            if (group0->cell_ptr[a].w.num_ptr_double != NULL) { // 检查权重数组是否已分配
                free(group0->cell_ptr[a].w.num_ptr_double); // 释放权重数组内存
                group0->cell_ptr[a].w.num_ptr_double = NULL;
            }
        }
        free(group0->cell_ptr); // 释放细胞数组内存
        group0->cell_ptr = NULL;
    }

    if (group0->cellnum.num_ptr_int != NULL) { // 检查纵向长度数组是否已分配
        free(group0->cellnum.num_ptr_int); // 释放纵向长度数组内存
        group0->cellnum.num_ptr_int = NULL;
    }

    printf("group free done\n");
}

double relu(double input) {// ReLU激活函数
    return input > 0 ? input : 0;
}

void forward(struct group *group0, unsigned long long cellnum_all) {//前馈神经网络
    
}

int main() {
    struct group group0 = {0}; // 初始化结构体，避免未定义行为
    unsigned long long cellnum_all = read(&group0); // 装载数据到神经网络
    if (cellnum_all != 1) { // 检查是否成功读取数据
        free_cellgroup(&group0, cellnum_all); // 释放神经网络内存
    } else {
        fprintf(stderr, "Failed to load neural network data\n");
    }
    return 0;
}