#include "fis_sugeno.h"

#define NUM_INPUTS 6
#define NUM_RULES 3

#include "input_array.c"
#include "output_array.c"

float K1(float* inputs) 
{
    return -0.5456 * inputs[0] + 108.3730 * inputs[1] +  4.0827 * inputs[2] +  0.5456 * inputs[3] + 1.0912 * inputs[4] + 0 * inputs[5] + 0;
}

float K2(float* inputs) 
{  
    return -16.9129 * inputs[0] + 423.9900 * inputs[1] +  194.2168 * inputs[2] +  16.9129  * inputs[3] + 33.8259 * inputs[4] + 0 * inputs[5] + 0;
}

float K3(float* inputs) 
{ 
    return -43.6463  * inputs[0] + 1080.2933  * inputs[1] +  786.3601 * inputs[2] + 43.6463  * inputs[3] + 87.2925 * inputs[4] + 0 * inputs[5] + 0;
}

#include <stdio.h>
#include <math.h>

int main(void)
{
    puts("Sugeno example in C");

    __FIS_MF_CreateTriangular(mf1, 0.1f, 0.1f, 3.1f);
    __FIS_MF_CreateTriangular(mf2, 0.1f, 3.1f, 8.1f);
    __FIS_MF_CreateTriangular(mf3, 3.1f, 8.1f, 8.1f);

    // Inputs 0–4: no MFs, so NULL
    FIS_MembershipFunction* input_mfs0[] = { NULL };
    FIS_MembershipFunction* input_mfs1[] = { NULL };
    FIS_MembershipFunction* input_mfs2[] = { NULL };
    FIS_MembershipFunction* input_mfs3[] = { NULL };
    FIS_MembershipFunction* input_mfs4[] = { NULL };
    FIS_MembershipFunction* input_mfs5[] = { &mf1, &mf2, &mf3 };

    // Full pointer array
    FIS_MembershipFunction** all_input_mfs[] = {
        input_mfs0,
        input_mfs1,
        input_mfs2,
        input_mfs3,
        input_mfs4,
        input_mfs5
    };

    // MF counts (0 for unused inputs)
    int mf_counts[] = { 0, 0, 0, 0, 0, 3 };

    int rule1_indices[] = { -1, -1, -1, -1, -1, 0 }; // input6 uses mf0
    int rule2_indices[] = { -1, -1, -1, -1, -1, 1 }; // input6 uses mf1
    int rule3_indices[] = { -1, -1, -1, -1, -1, 2 }; // input6 uses mf2

    FIS_Rule rules[] = {
        { rule1_indices, 6, K1, FIS_AND_MIN },
        { rule2_indices, 6, K2, FIS_AND_MIN },
        { rule3_indices, 6, K3, FIS_AND_MIN }
    };

    FIS_System my_fis = {
        .num_inputs = 6,
        .num_mfs_per_input = mf_counts,
        .input_mfs = all_input_mfs,
        .rules = rules,
        .num_rules = 3
    };
    
    for(int i = 0; i < 100; ++i)
    {
        float in[] = { inputs[i][0], inputs[i][1], inputs[i][2], inputs[i][3], inputs[i][4], inputs[i][5] };  
        float out = FIS_Evaluate(&my_fis, in);
        printf("Output C: %f\t Output MATLAB: %f\tError: %f\n", out, outputs[i], fabs(out - outputs[i]));
    }

    return 0;
}