%%% File info
%
% ************************************************************************
%
%  @file    AW_FuzzyController.m
%  @author  Adrian Wojcik
%  @version V1.0
%  @date    10-Mar-2025
%  @brief   Fuzzy PMSM Speed Controller
%
% ************************************************************************

%% Create FIS structure

    fis_name = 'PMSM Speed Controller';
    PMSM_SpeedController = sugfis('Name', fis_name);
   
    PMSM_SpeedController.andMethod = 'min';
    PMSM_SpeedController.orMethod = 'max';
    PMSM_SpeedController.defuzzMethod = 'wtaver';
    PMSM_SpeedController.impMethod = 'prod';
    PMSM_SpeedController.aggMethod = 'sum';
    
%% Add inputs

    m = 5; % number of inputs

    % Input #1: reference angular velocity 
    refAngularVelocity.name = 'refAngularVelocity';
    refAngularVelocity.number = 1;
    refAngularVelocity.range = [-5 5]; %[rad/s]
    PMSM_SpeedController = addInput(PMSM_SpeedController, refAngularVelocity.range, 'Name', refAngularVelocity.name);

    % Input #2: angular velocity
    angularVelocity.name = 'angularVelocity';
    angularVelocity.number = 2;
    angularVelocity.range = [-5 5]; %[rad/s]
    PMSM_SpeedController = addInput(PMSM_SpeedController, angularVelocity.range, 'Name', angularVelocity.name);

    % Input #3: angular velocity error integral
    errorIntegral.name = 'errorIntegral';
    errorIntegral.number = 3;
    errorIntegral.range = [-5 5]; %[rad]
    PMSM_SpeedController = addInput(PMSM_SpeedController, errorIntegral.range, 'Name', errorIntegral.name);

    % Input #4: reference angular velocity derivative
    refAngularVelocityDerivative.name = 'refAngularVelocityDerivative';
    refAngularVelocityDerivative.number = 4;
    refAngularVelocityDerivative.range = [-5 5]; %[rad/s^2] 
    PMSM_SpeedController = addInput(PMSM_SpeedController, refAngularVelocityDerivative.range, 'Name', refAngularVelocityDerivative.name);

    PMSM_SpeedController = addMF(PMSM_SpeedController, refAngularVelocityDerivative.name, ...
                    @trapmf, [-5 -5 -0.2 0], 'Name', 'negative');
    
    PMSM_SpeedController = addMF(PMSM_SpeedController, refAngularVelocityDerivative.name, ...
                    @trimf, [-0.5 0 0.5], 'Name', 'static');
    
    PMSM_SpeedController = addMF(PMSM_SpeedController, refAngularVelocityDerivative.name, ...
                    @trapmf, [0.0 0.2 5 5], 'Name', 'positive');

    % Input #5: angular velocity derivative
    angularVelocityDerivative.name = 'angularVelocityDerivative';
    angularVelocityDerivative.number = 5;
    angularVelocityDerivative.range = [-5 5]; %[rad/s^2] 
    PMSM_SpeedController = addInput(PMSM_SpeedController, angularVelocityDerivative.range, 'Name', angularVelocityDerivative.name);
                     
    
%% Add outputs

    n = 1; % number of outputs
    
    current.name = 'current';
    current.number = 1;
    current.range = [-50 50];
    
    current.mfName = {'PID_GA', 'PID_PP'};
    
    PMSM_SpeedController = addOutput(PMSM_SpeedController, current.range, 'Name', current.name);
    
    PMSM_SpeedController = addMF(PMSM_SpeedController, current.name, @linear, [PID_GA_Gains 0], 'Name', 'PID_GA');
    PMSM_SpeedController = addMF(PMSM_SpeedController, current.name, @linear, [PID_PP_Gains 0], 'Name', 'PID_PP');

        
%% Add rules

    rule1 = "If refAngularVelocityDerivative is positive then current is PID_PP";
    rule2 = "If refAngularVelocityDerivative is static then current is PID_GA";
    rule3 = "If refAngularVelocityDerivative is negative then current is PID_PP";
    rules = [rule1 rule2 rule3];
         
    PMSM_SpeedController = addRule(PMSM_SpeedController,rules);

%% Clear variables

    %clearvars -except n FullStateFeedbackController InvPendulumController sysRef 