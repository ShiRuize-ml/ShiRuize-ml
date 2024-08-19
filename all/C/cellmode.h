#include "js.h"

struct cell{
    int wlong;
    double *w_ptr;
    double b;
    double out;
    int inlong;
    double *in_ptr;
    int tdlong;
    double *td_ptr;
};
// 创建
#define B 0.0
#define OUT 0.0
#define W 1.0
#define IN 0.0
#define TD 0.0
void cellcj(struct cell *cell0) {  
    cell0->w_ptr = (double *)malloc(cell0->wlong * sizeof(double));  // 修正内存分配  
    cell0->in_ptr = (double *)malloc(cell0->inlong * sizeof(double));  // 修正内存分配  
    cell0->td_ptr = (double *)malloc(cell0->tdlong * sizeof(double));
    cell0->b = B;  
    cell0->out = OUT;
    for(int i = 0;i < cell0->wlong;i++) {
        cell0->w_ptr[i] = W;
        cell0->in_ptr[i] = IN;
        cell0->td_ptr[i] = TD;
    }
}
void cellkill(struct cell *cell0){
    free(cell0->w_ptr);
    cell0->w_ptr = NULL;
    free(cell0->in_ptr);
    cell0->in_ptr = NULL;
    free(cell0->td_ptr);
    cell0->td_ptr = NULL;
}
// 修改值(目前不用，所以没添加梯度)
void celltj_w(struct cell *cell0, double wtj, int wwz){cell0->w_ptr[wwz] = wtj;}
void celltj_b(struct cell *cell0, double btj) { (cell0->b) = btj; }
void celltj_in(struct cell *cell0, double intj, int inwz){cell0->in_ptr[inwz] = intj;}
// 计算
void celljs(struct cell *cell0) { (cell0->out) = hs((cell0->inlong), (cell0->in_ptr), (cell0->wlong), (cell0->w_ptr), (cell0->b)); }
