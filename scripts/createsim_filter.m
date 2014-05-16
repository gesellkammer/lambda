% This script sets up a Lambda sound propagation simulation.
% This is an advanced setup which demonstrates how to use digital filters
% as walls. In the simulation you see three columns. The first column is
% fed with a 500 Hz sinusoidal source array, the second column is fed
% with 1500 Hz and the third column is fed with 2500 Hz.
% At the lower end of the columns, there is a horizontal wall made of a
% 6th order digital bandpass filter with the corner frequencies 1000 and
% 2000 Hz. In the 1500 Hz column (middle), the signal is therefore almost
% totally reflected, while in the outer two colums the signal is passing
% through, because the source frequencies 500 and 2500 Hz are outside the
% filter's passband. This results in a standing wave in the middle column
% and two floating/passing waves in the outer columns.
% Refer to the manual for further information.
%
% In Lambda 2.0, a digital filter transfer function is defined in the form:
%
%               b0 + b1*z^-1 + b2*z^-2 + ... + bn*z^-n
%       H(z) = ----------------------------------------
%               a0 + a1*z^-1 + a2*z^-2 + ... + an*z^-n
%
%  Marco Ruhland, IHA Oldenburg, 2009

% Enter sim file name here
FILENAME = 'example_filter.sim';

% Define a sampling frequency (in Hz)
fs=44100;

% Configure simulation here
YSIZE=300;   % set y-size of simulation environment in nodes
XSIZE=300;   % set x-size of simulation environment in nodes
STEPS=1000;  % set # of iterations
C=340;       % set sound propagation speed in m/s
L=C*sqrt(2)/fs; % set node distance (tube length) in m
                % it is important to know that tube length, sound speed
                % and sampling frequency are closely linked together in
                % lambda! L=C*sqrt(2)/fs
RHO=1.204;   % set air density in kg/m^3

% Define environment here
ENVI=zeros(YSIZE,XSIZE);  % create empty space

ENVI(1:YSIZE,1)=1;      % make some vertical walls as separators between
ENVI(1:YSIZE,100)=1;    % the three columns
ENVI(1:YSIZE,200)=1;    % .
ENVI(1:YSIZE,300)=1;    % .

ENVI(200,1:XSIZE)=2;    % define the lower, horizontal digital filter
                        % wall. "2" is the ID of the filter.

% Define angular matrix here
ENVIANGLE=zeros(YSIZE,XSIZE);

ENVIANGLE(200,1:XSIZE)=90; % tell the filter wall to work correctly for
                           % sonic impact of 90 degrees direction (from
                           % top) (This also implies 270 deg direction).

% Define sources here
sources=[];
for kk=2:99
    actsource=[1 kk 5 1 500 0];  % define the 500 Hz sources
    sources=[sources actsource];
end
for kk=101:199
    actsource=[1 kk 5 1 1500 0]; % define the 1500 Hz sources
    sources=[sources actsource];
end
for kk=201:299
    actsource=[1 kk 5 1 2500 0]; % define the 2500 Hz sources
    sources=[sources actsource];
end

% Define digital filter(s) here
filters=[];
NUMFILTERS=1;       % we only need one filter
actfilter_id=2;     % the filter gets the ID "2"
N   = 6;            % Order of the filter
fc1 = 1000;         % First Cutoff Frequency
fc2 = 2000;         % Second Cutoff Frequency
[b,a] = butter(N/2, [fc1 fc2]/(fs/2));  % design the filter (butterworth design)
actfilter_a=a;
actfilter_b=b;
filters=[filters actfilter_id length(actfilter_a) length(actfilter_b) actfilter_a actfilter_b];

% All done, execute this script!
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% in this example, you see the complete writing procedure, including
% the FLT chunk and the ANG chunk.
simFile = fopen(FILENAME,'w');
fwrite(simFile,'LAMBDASIM200','uchar');
fwrite(simFile,'DEF','uchar');
fwrite(simFile,[YSIZE XSIZE STEPS C L RHO],'double');
fwrite(simFile,'ENV','uchar');
fwrite(simFile,ENVI','double');
fwrite(simFile,'ANG','uchar');
fwrite(simFile,ENVIANGLE','double');
fwrite(simFile,'FLT','uchar');
fwrite(simFile,NUMFILTERS,'double');
fwrite(simFile,filters,'double');
fwrite(simFile,'SRC','uchar');
fwrite(simFile,floor(length(sources)./6),'double');
fwrite(simFile,sources,'double');
fclose(simFile);
