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
InputFileLocation = '../FileROOT/nmssmROOTFiles/'
SubRootDir = 'OutFiles/'

def luminosity(CoMEnergy):
    if CoMEnergy == '_8TeV': return  19712 #19242
    if CoMEnergy == '_7TeV': return  4982

def XSection(mX, CoMEnergy):
    if CoMEnergy == '_8TeV':
        if mX == 'signal':      return 1.

        if mX == 'WWJetsTo2L2Nu': return 5.824
        if mX == 'WZJetsTo2L2Q':      return 2.207 
        if mX == 'WZJetsTo3LNu':      return 1.058 
        if mX == 'ZZJetsTo2L2Nu':      return 0.716 
        if mX == 'ZZJetsTo2L2Q':      return 2.502 
        if mX == 'ZZJetsTo4L':      return  	0.181

        if mX == 'TTJets_FullLeptMGDecays':      return 26.1975
        if mX == 'TTJets_SemiLeptMGDecays':      return 109.281
        if mX == 'TTJets_HadronicMGDecays':      return 114.0215
        if mX == 'Tbar_tW':      return 11.1
        if mX == 'T_tW':      return 11.1


        if mX == 'ggH_SM125':      return 1.23
        if mX == 'qqH_SM125':      return 0.100
        if mX == 'VH_SM125':      return 0.077
        if mX == 'TTEmbedded':      return 5.887


signal = ['ggH', 'bbH']
mass = [80,90,  100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]
W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']
Top_BackGround = ['TTJets_FullLeptMGDecays','TTJets_SemiLeptMGDecays',  'TTJets_HadronicMGDecays', 'Tbar_tW', 'T_tW']
#Top_BackGround = ['TTJets_FullLeptMGDecays', 'TTJets_HadronicMGDecays', 'TTJets_MassiveBinDECAY', 'TTJets_SemiLeptMGDecays']
#SingleTop_BackGround = ['Tbar_s', 'Tbar_t', 'Tbar_tW', 'T_s', 'T_t', 'T_tW']
DiBoson_BackGround = [ 'WWJetsTo2L2Nu',  'WZJetsTo2L2Q', 'WZJetsTo3LNu',  'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
#DiBoson_BackGround = ['WW', 'WWJetsTo2L2Nu', 'WZ', 'WZJetsTo2L2Q', 'WZJetsTo3LNu', 'ZZ', 'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
Embedded = ['Embeddedmutau', 'Embeddedetau']
DYJets = ['DYJetsAll']
WJets = ['WJetsAll']
Data = ['Data']
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']




#Histogram = "VisibleMass_"
#category = ["_inclusive"]
category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
channel = ["mutau", "etau"]
#channel = ["MuTau"]
lenghtSig = len(signal) * len(mass) +1
lenghtVV = len(DiBoson_BackGround) +1
lenghtTop = len(Top_BackGround) +1
lenghtZL = len(Z_BackGround) + 1
lenghtZJ = len(Z_BackGround) + 1
lenghtZTT = len(Z_BackGround) + 1
low_bin = 0
high_bin = 15000
digit = 3
verbos_ = True
QCDScaleFactor = 1.06


def getHistoNorm(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfile = TFile(InputFileLocation + Name +CoMEnergy+ '.root')
    HistoDenum = myfile.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral(low_bin,high_bin) * luminosity(CoMEnergy) / HistoDenum.GetBinContent(1)
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral(low_bin,high_bin)) * luminosity(CoMEnergy)  / HistoDenum.GetBinContent(1)
        valueEr = round(valueEr, digit)
    return value, valueEr

def getHistoNorm_BG(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral(low_bin,high_bin) * luminosity(CoMEnergy) 
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral(low_bin,high_bin)) * luminosity(CoMEnergy) 
        valueEr = round(valueEr, digit)
    return value, valueEr

def getEmbeddedWeight(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name+chan +CoMEnergy+ '.root') #need chan due to embedded name include MuTau
    HistoInclusive = myfileSub.Get(chan+Histogram+ "_inclusive"+PostFix )
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = HistoSub.Integral(low_bin,high_bin)/ HistoInclusive.Integral(low_bin,high_bin)
    return value

#embedToDYWeight= getEmbedToDYWeight(PostFix,CoMEnergy,channel[chl],Observable+"_mTLess30_OS")
def getEmbedToDYWeight(PostFix,CoMEnergy,chan,Histogram):

    #  Get Normalization from DY Sample in Inclusive
    DY_Files = TFile(SubRootDir + "out_DYJetsAll"+CoMEnergy+".root")
    DY_Histo=DY_Files.Get(chan+Histogram+ "_ZTT_inclusive")
    Normalization_DY= DY_Histo.Integral()*luminosity(CoMEnergy)
    print "Normalization_DY= ", Normalization_DY


    #  Get Normalization from TTEmbedded Sample in Inclusive
    EmbedTT_Files = TFile(SubRootDir + "out_TTEmbedded"+chan+CoMEnergy+".root")
    EmbedTT_Histo=EmbedTT_Files.Get(chan+Histogram+ "_inclusive")
    OriginFile_EmbedTT = TFile(InputFileLocation + "TTEmbedded"+chan+CoMEnergy+".root")
    HistoTotal = OriginFile_EmbedTT.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
    Normalization_EmbedTT= (EmbedTT_Histo.Integral()*luminosity(CoMEnergy) * XSection("TTEmbedded", CoMEnergy))/HistoTotal.Integral()
    print "Normalization_EmbedTT= " , Normalization_EmbedTT
    OriginFile_EmbedTT.Close()

    #  Get Normalization from Embedded Data Sample in Inclusive
    EmbedData_Files = TFile(SubRootDir + "out_Embedded"+chan+CoMEnergy+".root")
    EmbedData_Histo=EmbedData_Files.Get(chan+Histogram+ "_inclusive")
    
    return (Normalization_DY+ Normalization_EmbedTT)/(EmbedData_Histo.Integral()* luminosity(CoMEnergy))

def getWExtraPol(PostFix,CoMEnergy,Name,chan,cat,HistogramNum,HistogramDenum ):
    myfileSub = TFile(SubRootDir + "out_"+Name+CoMEnergy+ '.root')
    if cat=="_btag": cat = "_btagLoose" 
    HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
    HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )
    if not HistoNum or not HistoDenum:  #FIXME   I should find why WJets do not have statics for btag or no 
        cat = "_inclusive"
        HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
        HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )

    value = HistoNum.Integral(low_bin,high_bin)/ HistoDenum.Integral(low_bin,high_bin)
    return value

def getHistoIntegral(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral(low_bin,high_bin)
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral(low_bin,high_bin))
        valueEr = round(valueEr, digit)
    return value, valueEr

    

def makeSystematic2DTable(Observable,PostFix,CoMEnergy):
    myOut = TFile("Yield"+Observable+CoMEnergy+PostFix+".root", 'RECREATE')
    FullResults  = TH2F('FullResults', 'FullResults', 15, 0, 15, 60, 0, 60)

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

