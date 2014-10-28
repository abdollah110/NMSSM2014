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
firstCommand += "mv *.root *.pdf  OLD/\n"
OutFile.write(firstCommand)
outCommand = ""
for files in Sample.readlines():
    outCommand = outCommand + "./Estimate_Signal.exe  " + OutPutFileLocation + "out_" + files.replace('\n','') + " " + InputFileLocation + files
outCommand = outCommand + "root -l -q Step1_Hadding_TT_VV_Z.C \n"
outCommand = outCommand + "hadd -f  OutFiles/out_DYJetsAll_8TeV_Hadd.root   OutFiles/DYJetsToLL.root   OutFiles/DY1JetsToLL.root   OutFiles/DY2JetsToLL.root   OutFiles/DY3JetsToLL.root   OutFiles/DY4JetsToLL.root    \n"
outCommand = outCommand + "hadd -f  OutFiles/out_WJetsAll_8TeV_Hadd.root   OutFiles/WJetsToLNu.root   OutFiles/W1JetsToLNu.root   OutFiles/W2JetsToLNu.root   OutFiles/W3JetsToLNu.root   OutFiles/W4JetsToLNu.root    \n"
outCommand = outCommand + "root -l -q Step2_Stitching_DY.cc \n"
#outCommand = outCommand + "root -l -q Step3_Stitching_W.cc \n"
outCommand = outCommand + "python Step4_CalcNormalization.py \n"
outCommand = outCommand + "python Step5_SystematicSignal.py \n"
outCommand = outCommand + "python Step6_CalcQCDEstimation.py \n"
#outCommand = outCommand + "python Step7_MakeRootDataCards.py \n"
outCommand = outCommand + "python Step8_CopyStep7.py \n"

OutFile.write(outCommand)
    