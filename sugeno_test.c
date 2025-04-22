#include "fis_sugeno_config.h"

#include "input_array.c"
#include "output_array.c"

#include <stdio.h>
#include <math.h>

int main(void)
{
    puts("Sugeno example in C");

    // FIS definition: pointer variable + dedicated initialization fuction
    FIS_System* inv_pendulum_ctrl_fis;
    FIS_InvertedPendulumController_Init(&inv_pendulum_ctrl_fis); // in 'fis_sugeno_config.c'
    
    for(int i = 0; i < 2000; ++i)
    {
        // FIS evaluation: library function call
        float out = FIS_Evaluate(inv_pendulum_ctrl_fis, inputs[i]); // in 'fis_sugeno.c'

        printf("Output C: %f\t Output MATLAB: %f\tError: %f\n", out, outputs[i], fabs(out - outputs[i]));
    }

    return 0;
}