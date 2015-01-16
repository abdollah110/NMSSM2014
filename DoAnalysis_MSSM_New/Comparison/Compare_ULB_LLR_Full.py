#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="abdollahmohammadi"
__date__ ="$Jul 1, 2014 10:11:41 PM$"

import ROOT
from ROOT import TFile
from ROOT import TH1F
from ROOT import TDirectory
from ROOT import gDirectory
from ROOT import TKey
from ctypes import *
import array




#category=["_inclusive", "_nobtag_low", "_nobtag_medium", "_nobtag_high", "_btag_low", "_btag_high"]
category=["_inclusive", "_btag"]
channel=["muTau","eleTau"]


#def GetLLR_Channel(chan):
#    if chan=="muTau": return "Datacards_101214/htt_mt.inputs-mssm-8TeV-0.root"
#    elif chan == "eleTau":   return "Datacards_101214/htt_et.inputs-mssm-8TeV-0.root"
#    else: print   "!!!!!!!      LLR Name is not correct"
#
#def GetULB_Channel(chan):
#    if chan=="muTau": return "TotalRootForLimit_mutau_8TeV.root"
#    elif chan == "eleTau":   return "TotalRootForLimit_etau_8TeV.root"
#    else: print   "!!!!!!!      ULB Name is not correct"

def GetLLR_Channel(chan):
    if chan=="muTau": return "OLD/TotalRootForLimit_mutau_8TeV.root"
    elif chan == "eleTau":   return "OLD/TotalRootForLimit_etau_8TeV.root"
    else: print   "!!!!!!!      LLR Name is not correct"

def GetULB_Channel(chan):
    if chan=="muTau": return "New/TotalRootForLimit_mutau_8TeV.root"
    elif chan == "eleTau":   return "New/TotalRootForLimit_etau_8TeV.root"
    else: print   "!!!!!!!      ULB Name is not correct"



for cat in category:
    for chan in channel:

        Sample_LLR=GetLLR_Channel(chan)
        Sample_ULB=GetULB_Channel(chan)

        Tfile_LLR= TFile(Sample_LLR)
        Tdir_LLR=Tfile_LLR.Get(chan+cat)
        Tkey_LLR=Tdir_LLR.GetListOfKeys()
        
        Tfile_ULB= TFile(Sample_ULB)
        Tdir_ULB=Tfile_ULB.Get(chan+cat)
        Tkey_ULB=Tdir_ULB.GetListOfKeys()

        print "------------------------------------------------------------------------------------------------------------------------------"
        print cat, chan
        print "------------------------------------------------------------------------------------------------------------------------------"
        for i in range(10000):
            Histo_Name=Tkey_LLR[i].GetName()
            Histo_LLR = Tdir_LLR.Get(Histo_Name)
            Histo_ULB = Tdir_ULB.Get(Histo_Name)
#            if Histo_Name=="QCD": print "Here is QCD........."
            if Histo_LLR:
#                2 > 1
                if Histo_LLR.Integral() and (Histo_LLR.Integral() - Histo_ULB.Integral()) / (Histo_LLR.Integral())  > 0.01 :
                    print  Histo_Name, round(Histo_LLR.Integral(),4), round(Histo_ULB.Integral(),4)
            else:
#                2>1
                print "-------------> ",  Histo_Name,  "   does not exist in ULB"
            if Histo_Name==Tkey_LLR.Last().GetName():
                break

print "\n\n\n"

