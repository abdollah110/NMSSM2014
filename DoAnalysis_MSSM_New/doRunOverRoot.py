#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
#http://root.cern.ch/viewvc/trunk/tutorials/pyroot/hsimple.py?revision=20881&view=markup
__author__ = "abdollahmohammadi"
__date__ = "$Feb 23, 2013 10:39:33 PM$"

import os

InputFileLocation = '../FileROOT/MSSMROOTFiles/'
OutPutFileLocation = 'OutFiles/'
Sample = os.popen(("ls " + InputFileLocation + " | sort "))
OutFile = open("RunFullSamples.txt", 'w')
firstCommand = "./Make.sh Estimate_Signal.cc\n"
firstCommand += "rm OutFiles/*.root \n"
OutFile.write(firstCommand)
outCommand = ""
for files in Sample.readlines():
    outCommand = outCommand + "./Estimate_Signal.exe  " + OutPutFileLocation + "out_" + files.replace('\n','') + " " + InputFileLocation + files
outCommand = outCommand + "root -l -q hadd.C \n"
outCommand = outCommand + "root -l -q doStitching.cc \n"
outCommand = outCommand + "python do2DTableMaker.py \n"
outCommand = outCommand + "python doSystematics2DTable.py \n"
outCommand = outCommand + "python doMeasureShapeNormQCD.py \n"
#outCommand = outCommand + "python doFakeRateForQCD.py \n"
#outCommand = outCommand + "python doFakeRateForQCD_2.py \n"
outCommand = outCommand + "python doHistogram.py \n"

OutFile.write(outCommand)
    

