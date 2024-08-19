#include "cellgroup.h"
void cellgroup_main() {
    long long celllong = 0;  
    int *cellgroupnum = NULL;  
    get(&celllong, &cellgroupnum);  
    for (long long i = 0; i < celllong; i++) {  
        printf("%d\n", cellgroupnum[i]);  
    }
    checknum(&celllong, &cellgroupnum);
    //创建组织
    struct cell **cellgroup;   
    createcellgroup(&cellgroup, cellgroupnum,celllong);
    checkcellgroup(cellgroup, cellgroupnum,celllong);
    double input[2] = {1.0,1.0};
    double *output;
    output = runcellgroup(cellgroup, cellgroupnum,celllong,input);
    double true_out[2] = {1.0,1.0};
    for(int i = 0 ;i < 5;i++) {
        cellgroupupdate(cellgroup, cellgroupnum,celllong,true_out,output);
        output = runcellgroup(cellgroup, cellgroupnum,celllong,input);
    }
    //舍弃opcellgroup(cellgroup, cellgroupnum,celllong,output,true_out);
    free(output);
    freecellgroup(cellgroup, cellgroupnum,celllong);
}