import math

import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH2F
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import TTree
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gStyle
from ROOT import gSystem
from ROOT import TLegend
import os

# 2 changes in 25 Nov

outRootDir = outRootDir = '../ROOTFiles_V3/Results/'
#outRootDir = outRootDir = '..//ROOTFiles_V3/ROOTDebugScale_7TeV/'
inRootDir = "outRoot_V3/"
limitDir = "limitDir/"

signal = ['zhtt', 'zhww']
mass = [90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160]
BackGround = ['ZZ4L', 'GGToZZ2L2L', 'TTZJets', 'Data']

FitType = ["Visible_","SV_"] #Two types of the limit

def DoTheOptimization(Channel, tauIsolation, leptonIsolation, LTCut,CoMEnergy):
    print "\n\n*********************" , Channel
    OutFile = open("RunOpt_" + Channel + "_HighLT.txt", 'w')
    firstCommand = "./Make.sh Val1_FullHisto.cc\n"
    OutFile.write(firstCommand)
    for cut1 in tauIsolation:
        for cut2 in leptonIsolation:
            for cut3 in LTCut:
                outCommand = ""
                outCommand = "rm *.root\n"
                outCommand = outCommand + "rm " + inRootDir + "*.root\n"
                ###################################### Filling Signal ZH and WH ########
                for energy_i in CoMEnergy:
                    for sig in signal:
                        for m in range(len(mass)):#    for m in range(110, 145, 5):
                            outCommand = outCommand + "./Val1_FullHisto.exe  " + Channel + " " + str(cut1) + " " + str(cut2) + " " + str(cut3) + " "
                            outCommand = outCommand + inRootDir + str(sig) + str(mass[m]) + str(energy_i)+ ".root  " + outRootDir + str(sig) + str(mass[m]) +str(energy_i)+ ".root\n"

                    #######################################   Filling BG and Data  #########
                    for bg in range (len(BackGround)):
                        outCommand = outCommand + "./Val1_FullHisto.exe  " + Channel  + " " + str(cut1) + " " + str(cut2) + " " + str(cut3) + " "
                        outCommand = outCommand + inRootDir + str(BackGround[bg]) +str(energy_i)+ ".root  " + outRootDir + str(BackGround[bg]) +str(energy_i)+ ".root\n"

                    outCommand = outCommand + "cp  "+inRootDir+"Data"+str(energy_i)+".root valid_Mega.root \n"
                    outCommand = outCommand + "root .x -q Val2_FRMeasure.cc \n"
                    outCommand = outCommand + "root .x -q Val3_BGestimation.cc \n"
                    outCommand = outCommand + "mv "+ 'Reducible.root  ' + 'Reducible'+str(energy_i)+'.root\n'
                    outCommand = outCommand + "cp -r "+ 'results  ' + 'results'+str(energy_i)+'\n'


                outCommand = outCommand + "python make_Results_2DTable.py \n"
                outCommand = outCommand + "python make_Results_Histograms.py \n"
#                outCommand = outCommand + "python make_Results_HistogramsNewSV_Vegas.py \n"
                outCommand = outCommand + "python make_Results_Tex.py \n"
                OutFile.write(outCommand)
                
if __name__ == "__main__":

    tauIsolation = ["Total"]
    leptonIsolation = [0.3]
    LTCut = [0]
    comEnergy= ["_7TeV","_8TeV"]
#    comEnergy= ["_7TeV"]
    DoTheOptimization("Total", tauIsolation, leptonIsolation, LTCut,comEnergy)

