% This script is a simple converter for old Lambda 1.x-simfiles to the new
% Lambda 2.0 format.

% Marco Ruhland, IHA Oldenburg, 2009

INFILENAME='array.sim'; % name of the old 1.x-simfile (input)
OUTFILENAME='array2.sim'; % name of the new 2.0 simfile (output)

infile=fopen(INFILENAME,'rb'); % open the old 1.x-simfile
h=fread(infile,12,'uchar')'; % read the 1st 12 characters
fclose(infile); % close the 1.x-simfile
if h==[76 65 77 66 68 65 83 73 77 50 48 48] % =='LAMBDASIM200' ???
    disp('The input file is already a Lambda 2.0 sim file!'); % yes, already converted!
else
    infile=fopen(INFILENAME,'rb'); % open the old 1.x-simfile again
    m=fread(infile,5,'double'); % read the definitions
    YSIZE=m(1);
    XSIZE=m(2);
    STEPS=m(3);
    C=m(4);
    L=m(5);
    RHO=1.204; % in Lambda 2.0, rho is new. This is missing in 1.x-simfiles.
               % if you want another value for rho, then change it here.
    ENVI=fread(infile,[YSIZE,XSIZE],'double')'; % read the simulation environment
    sources=fread(infile,'double'); % read the sources
    fclose(infile); % close the 1.x-simfile

    outfile=fopen(OUTFILENAME,'w'); % create a new 2.0 simfile
    fwrite(outfile,'LAMBDASIM200','uchar'); % write the header
    fwrite(outfile,'DEF','uchar');          % write the DEF chunk:
    fwrite(outfile,[YSIZE XSIZE STEPS C L RHO],'double'); % def chunk data
    fwrite(outfile,'ENV','uchar');  % write the ENV chunk:
    fwrite(outfile,ENVI','double'); % env chunk data (simulation environment)
    fwrite(outfile,'SRC','uchar');                       % write the SRC chunk:
    fwrite(outfile,floor(length(sources)./6),'double');  % length of src data (new in 2.0)
    fwrite(outfile,sources,'double');                    % src data
    fclose(outfile);                   % close the new 2.0 simfile
    disp('Conversion successful.');    % here we go.
end
