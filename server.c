#include "math.h"
#include <rpc/rpc.h>

static int result;


int * add_1_svc (intpair* pair, struct svc_req *rqstp) {
    result = pair->a + pair->b;
    return &result;
}
        
int * multiply_1_svc (intpair* pair, struct svc_req *rqstp) {
    result = pair->a * pair->b;
    return &result;
}

int * cube_1_svc (int* number, struct svc_req *rqstp) {
    result = *number * *number * *number;
    return &result;
}