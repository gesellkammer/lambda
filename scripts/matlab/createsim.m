% This script sets up a Lambda sound propagation simulation.
% This is a simple setup without filters, without angular preemphasis
% and without velocity sources.
% Refer to the manual for further information.
%
%  Simon Ahrens, MB, IHA Oldenburg, 2006
%  Marco Ruhland, IHA Oldenburg, 2009

% Enter sim file name here
FILENAME = 'example.sim';

fs = 8000; 
% Configure simulation here
YSIZE=400;   % set y-size of simulation environment in nodes
XSIZE=400;   % set x-size of simulation environment in nodes
STEPS=100;  % set # of iterations
C=340;       % set sound propagation speed in m/s
% L=0.0109;    % set node distance (tube length) in m
% L = C * sqrt(2) / fs ;
L = C*sqrt(2) / fs;
RHO=1.204;   % set air density in kg/m^3

% Define environment here
ENVI=zeros(YSIZE,XSIZE);  % create empty space

%ENVI(1,1:400)=1;       % upper border is a rigid wall
%ENVI(400,1:400)=-1;    % lower border is a wall with 180 deg phase shift
%ENVI(1:400,1)=0;       % left border is none-reflective
%ENVI(1:400,400)=0.5;   % right border is a wall with r=0.5

%ENVI(300,300)=-2;     % place a receiver at y=300, x=300
                       % the data will be recorded when you check the
                       % "rce" checkbox in lambda, or use "-rce" command
                       % line option. Use the script "readrce.m" to load
                       % the receiver data.

% Define sources here
SOURCE1=[200 200 1 1 10 270]; % y=200, x=200, sine, amp=1, f=700 Hz, p=0 deg.
% SOURCE2=[200 100 1 1 1 180]; % y=200, x=100, rect, amp=0.5, f=700 Hz, p=180 deg.

sources=[SOURCE1]; %  SOURCE2];

% All done, execute this script!
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% in this example, the ANG chunk and the FLT chunk are missing!
simFile = fopen(FILENAME,'w');
fwrite(simFile,'LAMBDASIM200','uchar');
fwrite(simFile,'DEF','uchar');
fwrite(simFile,[YSIZE XSIZE STEPS C L RHO],'double');
fwrite(simFile,'ENV','uchar');
fwrite(simFile,ENVI','double');
fwrite(simFile,'SRC','uchar');
fwrite(simFile,floor(length(sources)./6),'double');
fwrite(simFile,sources,'double');
fclose(simFile);
