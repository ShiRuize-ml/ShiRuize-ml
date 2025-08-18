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

struct cellgroup {
    int length;
    int *num_ptr_int; // 指向每一层细胞数量的指针
    struct cell **cell;
};


/*文件格式：
1.(int）纵向长度
2.纵向长度个（int）横向长度
3.（double)所有W
*/

void makefile() {//创建配置文件
    printf("To make a new one\nSure?[Y/n]");
    char sure;
    scanf("%c", &sure);
    if(sure == 'n' || sure == 'N') { return; }
    FILE *fp;
    fp = fopen("data.bin", "w");// 创建配置文件
    printf("Creating data.bin...\n");
    int length = 0;//创建纵向长度
    printf("Input the length of the length: ");
    scanf("%d", &length);
    fwrite(&length, sizeof(int), 1, fp); // 写入纵向长度
    int num_ptr_int[length];//创建纵向长度数组
    for (int a = 0; a < length; a++) {
        printf("Input the length of the %dth dimension: ", a + 1);
        scanf("%d", &num_ptr_int[a]); // 输入每一纵列的长度
        fwrite(&num_ptr_int[a], sizeof(int), 1, fp); // 写入每一纵列的长度
    }
    printf("Num.Done\n");
    double w = 0.0;//创建权重
    for(int a = 1; a < length; a++) {
        for (int b = 0; b < num_ptr_int[a]; b++) {
            for(int c = 0;c < num_ptr_int[a-1];c++) {
                printf("Input the weight for cell [%d][%d]w.[%d]: \n", a, b,c);
                scanf("%lf", &w); // 输入每个细胞的权重
                fwrite(&w, sizeof(double), 1, fp); // 写入权重
            }
        }
    }
    printf("W Done\n");
    fclose(fp); // 关闭文件
}
short read(struct cellgroup *cellgroup0) {
    FILE *fp;
    fp = fopen("data.bin", "r");
    if (fp == NULL) {
        fprintf(stderr, "File open failed\n");
        return 1;
    }

    fread(&cellgroup0->length, sizeof(int), 1, fp); // 读取纵向长度
    //创建**cell内存
    cellgroup0->cell = (struct cell **)malloc(cellgroup0->length * sizeof(struct cell *));
    if (cellgroup0->cell == NULL) {
        fprintf(stderr, "Memory allocation failed[cell]\n");
        fclose(fp);
        return 1;
    }

    printf("The length:%d\n",cellgroup0->length);
    cellgroup0->num_ptr_int = (int *)malloc(cellgroup0->length * sizeof(int)); // 创建纵向长度数组
    //检查
    if (cellgroup0->num_ptr_int == NULL) {
        fprintf(stderr, "Memory allocation failed[cellnum]\n");
        fclose(fp);
        return 1;
    }

    //初始化
    for (int i = 0; i < cellgroup0->length; i++) cellgroup0->num_ptr_int[i] = 0; // 初始化数组元素为0

    fread(&cellgroup0->num_ptr_int[0], sizeof(int), 1, fp); // 读取第一纵列的长度
    cellgroup0->cell[0] = (struct cell *)malloc(cellgroup0->num_ptr_int[0] * sizeof(struct cell));//分配输入层
    // 将输入层细胞权重数初始化为1并分配
    for (int a = 0; a < cellgroup0->num_ptr_int[0]; a++) {
        cellgroup0->cell[0][a].w.length = 1;
        cellgroup0->cell[0][a].w.num_ptr_double = (double *)malloc(sizeof(double)); // 输入层权重数为1
        cellgroup0->cell[0][a].w.num_ptr_double[0] = 1.0; // 初始化输入层权重为1
        cellgroup0->cell[0][a].in = 0.0;cellgroup0->cell[0][a].out = 0.0; // 初始化输入出为0
    }


    for (int a = 1; a < cellgroup0->length; a++) { // 分配纵向
        fread(&cellgroup0->num_ptr_int[a], sizeof(int), 1, fp); // 读取每一纵列的长度
        printf("%d\n", cellgroup0->num_ptr_int[a]); // 回显纵向长度数组的每一项
        cellgroup0->cell[a] = (struct cell *)malloc(cellgroup0->num_ptr_int[a] * sizeof(struct cell));
        for(int b = 0;b < cellgroup0->num_ptr_int[a];b++) {//分配W
            cellgroup0->cell[a][b].w.num_ptr_double = (double *)malloc(cellgroup0->num_ptr_int[a-1]*sizeof(double));
            cellgroup0->cell[a][b].w.length = cellgroup0->num_ptr_int[a-1];//初始化W长度
            for(int c = 0;c < cellgroup0->cell[a][b].w.length;c++) {
                cellgroup0->cell[a][b].w.num_ptr_double[c] = 0.0;//初始化W
            }
            cellgroup0->cell[a][b].in = 0.0;cellgroup0->cell[a][b].out = 0.0;//初始化输入出
        }
    }

    //写入w
    for (int a = 1; a < cellgroup0->length; a++) {for(int b = 0;b < cellgroup0->num_ptr_int[a];b++) {for(int c = 0;c < cellgroup0->cell[a][b].w.length;c++) {
        fread(&cellgroup0->cell[a][b].w.num_ptr_double[c], sizeof(double), 1, fp); // 读取权重
        printf("cell[%d][%d]:w[%d] : %lf\n",a,b,c, cellgroup0->cell[a][b].w.num_ptr_double[c]); // 回显权重
    }}}

    printf("read mode Done!\n");

    fclose(fp);
    return 0;
}

void free_cellgroup(struct cellgroup *cellgroup0) { // 释放神经网络内存
    printf("free void loading...\n");
    if (cellgroup0->cell != NULL) { // 检查细胞数组是否已分配
        for(int a = 0; a < cellgroup0->length; a++) { 
            if (cellgroup0->cell[a] != NULL) {
                for(int b = 0;b < cellgroup0->num_ptr_int[a];b++) {
                    if (cellgroup0->cell[a][b].w.num_ptr_double != NULL) { // 检查权重数组是否已分配
                        free(cellgroup0->cell[a][b].w.num_ptr_double); // 释放权重数组内存
                        cellgroup0->cell[a][b].w.num_ptr_double = NULL; // 设置为NULL以避免悬空指针
                    }
                }
                free(cellgroup0->cell[a]); // 释放每层细胞数组内存
                cellgroup0->cell[a] = NULL; // 设置为NULL以避免悬空指针
            }
        }
        free(cellgroup0->cell);
        cellgroup0->cell = NULL; // 设置为NULL以避免悬空指针
        printf("cell done\n");
    }

    if (cellgroup0->num_ptr_int != NULL) { // 检查纵向长度数组是否已分配
        free(cellgroup0->num_ptr_int); // 释放纵向长度数组内存
        cellgroup0->num_ptr_int = NULL; // 设置为NULL以避免悬空指针
        printf("length done\n");
    }

    printf("group free done\n");
}

double relu(double input) {// ReLU激活函数
    return input > 0 ? input : 0;
}

void forward(struct cellgroup *cellgroup0) {
    printf("forwarding mode loading...\n");
    
    for(int a = 0;a < cellgroup0->num_ptr_int[0];a++) {
        printf("input cell[0][%d]:", a);
        scanf("%lf", &cellgroup0->cell[0][a].in); // 输入输入层细胞的输入
        printf("input cell[0][%d]:%lf\n",a, cellgroup0->cell[0][a].in); // 回显输入层细胞的输入
    }

    
    // ===== 第一步：设置输入层的输出等于输入 =====
    for (int i = 0; i < cellgroup0->num_ptr_int[0]; i++) {
        cellgroup0->cell[0][i].out = cellgroup0->cell[0][i].in; // 输入层直接传递
    }

    // ===== 第二步：逐层进行前向传播 =====
    //for()
    for(int a = 1;a < cellgroup0->length; a++) {
        for(int b = 0;b < cellgroup0->num_ptr_int[a];b++) {
            cellgroup0->cell[a][b].in = 0.0; // 初始化当前细胞的输入
            for(int c = 0;c < cellgroup0->cell[a][b].w.length;c++) {
                cellgroup0->cell[a][b].in += cellgroup0->cell[a-1][c].out * cellgroup0->cell[a][b].w.num_ptr_double[c]; // 累加输入
            }
            cellgroup0->cell[a][b].out = relu(cellgroup0->cell[a][b].in); // 应用ReLU激活函数
            printf("[%d][%d]in=%lf,out=%lf ||", a, b, cellgroup0->cell[a][b].in, cellgroup0->cell[a][b].out); // 回显当前细胞的输入和输出
        }
        putchar('\n');
    }
    
    // ===== 第三步：输出结果 =====
    printf("Forward Results:\n");
    for(int a = 0;a < cellgroup0->num_ptr_int[cellgroup0->length - 1]; a++) {
        printf("Output of last layer cell[%d][%d]: %lf\n", cellgroup0->length - 1, a, cellgroup0->cell[cellgroup0->length - 1][a].out); // 输出最后一层细胞的输出
    }

    printf("Forward Done...\n");
    printf("Press Enter to continue...");
    getchar();
    getchar();
}

void welcome() { // 修正函数名拼写
    printf("****************\n");
    printf("* Welcome to the neural network program!\n");
    printf("****************\n");
    
    FILE *fp = fopen("logo.txt", "r");
    if (fp == NULL) { // 检查文件是否成功打开
        perror("Error opening logo.txt\n");
        exit(EXIT_FAILURE);
    }
    char ch;
    while (fread(&ch, sizeof(char), 1, fp) != NULL) {
        printf("%c", ch);
    }
    printf("\n//////////////////////////\n");
    fclose(fp); // 正确关闭文件指针
}

int main() {
    welcome();//欢迎界面
    printf("init cellgroup...\n");
    struct cellgroup cellgroup0 = {0}; // 初始化结构体，避免未定义行为
    printf("Read data.bin\n");
    short io = read(&cellgroup0); // 装载数据到神经网络
    if (io == 0) { // 检查是否成功读取数据
        printf("Neural network data loaded successfully");
        forward(&cellgroup0); // 运算前馈神经网络
        // ...
        free_cellgroup(&cellgroup0); // 释放神经网络内存
    } else {
        printf("Failed to load neural network data, creating new configuration file...\n");
        printf("Press Enter to continue...");
        getchar();
        makefile(); // 创建配置文件，若不存在则创建
    }
    printf("Program completed successfully.\n");//成功释放内存
    printf("Bye...^-^\n");
    printf("///////////////////////////\n");
    return 0;
}