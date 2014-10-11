#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
#    ************************************************************************************
# To run the svMass one need the following steps
# 1) put all the root files in a directory: InitialRootLocation = "/afs/cern.ch/work/a/abdollah/SVFit/8TeV_ROOT/"
# 2) ls *.root > textSamples.txt  (this is the name of all rootfiles to be processes)
# 3) put the textSamples.txt in the current directory
# 4) Run the python code :   python SVMass_Batch_Submit.py
# 5) the above python command produces a txt file (Prepare_JobsSubmission.sh) and a directory (OutRoot)
#  where all the submission files are there
# 6) Please first check them to be sure
# 7) Last step is soursing the script t submit the jobs ion lxbatch  "src Prepare_JobsSubmission.sh"
#    ************************************************************************************


__author__ = "abdollah"
__date__ = "$Feb 23, 2010 5:15:54 PM$"

from array import array
import math

import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH2F
from ROOT import TLatex
from ROOT import TLegend
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import TTree
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gStyle
from ROOT import gSystem
import numpy as n
import os

if __name__ == "__main__":

#    location = os.getcwd()
#
#    OutLocation = "ALLROOTVegas"
#    if os.path.exists(OutLocation):
#        shutil.rmtree(OutLocation)
#    if not os.path.exists(OutLocation):
#        os.makedirs(OutLocation)

#    TextSamples = open("testSamples.txt", "r")

    FinalState = ['mmtt_tot','mmet_tot','mmmt_tot','mmme_tot','eett_tot','eemt_tot','eeet_tot','eeem_tot']
    ChannelNumber = [1,2,3,4,5,6,7,8]
    ScaleValue = [0]

    OutRoot = "ALLROOTVegas"
#    TextSamples = open("NNN.txt", "r")
#    for Sample in TextSamples.readlines():
#    for Sample in TextSamples.readlines():


#        Tag = Sample[:-6]
    Tag = "ZZ4L_8TeV"
    InitialRootLocation =  "outRoot_V3/" + Tag   # for lxplus
    for chanl in range(len(FinalState)):
        for scale in ScaleValue:


            isbuggy = 0
            myName= Tag+str(FinalState[chanl]) +str(scale)+ ".root"
        #        RootLocation =  "Results/" + Sample    # For M Machines
    #        InitialRootLocation = "/afs/cern.ch/work/a/abdollah/SVFit/8TeV_ROOT/"+ Sample   # for lxplus
            InitialRootLocation =   OutRoot + "/"+myName   # for lxplus
#                print InitialRootLocation
            FileMass = ROOT.TFile.Open(InitialRootLocation, "READ")
    #        FileMass = ROOT.TFile.Open("ZZ4L_8TeVmmmt_tot0.root", "READ")
            firstTr = FileMass.Get("Mass_tree")
    #
            for entry in xrange(firstTr.GetEntries()):
    #                if (entry % 100 == 0): print "Entry is : ", entry
                    firstTr.GetEntry(entry)
                    if firstTr.Chanel_ == 0:
                        isbuggy = 1
            if isbuggy == 1:
                print "___________________________", myName
#                firstTr.Close()
#                FileMass.Close()
        
        
