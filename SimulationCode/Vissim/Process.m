%% Import data from text file

result=[];
for t=1:10
%% Set up the Import Options and import the data

opts = delimitedTextImportOptions("NumVariables", 4);

% Specify range and delimiter
opts.DataLines = [15, Inf];
opts.Delimiter = ";";

% Specify column names and types
opts.VariableNames = ["SimSec", "No", "Speed", "VehType"];
opts.VariableTypes = ["double", "double", "double", "double"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, ["SimSec", "No", "Speed", "VehType"], "ThousandsSeparator", ",");
if t==10
    I75001 = readtable("E:\Programing\FHWA\FHWA project\Result1006\I-75_0"+t+".fzp", opts);
else 
% Import the data
    I75001 = readtable("E:\Programing\FHWA\FHWA project\Result1006\I-75_00"+t+".fzp", opts);
end

%% Clear temporary variables
clear opts

%% Calculation
hv=[];
av=[];
all=[];
I75001=I75001(I75001.SimSec>1200,:);
for i=1:max(I75001.No)
    datai=I75001.Speed(I75001.No==i);
    s=std(datai);
    type=max(I75001.VehType(I75001.No==i));
    if(type==640)
        hv=[hv,s];
        all=[all,s];
    elseif(type==630)
        av=[av,s];
        all=[all,s];
    end
end
t
result=[result;[mean(av),mean(hv),mean(all)]];
end