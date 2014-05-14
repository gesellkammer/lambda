// This script sets up a Lambda sound propagation simulation.
// This is an advanced setup which demonstrates how to use digital filters
// as walls. In the simulation you see three columns. The first column is
// fed with a 500 Hz sinusoidal source array, the second column is fed
// with 1500 Hz and the third column is fed with 2500 Hz.
// At the lower end of the columns, there is a horizontal wall made of a
// 2nd order digital bandpass filter with the corner frequencies 1000 and
// 2000 Hz. In the 1500 Hz column (middle), the signal is therefore almost
// totally reflected, while in the outer two colums the signal is passing
// through, because the source frequencies 500 and 2500 Hz are outside the
// filter's passband. This results in a standing wave in the middle column
// and two floating/passing waves in the outer columns.
// See also the Matlab example createsim_filter.m for a 6th order design.
// Refer to the manual for further information.
//
// In Lambda 2.0, a digital filter transfer function is defined in the form:
//
//               b0 + b1*z^-1 + b2*z^-2 + ... + bn*z^-n
//       H(z) = ----------------------------------------
//               a0 + a1*z^-1 + a2*z^-2 + ... + an*z^-n
//
//  Marco Ruhland, IHA Oldenburg, 2009

// Enter sim file name here
FILENAME = 'example_filter.sim';

// Define a sampling frequency (in Hz)
// fs=44100;
fs=44100;

// Configure simulation here
YSIZE=300;   // set y-size of simulation environment in nodes
XSIZE=300;   // set x-size of simulation environment in nodes
STEPS=2 * 44100;  // set # of iterations
C=340;       // set sound propagation speed in m/s
L=C*sqrt(2)/fs; // set node distance (tube length) in m
                // it is important to know that tube length, sound speed
                // and sampling frequency are closely linked together in
                // lambda! L=C*sqrt(2)/fs
RHO=1.204;   // set air density in kg/m^3

// Define environment here
ENVI=zeros(YSIZE,XSIZE);  // create empty space

ENVI(1:YSIZE,1)=1;      // make some vertical walls as separators between
ENVI(1:YSIZE,100)=1;    // the three columns
ENVI(1:YSIZE,200)=1;    // .
ENVI(1:YSIZE,300)=1;    // .

ENVI(200,1:XSIZE)=4;    // define the lower, horizontal digital filter
                        // wall. "4" is the ID of the filter.
ENVI(198, 150) = -2;
						
// Define angular matrix here
ENVIANGLE=zeros(YSIZE,XSIZE);

ENVIANGLE(200,1:XSIZE)=90; // tell the filter wall to work correctly for
                           // sonic impact of 90 degrees direction (from
                           // top) (This also implies 270 deg direction).

// Define sources here
sources=[];
for kk=2:99
    actsource=[1 kk 5 1 500 0];  // define the 500 Hz sources
    sources=[sources actsource];
	actsource=[10 kk 5 0.1 750 0];  // define the 500 Hz sources
	sources=[sources actsource];
end
for kk=101:199
    actsource=[1 kk 5 1 1500 0]; // define the 1500 Hz sources
    sources=[sources actsource];
end
for kk=201:299
    actsource=[1 kk 5 1 2500 0]; // define the 2500 Hz sources
    sources=[sources actsource];
end

// Define digital filter(s) here
filters=[];
NUMFILTERS=1;       // we only need one filter
actfilter_id=4;     // the filter gets the ID "4"
// b=[0.066605780250182 0 -0.066605780250182];
// a=[1 -1.828957870144994 0.866788439499635];
// b=[0.9879197865753899, -1.9758395731507798, 0.9879197865753899]
b = [0.784791786136 0.0 -3.92395893068 1.75987614775e-15 7.84791786136 -2.74980648086e-15 -7.84791786136 1.75987614775e-15 3.92395893068 0.0 -0.784791786136]

// a=[1 -1.9657784768560354, 0.9859006694455243]
a = [1.0 0.437964225605 -4.42179846068 -1.53136015393 7.93807725797 2.02775078487 -7.21710317414 -1.20335180505 3.3174669317 0.26974135532 -0.615898147584]
actfilter_a=a;
actfilter_b=b;
filters=[filters actfilter_id length(actfilter_a) length(actfilter_b) actfilter_a actfilter_b];

// All done, execute this script!
///////////////////////////////////////////////////////////////////////////

// in this example, you see the complete writing procedure, including
// the FLT chunk and the ANG chunk.
simFile = mopen(FILENAME,'wb');
mputstr('LAMBDASIM200',simFile);
mputstr('DEF',simFile);
mput([YSIZE XSIZE STEPS C L RHO],'dl',simFile);
mputstr('ENV',simFile);
mput(ENVI','dl',simFile);
mputstr('ANG',simFile);
mput(ENVIANGLE','dl',simFile);
mputstr('FLT',simFile);
mput(NUMFILTERS,'dl',simFile);
mput(filters,'dl',simFile);
mputstr('SRC',simFile);
mput(floor(length(sources)./6),'dl',simFile);
mput(sources,'dl',simFile);
mclose(simFile);

