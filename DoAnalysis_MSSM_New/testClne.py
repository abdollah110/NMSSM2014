#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
#http://root.cern.ch/viewvc/trunk/tutorials/pyroot/hsimple.py?revision=20881&view=markup
__author__ = "abdollahmohammadi"
__date__ = "$Feb 23, 2013 10:39:33 PM$"

import math

import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH1
from ROOT import TH2F
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gStyle
from ROOT import gSystem

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
InputFileLocation = '../FileROOT/MSSMROOTFiles/'
SubRootDir = 'OutFiles/'
verbosity_ = True


#def getHistoShape_BG(PostFix,CoMEnergy,Name,chan,cat,Histogram):
def getHistoShape_BG():
    myfileSub = TFile(SubRootDir + "out_Data_8TeV.root")
    HistoSub = myfileSub.Get("mutau_SVMass_mTLess30_OS_inclusive")
    print HistoSub.Integral()
    NewFile= TFile("NewTiFile","RECREATE")
    NewFile.WriteObject(HistoSub,"HistoSub")
#    MMM = HistoSub.Clone("hnew")
#    print "of the Clone is ",MMM.Integral()
    return NewFile




#    if HistoSub:
#        MMM = HistoSub.Clone("hnew")
##        MMM= HistoSub.Clone()
#        print  "test for cloning  show the integral() is ", SubRootDir + "out_"+Name +CoMEnergy+ '.root', chan+Histogram+ cat+PostFix , "  ", HistoSub.Integral(), " and another testfor MMM = ", MMM.Integral()
#        return MMM
#    else:
#        return HistoSub
    

def GetShape_BackGround():
#
    BG_EstimateShapeF= getHistoShape_BG()
    BG_EstimateShape=BG_EstimateShapeF.Get("HistoSub")
    print  "Now Print it again", BG_EstimateShape.Integral()
#    print "test for Step1 TT_ForqcdShape  estimate  _______", BG_EstimateShape.Integral()
#
#    return BG_EstimateShape


getHistoShape_BG()
GetShape_BackGround()



