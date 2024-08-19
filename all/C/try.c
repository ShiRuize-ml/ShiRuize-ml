#include "cellgroup.h"
int main() {
    int longth = 20;
    double outputtrue[20] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    double output[20] = {3.0,2.0,0.0,-1.0,3.0,2.0,0.0,-1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
    opcellgroup(output,outputtrue,longth);
    return 0;
}