/**
  ******************************************************************************
  * @file		: fis_sugeno.h
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.0.0
  * @date    	: Feb 19, 2025
  * @brief   	: Takagi-Sugeno-Kang (Sugeno) Fuzzy Inference System for 
  *               embedded control systems
  *
  ******************************************************************************
  */

#ifndef INC_FIS_SUGENO_H_
#define INC_FIS_SUGENO_H_
  
/* Public includes -----------------------------------------------------------*/
  
/* Public typedef ------------------------------------------------------------*/
typedef float (*FIS_MF_Eval)(float input, void* params);
typedef float (*FIS_ConsequentFunction)(float* inputs);

typedef enum 
{
    FIS_AND_MIN,
    FIS_AND_PRODUCT,
    FIS_OR_MAX,
    FIS_OR_PROB_SUM
} FIS_LogicType;

typedef struct 
{
    FIS_MF_Eval eval;   
    void* params;       
} FIS_MembershipFunction;

typedef struct 
{
    int* mf_indices;                    
    int input_count;
    FIS_ConsequentFunction consequent;  
    FIS_LogicType logic_type;               
} FIS_Rule;

typedef struct 
{
    int num_inputs;
    int* num_mfs_per_input;                    
    FIS_MembershipFunction*** input_mfs;       
    FIS_Rule* rules;                           
    int num_rules;
} FIS_System;

    /* Membership function types */
    typedef struct 
    {
        float a; // Left foot
        float b; // Peak
        float c; // Right foot
    } FIS_MF_TriangularParams;

/* Public define -------------------------------------------------------------*/
#define FIS_MAX_INPUTS     6
#define FIS_MAX_MFS        5
#define FIS_MAX_RULES     10

/* Public macro --------------------------------------------------------------*/
#define __FIS_MF_CreateTriangular(name, a_, b_, c_)                              \
    FIS_MF_TriangularParams name##_params = { .a = (a_), .b = (b_), .c = (c_) }; \
    FIS_MembershipFunction name = {                                              \
        .eval = FIS_MF_TriangularEval,                                           \
        .params = &name##_params                                                 \
    };
  
/* Public variables ----------------------------------------------------------*/
  
/* Public function prototypes ------------------------------------------------*/
float FIS_EvaluateMemberFunction(float input, FIS_MembershipFunction* mf);

void FIS_FuzzifyInput(float input, FIS_MembershipFunction** mf_array, int mf_count, float* output_degrees);

float FIS_MF_TriangularEval(float input, void* params);

float FIS_EvaluateRule(FIS_Rule* rule, float input_degrees[FIS_MAX_INPUTS][FIS_MAX_MFS], float* inputs, float* weight_out);

float FIS_DefuzzifyOutput(float* wz, float* w, int w_count);

float FIS_Evaluate(FIS_System* fis, float* inputs);   

#endif /* INC_FIS_SUGENO_H_ */
