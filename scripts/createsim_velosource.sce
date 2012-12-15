// This script sets up a Lambda sound propagation simulation.
// This is a more advanced setup with generates two antiphasic rows of 
// velocity sources.
// Refer to the manual for further information.
//
//  Marco Ruhland, IHA Oldenburg, 2009

// Enter sim file name here
FILENAME = 'example_velosource.sim';

// Configure simulation here
YSIZE=400;   // set y-size of simulation environment in nodes
XSIZE=400;   // set x-size of simulation environment in nodes
STEPS=1000;  // set # of iterations
C=340;       // set sound propagation speed in m/s
L=0.0109;    // set node distance (tube length) in m
RHO=1.204;   // set air density in kg/m^3

// Define environment here
ENVI=zeros(YSIZE,XSIZE);  // create empty space

ENVI(200,150:250)=1;      // create a rigid wall between the velocity source rows

// Define Angular matrix here
ENVIANGLE=zeros(YSIZE,XSIZE);

ENVIANGLE(199,150:250)=270; // set main direction for upper velo sources to 270 degrees
                            // (that means scattering to the top)
ENVIANGLE(201,150:250)=90;  // set main direction for lower velo sources to 90 degrees
                            // (that means scattering to the bottom)

// Define Sources here
sources=[];
for kk=150:250
    actsource=[199 kk 6 0.001727 300 0]; // upper row of velo sources
    sources=[sources actsource];
end
for kk=150:250
    actsource=[201 kk 6 0.001727 300 180]; // lower row of velo sources, antiphasic
    sources=[sources actsource];
end

// All done, execute this script!
///////////////////////////////////////////////////////////////////////////

// in this example, the FLT chunk is missing!
simFile = mopen(FILENAME,'wb');
mputstr('LAMBDASIM200',simFile);
mputstr('DEF',simFile);
mput([YSIZE XSIZE STEPS C L RHO],'dl',simFile);
mputstr('ENV',simFile);
mput(ENVI','dl',simFile);
mputstr('ANG',simFile);
mput(ENVIANGLE','dl',simFile);
mputstr('SRC',simFile);
mput(floor(length(sources)./6),'dl',simFile);
mput(sources,'dl',simFile);
mclose(simFile);

