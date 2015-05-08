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
from ROOT import gSystem
from ctypes import *
import ROOT as r
import array

gROOT.Reset()
import os




ROOT.gROOT.SetBatch(True)
#ROOT.gROOT.ProcessLine('.x rootlogon.C')
SubRootDir = 'OutFiles/'


doFineBinning = False
MSSMSignalUncertainty = False
TopUncertainty = True
TopShapeUncertainty= True
ZLUncertainty = True
WShapeUncertainty = True
verbos_ = False

high_bin = 200
ptBinning = 200
FineBinVal=5
digit = 3
QCDScaleFactor = 1.06
UpperRange = 200

#Binning_NoBTag = array.array("d",[0,7,14,21, 28, 35, 42, 49, 60,70,80,90,100,110,120,130,140,160,180,200])
#Binning_BTag = array.array("d",[0,7,14,21, 28, 35, 42, 49, 60,70,80,90,100,110,120,130,140,160,180,200])
#Binning_NoBTag = array.array("d",[0,6,12,18,24,30,36,42,48,54,60,70,80,90,100,110,120,130,140,160,180,200])
#Binning_BTag = array.array("d",[0,6,12,18,24,30,36,42,48,54,60,70,80,90,100,110,120,130,140,160,180,200])
#Binning_NoBTag = array.array("d",[0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,54,60,70,80,90,100,110,120,130,140,160,180,190,200])
#Binning_BTag = array.array("d",[0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,54,60,70,80,90,100,110,120,130,140,160,180,190,200])
#Binning_NoBTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300])
#Binning_BTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300])
#Binning_NoBTag = array.array("d",[0,5,10,15,20,25,30,35,40,45,50,60,70,80,90,100,110,120,130,140,160,180,200])
#Binning_BTag = array.array("d",[0,5,10,15,20,25,30,35,40,45,50,60,70,80,90,100,110,120,130,140,160,180,200])
#Binning_BTag = array.array("d",[0,15,20,25,30,35,40,50,70,100,150,200])
#Binning_NoBTag = array.array("d",[0,15,20,25,30,35,40,50,70,100,150,200])
#Binning_NoBTag = array.array("d",[-2.5,-2.3,-2.1,-1.9,-1.7,-1.5,-1.3,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5])
#Binning_BTag = array.array("d",[-2.5,-2.3,-2.1,-1.9,-1.7,-1.5,-1.3,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5])
#Binning_BTag = array.array("d",[0,5,10,15,20,25,30,60])
#Binning_NoBTag = array.array("d",[0,5,10,15,20,25,30,60])
Binning_BTag = array.array("d",[0,5,10,15,20,25,30,35,40,45,50,55,60,70,85,100,150,200])
Binning_NoBTag = array.array("d",[0,5,10,15,20,25,30,35,40,45,50,55,60,70,85,100,150,200])

TauScale = ["Down", "", "Up"]
#POSTFIX=["","Up","Down"]

signal = ['bba1GenFil_']
signalName = ['bba1']
mass = [25,30,  35, 40, 45, 50, 55,  60, 65, 70, 75, 80]
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']
Other_BackGround = ['DYJetsAllMassLow']
lenghtSig = len(signal) * len(mass) +1

#category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
category = ["_inclusive",  "_btag", "_btagLoose"]

channelDirectory = ["muTau", "eleTau"]
SystematicSignal = ["_SVMassTauHighPtRWUp","_SVMassTauHighPtRWDown","_SVMassHiggPtRWUp","_SVMassHiggPtRWDown"]
SystematicSignalScale = ["_SVMassHiggPtRWUp","_SVMassHiggPtRWDown"]

####################################################
##   Functions
####################################################

#def luminosity(CoMEnergy):
#    if CoMEnergy == '_8TeV': return  19710 #19242
#    if CoMEnergy == '_7TeV': return  4982
#
#def XSection(mX, CoMEnergy):
#    if CoMEnergy == '_8TeV':
#        if mX == 'ggH_SM125':      return 1.23

    
#def QCDUncertaintyNameFR(unc,channel,NameCat,CoMEnergy):
#    if unc=="": return 'QCD'
#    if unc== "Up": return "QCD_CMS_htt_QCDfrShape_"+channel+CoMEnergy+"Up"
#    if unc== "Down": return "QCD_CMS_htt_QCDfrShape_"+channel+CoMEnergy+"Down"
#
#def QCDUncertaintyName(unc,channel,NameCat,CoMEnergy):
#    if unc=="": return 'QCD_'
#    if unc== "Up": return "QCD_CMS_htt_QCDShape_"+channel+NameCat+CoMEnergy+"Up"
#    if unc== "Down": return "QCD_CMS_htt_QCDShape_"+channel+NameCat+CoMEnergy+"Down"


def getTauFakeCorrection(pt):
    #new corrections (Mar14, new T-ES correction)
    if pt > 200: pt =200
    correction = 0;
    p0 =  0.787452;
    p1 = -0.146412;
    p2 = -0.0276803;
    p3 = -0.0824184;
    X = (pt-149.832)/100;  #//(x-meanPt)/100
    correction = p0+p1*X+p2*X*X+p3*X*X*X;
    return correction;

#def getTauFakeCorrection(pt):
#  #new corrections (Mar14, new T-ES correction)
#  if pt > 150: pt =150
#  correction = 0;
#  p0 =  4.14209e-01 #0.787452;
#  p1 = -7.60796e-03 #-0.146412;
#  p2 = -1.15766e-04 #-0.0276803;
#  p3 = -8.04130e-07 #-0.0824184;
#  X = (pt-1.37945e+02)/100;  #//(x-149.832)/100
#  correction = p0+p1*X+p2*X*X+p3*X*X*X;
#  return correction;

#def getHistoNorm(PostFix,CoMEnergy,Name,chan,cat,Histogram):
#    myfile = TFile(InputFileLocation + Name +CoMEnergy+ '.root')
#    HistoDenum = myfile.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
#    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
#    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
#    value = 10E-7
#    if (HistoSub):
#        value = HistoSub.Integral() * luminosity(CoMEnergy) / HistoDenum.GetBinContent(1)
#        value = round(value, digit)
#    return value

############################################################################################################
def _Return_SigBGData_Shape(Name, channel,cat,HistoName,PostFix,CoMEnergy,changeHistoName):
    myfile = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    if cat=="_btag" and changeHistoName : cat = "_btagLoose"
#    if cat=="_btag_high" and changeHistoName : cat = "_btagLoose_high"
    Histo =  myfile.Get(channel+HistoName + cat+ PostFix)
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewFile.WriteObject(Histo,"XXX")
    myfile.Close()
    return NewFile
############################################################################################################
def _Return_Embed_Shape(Name, channel,cat,HistoName,PostFix,CoMEnergy,normal,normalTT,changeHistoName):

    DataEmbedded = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    Histo =  DataEmbedded.Get(channel+HistoName + cat+ PostFix)
    Histo.Scale(normal/Histo.Integral())

    TTEmbedded=TFile(SubRootDir + "out_TT"+ Name +CoMEnergy+ '.root')
    HistoTTEmbedded= TTEmbedded.Get(channel+HistoName + cat+ PostFix)
    HistoTTEmbedded.Scale(normalTT/HistoTTEmbedded.Integral())

    Histo.Add(HistoTTEmbedded,-1)   # Subtract from them

    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewFile.WriteObject(Histo,"XXX")
    DataEmbedded.Close()
    TTEmbedded.Close()
    return NewFile
############################################################################################################
def _Return_QCD_Shape(channel,cat,Histo,PostFix,CoMEnergy):
    myfile = TFile("QCDFinalFile_ControlPlots.root")
    HistoNorm =  myfile.Get(channel+Histo + cat+PostFix)
    if cat=="_btag" : cat = "_btagLoose"
    HistoShape =  myfile.Get(channel+Histo + cat+PostFix)
    HistoShape.Scale(HistoNorm.Integral()/HistoShape.Integral())
    #    NewFile=TFile("Extra/XXX2out_QCD" +CoMEnergy+channel+ cat+PostFix+".root","RECREATE")
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewFile.WriteObject(HistoShape,"XXX")
    myfile.Close()
    return NewFile
############################################################################################################
def _Return_TOP_Shape(Name, channel,cat,HistoName,PostFix,CoMEnergy,changeHistoName):
#def _Return_TOP_Shape(channel,cat,HistoName,PostFix,CoMEnergy,changeHistoName):
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewHIST =TH1F("XXX","",high_bin,0,high_bin)
    NewHIST.SetDefaultSumw2()
    NewHISTUp =TH1F("XXXUp","",high_bin,0,high_bin)
    NewHISTUp.SetDefaultSumw2()
    NewHISTDown =TH1F("XXXDown","",high_bin,0,high_bin)
    NewHISTDown.SetDefaultSumw2()
    TOPShapeFile = TFile(SubRootDir + "out_TTAll"+CoMEnergy+ '.root')
#    if cat=="_btag_low" and changeHistoName : cat = "_btagLoose_low"
#    if cat=="_btag_high" and changeHistoName : cat = "_btagLoose_high"
#    if NameCat=="_btag" and changeHistoName : NameCat = "_btagLoose"
    Histo = TOPShapeFile.Get(channel+"_2DSVMassPt_W_mTLess30_OS"+cat+PostFix)
    print "Name is=", channel+"_2DSVMassPt_W_mTLess30_OS"+cat+PostFix
    preTOPYeild= Histo.Integral()
    for bb in range(high_bin):
        for ss in range(300):
            weight= getTauFakeCorrection(ss+0.5)
            NormInPtBin = weight*Histo.GetBinContent(bb+1,ss+1)
            NormInPtBinUp = (weight + 0.5*(1-weight))*Histo.GetBinContent(bb+1,ss+1)
            NormInPtBinDown = (weight - 0.5*(1-weight))*Histo.GetBinContent(bb+1,ss+1)
            if NormInPtBin : NewHIST.Fill(bb,NormInPtBin * 1./preTOPYeild)
            if NormInPtBinUp : NewHISTUp.Fill(bb,NormInPtBinUp* 1./preTOPYeild)
            if NormInPtBinDown : NewHISTDown.Fill(bb,NormInPtBinDown* 1./preTOPYeild)



    NewFile.WriteObject(NewHIST,"XXX")
    NewFile.WriteObject(NewHISTUp,"XXXUp")
    NewFile.WriteObject(NewHISTDown,"XXXDown")
    return NewFile
############################################################################################################
def _Return_W_Shape(channel,cat,CoMEnergy,PostFix,changeHistoName):
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewHIST =TH1F("XXX","",high_bin,0,high_bin)
    NewHIST.SetDefaultSumw2()
    NewHISTUp =TH1F("XXXUp","",high_bin,0,high_bin)
    NewHISTUp.SetDefaultSumw2()
    NewHISTDown =TH1F("XXXDown","",high_bin,0,high_bin)
    NewHISTDown.SetDefaultSumw2()
    WShapeFile = TFile(SubRootDir + "out_WJetsAll"+CoMEnergy+ '.root')
#    if cat=="_btag_low" and changeHistoName : cat = "_btagLoose_low"
#    if cat=="_btag_high" and changeHistoName : cat = "_btagLoose_high"
    if cat=="_btag" and changeHistoName : cat = "_btagLoose"
    Histo = WShapeFile.Get(channel+"_2DSVMassPt_W_mTLess30_OS_RelaxIso"+cat+PostFix)
    preWYeild= Histo.Integral()
    for bb in range(high_bin):
        for ss in range(300):
            weight= getTauFakeCorrection(ss+0.5)
            NormInPtBin = weight*Histo.GetBinContent(bb+1,ss+1)
            NormInPtBinUp = (weight + 0.5*(1-weight))*Histo.GetBinContent(bb+1,ss+1)
            NormInPtBinDown = (weight - 0.5*(1-weight))*Histo.GetBinContent(bb+1,ss+1)
            if NormInPtBin : NewHIST.Fill(bb,NormInPtBin* 1./preWYeild)
            if NormInPtBinUp : NewHISTUp.Fill(bb,NormInPtBinUp* 1./preWYeild)
            if NormInPtBinDown : NewHISTDown.Fill(bb,NormInPtBinDown* 1./preWYeild)

    NewFile.WriteObject(NewHIST,"XXX")
    NewFile.WriteObject(NewHISTUp,"XXXUp")
    NewFile.WriteObject(NewHISTDown,"XXXDown")
    return NewFile
############################################################################################################

####################################################
##   Start Making the Datacard Histograms
####################################################
def MakeTheHistogram(channel,Observable,CoMEnergy,chl):
    #################### Different Normalization Tables
    #ScaleUp
    Table_FileUp = TFile("Yield"+CoMEnergy+"Up"+".root")
    NormTableUp = Table_FileUp.Get('FullResults')
    #Norm
    Table_File = TFile("Yield"+CoMEnergy+""+".root")
    NormTable_ = Table_File.Get('FullResults')
    #ScaleDown
    Table_FileDown = TFile("Yield"+CoMEnergy+"Down"+".root")
    NormTableDown = Table_FileDown.Get('FullResults')


    NormTable=[NormTableDown,NormTable_,NormTableUp]
#    NormTable=[NormTable_]


    
    TauScaleOut = ["_CMS_scale_t_"+channel+CoMEnergy+"Down", "", "_CMS_scale_t_"+channel+CoMEnergy+"Up"]
    Signal_Unc_glugluHiggs = ["_CMS_htt_higgsPtReweight"+CoMEnergy+"Up","_CMS_htt_higgsPtReweight"+CoMEnergy+"Down", "_CMS_htt_higgsPtReweight_scale"+CoMEnergy+"Up","_CMS_htt_higgsPtReweight_scale"+CoMEnergy+"Down"]
    Signal_Unc_HighPtTau = ["_CMS_eff_t_mssmHigh_"+channel+CoMEnergy+"Up","_CMS_eff_t_mssmHigh_"+channel+CoMEnergy+"Down"]

#    TH1.AddDirectory(0)
    myOut = TFile("cnt_TotalRootForLimit_"+Observable+channel + CoMEnergy+".root" , 'RECREATE') # Name Of the output file

    icat=-1
    for NameCat in category:
        icat =icat +1
        print "starting NameCat and channel", NameCat, channel
        if NameCat=="_btag_low" or NameCat=="_btag_high" or NameCat == "_btagLoose_Low"or NameCat == "_btagLoose_high":
            BinCateg = Binning_BTag
        else:
            BinCateg = Binning_NoBTag
        tDirectory= myOut.mkdir(channelDirectory[chl] + str(NameCat))
        tDirectory.cd()
        for tscale in range(len(TauScale)):

           ################################################
           #   Filling Signal
           ################################################
            for sig in range(len(signal)):
                for m in range(len(mass)):#    for m in range(110, 145, 5):

                    print "Now is processing", signal[sig],mass[m]
                    tDirectory.cd()
                    Histogram = Observable+"_mTLess30_OS"
                    XLoc= icat + len(category)*chl + 1
                    YLoc= sig * len(mass) + m + 1
                    normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
                    Name= str(signal[sig])+str(mass[m])
                    NameOut= str(signalName[sig]) +str(mass[m])+str(TauScaleOut[tscale])

                    SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
                    SampleHisto=SampleFile.Get("XXX")
                    if SampleHisto:
                        SampleHisto.Scale(normal/SampleHisto.Integral())
                    else:
                        SampleFile= _Return_SigBGData_Shape(Name, channel,"_inclusive", Histogram, TauScale[tscale],CoMEnergy,False)
                        SampleHisto=SampleFile.Get("XXX")
                        SampleHisto.Scale(.0000001)
                        
                    RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                    tDirectory.WriteObject(RebinedHist,NameOut)
                    
                   ###############  Systematics on Shape and Norm for Higgs Pt Jusy GluGluHiggs   ####
 

           ################################################
           #   Filling VV
           ################################################
            print "Doing VV, BG estimation"
            tDirectory.cd()

            Histogram = Observable+"_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig  +1
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "VVAll"
            NameOut= "VV"+str(TauScaleOut[tscale])

            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
            SampleHisto=SampleFile.Get("XXX")
            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")
                

            ################################################
            #  Filling TOP
            ################################################
            print "Doing TOP, BG estimation"
            tDirectory.cd()

            Histogram = Observable+"_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig+2
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "TTAll"
            NameOut= "TT"+str(TauScaleOut[tscale])

            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
            SampleHisto=SampleFile.Get("XXX")
#            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
            if SampleHisto.Integral(): SampleHisto.Scale(normal*1.05/SampleHisto.Integral())
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")



            ################################################
            #  Filling ZL
            ################################################
            print "Doing ZL, BG estimation"
            tDirectory.cd()

            Histogram = Observable+"_mTLess30_OS_ZL"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig  +3
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "DYJetsAll"
            NameOut= "ZL"+str(TauScaleOut[tscale])

            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)
            SampleHisto=SampleFile.Get("XXX")
            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())  # The shape is from btag-Loose Need get back norm
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")


            ################################################
            #  Filling ZJ
            ################################################
            print "Doing ZJ, BG estimation"
            tDirectory.cd()
            Histogram = Observable+"_mTLess30_OS_ZJ"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig + 4
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "DYJetsAll"
            NameOut= "ZJ"+str(TauScaleOut[tscale])

            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)
            SampleHisto=SampleFile.Get("XXX")
            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())  # The shape is from btag-Loose Need get back norm
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")

            ################################################
            #  Filling ZTT
            ################################################
            print "Doing ZTT, BG estimation"
            tDirectory.cd()
            Histogram = Observable+"_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig + 5
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            normalTTEmbedded= NormTable[tscale].GetBinContent(XLoc,lenghtSig  +7)    #Get the Noralization
            Name= "Embedded"+ channel
            NameOut= "ZTT"+str(TauScaleOut[tscale])


            SampleFile= _Return_Embed_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,normal,normalTTEmbedded,False)
            SampleHisto=SampleFile.Get("XXX")
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")

            ################################################
            #  Filling W
            ################################################
            print "Doing W, BG estimation"
            tDirectory.cd()
            Histogram = Observable+"_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig + 6
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization Also for Uncertainties
            Name='WJetsAll'
            NameOut= "W"+str(TauScaleOut[tscale])

            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)
            SampleHisto=SampleFile.Get("XXX")
            print "----------> Debug on W",channel,NameCat, SampleHisto.Integral()
            if SampleHisto.Integral(): SampleHisto.Scale(normal*1.05/SampleHisto.Integral())  # Due to change in _Return_W_Shape
#            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")



            ################################################
            #  Filling QCD  QCD shape is driven from AntiLepton, relaxed tau and subtracted from other final states
            ################################################
            if tscale ==1:
                    print "Doing QCD, BG estimation"
        
                    tDirectory.cd()
                    HistogramNorm = "_QCDShapeNormTotalFROSSS"   #channelName+"_QCDShapeNormTotal"+catName+shiftFR+"FR"+shiftOSSS+"OSSS"
      
                    Name='Data'
                    NameOut= 'QCD'
                    
                    SampleFile= _Return_QCD_Shape(channel,NameCat, HistogramNorm, TauScale[tscale],CoMEnergy)
                    SampleHisto=SampleFile.Get("XXX")
                    RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                    tDirectory.WriteObject(RebinedHist,"QCD")


            ################################################
            #  Filling Data
            ################################################
            if tscale ==1:
                print "Doing Data estimation"
                tDirectory.cd()
                Histogram = Observable+"_mTLess30_OS"
                XLoc= icat + len(category)*chl + 1
                YLoc= lenghtSig + 8
                normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
                Name='Data'
                NameOut='data_obs'

                SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
                SampleHisto=SampleFile.Get("XXX")
#                if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NameOut)

            ################################################
            #  Filling SM Higgs 125 GeV
            ################################################
            print "Doing SM HIggs Background"
            for HiggsBG in range(len(SMHiggs_BackGround)):

                tDirectory.cd()
                Histogram = Observable+"_mTLess30_OS"
                XLoc= icat + len(category)*chl + 1
                YLoc= lenghtSig + 9 + HiggsBG
                normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
                Name= str(SMHiggs_BackGround[HiggsBG])
                NameOut= Name+str(TauScaleOut[tscale])

                SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
                SampleHisto=SampleFile.Get("XXX")
                if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NameOut)
#                tDirectory.WriteObject(RebinedHist,Name+"_CMS_eff_t_mssmHigh_mutau_8TeVUp")
#                tDirectory.WriteObject(RebinedHist,Name+"_CMS_eff_t_mssmHigh_mutau_8TeVDown")

                

        ################################################
        #  Filling Other Nackground
        ################################################
            print "Doing SM HIggs Background"
            for OtherBG in range(len(Other_BackGround)):
        
                   tDirectory.cd()
                   Histogram = Observable+"_mTLess30_OS"
                   XLoc= icat + len(category)*chl + 1
                   YLoc= lenghtSig + 15 + OtherBG
                   normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
                   Name= str(Other_BackGround[OtherBG])
                   NameOut= "ZTT_lowMass"+str(TauScaleOut[tscale])
                
                #FIXME   Due to the lack of statitics I get the shaoe from inclusive
                #                NameCat= "_inclusive"
                   SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)
                   SampleHisto=SampleFile.Get("XXX")
                   if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
                   RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                   tDirectory.WriteObject(RebinedHist,NameOut)


    myOut.Close()





            
if __name__ == "__main__":

#    MakeTheHistogram("mutau","_SVMass","_8TeV",0)
#    MakeTheHistogram("etau","_SVMass","_8TeV",1)
#    MakeTheHistogram("mutau","_l1Pt","_8TeV",0)
#    MakeTheHistogram("etau","_l1Pt","_8TeV",1)
#    MakeTheHistogram("mutau","_l2Pt","_8TeV",0)
#    MakeTheHistogram("etau","_l2Pt","_8TeV",1)
#    MakeTheHistogram("mutau","_l1Eta","_8TeV",0)
#    MakeTheHistogram("etau","_l1Eta","_8TeV",1)
#    MakeTheHistogram("mutau","_l2Eta","_8TeV",0)
#    MakeTheHistogram("etau","_l2Eta","_8TeV",1)
    MakeTheHistogram("mutau","_MET","_8TeV",0)
    MakeTheHistogram("etau","_MET","_8TeV",1)

