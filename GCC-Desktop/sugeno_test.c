#include "fis_sugeno_config.h"

#include "test1_input_array.c"
#include "test1_output_array.c"

#include "test2_input_array.c"
#include "test2_output_array.c"

#include <stdio.h>
#include <math.h>

int main(void)
{
    puts("Sugeno example in C: Test #1 - Inverted pendulum controller");

    // FIS definition: pointer variable + dedicated initialization function
    FIS_System* inv_pendulum_ctrl_fis;
    FIS_InvertedPendulumController_Init(&inv_pendulum_ctrl_fis); // in 'fis_sugeno_config.c'
    
    float error = 0.0;
    for(int i = 0; i < 2000; ++i)
    {
        // FIS evaluation: library function call
        float out = FIS_Evaluate(inv_pendulum_ctrl_fis, test1_inputs[i]); // in 'fis_sugeno.c'

        if(fabs(out - test1_outputs[i]) > error)
            error = fabs(out - test1_outputs[i]);
        printf("Output C: %f\t Output MATLAB: %f\tError: %f\n", out, test1_outputs[i], fabs(out - test1_outputs[i]));
    }
    printf("Max error: %f\n", error);

    puts("\nSugeno example in C: Test #2 - PMSM speed controller");

    // FIS definition: pointer variable + dedicated initialization function
    FIS_System* pmsm_speed_ctrl_fis;
    FIS_PMSM_SpeedController_Init(&pmsm_speed_ctrl_fis); // in 'fis_sugeno_config.c'

    error = 0.0;
    for(int i = 0; i < 2000; ++i)
    {
        // FIS evaluation: library function call
        float out = FIS_Evaluate(pmsm_speed_ctrl_fis, test2_inputs[i]); // in 'fis_sugeno.c'

        if(fabs(out - test2_outputs[i]) > error)
            error = fabs(out - test2_outputs[i]);
        printf("Output C: %f\t Output MATLAB: %f\tError: %f\n", out, test2_outputs[i], fabs(out - test2_outputs[i]));
    }
    printf("Max error: %.15f\n", error);

    return 0;
}