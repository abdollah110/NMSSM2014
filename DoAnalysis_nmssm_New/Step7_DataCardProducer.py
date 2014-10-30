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
ZLUncertainty = True
WShapeUncertainty = True
verbos_ = False

high_bin = 300
ptBinning = 300
FineBinVal=5
digit = 3
QCDScaleFactor = 1.06

Binning_NoBTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300])
Binning_BTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300])
TauScale = ["Down", "", "Up"]
POSTFIX=["","Up","Down"]

signal = ['bba1GenFil_']
signalName = ['bba1_']
mass = [25,30,  35, 40, 45, 50, 55,  60, 65, 70, 75, 80]
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']
lenghtSig = len(signal) * len(mass) +1

category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
channelDirectory = ["muTau", "eleTau"]
SystematicSignal = ["_SVMassTauHighPtRWUp","_SVMassTauHighPtRWDown","_SVMassHiggPtRWUp","_SVMassHiggPtRWDown"]
SystematicSignalScale = ["_SVMassHiggPtRWUp","_SVMassHiggPtRWDown"]

####################################################
##   Functions
####################################################

def luminosity(CoMEnergy):
    if CoMEnergy == '_8TeV': return  19710 #19242
    if CoMEnergy == '_7TeV': return  4982
    
def QCDUncertaintyNameFR(unc,channel,NameCat,CoMEnergy):
    if unc=="": return 'QCD'
    if unc== "Up": return "QCD_CMS_htt_QCDfrShape_"+channel+CoMEnergy+"Up"
    if unc== "Down": return "QCD_CMS_htt_QCDfrShape_"+channel+CoMEnergy+"Down"

def QCDUncertaintyName(unc,channel,NameCat,CoMEnergy):
    if unc=="": return 'QCD_'
    if unc== "Up": return "QCD_CMS_htt_QCDShape_"+channel+NameCat+CoMEnergy+"Up"
    if unc== "Down": return "QCD_CMS_htt_QCDShape_"+channel+NameCat+CoMEnergy+"Down"

def getTauFakeCorrection(pt):

  #new corrections (Mar14, new T-ES correction)
  correction = 0;
  p0 =  0.787452;
  p1 = -0.146412;
  p2 = -0.0276803;
  p3 = -0.0824184;
  X = (pt-149.832)/100;  #//(x-meanPt)/100
  correction = p0+p1*X+p2*X*X+p3*X*X*X;

  return correction;


def _Return_SigBGData_Shape(Name, channel,cat,HistoName,PostFix,CoMEnergy,changeHistoName):
    myfile = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    if cat=="_btag" and changeHistoName : cat = "_btagLoose" 
    Histo =  myfile.Get(channel+HistoName + cat+ PostFix)
#    NewFile=TFile("Extra/XXX2out_"+Name +CoMEnergy+channel+HistoName+ cat+PostFix+".root","RECREATE")
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewFile.WriteObject(Histo,"XXX")
    myfile.Close()
    return NewFile

def _Return_Embed_Shape(Name, channel,cat,HistoName,PostFix,CoMEnergy,normal,normalTT,changeHistoName):

    DataEmbedded = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    Histo =  DataEmbedded.Get(channel+HistoName + cat+ PostFix)
    Histo.Scale(normal/Histo.Integral())
    print "data embed= ", Histo.Integral()
    TTEmbedded=TFile(SubRootDir + "out_TT"+ Name +CoMEnergy+ '.root')
    HistoTTEmbedded= TTEmbedded.Get(channel+HistoName + cat+ PostFix)
    if HistoTTEmbedded:
        HistoTTEmbedded.Scale(normalTT/HistoTTEmbedded.Integral())
        print "MC embed= ", HistoTTEmbedded.Integral()
        Histo.Add(HistoTTEmbedded,-1)

#    NewFile=TFile("Extra/XXX2out_"+Name +CoMEnergy+channel+HistoName+ cat+PostFix+".root","RECREATE")
    NewFile=TFile("Extra/XXX.root","RECREATE")
    print "Final test embed= ", Histo.Integral()
    NewFile.WriteObject(Histo,"XXX")
    DataEmbedded.Close()
    TTEmbedded.Close()
    return NewFile

def _Return_QCD_Shape(channel,cat,Histo,UncShape,PostFix,CoMEnergy):
    myfile = TFile("QCDFinalFile.root")
    HistoNorm =  myfile.Get(channel+Histo + cat+ UncShape+PostFix)
    if cat=="_btag" : cat = "_btagLoose" 
    HistoShape =  myfile.Get(channel+Histo + cat+ UncShape+PostFix)
    HistoShape.Scale(HistoNorm.Integral()/HistoShape.Integral())
#    NewFile=TFile("Extra/XXX2out_QCD" +CoMEnergy+channel+ cat+PostFix+".root","RECREATE")
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewFile.WriteObject(HistoShape,"XXX")
    myfile.Close()
    return NewFile

def _Return_W_Shape(channel,NameCat,Histogram,CoMEnergy,PostFix,changeHistoName):
#    NewFile=TFile("Extra/XXX2out_W" +CoMEnergy+channel+ NameCat+PostFix+".root","RECREATE")
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewHIST =TH1F("XXX","",high_bin,0,high_bin)
    NewHISTUp =TH1F("XXXUp","",high_bin,0,high_bin)
    NewHISTDown =TH1F("XXXDown","",high_bin,0,high_bin)
    WShapeFile = TFile(SubRootDir + "out_WJetsAll"+CoMEnergy+ '.root')
    if NameCat=="_btag" and changeHistoName : NameCat = "_btagLoose"
    Histo = WShapeFile.Get(channel+Histogram+NameCat+PostFix)
    for qq in range(high_bin):
        NormInPtBin=0
        NormInPtBinUp=0
        NormInPtBinDown=0
        for ss in range(ptBinning):
            weight= getTauFakeCorrection(ss+0.5)
            NormInPtBin += weight*Histo.GetBinContent(qq+1,ss+1)
            NormInPtBinUp += (weight + 0.5*(1-weight))*Histo.GetBinContent(qq+1,ss+1)
            NormInPtBinDown += (weight - 0.5*(1-weight))*Histo.GetBinContent(qq+1,ss+1)
        NewHIST.SetBinContent(qq+1,NormInPtBin)
        NewHISTUp.SetBinContent(qq+1,NormInPtBinUp)
        NewHISTDown.SetBinContent(qq+1,NormInPtBinDown)
    NewFile.WriteObject(NewHIST,"XXX")
    NewFile.WriteObject(NewHISTUp,"XXXUp")
    NewFile.WriteObject(NewHISTDown,"XXXDown")
    return NewFile


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


    Table_HighPtRWUp = TFile("Yield_SVMassTauHighPtRWUp"+CoMEnergy+""+".root")
    Histo_HighPtRWUp = Table_HighPtRWUp.Get('FullResults')
    Table_HighPtRWDown = TFile("Yield_SVMassTauHighPtRWDown"+CoMEnergy+""+".root")
    Histo_HighPtRWDown = Table_HighPtRWDown.Get('FullResults')
    Table_HiggsPtRWUp = TFile("Yield_SVMassHiggPtRWUp"+CoMEnergy+""+".root")
    Histo_HiggsPtRWUp = Table_HiggsPtRWUp.Get('FullResults')
    Table_HiggsPtRWDown = TFile("Yield_SVMassHiggPtRWDown"+CoMEnergy+""+".root")
    Histo_HiggsPtRWDown = Table_HiggsPtRWDown.Get('FullResults')

    SysTable= [Histo_HighPtRWUp,Histo_HighPtRWDown,Histo_HiggsPtRWUp,Histo_HiggsPtRWDown]


    
    TauScaleOut = ["_CMS_scale_t_"+channel+CoMEnergy+"Down", "", "_CMS_scale_t_"+channel+CoMEnergy+"Up"]
    Signal_Unc_Out = ["_CMS_eff_t_mssmHigh_"+channel+CoMEnergy+"Up","_CMS_eff_t_mssmHigh_"+channel+CoMEnergy+"Down", "_CMS_htt_higgsPtReweight"+CoMEnergy+"Up","_CMS_htt_higgsPtReweight"+CoMEnergy+"Down"]
    Signal_Unc_OutScale = ["_CMS_htt_higgsPtReweight_scale"+CoMEnergy+"Up","_CMS_htt_higgsPtReweight_scale"+CoMEnergy+"Down","_CMS_htt_higgsPtReweight_scaletest"+CoMEnergy+"Up","_CMS_htt_higgsPtReweight_scaletest"+CoMEnergy+"Down"]

#    TH1.AddDirectory(0)
    myOut = TFile("TotalRootForLimit_"+channel + CoMEnergy+".root" , 'RECREATE') # Name Of the output file

    icat=-1
    for NameCat in category:
        icat =icat +1
        print "starting NameCat and channel", NameCat, channel
        if NameCat=="_nobtag" or NameCat=="_inclusive"  : BinCateg = Binning_NoBTag
        if NameCat=="_btag" or   NameCat == "_btagLoose": BinCateg = Binning_BTag
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
                    if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
                    RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                    tDirectory.WriteObject(RebinedHist,NameOut)
                    
                   ###############  Systematics on Shape and Norm for Higgs Pt nad High Tau Pt   ####
                    if MSSMSignalUncertainty and tscale==1 and str(signal[sig]) == 'ggH':
                        for syst in range(len(SystematicSignal)):
                            tDirectory.cd()
                            Histogram = SystematicSignal[syst]+"_mTLess30_OS"
                            normal = SysTable[syst].GetBinContent(XLoc,YLoc)    #Get the Noralization
                            NameOut= str(signal[sig]) +str(mass[m])+str(Signal_Unc_Out[syst])
                            NameOutScale= str(signal[sig]) +str(mass[m])+str(Signal_Unc_OutScale[syst])

                            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
                            SampleHisto=SampleFile.Get("XXX")
                            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
                            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                            tDirectory.WriteObject(RebinedHist,NameOut)
                            tDirectory.WriteObject(RebinedHist,NameOutScale) # This part need to get updated  FIXME

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
            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")


            ######   Top Uncertainty     ##########################################
            if TopUncertainty and tscale==1:
                
                HistogramttbarUp = Observable+"TopPtRWUp_mTLess30_OS"
                HistogramttbarDown = Observable+"TopPtRWDown_mTLess30_OS"
                NamettbarUp="TT_CMS_htt_ttbarPtReweight"+CoMEnergy+"Up"
                NamettbarDown="TT_CMS_htt_ttbarPtReweight"+CoMEnergy+"Down"
                
                SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, HistogramttbarUp, TauScale[tscale],CoMEnergy,False)
                SampleHisto=SampleFile.Get("XXX")
                if SampleHisto.Integral(): SampleHisto.Scale(luminosity(CoMEnergy))
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NamettbarUp)

                SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, HistogramttbarDown, TauScale[tscale],CoMEnergy,False)
                SampleHisto=SampleFile.Get("XXX")
                if SampleHisto.Integral(): SampleHisto.Scale(luminosity(CoMEnergy))
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NamettbarDown)

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

            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
            SampleHisto=SampleFile.Get("XXX")
            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")

            #######   ZL Uncertainty   just for eleTau channel Basically ######################################################
            if ZLUncertainty and tscale==1:

                HistoZLScaleUp = "_SVMassZLScaleUp_mTLess30_OS_ZL"
                HistoZLScaleDown = "_SVMassZLScaleDown_mTLess30_OS_ZL"
                NameOutZLUp= "ZL_CMS_htt_ZLScale_"+channel+CoMEnergy+"Up"
                NameOutZLDown= "ZL_CMS_htt_ZLScale_"+channel+CoMEnergy+"Down"

                SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, HistoZLScaleUp, TauScale[tscale],CoMEnergy,False)
                SampleHisto=SampleFile.Get("XXX")
                if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())  # Same Normalization as ZL
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NameOutZLUp)

                SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, HistoZLScaleDown, TauScale[tscale],CoMEnergy,False)
                SampleHisto=SampleFile.Get("XXX")
                if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())  # Same Normalization as ZL
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NameOutZLDown)

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

            SampleFile= _Return_SigBGData_Shape(Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)
            SampleHisto=SampleFile.Get("XXX")
            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
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
#            Histogram = Observable+"Wshape2D_mTLess30_OS"
            Histogram = "_Wshape2D_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig + 6
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization Also for Uncertainties
            Name='WJetsAll'
            NameOut= "W"+str(TauScaleOut[tscale])

            SampleFile= _Return_W_Shape(channel,NameCat,Histogram,CoMEnergy,TauScale[tscale],True)
            SampleHisto=SampleFile.Get("XXX")
            if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
            RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
            tDirectory.WriteObject(RebinedHist,NameOut)
            if doFineBinning:
                RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                tDirectory.WriteObject(RebinedHistFinBin,NameOut+"_fine_binning")

            #######   W Shape Uncertainty  ######################################################
            if WShapeUncertainty and tscale==1:
                NameOutUp= "W_CMS_htt_WShape_"+channel+NameCat+CoMEnergy+"Up"
                NameOutDown= "W_CMS_htt_WShape_"+channel+NameCat+CoMEnergy+"Down"

                SampleHisto=SampleFile.Get("XXXUp")
                if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NameOutUp)
                if doFineBinning:
                    RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                    tDirectory.WriteObject(RebinedHistFinBin,NameOutUp+"_fine_binning")

                SampleHisto=SampleFile.Get("XXXDown")
                if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
                RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                tDirectory.WriteObject(RebinedHist,NameOutDown)
                if doFineBinning:
                    RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                    tDirectory.WriteObject(RebinedHistFinBin,NameOutDown+"_fine_binning")

            ################################################
            #  Filling QCD
            ################################################
            if tscale ==1:
                print "Doing QCD, BG estimation"
                for unc in POSTFIX:
                    tDirectory.cd()
                    Histogram = "_QCDShapeNormTotal"
                    Name='Data'
                    NameOut= 'QCD'

                    SampleFile= _Return_QCD_Shape(channel,NameCat, Histogram,unc, TauScale[tscale],CoMEnergy)
                    SampleHisto=SampleFile.Get("XXX")
                    RebinedHist= SampleHisto.Rebin(len(BinCateg)-1,"",BinCateg)
                    tDirectory.WriteObject(RebinedHist,QCDUncertaintyNameFR(unc, channel, NameCat, CoMEnergy))
                    tDirectory.WriteObject(RebinedHist,QCDUncertaintyName(unc, channel, NameCat, CoMEnergy))
                    if doFineBinning:
                        RebinedHistFinBin= SampleHisto.Rebin(FineBinVal)
                        tDirectory.WriteObject(RebinedHistFinBin,QCDUncertaintyNameFR(unc, channel, NameCat, CoMEnergy)+"_fine_binning")
                        tDirectory.WriteObject(RebinedHistFinBin,QCDUncertaintyName(unc, channel, NameCat, CoMEnergy)+"_fine_binning")

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
                if SampleHisto.Integral(): SampleHisto.Scale(normal/SampleHisto.Integral())
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
                tDirectory.WriteObject(RebinedHist,Name+"_CMS_eff_t_mssmHigh_mutau_8TeVUp")
                tDirectory.WriteObject(RebinedHist,Name+"_CMS_eff_t_mssmHigh_mutau_8TeVDown")
                tDirectory.WriteObject(RebinedHist,Name+"_CMS_htt_higgsPtReweightSM_8TeVUp")
                tDirectory.WriteObject(RebinedHist,Name+"_CMS_htt_higgsPtReweightSM_8TeVDown")


    myOut.Close()



        

            
if __name__ == "__main__":

    MakeTheHistogram("mutau","_SVMass","_8TeV",0)
    MakeTheHistogram("etau","_SVMass","_8TeV",1)

