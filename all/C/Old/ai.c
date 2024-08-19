#include <stdio.h>
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
struct cell
{
    int wlong;
    double *w_ptr;
    double b;
    double out;
    int inlong;
    double *in_ptr;
};
// 创建
void cellcj(struct cell *cell0)
{
    (cell0->w_ptr) = (double **)malloc(sizeof(double *) * (cell0->wlong));
    (cell0->in_ptr) = (double **)malloc(sizeof(double *) * (cell0->inlong));
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
void celltj_in(struct cell *cell0, double intj, int inwz)
{
    double *inmem = (cell0->in_ptr);
    for (int i = 0; i < inwz; i++)
    {
        inmem++;
    }
    *inmem = intj;
}
// 计算
void celljs(struct cell *cell0) { (cell0->out) = hs((cell0->inlong), (cell0->in_ptr), (cell0->wlong), (cell0->w_ptr), (cell0->b)); }
////细胞组部分
struct cellgroup
{
    struct cell *cellgroup_ptr;
};

struct cell *cellgroupcj(int cellgroupc, int cellgroup[], struct cellgroup *cellgroup0)
{
    int cd = 0;
    struct cell *cell_ptr;
    for (int i = 0; i < cellgroupc; i++)
    {
        cd = cd + cellgroup[i];
    }
    (cellgroup0->cellgroup_ptr) = (struct cell **)malloc(sizeof(struct cell *) * (cd));
    for (int i = 0; i < cd; i++)
    {
        cellcj(cell_ptr);
        cell_ptr++;
    }
    cell_ptr = (cellgroup0->cellgroup_ptr);
    for (int i = 0; i < cellgroup[0]; i++)
    {
        (cell_ptr->inlong) = 1;
    }
    for (int i = 1; i < (cd - cellgroup[cellgroupc]); i++)
    {
        for (int a = 0; a < cellgroup[i]; a++)
        {
        }
    }
}

void cellgroupcb(struct cell *cellgroup0) {}

// 主程序
/********************************************************************************
int main() {
    struct cell cell0;
    cell0.wlong = 1;
    cell0.inlong = 1;
    cellcj(&cell0);
    celltj_in(&cell0,0.1,0);
    celltj_w(&cell0,0.25,0);
    celljs(&cell0);
    printf("Result: %f\n", cell0.out);
    return 0;
}
********************************************************************************/

int main()
{
    struct cellgroup cellgroup0;
    cellgroupcj(4, 4, &cellgroup0);
    return 0;
}