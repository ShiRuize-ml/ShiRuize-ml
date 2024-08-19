#include <stdio.h>
#include <stdlib.h>
////计算部分
double fc(int vlong, double *v_ptr, int slong, double *s_ptr)
{
    double *v_ptr_mem = v_ptr;
    double *s_ptr_num = s_ptr;
    double v[vlong];
    double s[slong];
    for (int i = 0; i < vlong; i++)
    {
        v[i] = *v_ptr_mem;
        s[i] = *s_ptr_num;
        v_ptr_mem++;
        s_ptr_num++;
    }
    double out = 0.0;
    for (int i = 0; i < vlong; i++)
    {
        out = out + ((v[i] - s[i]) * (v[i] - s[i]));
    }
    out = out / slong;
    return out;
}

double hs(int inlong, double *in_ptr, int wlong, double *w_ptr, double b)
{
    double *in_ptr_mem = in_ptr;
    double *w_ptr_mem = w_ptr;
    double in[inlong];
    double w[wlong];
    for (int i = 0; i < inlong; i++)
    {
        in[i] = *in_ptr_mem;
        in_ptr_mem++;
        w[i] = *w_ptr_mem;
        w_ptr_mem++;
    }
    double out = 0.0;
    for (int i = 0; i < inlong; i++)
    {
        out = out + (in[i] * w[i]);
    }
    out = out + b;
    if (out > 0)
        return out;
    else
        return 0;
}

////神经元部分
struct cell{
    int wlong;
    double *w_ptr;
    double b;
    double out;
    int inlong;
    double *in_ptr;
};
// 创建
void cellcj(struct cell *cell0) {  
    cell0->w_ptr = (double *)malloc(cell0->wlong * sizeof(double));  // 修正内存分配  
    cell0->in_ptr = (double *)malloc(cell0->inlong * sizeof(double));  // 修正内存分配  
    cell0->b = 0.0;  
    cell0->out = 0.0;  
}
void cellkill(struct cell *cell0){
    free(cell0->w_ptr);
    cell0->w_ptr = NULL;
    free(cell0->in_ptr);
    cell0->in_ptr = NULL;
    (cell0->b) = 0.0;
    (cell0->out) = 0.0;
}
// 修改值
void celltj_w(struct cell *cell0, double wtj, int wwz)
{
    double *wmem = (cell0->w_ptr);
    for (int i = 0; i < wwz; i++)
    {
        wmem++;
    }
    *wmem = wtj;
}
void celltj_b(struct cell *cell0, double btj) { (cell0->b) = btj; }
void celltj_in(struct cell *cell0, double intj, int inwz){
    double *inmem = (cell0->in_ptr);
    for (int i = 0; i < inwz; i++){
        inmem++;
    }
    *inmem = intj;
}
// 计算
void celljs(struct cell *cell0) { (cell0->out) = hs((cell0->inlong), (cell0->in_ptr), (cell0->wlong), (cell0->w_ptr), (cell0->b)); }

//////////// 细胞组织部分
// 抓取
#define MAX_LINE_LENGTH 12
void get(long long *celllong, int **cellgroup) {  
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
//创建
#define B 0.0
#define OUT 0.0
void createcellgroup(struct cell ***array, int rows[], int numRows) {  
    *array = (struct cell **)malloc(numRows * sizeof(struct cell *));  
    for (int i = 1; i < numRows; i++) {  
        (*array)[i] = (struct cell *)malloc(rows[i] * sizeof(struct cell));  
        // 初始化结构体数组
        for (int j = 0; j < rows[i]; j++) {
            (*array)[i][j].wlong = rows[i-1];
            (*array)[i][j].inlong = rows[i-1];
            (*array)[i][j].b = B;
            (*array)[i][j].out = OUT;
            //cellcj(&(*array)[i][j]);
        }  
    }
    // 初始化第零层（特例）
    for (int j = 0; j < rows[0]; j++) {
        (*array)[0][j].wlong = 1;
        (*array)[0][j].inlong = 1;
        (*array)[0][j].b = B;
        (*array)[0][j].out = OUT;
        //cellcj(&(*array)[0][j]);
        // 如果需要，可以添加类似 w_ptr = malloc(some_size * sizeof(double)); 的代码  
    } 
}  
void printcellgroup(struct cell **array, int rows[], int numRows) {  
    for (int i = 0; i < numRows; i++) {  
        printf("Row %d: ", i);  
        for (int j = 0; j < rows[i]; j++) {  
            printf("(%d, %.2f, %.2f, %.2f, %d) ",  
                   array[i][j].wlong, array[i][j].b, array[i][j].out,  
                   (array[i][j].w_ptr ? array[i][j].w_ptr[0] : 0.0), // 假设w_ptr至少有一个元素  
                   array[i][j].inlong);  
            // 注意：这里假设了w_ptr和in_ptr已经分配了内存并填充了数据  
            // 如果它们可能为空，则需要添加检查  
        }  
        printf("\n");  
    }  
}   
void freecellgroup(struct cell **array, int numRows) {  
    for (int i = 0; i < numRows; i++) {  
        for (int j = 0; j < numRows; j++) {  // 假设numRows是正确的数组长度（可能需要从外部获取）  
            cellkill(&array[i][j]);  // 注意这里传递的是 &array[i][j]，即struct cell *  
        }  
        free(array[i]);  
    }  
    free(array);  
}

int main() {  
    long long celllong = 0;  
    int *cellgroupnum = NULL;  
    get(&celllong, &cellgroupnum);  
    for (long long i = 0; i < celllong; i++) {  
        printf("%d\n", cellgroupnum[i]);  
    }

    //创建组织
    struct cell **cellgroup;   
    createcellgroup(&cellgroup, cellgroupnum,celllong);

    //运行组织
    printcellgroup(cellgroup, cellgroupnum,celllong);

    //释放内存
    freecellgroup(cellgroup,celllong);
    free(cellgroupnum);
    return 0;  
}
