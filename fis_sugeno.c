/**
  ******************************************************************************
  * @file		: fis_sugeno.c
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.0.0
  * @date    	: Feb 19, 2025
  * @brief   	: Takagi-Sugeno-Kang (Sugneo) Fuzzy Inference System for 
  *               embedded control systems
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include "fis_sugeno.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
float FIS_EvaluateMemberFunction(float input, FIS_MembershipFunction* mf) 
{
    if(mf != NULL)
        return mf->eval(input, mf->params);
    else
        return -1.0f;
}

void FIS_FuzzifyInput(float input, FIS_MembershipFunction** mf_array, int mf_count, float* output_degrees) 
{
    for (int i = 0; i < mf_count; ++i) {
        output_degrees[i] = FIS_EvaluateMemberFunction(input, mf_array[i]);
    }
}

float FIS_MF_TriangularEval(float input, void* params)
{
    FIS_MF_TriangularParams* p = (FIS_MF_TriangularParams*)params;
    float output = 0.0;

    if (input < p->a || input > p->c)
        output = 0.0f;
    else if (input == p->b)
        output =1.0f;
    else if (input < p->b)
        output = (input - p->a) / (p->b - p->a);
    else // input > p->b
        output = (p->c - input) / (p->c - p->b);

    if (output > 1.0f)
        output = 1.0f;
    else if (output < 0.0f)
        output = 0.0f;

    return output;
}

float FIS_EvaluateRule(FIS_Rule* rule, float input_degrees[FIS_MAX_INPUTS][FIS_MAX_MFS], float* inputs, float* weight_out) 
{
    float weight;

    if (rule->logic_type == FIS_AND_PRODUCT)
        weight = 1.0f;
    else if (rule->logic_type == FIS_AND_MIN)
        weight = 1.0f;
    else if (rule->logic_type == FIS_OR_MAX)
        weight = 0.0f;
    else if (rule->logic_type == FIS_OR_PROB_SUM)
        weight = 0.0f;

    for (int i = 0; i < rule->input_count; ++i) 
    {
        int mf_index = rule->mf_indices[i];
        if(mf_index < 0)
            continue;

        float degree = input_degrees[i][mf_index];

        switch (rule->logic_type) {
            case FIS_AND_PRODUCT:
                weight *= degree;
                break;
            case FIS_AND_MIN:
                if (degree < weight)
                    weight = degree;
                break;
            case FIS_OR_MAX:
                if (degree > weight)
                    weight = degree;
                break;
            case FIS_OR_PROB_SUM:
                weight = weight + degree - (weight * degree);
                break;
        }
    }

    float output = rule->consequent(inputs);
    *weight_out = weight;
    return weight * output;
}

float FIS_DefuzzifyOutput(float* wz, float* w, int w_count)
{
    float numerator = 0.0f;
    float denominator = 0.0f;

    for (int r = 0; r < w_count; ++r) 
    {
        numerator += wz[r];
        denominator += w[r];
    } 
    
    if (denominator == 0.0f)
        return 0.0f;

    return numerator / denominator;
}



float FIS_Evaluate(FIS_System* fis, float* inputs) 
{
    static float input_degrees[FIS_MAX_INPUTS][FIS_MAX_MFS];

    // Fuzzification step for all inputs
    for (int i = 0; i < fis->num_inputs; ++i) 
    {
        FIS_FuzzifyInput(
            inputs[i],                         // input value
            fis->input_mfs[i],                 // array of MFs for this input
            fis->num_mfs_per_input[i],         // number of MFs
            input_degrees[i]                   // output degrees
        );
    }

    // Arrays to accumulate weighted outputs and weights
    float weighted_outputs[FIS_MAX_RULES];
    float weights[FIS_MAX_RULES];

    // Evaluate each rule
    for (int r = 0; r < fis->num_rules; ++r) {
        weighted_outputs[r] = FIS_EvaluateRule(
            &fis->rules[r],
            input_degrees,
            inputs,
            &weights[r]
        );
    }

    // Defuzzify final result
    return FIS_DefuzzifyOutput(weighted_outputs, weights, fis->num_rules);
}