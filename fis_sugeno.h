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
typedef float (*FIS_ConsequentFunction)(const float* inputs);

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
    FIS_ConsequentFunction consequent;  
    FIS_LogicType logic_type;               
} FIS_Rule;

typedef struct {
    float weight;  
    float output; 
} FIS_RuleOutput;

typedef struct 
{
    int num_inputs;
    int* num_mfs_per_input;                    
    FIS_MembershipFunction*** input_mfs;       
    FIS_Rule* rules;                           
    int num_rules;
} FIS_System;

/* Public typedef - membership functions parameters types --------------------*/

/**
 * @brief Parameters for a triangular membership function.
 *        Represents points a, b, c where:
 *        a <= b <= c
 */
typedef struct 
{
    float a; // Left foot
    float b; // Peak
    float c; // Right foot
} FIS_MF_TriangularParams;

/**
 * @brief Parameters for a trapezoidal membership function.
 *        Represents points a, b, c, d where:
 *        a <= b <= c <= d
 */
typedef struct {
    float a;
    float b;
    float c;
    float d;
} FIS_MF_TrapezoidalParams;


/* Public define -------------------------------------------------------------*/
#define FIS_MAX_INPUTS     6
#define FIS_MAX_MFS        3
#define FIS_MAX_RULES      3

/* Public macro --------------------------------------------------------------*/
#define __FIS_MF_CreateTriangular(name, a_, b_, c_)                              \
    FIS_MF_TriangularParams name##_params = { .a = (a_), .b = (b_), .c = (c_) }; \
    FIS_MembershipFunction name = {                                              \
        .eval = FIS_MF_TriangularEval,                                           \
        .params = &name##_params                                                 \
    };

#define __FIS_MF_CreateTriangular_Static(name, a_, b_, c_)                              \
    static FIS_MF_TriangularParams name##_params = { .a = (a_), .b = (b_), .c = (c_) }; \
    static FIS_MembershipFunction name = {                                              \
        .eval = FIS_MF_TriangularEval,                                                  \
        .params = &name##_params                                                        \
    };  
  
#define __FIS_MF_CreateTrapezoidal(name, a_, b_, c_, d_)                                         \
    FIS_MF_TrapezoidalParams name##_params = { .a = (a_), .b = (b_), .c = (c_), .d = (d_) }; \
    FIS_MembershipFunction name = {                                                          \
        .eval = FIS_MF_TrapezoidalEval,                                                      \
        .params = &name##_params,                                                            \
    };

#define __FIS_MF_CreateTrapezoidal_Static(name, a_, b_, c_, d_)                                         \
    static FIS_MF_TrapezoidalParams name##_params = { .a = (a_), .b = (b_), .c = (c_), .d = (d_) }; \
    static FIS_MembershipFunction name = {                                                          \
        .eval = FIS_MF_TrapezoidalEval,                                                             \
        .params = &name##_params,                                                                   \
    };

/* Public function prototypes ------------------------------------------------*/
/**
 * @brief Evaluates a single membership function for a given input value.
 *
 * @param[in] input     Input value to be evaluated.
 * @param[in] mf        Pointer to the membership function definition.
 * @return              Degree of membership (between 0.0 and 1.0).
 */
float FIS_EvaluateMemberFunction(float input, const FIS_MembershipFunction* mf);

/**
 * @brief Computes the degree of membership for all membership functions
 *        associated with a single input.
 *
 * @param[in]  input            Input value to be fuzzified.
 * @param[in]  mf_array         Array of pointers to membership functions.
 * @param[in]  mf_count         Number of membership functions for the input.
 * @param[out] input_degrees   Output array to store calculated degrees of membership.
 */
void FIS_FuzzifyInput(float input, FIS_MembershipFunction** mf_array, int mf_count, float* input_degrees);

/**
 * @brief Evaluates a single fuzzy rule, including computing rule weight and weighted output.
 *
 * @param[in]  rule              Pointer to the rule to be evaluated.
 * @param[in]  input_degrees     2D array of fuzzified degrees for all inputs and their MFs.
 * @param[in]  inputs            Original crisp input values.
 * @param[in]  input_count       Number of inputs in the system.
 * @return                       Struct containing rule weight and weighted output.
 */
FIS_RuleOutput FIS_EvaluateRule(FIS_Rule* rule, float input_degrees[FIS_MAX_INPUTS][FIS_MAX_MFS], const float* inputs, int input_count);

/**
 * @brief Performs weighted average defuzzification on the rule outputs.
 *
 * @param[in] rule_output   Array of rule outputs containing weights and weighted values.
 * @param[in] rule_count    Total number of evaluated rules.
 * @return                  Defuzzified crisp output value.
 */
float FIS_DefuzzifyOutput(FIS_RuleOutput* rule_output, int rule_count);

/**
 * @brief Main evaluation function for a complete Sugeno-type fuzzy inference system.
 *
 * @param[in] fis       Pointer to the FIS system definition.
 * @param[in] inputs    Array of crisp input values.
 * @return              Final crisp output after inference and defuzzification.
 */
float FIS_Evaluate(FIS_System* fis, float* inputs);

/* Public function prototypes - membership functions evaluation --------------*/
/**
 * @brief Triangular membership function evaluation implementation.
 *
 * @param[in] input     Input value to be evaluated.
 * @param[in] params    Pointer to array of 3 floats: {a, b, c} for triangle shape.
 * @return              Degree of membership (between 0.0 and 1.0).
 */
float FIS_MF_TriangularEval(float input, void* params);

/**
 * @brief Evaluates a trapezoidal membership function.
 *
 * @param[in] input   Input value to evaluate.
 * @param[in] params  Pointer to FIS_MF_TrapezoidalParams structure.
 * @return            Degree of membership (between 0.0 and 1.0).
 */
float FIS_MF_TrapezoidalEval(float input, void* params);
  
#endif /* INC_FIS_SUGENO_H_ */
