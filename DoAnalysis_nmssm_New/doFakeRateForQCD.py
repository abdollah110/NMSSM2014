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


W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']
Top_BackGround = ['TTJets_FullLeptMGDecays','TTJets_SemiLeptMGDecays',  'TTJets_HadronicMGDecays', 'Tbar_tW', 'T_tW']
DiBoson_BackGround = [ 'WWJetsTo2L2Nu',  'WZJetsTo2L2Q', 'WZJetsTo3LNu',  'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
Embedded = ['Embeddedmutau', 'Embeddedetau']
DYJets = ['DYJetsAll']
WJets = ['WJetsAll']
Data = ['Data']
#SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']




#category = ["_inclusive"]
#category = ["_inclusive", "_nobtag", "_btag"]
#category = ["_inclusive",  "_btag", "_btagLoose"]
category = ["_inclusive", "_btag", "_btagLoose", "_btagLowdR", "_btagMediumdR", "_btagHighdR"]
#channel = ["mutau"]
channel = ["mutau", "etau"]
lenghtSig = 0
#lenghtSig = len(signal) * len(mass) +1
lenghtVV = len(DiBoson_BackGround) +1
lenghtTop = len(Top_BackGround) +1
lenghtZL = len(Z_BackGround) + 1
lenghtZJ = len(Z_BackGround) + 1
lenghtZTT = len(Z_BackGround) + 1
low_bin = 0
high_bin = 300
digit = 1
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

def getWExtraPol(PostFix,CoMEnergy,Name,chan,cat,HistogramNum,HistogramDenum ):
    myfileSub = TFile(SubRootDir + "out_"+Name+CoMEnergy+ '.root')
    if cat=="_btag": cat = "_btagLoose" 
    HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
    HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )
    if not HistoNum or not HistoDenum:  #FIXME   I should find why WJets do not have statics for btag or no 
        cat = "_inclusive"
        HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
        HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )

    if HistoNum and HistoDenum:
        value = HistoNum.Integral(low_bin,high_bin)/ HistoDenum.Integral(low_bin,high_bin)
    else:
        value =0
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

    

def make2DTable(Observable,PostFix,CoMEnergy,etaRange):
    myOut = TFile("Yield"+etaRange+CoMEnergy+PostFix+".root", 'RECREATE')
    FullResultsOSIso  = TH2F('FullResultsOSIso', 'FullResultsOSIso', 15, 0, 15, 10, 0, 10)
    FullResultsSSIso  = TH2F('FullResultsSSIso', 'FullResultsSSIso', 15, 0, 15, 10, 0, 10)
    FullResultsOSRelax  = TH2F('FullResultsOSRelax', 'FullResultsOSRelax', 15, 0, 15, 10, 0, 10)
    FullResultsSSRelax  = TH2F('FullResultsSSRelax', 'FullResultsSSRelax', 15, 0, 15, 10, 0, 10)
    FullResultsSSIsoQCDNorm  = TH2F('FullResultsSSIsoQCDNorm', 'FullResultsSSIsoQCDNorm', 15, 0, 15, 10, 0, 10)

    for categ in range(len(category)):
        for chl in range(len(channel)):
            print "\n##################################################################################################"
            print "starting category and channel", category[categ], channel[chl]
            print "##################################################################################################\n"
             ##################################################################################################
            #   VV Estimation
            ##################################################################################################
            print "\nDoing VV, BG estimation"

            DYIndex = ""
            Name= "VVAll"
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig +1

            # For Tau Falke Rate
            HistoTauPtHighMTOSIso = "_TauPt_mTHigher70_OS"+ etaRange +DYIndex
            HistoTauPtHighMTSSIso = "_TauPt_mTHigher70_SS"+ etaRange +DYIndex
            HistoTauPtHighMTOSRelax = "_TauPt_mTHigher70_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtHighMTSSRelax = "_TauPt_mTHigher70_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormHighMTSSIso = "_QCDNorm_mTHigher70_SS_Iso"+ etaRange +DYIndex
            HistoTauPtLowMTOSIso = "_TauPt_mTLess30_OS"+ etaRange +DYIndex
            HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+ etaRange +DYIndex
            HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange +DYIndex

            # For Tau Falke Rate
            VV_NormHighMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSIso)[0]
            VV_NormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSIso)[0]
            VV_NormHighMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSRelax)[0]
            VV_NormHighMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSRelax)[0]
            VV_QCDNormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormHighMTSSIso)[0]
            VV_NormLowMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSIso)[0]
            VV_NormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSIso)[0]
            VV_NormLowMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSRelax)[0]
            VV_NormLowMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSRelax)[0]
            VV_QCDNormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormLowMTSSIso)[0]

            FullResultsOSIso.SetBinContent(XLoc,YLoc , VV_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , VV_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , VV_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , VV_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , VV_QCDNormLowMTSSIso)
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, Name)
#            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr
            ##################################################################################################
            #   TT Estimation
            ##################################################################################################
            print "\nDoing TT, BG estimation"

            DYIndex = ""
            Name= "TTAll"
            YLoc= lenghtSig  +2
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1

            # For Tau Falke Rate
            HistoTauPtHighMTOSIso = "_TauPt_mTHigher70_OS"+ etaRange +DYIndex
            HistoTauPtHighMTSSIso = "_TauPt_mTHigher70_SS"+ etaRange +DYIndex
            HistoTauPtHighMTOSRelax = "_TauPt_mTHigher70_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtHighMTSSRelax = "_TauPt_mTHigher70_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormHighMTSSIso = "_QCDNorm_mTHigher70_SS_Iso"+ etaRange +DYIndex
            HistoTauPtLowMTOSIso = "_TauPt_mTLess30_OS"+ etaRange +DYIndex
            HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+ etaRange +DYIndex
            HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange +DYIndex


            # For Tau Falke Rate
            TT_NormHighMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSIso)[0]
            TT_NormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSIso)[0]
            TT_NormHighMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSRelax)[0]
            TT_NormHighMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSRelax)[0]
            TT_QCDNormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormHighMTSSIso)[0]
            TT_NormLowMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSIso)[0]
            TT_NormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSIso)[0]
            TT_NormLowMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSRelax)[0]
            TT_NormLowMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSRelax)[0]
            TT_QCDNormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormLowMTSSIso)[0]

            FullResultsOSIso.SetBinContent(XLoc,YLoc , TT_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , TT_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , TT_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , TT_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , TT_QCDNormLowMTSSIso)
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, Name)

            ##################################################################################################
            #   ZL Estimation
            ##################################################################################################
            print "\nDoing ZL, BG estimation"
#            for BG_ZL in range(len(Z_BackGround)):
                
            DYIndex = "_ZL"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 3
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
 
            # For Tau Falke Rate
            HistoTauPtHighMTOSIso = "_TauPt_mTHigher70_OS"+ etaRange +DYIndex
            HistoTauPtHighMTSSIso = "_TauPt_mTHigher70_SS"+ etaRange +DYIndex
            HistoTauPtHighMTOSRelax = "_TauPt_mTHigher70_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtHighMTSSRelax = "_TauPt_mTHigher70_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormHighMTSSIso = "_QCDNorm_mTHigher70_SS_Iso"+ etaRange +DYIndex
            HistoTauPtLowMTOSIso = "_TauPt_mTLess30_OS"+ etaRange +DYIndex
            HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+ etaRange +DYIndex
            HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange +DYIndex

            # For Tau Falke Rate
            ZL_NormHighMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSIso)[0]
            ZL_NormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSIso)[0]
            ZL_NormHighMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSRelax)[0]
            ZL_NormHighMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSRelax)[0]
            ZL_QCDNormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormHighMTSSIso)[0]
            ZL_NormLowMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSIso)[0]
            ZL_NormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSIso)[0]
            ZL_NormLowMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSRelax)[0]
            ZL_NormLowMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSRelax)[0]
            ZL_QCDNormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormLowMTSSIso)[0]

            FullResultsOSIso.SetBinContent(XLoc,YLoc , ZL_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , ZL_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , ZL_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , ZL_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , ZL_QCDNormLowMTSSIso)
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, Name)

            ##################################################################################################
            #   ZJ Estimation
            #################################################################################################
            print "\nDoing ZJ, BG estimation"

            DYIndex = "_ZJ"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 4
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1

            # For Tau Falke Rate
            HistoTauPtHighMTOSIso = "_TauPt_mTHigher70_OS"+ etaRange +DYIndex
            HistoTauPtHighMTSSIso = "_TauPt_mTHigher70_SS"+ etaRange +DYIndex
            HistoTauPtHighMTOSRelax = "_TauPt_mTHigher70_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtHighMTSSRelax = "_TauPt_mTHigher70_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormHighMTSSIso = "_QCDNorm_mTHigher70_SS_Iso"+ etaRange +DYIndex
            HistoTauPtLowMTOSIso = "_TauPt_mTLess30_OS"+ etaRange +DYIndex
            HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+ etaRange +DYIndex
            HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange +DYIndex

           # For Tau Falke Rate
            ZJ_NormHighMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSIso)[0]
            ZJ_NormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSIso)[0]
            ZJ_NormHighMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSRelax)[0]
            ZJ_NormHighMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSRelax)[0]
            ZJ_QCDNormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormHighMTSSIso)[0]
            ZJ_NormLowMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSIso)[0]
            ZJ_NormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSIso)[0]
            ZJ_NormLowMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSRelax)[0]
            ZJ_NormLowMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSRelax)[0]
            ZJ_QCDNormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormLowMTSSIso)[0]

            FullResultsOSIso.SetBinContent(XLoc,YLoc , ZJ_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , ZJ_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , ZJ_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , ZJ_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , ZJ_QCDNormLowMTSSIso)
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, Name)
            
#            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr
        ##################################################################################################
        #   ZTT Estimation
        ##################################################################################################
            print "\nDoing ZTT, BG estimation"

            DYIndex = "_ZTT"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 5
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1

            # For Tau Falke Rate
            HistoTauPtHighMTOSIso = "_TauPt_mTHigher70_OS"+ etaRange +DYIndex
            HistoTauPtHighMTSSIso = "_TauPt_mTHigher70_SS"+ etaRange +DYIndex
            HistoTauPtHighMTOSRelax = "_TauPt_mTHigher70_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtHighMTSSRelax = "_TauPt_mTHigher70_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormHighMTSSIso = "_QCDNorm_mTHigher70_SS_Iso"+ etaRange +DYIndex
            HistoTauPtLowMTOSIso = "_TauPt_mTLess30_OS"+ etaRange +DYIndex
            HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+ etaRange +DYIndex
            HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange +DYIndex

            # For Tau Falke Rate
            ZTT_NormHighMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSIso)[0]
            ZTT_NormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSIso)[0]
            ZTT_NormHighMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTOSRelax)[0]
            ZTT_NormHighMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtHighMTSSRelax)[0]
            ZTT_QCDNormHighMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormHighMTSSIso)[0]
            ZTT_NormLowMTOSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSIso)[0]
            ZTT_NormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSIso)[0]
            ZTT_NormLowMTOSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSRelax)[0]
            ZTT_NormLowMTSSRelax = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSRelax)[0]
            ZTT_QCDNormLowMTSSIso = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormLowMTSSIso)[0]

            FullResultsOSIso.SetBinContent(XLoc,YLoc , ZTT_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , ZTT_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , ZTT_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , ZTT_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , ZTT_QCDNormLowMTSSIso)
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, Name)
            
        ##################################################################################################
        #   W Estimation
        ##################################################################################################
            print "\nDoing W BG estimation"

            DYIndex = ""
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 6
            W_mcName= "WJetsAll"
            W_dataName='Data'

            #Numerator for lowMT to High Mt Extrapolation Factor
            numeratorWOSIso="_TauPt_mTLess30_OS"+ etaRange +DYIndex
            numeratorWSSIso="_TauPt_mTLess30_SS"+ etaRange +DYIndex
            numeratorWOSRelax="_TauPt_mTLess30_OS_RelaxIso"+ etaRange +DYIndex
            numeratorWSSRelax="_TauPt_mTLess30_SS_RelaxIso"+ etaRange +DYIndex
            numeratorWSSIsoQCDNorm="_QCDNorm_mTLess30_SS"+ etaRange +DYIndex

            #Denumerator for lowMT to High Mt Extrapolation Factor
            denumeratorWOSIso="_TauPt_mTHigher70_OS"+ etaRange +DYIndex
            denumeratorWSSIso="_TauPt_mTHigher70_SS"+ etaRange +DYIndex
            denumeratorWOSRelax="_TauPt_mTHigher70_OS_RelaxIso"+ etaRange +DYIndex
            denumeratorWSSRelax="_TauPt_mTHigher70_SS_RelaxIso"+ etaRange +DYIndex
            denumeratorWSSIsoQCDNorm="_QCDNorm_mTHigher70_SS"+ etaRange +DYIndex

            # This is used from data to apply the extrapolationfactor  [same as senumerator but applied on data]
            HistogramHighMTOSIso = "_TauPt_mTHigher70_OS"+ etaRange +DYIndex
            HistogramHighMTSSIso = "_TauPt_mTHigher70_SS"+ etaRange +DYIndex
            HistogramHighMTOSRelax = "_TauPt_mTHigher70_OS_RelaxIso"+ etaRange +DYIndex
            HistogramHighMTSSRelax = "_TauPt_mTHigher70_SS_RelaxIso"+ etaRange +DYIndex
            HistogramHighMTWSSIsoQCDNorm="_QCDNorm_mTHigher70_SS"+ etaRange +DYIndex

            # Measuring extrapolation Factor
            ExtraPolFactorOSIso = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorWOSIso,denumeratorWOSIso)
            ExtraPolFactorSSIso = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorWSSIso,denumeratorWSSIso)
            ExtraPolFactorOSRelax = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorWOSRelax,denumeratorWOSRelax)
            ExtraPolFactorSSRelax = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorWSSRelax,denumeratorWSSRelax)
            ExtraPolFactorSSIsoQCDNorm = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorWSSIsoQCDNorm,denumeratorWSSIsoQCDNorm)

            # W in High MT sideBand from data
            W_NormDataHighMTOSIso=getHistoIntegral(PostFix,CoMEnergy,W_dataName ,channel[chl],category[categ],HistogramHighMTOSIso)[0]
            W_NormDataHighMTSSIso=getHistoIntegral(PostFix,CoMEnergy,W_dataName ,channel[chl],category[categ],HistogramHighMTSSIso)[0]
            W_NormDataHighMTOSRelax=getHistoIntegral(PostFix,CoMEnergy,W_dataName ,channel[chl],category[categ],HistogramHighMTOSRelax)[0]
            W_NormDataHighMTSSRelax=getHistoIntegral(PostFix,CoMEnergy,W_dataName ,channel[chl],category[categ],HistogramHighMTSSRelax)[0]
            W_NormDataHighMTSSIsoQCDNorm=getHistoIntegral(PostFix,CoMEnergy,W_dataName ,channel[chl],category[categ],HistogramHighMTWSSIsoQCDNorm)[0]

            # Measureing W Normalization: High MT data minues other MC times extrapolation Factor
            W_NormLowMTOSIso =(W_NormDataHighMTOSIso - (VV_NormHighMTOSIso + TT_NormHighMTOSIso +ZL_NormHighMTOSIso + ZJ_NormHighMTOSIso + ZTT_NormHighMTOSIso )) * ExtraPolFactorOSIso
            W_NormLowMTSSIso =(W_NormDataHighMTSSIso - (VV_NormHighMTSSIso + TT_NormHighMTSSIso +ZL_NormHighMTSSIso + ZJ_NormHighMTSSIso + ZTT_NormHighMTSSIso )) * ExtraPolFactorSSIso
            W_NormLowMTOSRelax =(W_NormDataHighMTOSRelax - (VV_NormHighMTOSRelax + TT_NormHighMTOSRelax +ZL_NormHighMTOSRelax + ZJ_NormHighMTOSRelax + ZTT_NormHighMTOSRelax )) * ExtraPolFactorOSRelax
            W_NormLowMTSSRelax =(W_NormDataHighMTSSRelax - (VV_NormHighMTSSRelax + TT_NormHighMTSSRelax +ZL_NormHighMTSSRelax + ZJ_NormHighMTSSRelax + ZTT_NormHighMTSSRelax )) * ExtraPolFactorSSRelax
            W_NormLowMTSSIsoQCDNorm =(W_NormDataHighMTSSIsoQCDNorm - (VV_QCDNormHighMTSSIso + TT_QCDNormHighMTSSIso +ZL_QCDNormHighMTSSIso + ZJ_QCDNormHighMTSSIso + ZTT_QCDNormHighMTSSIso )) * ExtraPolFactorSSIsoQCDNorm

            FullResultsOSIso.SetBinContent(XLoc,YLoc , W_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , W_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , W_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , W_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , W_NormLowMTSSIsoQCDNorm)
            
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, "W")
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, "W")
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, "W")
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, "W")
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, "W")
            
                         ##################################################################################################
            #   Data Estimation
            ##################################################################################################
            print "\nDoing Data,  estimation"

            DYIndex = ""
            Name= "Data"
            YLoc= lenghtSig +7
            XLoc= categ + len(category)*chl + 1

            # For Tau Falke Rate
            HistoTauPtLowMTOSIso = "_TauPt_mTLess30_OS"+ etaRange +DYIndex
            HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+ etaRange +DYIndex
            HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+ etaRange +DYIndex
            HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+ etaRange +DYIndex
            HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange +DYIndex

            # For Tau Falke Rate
            Data_NormLowMTOSIso = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSIso)[0]
            Data_NormLowMTSSIso = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSIso)[0]
            Data_NormLowMTOSRelax = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTOSRelax)[0]
            Data_NormLowMTSSRelax = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSRelax)[0]
            Data_QCDNormLowMTSSIso = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDNormLowMTSSIso)[0]

            FullResultsOSIso.SetBinContent(XLoc,YLoc , Data_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , Data_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , Data_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , Data_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , Data_QCDNormLowMTSSIso)
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, Name)
            
#        ########################################################################
            FullResultsOSIso.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
            FullResultsSSIso.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
            FullResultsOSRelax.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
            FullResultsSSRelax.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
            FullResultsSSIsoQCDNorm.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
#        ########################################################################
    myOut.Write()

    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResultsOSIso.Draw('text')
    myCanvas.SaveAs("TableOSIso"+etaRange+PostFix+CoMEnergy+".pdf")
    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResultsSSIso.Draw('text')
    myCanvas.SaveAs("TableSSIso"+etaRange+PostFix+CoMEnergy+".pdf")
    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResultsOSRelax.Draw('text')
    myCanvas.SaveAs("TableOSRelax"+etaRange+PostFix+CoMEnergy+".pdf")
    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResultsSSRelax.Draw('text')
    myCanvas.SaveAs("TableSSRelax"+etaRange+PostFix+CoMEnergy+".pdf")
    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResultsSSIsoQCDNorm.Draw('text')
    myCanvas.SaveAs("TableSSIsoQCDNorm"+etaRange+PostFix+CoMEnergy+".pdf")
    


if __name__ == "__main__":
    make2DTable("_TMass","", "_8TeV", "_Bar")
    make2DTable("_TMass","", "_8TeV", "_Cen")
    make2DTable("_TMass","", "_8TeV", "_End")
#    make2DTable("_SVMass","", "_8TeV", "_Bar")
#    make2DTable("_SVMass","", "_8TeV", "_Cen")
#    make2DTable("_SVMass","", "_8TeV", "_End")

