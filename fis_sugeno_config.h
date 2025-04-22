/**
  ******************************************************************************
  * @file		: fis_sugeno_config.h
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.0.0
  * @date    	: Feb 19, 2025
  * @brief   	: Takagi-Sugeno-Kang (Sugeno) Fuzzy Inference System for 
  *               embedded control systems
  *               Configuration file: FIS structure initialization
  *
  ******************************************************************************
  */

  #ifndef INC_FIS_SUGENO_CONFIG_H_
  #define INC_FIS_SUGENO_CONFIG_H_

  /* Public includes -----------------------------------------------------------*/
  #include "fis_sugeno.h"
  
  /* Public typedef ------------------------------------------------------------*/
  
  /* Public define -------------------------------------------------------------*/
  
  /* Public macro --------------------------------------------------------------*/
  
  /* Public variables ----------------------------------------------------------*/
  
  /* Public function prototypes ------------------------------------------------*/
  void FIS_InvertedPendulumController_Init(FIS_System** fis);

  #endif /* INC_FIS_SUGENO_CONFIG_H_ */
  