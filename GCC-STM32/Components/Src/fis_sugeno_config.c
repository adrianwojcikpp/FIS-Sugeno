/**
  ******************************************************************************
  * @file		: fis_sugeno_config.c
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.0.0
  * @date    	: Feb 19, 2025
  * @brief   	: Takagi-Sugeno-Kang (Sugeno) Fuzzy Inference System for 
  *               embedded control systems
  *               Configuration file: FIS structure initialization
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include "fis_sugeno.h"

/* Private functions ---------------------------------------------------------*/
float PID_GA(const float* x) 
{
    return 4.772 * x[0] + (-4.772) * x[1] + 31189.5424836601 * x[2] + 0.1087128408 * x[3] + (-0.4213676) * x[4] + 0;
}

float PID_PP(const float* x) 
{
    return 85.1622936535121 * x[0] + (-85.1622936535121) * x[1] + 2972.72484560811 * x[2] + 0 * x[3] + 0 * x[4]  + 0;
}

/* Public functions ----------------------------------------------------------*/
void FIS_PMSM_SpeedController_Init(FIS_System** fis)
{
    /* Membership function definition */
    __FIS_MF_CreateTrapezoidal_Static(mf_positive, -5.0f, -5.0f, -0.2f, 0.0f);
    __FIS_MF_CreateTriangular_Static(mf_static, -0.5f, 0.0f, 0.5f);
    __FIS_MF_CreateTrapezoidal_Static(mf_negative, 0.0f, 0.2f, 5.0f, 5.0f);

    static FIS_MembershipFunction* null_mf[] = { NULL };
    static FIS_MembershipFunction* ref_speed_derivative_mf[] = { &mf_positive, &mf_static, &mf_negative };

    static FIS_MembershipFunction** all_input_mfs[] = {
        null_mf, // reference angular velocity 
        null_mf, // angular velocity
        null_mf, // angular velocity error integral
        ref_speed_derivative_mf, // reference angular velocity derivative
        null_mf // angular velocity derivative
    };

    static int mf_counts[] = { 0, 0, 0, 3, 0 };

    /* Rules definition */
    /*                       input: 0   1   2   3   4  */
    static int rule0_indices[] = { -1, -1, -1,  0, -1 }; // if input(3) is mf_positive
    static int rule1_indices[] = { -1, -1, -1,  1, -1 }; // if input(3) is mf_static
    static int rule2_indices[] = { -1, -1, -1,  2, -1 }; // if input(3) is mf_negative

    static FIS_Rule rules[] = {
        { rule0_indices, PID_PP, FIS_AND_MIN }, // if input(3) is mf_positive then PID_PP
        { rule1_indices, PID_GA, FIS_AND_MIN }, // if input(3) is mf_static then PID_GA
        { rule2_indices, PID_PP, FIS_AND_MIN }  // if input(3) is mf_negative then PID_PP
    };

    static FIS_System _fis = {
        .num_inputs = 5,
        .num_mfs_per_input = mf_counts,
        .input_mfs = all_input_mfs,
        .rules = rules,
        .num_rules = 3
    };

    *fis = &_fis;
}
