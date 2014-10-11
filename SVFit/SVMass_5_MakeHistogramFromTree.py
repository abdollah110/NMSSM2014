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



import math
import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH2F
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gSystem

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
import os
import shutil

n_bin = 400
low_bin = 0
high_bin = 400
reb_ = high_bin / n_bin

#Year = ["8TeV","7TeV"]
FinalState = ['mmtt','mmet','mmmt','mmme','eett','eemt','eeet','eeem']



channel = ["mmtt", "mmet", "mmmt", "mmme", "eett", "eemt", "eeet", "eeem"]
signal = ['zhtt', 'zhww']
mass = [90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160]
BackGround = ['ZZ4L', 'GGToZZ2L2L', 'TTZJets']
#BackGround = ['ZZ4L', 'GGToZZ2L2L', 'TTZJets', 'Data','DY']
ScaleValue = [-0.03, 0, 0.03]
ScaleName = ["_Down", "", "_Up"]
#FilesFromLxplus= "VegasFiles_1"
FilesFromLxplus= "VegasFiles_2"


#This for Making Histogram from Tree given by SVMass
#Data_7TeVeeem_tot0.root
#SVMass_mmtt_pp

def HistoFromTree(CoMEnergy):
#    for chl in range(len(channel)):
#        print "starting channel", channel[chl]
        ###################################### Filling Signal ZH and WH ########
    for sig in range(len(signal)):
        for m in range(len(mass)):#    for m in range(110, 145, 5):
            print "Processing ", str(signal[sig]) + str(mass[m])+"_" +CoMEnergy
            FinalFile=  TFile(str(signal[sig]) + str(mass[m])+"_" +CoMEnergy+".root","RECREATE")
            for state in FinalState:
                for scale in range(len(ScaleValue)):
                    FinalFile.cd()
                    dataHist = TH1F("SVMass_"+state+"_pp"+ScaleName[scale],"",n_bin,low_bin,high_bin)
                    dataFile= TFile(FilesFromLxplus+"/"+str(signal[sig]) + str(mass[m])+"_"+CoMEnergy+state+"_tot"+str(ScaleValue[scale])+".root")
                    dataTree = dataFile.Get("Mass_tree")
                    for entry in xrange(dataTree.GetEntries()):
                        dataTree.GetEntry(entry)
                        dataHist.Fill(dataTree.SVmass)
    #                        print "dataTree.SVmass = ", dataTree.SVmass
                    FinalFile.Write()

    for bg in range (len(BackGround)):
        print "Processing ",str(BackGround[bg]) +"_"+ CoMEnergy
        FinalFile = TFile( str(BackGround[bg]) +"_"+ CoMEnergy+'.root',"RECREATE")
        for state in FinalState:
            for scale in range(len(ScaleValue)):
                FinalFile.cd()
                dataHist = TH1F("SVMass_"+state+"_pp"+ScaleName[scale],"",n_bin,low_bin,high_bin)
                dataFile= TFile(FilesFromLxplus+"/"+ str(BackGround[bg])+"_"+CoMEnergy+state+"_tot"+str(ScaleValue[scale])+".root")
                dataTree = dataFile.Get("Mass_tree")
                for entry in xrange(dataTree.GetEntries()):
                    dataTree.GetEntry(entry)
                    dataHist.Fill(dataTree.SVmass)
#                        print "dataTree.SVmass = ", dataTree.SVmass
                FinalFile.Write()
            
if __name__ == "__main__":
#    HistoFromTree("8TeV")
    HistoFromTree("7TeV")


#
#    for year in Year:
#        FinalFile=  TFile("ZZ4L_"+year+".root","RECREATE")
#        for state in FinalState:
#            FinalFile.cd()
#            dataHist = TH1F("SVMass_"+state+"_pp","",n_bin,low_bin,high_bin)
#            dataFile= TFile("VegasFiles/ZZ4L_"+year+state+"_tot0.root")
#            dataTree = dataFile.Get("Mass_tree")
#
#            for entry in xrange(dataTree.GetEntries()):
#                dataTree.GetEntry(entry)
#                dataHist.Fill(dataTree.SVmass)
#                print "dataTree.SVmass = ", dataTree.SVmass
#            FinalFile.Write()

            

   