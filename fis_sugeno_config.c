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
float K0(const float* x) 
{
    return -0.5456 * x[0] + 108.3730 * x[1] +  4.0827 * x[2] +  0.5456 * x[3] + 1.0912 * x[4] + 0 * x[5] + 0;
}

float K1(const float* x) 
{  
    return -16.9129 * x[0] + 423.9900 * x[1] + 194.2168 * x[2] + 16.9129 * x[3] + 33.8259 * x[4] + 0 * x[5] + 0;
}

float K2(const float* x) 
{ 
    return -43.6463 * x[0] + 1080.2933 * x[1] + 786.3601 * x[2] + 43.6463 * x[3] + 87.2925 * x[4] + 0 * x[5] + 0;
}

float PID_GA(const float* x) 
{
    return 4.772 * x[0] + (-4.772) * x[1] + 31189.5424836601 * x[2] + 0.1087128408 * x[3] + (-0.4213676) * x[4] + 0;
}

float PID_PP(const float* x) 
{
    return 85.1622936535121 * x[0] + (-85.1622936535121) * x[1] + 2972.72484560811 * x[2] + 0 * x[3] + 0 * x[4]  + 0;
}

/* Public functions ----------------------------------------------------------*/
void FIS_InvertedPendulumController_Init(FIS_System** fis)
{
    /* Membership function definition */
    __FIS_MF_CreateTriangular_Static(mf0, 0.1f, 0.1f, 3.1f);
    __FIS_MF_CreateTriangular_Static(mf1, 0.1f, 3.1f, 8.1f);
    __FIS_MF_CreateTriangular_Static(mf2, 3.1f, 8.1f, 8.1f);

    static FIS_MembershipFunction* null_mf[] = { NULL };
    static FIS_MembershipFunction* input_mfs5[] = { &mf0, &mf1, &mf2 };

    static FIS_MembershipFunction** all_input_mfs[] = {
        null_mf,
        null_mf,
        null_mf,
        null_mf,
        null_mf,
        input_mfs5
    };

    static int mf_counts[] = { 0, 0, 0, 0, 0, 3 };

    /* Rules definition */
    /*                       input: 0   1   2   3   4  5 */
    static int rule0_indices[] = { -1, -1, -1, -1, -1, 0 }; // if input(5) is mf0
    static int rule1_indices[] = { -1, -1, -1, -1, -1, 1 }; // if input(5) is mf1
    static int rule2_indices[] = { -1, -1, -1, -1, -1, 2 }; // if input(5) is mf2

    static FIS_Rule rules[] = {
        { rule0_indices, K0, FIS_AND_MIN }, // if input(5) is mf0 then K0 
        { rule1_indices, K1, FIS_AND_MIN }, // if input(5) is mf1 then K1 
        { rule2_indices, K2, FIS_AND_MIN }  // if input(5) is mf2 then K2
    };

    static FIS_System _fis = {
        .num_inputs = 6,
        .num_mfs_per_input = mf_counts,
        .input_mfs = all_input_mfs,
        .rules = rules,
        .num_rules = 3
    };

    *fis = &_fis;
}

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
        { rule1_indices, PID_GA, FIS_AND_MIN }, // if input(3) is mf_negative then PID_GA
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