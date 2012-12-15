// This script is a simple converter for old Lambda 1.x-simfiles to the new
// Lambda 2.0 format.

// Marco Ruhland, IHA Oldenburg, 2009

INFILENAME='silencer.sim';   // name of the old 1.x-simfile (input)
OUTFILENAME='silencer2.sim'; // name of the new 2.0 simfile (output)

infile=mopen(INFILENAME,'rb'); // open the old 1.x-simfile
h=mgetstr(12,infile)'; // read the 1st 12 characters
mclose(infile); // close the 1.x-simfile
if h=='LAMBDASIM200' // =='LAMBDASIM200' ???
    disp('The input file is already a Lambda 2.0 sim file!'); // yes, already converted!
else
    infile=mopen(INFILENAME,'rb'); // open the old 1.x-simfile again
    m=mget(5,'dl',infile); // read the definitions
    YSIZE=m(1);
    XSIZE=m(2);
    STEPS=m(3);
    C=m(4);
    L=m(5);
    RHO=1.204; // in Lambda 2.0, rho is new. This is missing in 1.x-simfiles.
               // if you want another value for rho, then change it here.
    ENVI=[];
    for kk=1:YSIZE           
      e=mget(XSIZE,'dl',infile)'; // read the simulation environment
      ENVI=[ENVI e];
    end
    ENVI=ENVI';
    sources=[];
    while (meof(infile)==0)
      s=mget(1,'dl',infile); // read the sources
      sources=[sources s];
    end
    mclose(infile); // close the 1.x-simfile

    outfile=mopen(OUTFILENAME,'wb'); // create a new 2.0 simfile
    mputstr('LAMBDASIM200',outfile); // write the header
    mputstr('DEF',outfile);          // write the DEF chunk:
    mput([YSIZE XSIZE STEPS C L RHO],'dl',outfile); // def chunk data
    mputstr('ENV',outfile);  // write the ENV chunk:
    mput(ENVI','dl',outfile); // env chunk data (simulation environment)
    mputstr('SRC',outfile);                        // write the SRC chunk:
    mput(floor(length(sources)./6),'dl',outfile);  // length of src data (new in 2.0)
    mput(sources,'dl',outfile);                    // src data
    mclose(outfile);                   // close the new 2.0 simfile
    disp('Conversion successful.');    // here we go.
end

