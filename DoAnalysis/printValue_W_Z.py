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
InputFileLocation = '../FileROOT/NewROOTFiles/'


def GetNumber(sample):
    myfile = TFile(InputFileLocation+sample+'_8TeV.root')
    HistoDenum = myfile.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
    return HistoDenum.GetBinContent(1)

    
W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']

for sample in W_BackGround:
    print sample, "has =  ", GetNumber(sample), "  events"
for sample in Z_BackGround:
    print sample, "has =  ", GetNumber(sample), "  events"


#WJetsToLNu has =   6099748.0   events
#W1JetsToLNu has =   20683116.0   events
#W2JetsToLNu has =   13779759.0   events
#W3JetsToLNu has =   14593791.0   events
#W4JetsToLNu has =   4378103.0   events
#DYJetsToLL has =   14476833.0   events
#DY1JetsToLL has =   5214585.0   events
#DY2JetsToLL has =   2352304.0   events
#DY3JetsToLL has =   5542076.0   events
#DY4JetsToLL has =   5059363.0   events