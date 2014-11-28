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
InputFileLocation = '../FileROOT/MSSMROOTFiles/'
SubRootDir = 'OutFiles/'

def luminosity(CoMEnergy):
    if CoMEnergy == '_8TeV': return  19710 #19242
    if CoMEnergy == '_7TeV': return  4982

def XSection(mX, CoMEnergy):
    if CoMEnergy == '_8TeV':
        if mX == 'signal':      return 1.
        if mX == 'ggH_SM125':      return 1.23
        if mX == 'qqH_SM125':      return 0.100
        if mX == 'VH_SM125':      return 0.077


signal = ['ggH', 'bbH']
mass = [80,90,  100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]




#category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
category =["_inclusive", "_nobtag_low", "_nobtag_medium", "_nobtag_high", "_btag_low", "_btag_high", "_btagLoose_low", "_btagLoose_high"]
channel = ["mutau", "etau"]

high_bin = 1500
digit = 3
verbos_ = True


def getHistoNorm(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfile = TFile(InputFileLocation + Name +CoMEnergy+ '.root')
    HistoDenum = myfile.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral() * luminosity(CoMEnergy) / HistoDenum.GetBinContent(1)
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral()) * luminosity(CoMEnergy)  / HistoDenum.GetBinContent(1)
        valueEr = round(valueEr, digit)
    return value, valueEr


def makeSystematic2DTable(Observable,PostFix,CoMEnergy):
    myOut = TFile("Yield"+Observable+CoMEnergy+PostFix+".root", 'RECREATE')
    FullResults  = TH2F('FullResults', 'FullResults', 16, 0, 16, 60, 0, 60)

    for categ in range(len(category)):
        for chl in range(len(channel)):
            print "\n##################################################################################################"
            print "starting category and channel", category[categ], channel[chl]
            print "##################################################################################################\n"
            ##################################################################################################
            #   Signal Estimation
            ##################################################################################################
            print "\nDoing Signal estimation"
            for sig in range(len(signal)):
                for m in range(len(mass)):#    for m in range(110, 145, 5):

                    Histogram = Observable+"_mTLess30_OS"
                    XLoc= categ + len(category)*chl + 1
                    YLoc= sig * len(mass) + m + 1
                    Name= str(signal[sig]) +str(mass[m])

                    value = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0] * XSection("signal", CoMEnergy)
                    FullResults.SetBinContent(XLoc,YLoc , value)
                    FullResults.GetYaxis().SetBinLabel(YLoc, Name)

#        ########################################################################
            FullResults.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
    myOut.Write()
    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResults.Draw('text')
    myCanvas.SaveAs("SystematicTableAll"+Observable+PostFix+CoMEnergy+".pdf")


if __name__ == "__main__":
    makeSystematic2DTable("_SVMassTauHighPtRWUp","", "_8TeV")
    makeSystematic2DTable("_SVMassTauHighPtRWDown","", "_8TeV")
    makeSystematic2DTable("_SVMassHiggPtRWUp","", "_8TeV")
    makeSystematic2DTable("_SVMassHiggPtRWDown","", "_8TeV")
    makeSystematic2DTable("_SVMassHiggPtRWScaleUp","", "_8TeV")
    makeSystematic2DTable("_SVMassHiggPtRWScaleDown","", "_8TeV")

