%%% File info
%
% ************************************************************************
%
% %  @file  AW_RippleModel_Init.m
%  @author  Adrian Wojcik
%  @version V1.0
%  @date    10-Mar-2025
%  @brief   Parameteres torque ripples model
%
% ************************************************************************

% syms F1s F2s F3s F4s th iq
% 
% % flux harmonics
% 
% F1 = @(theta, iq)( 0.7*(0.0274*iq+0.0438)*Tn*sin(6*(theta*p))+(0.0274*iq+0.0438)*0.07*Tn*sin(12*(theta*p)) );
% F1s = F1(th, iq); vpa(F1s)   ;
% 
% f1 = @(theta, iq)( sin(288.0*th)*(0.0959*iq + 0.1533) + sin(144.0*th)*(0.959*iq + 1.533) );
% 
% % coggingg torque
% 
% F2 = @(theta)( 1.1*0.02*Tn*sin(9*24*theta) );
% F2s = F2(th); vpa(F2s)       ;
% 
% f2 = @(theta)( 1.1*sin(216.0*th) );
% 
% % offset current error
% 
% F3 = @(theta)( delta_T*cos((theta*p)+alfa) );
% F3s = F3(th); vpa(F3s)        ;
% 
% f3 = @(theta)( 0.28578809165767099509380955169036*cos(24.0*th + pi/6) );
% 
% % offset scaling error
% 
% F4 = @(theta, iq)( (1.01-0.99)/(1.01*0.99)*K_t/sqrt(3)*iq*cos(2*(theta*p)+pi/3)+(1.01-0.99)/(1.01*0.99)*K_t/2*sqrt(3)*iq );
% F4s = F4(th, iq); vpa(F4s)     ;
% 
% f4 = @(theta, iq)( 0.30313920524507825815518913259439*iq + 0.20209280349671887577756024256814*iq*cos(48.0*th + pi/3) );

%% SENE 2011

% only angular position
D1 = @(theta)    ( 1.1000*sin(216*theta) );                           % coggingg torque
D2 = @(theta)    (-0.2857*sin( 24*theta - pi/3));                     % offset current error
%D2 = @(theta)    ( 0.2857*cos( 24*theta + pi/6));                     % offset current error

% angular position and current
%D3 = @(theta, iq)( iq*( 0.0959*cos(288*theta) + 0.959*cos(144*theta)) ); % flux harmonics
D3 = @(theta, iq)( iq*(-0.0959*sin(288*theta - pi/2) - 0.959*sin(144*theta - pi/2)) ); % flux harmonics
D4 = @(theta, iq)( iq*(-0.2021*(sin(48*theta + pi/6) - 1/2)) );               % offset scaling error
%D4 = @(theta, iq)( iq*( 0.2021*cos(48*theta + pi/3)) );               % offset scaling error

%%
 d1param = [1.1000 -0.2857];       %% ideal params
%d1param = solution(1:2);
d1 = @(theta) (d1param * [sin(216*theta); sin( 24*theta - pi/3)]);
d2param = [-0.0959 -0.959 -0.2021]; %% ideal params
%d2param = solution(3:end);
d2 = @(theta, iq) (iq * d2param * [sin(288*theta - pi/2); sin(144*theta - pi/2); sin(48*theta + pi/6) - 1/2]);
