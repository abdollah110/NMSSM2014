import os

InputFileLocation = '../FileROOT/NewROOTFiles/'
OutPutFileLocation = 'OutFiles/'
Sample = os.popen(("ls " + InputFileLocation + " | sort "))
OutFile = open("RunFullSamples.txt", 'w')
firstCommand = "./Make.sh Estimate_Signal.cc\n"
OutFile.write(firstCommand)
outCommand = ""
for files in Sample.readlines():
    outCommand = outCommand + "./Estimate_Signal.exe  " + OutPutFileLocation + "out_" + files.replace('\n','') + " " + InputFileLocation + files + "\n"
OutFile.write(outCommand)
    

