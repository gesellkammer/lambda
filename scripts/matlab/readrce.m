% This script reads Lambda receiver output files and puts
% the receiver data into a matrix called REC. 
% Refer to the manual for further information.
%
% Simon Ahrens, IHA Oldenburg, 2006

% Enter the file name here
FILENAME = 'example.rce';

% All done, execute this script!
%-------------------------------------------------------------------

REC=[];
outFile = fopen(FILENAME,'rb');
if outFile~=-1
    content = fread(outFile,'double');
    fclose(outFile);
    if (length(content)>1) && (content(1)>0)
        nRec = content(1); recVec = content(2:end);
        for n=1:floor(length(recVec)/nRec) 
            REC(:,n) = recVec((n-1)*nRec+1:n*nRec);
        end
    end 
end
