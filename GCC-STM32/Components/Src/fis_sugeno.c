/**
  ******************************************************************************
  * @file		: fis_sugeno.c
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.0.0
  * @date    	: Feb 19, 2025
  * @brief   	: Takagi-Sugeno-Kang (Sugeno) Fuzzy Inference System for 
  *               embedded control systems
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include "fis_sugeno.h"

/* Public functions ----------------------------------------------------------*/
float FIS_EvaluateMemberFunction(float input, const FIS_MembershipFunction* mf) 
{
    if(mf != NULL)
        return mf->eval(input, mf->params);
    else
        return -1.0f;
}

void FIS_FuzzifyInput(float input, FIS_MembershipFunction** mf_array, int mf_count, float* input_degrees) 
{
    for (int i = 0; i < mf_count; ++i) {
        input_degrees[i] = FIS_EvaluateMemberFunction(input, mf_array[i]);
    }
}

FIS_RuleOutput FIS_EvaluateRule(FIS_Rule* rule, float input_degrees[FIS_MAX_INPUTS][FIS_MAX_MFS], const float* inputs, int input_count) 
{
    float weight = 1.0f;;

    /*
    if (rule->logic_type == FIS_AND_PRODUCT)
        weight = 1.0f;
    else if (rule->logic_type == FIS_AND_MIN)
        weight = 1.0f;
    else if (rule->logic_type == FIS_OR_MAX)
        weight = 0.0f;
    else if (rule->logic_type == FIS_OR_PROB_SUM)
        weight = 0.0f;
     */

    for (int i = 0; i < input_count; ++i) 
    {
        int mf_index = rule->mf_indices[i];
        if(mf_index < 0)
            continue;

        float degree = input_degrees[i][mf_index];

        if (degree < weight)
           weight = degree;

        /*
        switch (rule->logic_type) 
        {
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
        */
    }

    float output = rule->consequent(inputs);
    FIS_RuleOutput rule_out = {.output = weight * output, .weight = weight };
    return rule_out;
}

float FIS_DefuzzifyOutput(FIS_RuleOutput* rule_output, int rule_count)
{
    float numerator = 0.0f;
    float denominator = 0.0f;

    // weight average method
    for (int r = 0; r < rule_count; ++r) 
    {
        numerator += rule_output[r].output;
        denominator += rule_output[r].weight;
    } 
    
    if (denominator == 0.0f)
        return 0.0f;

    return numerator / denominator;
}

float FIS_Evaluate(FIS_System* fis, float* inputs) 
{
    // Arrays to accumulate degrees of membership
    static float input_degrees[FIS_MAX_INPUTS][FIS_MAX_MFS];

    // Fuzzification step for all inputs
    for (int i = 0; i < fis->num_inputs; ++i) 
        FIS_FuzzifyInput(inputs[i], fis->input_mfs[i], fis->num_mfs_per_input[i], input_degrees[i]);

    // Arrays to accumulate weighted outputs and weights
    FIS_RuleOutput rule_output[FIS_MAX_RULES];

    // Evaluate each rule
    for (int r = 0; r < fis->num_rules; ++r)
        rule_output[r] = FIS_EvaluateRule(&fis->rules[r], input_degrees, inputs, fis->num_inputs);

    // Defuzzify final result
    return FIS_DefuzzifyOutput(rule_output, fis->num_rules);
}

/* Public function  - membership functions evaluation ------------------------*/
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

float FIS_MF_TrapezoidalEval(float input, void* params)
{
    FIS_MF_TrapezoidalParams* p = (FIS_MF_TrapezoidalParams*)params;
    float output = 0.0;

    if (input < p->a || input > p->d)
         output = 0.0f;
    else if (input >= p->b && input <= p->c)
        output = 1.0f;
    else if (input > p->a && input < p->b)
        output = (input - p->a) / (p->b - p->a);
    else // input > p->c && input < p->d
        output = (p->d - input) / (p->d - p->c);

    if (output > 1.0f)
        output = 1.0f;
    else if (output < 0.0f)
        output = 0.0f;

    return output;
}
