%%% File info
%
% ************************************************************************
%
% %  @file  AW_RippleModel_Init.m
%  @author  Adrian Wojcik
%  @version V1.0
%  @date    10-Mar-2025
%  @brief   Parameteres of direct drive with torque ripples
%
% ************************************************************************

Jz = 0.753;
ts = 500e-6;

%PID_Tref = 0.01; % [s]
%PID_Kp = Jz / PID_Tref;
%PID_Ki = 1.0;

%% pole placement
fref = 10;
eref = 0.9;

PID_Ki = Jz  * ((2*pi*fref)^2);
PID_Kp = (2*eref*Jz) * (2*pi*fref);

PID_PP_Gains = [PID_Kp -PID_Kp PID_Ki 0 0];

s = tf('s');

Gc = ((PID_Kp/PID_Ki)*s + 1)/((Jz/PID_Ki)*s^2 + (PID_Kp/PID_Ki)*s  + 1);
Gi = 1/((Jz/PID_Ki)*s^2 + (PID_Kp/PID_Ki)*s  + 1);

LIN_CTRL = pid2(PID_Kp,PID_Ki,0,1/100);

obj.K = Inf;

%% current loop
    % opó¿nienie 
    iq_tau = 0.2e-3;  % 1e-4;
    % stala czasowa typowo 
    iq_Tu = 0.3e-3;   % 1.6e-4
    % szybkoœc narastania ty
    iq_RateLimit = 600; % 2500;
    
    iq_max = 6.0;  % [A]
    
    %transmitancja
    h_curr = tf(1, [iq_Tu 1],'InputDelay',iq_tau);  

%%
p = 24; % liczba PAR biegunow

K_t = 17.5;            % [Nm/A] (me(t)/iq(t))
Tn = 50;               % [Nm] 

dia = 0.0001; % 0.1 mA b³êdu
dib = 0.0001; % 0.1 mA b³êdu

diqA = (2/sqrt(3))*sqrt(dia*dia + dia*dib + dib+dib);

delta_T = diqA*K_t;
alfa = atan(sqrt(3)*dia / (dia + 2*dib));


%%
AW_Disturbance_Harmonics;

%% test period

PERIOD = 10; % [s]
SEED = 123;

%% GA optimization

PID_GA_Kp = 4.772;
PID_GA_Ki = 4.772/(0.153/1000);
PID_GA_Kd = 4.772*(88.300/1000);
PID_GA_b = 1;
PID_GA_c = 0.258;

PID_GA_Gains = [PID_GA_b*PID_GA_Kp -PID_GA_Kp PID_GA_Ki PID_GA_c*PID_GA_Kd -PID_GA_Kd];

