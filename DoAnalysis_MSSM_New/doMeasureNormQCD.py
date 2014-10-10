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
    if CoMEnergy == '_8TeV': return  19712 #19242
    if CoMEnergy == '_7TeV': return  4982



#category = ["_inclusive"]
#category = ["_inclusive", "_nobtag", "_btag"]
category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]

#channel = ["mutau"]
channel = ["mutau", "etau"]

lenghtSig = 0
low_bin = 0
high_bin = 15000
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

def GetNorm_BackGround(BackGround,PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange):

    if BackGround=="VV" : DYIndex= "" ;  Name= "VVAll"
    if BackGround=="TT" : DYIndex= "" ;  Name= "TTAll"
    if BackGround=="ZL" : DYIndex= "_ZL" ;  Name= "DYJetsAll"
    if BackGround=="ZJ" : DYIndex= "_ZJ" ;  Name= "DYJetsAll"
    if BackGround=="ZTT" : DYIndex= "_ZTT" ;  Name= "DYJetsAll"

    Name_Histo=HistoName+etaRange+DYIndex
    BG_Estimate= getHistoNorm_BG(PostFix,CoMEnergy,Name, channelName,categoryName,Name_Histo)[0]
    return BG_Estimate

def GetNorm_W(PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange):

    #Get the extrapolation factor from MC
    HistoForNumerator=HistoName+etaRange
    HistoForDeNumerator=HistoForNumerator.replace("mTLess30", "mTHigher70")
    ExtraPolFactor=getWExtraPol(PostFix,CoMEnergy, "WJetsAll",channelName,categoryName,HistoForNumerator,HistoForDeNumerator)

    HistoForHighMT=HistoName.replace("mTLess30", "mTHigher70")
    VVinHighMT=GetNorm_BackGround("VV",PostFix,CoMEnergy,channelName,categoryName,HistoForHighMT,etaRange)
    TTinHighMT=GetNorm_BackGround("TT",PostFix,CoMEnergy,channelName,categoryName,HistoForHighMT,etaRange)
    ZLinHighMT=GetNorm_BackGround("ZL",PostFix,CoMEnergy,channelName,categoryName,HistoForHighMT,etaRange)
    ZJinHighMT=GetNorm_BackGround("ZJ",PostFix,CoMEnergy,channelName,categoryName,HistoForHighMT,etaRange)
    ZTTinHighMT=GetNorm_BackGround("ZTT",PostFix,CoMEnergy,channelName,categoryName,HistoForHighMT,etaRange)
    DatainHighMT=getHistoIntegral(PostFix,CoMEnergy, "Data",channelName,categoryName,HistoForHighMT+etaRange)[0]

    W_Estimate=(DatainHighMT - (VVinHighMT+TTinHighMT+ZLinHighMT+ZJinHighMT+ZTTinHighMT))* ExtraPolFactor
    return W_Estimate

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

            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig +1
            Name= "VVAll"

            # For Tau Falke Rate
            VV_NormLowMTOSIso = GetNorm_BackGround("VV",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS", etaRange)
            VV_NormLowMTSSIso = GetNorm_BackGround("VV",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS", etaRange)
            VV_NormLowMTOSRelax = GetNorm_BackGround("VV",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS_RelaxIso", etaRange)
            VV_NormLowMTSSRelax = GetNorm_BackGround("VV",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS_RelaxIso", etaRange)
            VV_QCDNormLowMTSSIso = GetNorm_BackGround("VV",PostFix,CoMEnergy, channel[chl],category[categ],"_QCDNorm_mTLess30_SS", etaRange)

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
            ##################################################################################################
            #   TT Estimation
            ##################################################################################################
            print "\nDoing TT, BG estimation"

            Name= "TTAll"
            YLoc= lenghtSig  +2
            XLoc= categ + len(category)*chl + 1
            
            TT_NormLowMTOSIso = GetNorm_BackGround("TT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS", etaRange)
            TT_NormLowMTSSIso = GetNorm_BackGround("TT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS", etaRange)
            TT_NormLowMTOSRelax = GetNorm_BackGround("TT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS_RelaxIso", etaRange)
            TT_NormLowMTSSRelax = GetNorm_BackGround("TT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS_RelaxIso", etaRange)
            TT_QCDNormLowMTSSIso = GetNorm_BackGround("TT",PostFix,CoMEnergy, channel[chl],category[categ],"_QCDNorm_mTLess30_SS", etaRange)

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
            Name= "ZL"
            YLoc= lenghtSig + 3
            XLoc= categ + len(category)*chl + 1
 
            ZL_NormLowMTOSIso = GetNorm_BackGround("ZL",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS", etaRange)
            ZL_NormLowMTSSIso = GetNorm_BackGround("ZL",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS", etaRange)
            ZL_NormLowMTOSRelax = GetNorm_BackGround("ZL",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS_RelaxIso", etaRange)
            ZL_NormLowMTSSRelax = GetNorm_BackGround("ZL",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS_RelaxIso", etaRange)
            ZL_QCDNormLowMTSSIso = GetNorm_BackGround("ZL",PostFix,CoMEnergy, channel[chl],category[categ],"_QCDNorm_mTLess30_SS", etaRange)

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

            Name= "ZJ"
            YLoc= lenghtSig + 4
            XLoc= categ + len(category)*chl + 1

            ZJ_NormLowMTOSIso = GetNorm_BackGround("ZJ",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS", etaRange)
            ZJ_NormLowMTSSIso = GetNorm_BackGround("ZJ",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS", etaRange)
            ZJ_NormLowMTOSRelax = GetNorm_BackGround("ZJ",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS_RelaxIso", etaRange)
            ZJ_NormLowMTSSRelax = GetNorm_BackGround("ZJ",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS_RelaxIso", etaRange)
            ZJ_QCDNormLowMTSSIso = GetNorm_BackGround("ZJ",PostFix,CoMEnergy, channel[chl],category[categ],"_QCDNorm_mTLess30_SS", etaRange)

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

            Name= "ZTT"
            YLoc= lenghtSig + 5
            XLoc= categ + len(category)*chl + 1

            ZTT_NormLowMTOSIso = GetNorm_BackGround("ZTT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS", etaRange)
            ZTT_NormLowMTSSIso = GetNorm_BackGround("ZTT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS", etaRange)
            ZTT_NormLowMTOSRelax = GetNorm_BackGround("ZTT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS_RelaxIso", etaRange)
            ZTT_NormLowMTSSRelax = GetNorm_BackGround("ZTT",PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS_RelaxIso", etaRange)
            ZTT_QCDNormLowMTSSIso = GetNorm_BackGround("ZTT",PostFix,CoMEnergy, channel[chl],category[categ],"_QCDNorm_mTLess30_SS", etaRange)

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

            Name= "W"
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 6

            W_NormLowMTOSIso = GetNorm_W(PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS", etaRange)
            W_NormLowMTSSIso = GetNorm_W(PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS", etaRange)
            W_NormLowMTOSRelax = GetNorm_W(PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_OS_RelaxIso", etaRange)
            W_NormLowMTSSRelax = GetNorm_W(PostFix,CoMEnergy, channel[chl],category[categ],"_TauPt_mTLess30_SS_RelaxIso", etaRange)
            W_QCDNormLowMTSSIso = GetNorm_W(PostFix,CoMEnergy, channel[chl],category[categ],"_QCDNorm_mTLess30_SS", etaRange)
            
            FullResultsOSIso.SetBinContent(XLoc,YLoc , W_NormLowMTOSIso)
            FullResultsSSIso.SetBinContent(XLoc,YLoc , W_NormLowMTSSIso)
            FullResultsOSRelax.SetBinContent(XLoc,YLoc , W_NormLowMTOSRelax)
            FullResultsSSRelax.SetBinContent(XLoc,YLoc , W_NormLowMTSSRelax)
            FullResultsSSIsoQCDNorm.SetBinContent(XLoc,YLoc , W_QCDNormLowMTSSIso)
            FullResultsOSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIso.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsOSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSRelax.GetYaxis().SetBinLabel(YLoc, Name)
            FullResultsSSIsoQCDNorm.GetYaxis().SetBinLabel(YLoc, Name)


            
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
            Data_NormLowMTOSRelax = getHistoIntegral(PostFix,CoMEnergy,Name, channel[chl],category[categ],HistoTauPtLowMTOSRelax)[0]
            Data_NormLowMTSSRelax = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoTauPtLowMTSSRelax)[0]
            Data_QCDNormLowMTSSIso = getHistoIntegral(PostFix,CoMEnergy,Name, channel[chl],category[categ],HistoQCDNormLowMTSSIso)[0]

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
    make2DTable("_SVMass","", "_8TeV", "_Bar")
    make2DTable("_SVMass","", "_8TeV", "_Cen")
    make2DTable("_SVMass","", "_8TeV", "_End")

