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
import array

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
#ROOT.gROOT.ProcessLine('.x rootlogon.C')
SubRootDir = 'OutFiles/'

n_bin = 50
low_bin = 0
high_bin = 1000
reb_ = high_bin / n_bin
DIR_ROOT = 'outRoot/'

#signal = ['ggh', 'bbh']
signal = ['ggH', 'bbH']
mass = [80,90,  100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]
W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']
Top_BackGround = ['TTJets_FullLeptMGDecays','TTJets_SemiLeptMGDecays',  'TTJets_HadronicMGDecays', 'Tbar_tW', 'T_tW']
DiBoson_BackGround = [ 'WWJetsTo2L2Nu',  'WZJetsTo2L2Q', 'WZJetsTo3LNu',  'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
#Embedded = ['EmbeddedMuTau', 'EmbeddedETau']
Embedded = ['EmbeddedmuTau', 'EmbeddedeleTau']
Data = ['Data']
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']






#lenghtSig = len(signal) * len(mass)
#Histogram = "VisibleMass_"
#category = ["_inclusive"]
category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#category = ["_inclusive", "_nobtag"]
channelDirectory = ["muTau", "eleTau"]
#    makeSystematic2DTable("_SVMassTauHighPtRWUp","", "_8TeV")
#    makeSystematic2DTable("_SVMassTauHighPtRWDown","", "_8TeV")
#    makeSystematic2DTable("_SVMassHiggPtRWUp","", "_8TeV")
#    makeSystematic2DTable("_SVMassHiggPtRWDown","", "_8TeV")
SystematicSignal = ["_SVMassTauHighPtRWUp","_SVMassTauHighPtRWDown","_SVMassHiggPtRWUp","_SVMassHiggPtRWDown"]
#channel = ["MuTau"]
lenghtSig = len(signal) * len(mass) +1
lenghtVV = len(DiBoson_BackGround) +1
lenghtTop = len(Top_BackGround) +1
lenghtZL = len(Z_BackGround) + 1
lenghtZJ = len(Z_BackGround) + 1
lenghtZTT = len(Z_BackGround) + 1
low_bin = 0
high_bin = 1000
digit = 3
verbos_ = True
QCDScaleFactor = 1.06
DoFakeRateUsingSSOverOSRatio = False

Binning_NoBTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500])
Binning_BTag = array.array("d",[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500])
TauScale = ["Down", "", "Up"]
#TauScale = [ ""]

def _Return_Value_Signal(bb,Name, channel,cat,HistoName,PostFix,CoMEnergy,changeHistoName ):
    myfile = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    if cat=="_btag" and changeHistoName : cat = "_btagLoose" #; print "___________+++++++++++++++++++", str(channel)+str(Histo) + str(cat)
    Histo =  myfile.Get(channel+HistoName + cat+ PostFix)
    binCont = 0
    binErr = 0
    if Histo:
        if cat=="_nobtag" or cat=="_inclusive"  : RebinedHist= Histo.Rebin(len(Binning_NoBTag)-1,"NoBTag",Binning_NoBTag)
        if cat=="_btag" or   cat == "_btagLoose": RebinedHist = Histo.Rebin(len(Binning_BTag)-1,"BTag",Binning_BTag)

        binCont = RebinedHist.GetBinContent(bb)
        binErr = RebinedHist.GetBinError(bb)
    myfile.Close()
    return binCont , binErr

def _Return_Value_Embedded(bb,Name, channel,cat,HistoName,PostFix,CoMEnergy,changeHistoName,normal,normalTT ):
    myfile = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
#    if cat=="_btag" and changeHistoName : cat = "_btagLoose" #; print "___________+++++++++++++++++++", str(channel)+str(Histo) + str(cat)

    Histo =  myfile.Get(channel+HistoName + cat+ PostFix)
    if Histo:
        Histo.Scale(normal/Histo.Integral())

    TTEmbedded=TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    HistoTTEmbedded= TTEmbedded.Get(channel+HistoName + cat+ PostFix)
    if HistoTTEmbedded:
        HistoTTEmbedded.Scale(normalTT/HistoTTEmbedded.Integral())
        Histo.Add(HistoTTEmbedded,-1)
    binCont = 0
    binErr = 0
    if Histo:
        if cat=="_nobtag" or cat=="_inclusive"  : RebinedHist= Histo.Rebin(len(Binning_NoBTag)-1,"NoBTag",Binning_NoBTag)
        if cat=="_btag" or   cat == "_btagLoose": RebinedHist = Histo.Rebin(len(Binning_BTag)-1,"BTag",Binning_BTag)

        binCont = RebinedHist.GetBinContent(bb)
        binErr = RebinedHist.GetBinError(bb)
    myfile.Close()
    return binCont , binErr

def _Return_Value_QCD(bb,Name, channel,cat,Histo,PostFix,CoMEnergy,changeHistoName ):
    myfile = TFile("QCDFinalFile.root")
#    if cat=="_btag" and changeHistoName : cat = "_btagLoose" #; print "___________+++++++++++++++++++", str(channel)+str(Histo) + str(cat)
    Histo =  myfile.Get(channel+Histo + cat+ PostFix)
    binCont = 0
    binErr = 0
    if Histo:
        if cat=="_nobtag" or cat=="_inclusive"  : RebinedHist= Histo.Rebin(len(Binning_NoBTag)-1,"NoBTag",Binning_NoBTag)
        if cat=="_btag" or   cat == "_btagLoose": RebinedHist = Histo.Rebin(len(Binning_BTag)-1,"BTag",Binning_BTag)

        binCont = RebinedHist.GetBinContent(bb)
        binErr = RebinedHist.GetBinError(bb)
    myfile.Close()
    return binCont , binErr
def retrunQCDNormalization(channel,Histo,cat):
    myfile = TFile("QCDFinalFile.root")
    Histo =  myfile.Get(channel+Histo + cat)
    Normal= Histo.Integral()
    myfile.Close()
    return Normal

def MakeTheHistogram(channel,Observable,CoMEnergy,chl):
    #################### Name Of the output file
    myOut = TFile("TotalRootForLimit_"+channel + CoMEnergy+".root" , 'RECREATE')

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


    #################### Different Normalization Tables for QCD
#    Table_FileQCD = TFile("YieldQCD"+CoMEnergy+""+".root")
#    NormTableQCD_ = Table_FileQCD.Get('FullResults')
    
    TauScaleOut = ["_CMS_scale_t_"+channel+CoMEnergy+"Down", "", "_CMS_scale_t_"+channel+CoMEnergy+"Up"]
    Signal_Unc_Out = ["_CMS_eff_t_mssmHigh"+channel+CoMEnergy+"Up","_CMS_eff_t_mssmHigh"+channel+CoMEnergy+"Down", "_CMS_htt_higgsPtReweight"+CoMEnergy+"Up","_CMS_htt_higgsPtReweight"+CoMEnergy+"Down"]

    
    icat=-1
    for NameCat in category:
        icat =icat +1
        print "starting NameCat and channel", NameCat, channel
        if NameCat=="_nobtag" or NameCat=="_inclusive"  : BinCateg = Binning_NoBTag
        if NameCat=="_btag" or   NameCat == "_btagLoose": BinCateg = Binning_BTag
        tDirectory= myOut.mkdir(channelDirectory[chl] + str(NameCat))
        tDirectory.cd()
        for tscale in range(len(TauScale)):
        ###################################### Filling Signal ZH and WH ########
            for sig in range(len(signal)):
                for m in range(len(mass)):#    for m in range(110, 145, 5):
                    print "Now is processing", signal[sig],mass[m]
                    tDirectory.cd()
                    Histogram = Observable+"_mTLess30_OS"
                    XLoc= icat + len(category)*chl + 1
                    YLoc= sig * len(mass) + m + 1
                    normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
#                    Name= str(signal[sig])+"_" +str(mass[m])
                    Name= str(signal[sig])+str(mass[m])
                    NameOut= str(signal[sig]) +str(mass[m])+str(TauScaleOut[tscale])
                    NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)

                    for bb in range(0,len(BinCateg)-1):
                        NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)[0])
#                        NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,False)[1])

                    if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
                    myOut.Write()

           ################################################
           #  Systematics on Shaoe and Norm for Higgs Pt nad High Tau Pt
           ################################################
                    if tscale==1:
                        for syst in range(len(SystematicSignal)):
                            tDirectory.cd()
                            Histogram = SystematicSignal[syst]+"_mTLess30_OS"
                            normal = SysTable[syst].GetBinContent(XLoc,YLoc)    #Get the Noralization
                            NameOut= str(signal[sig]) +str(mass[m])+str(Signal_Unc_Out[syst])
                            NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)

                            for bb in range(0,len(BinCateg)-1):
                                NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)[0])
                                NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)[1])

                            if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
                            myOut.Write()

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
            NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)


            for bb in range(0,len(BinCateg)-1):
                NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)[0])
#                NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,True)[1])

            if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
            myOut.Write()
                ################################################
                #  Filling TOP
                ################################################
            print "Doing TOP, BG estimation"
            tDirectory.cd()

            Histogram = Observable+"_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig  +2
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "TTAll"
            NameOut= "TT"+str(TauScaleOut[tscale])
            NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)


            for bb in range(0,len(BinCateg)-1):
                NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)[0])
#                NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,True)[1])

            if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
            myOut.Write()
                ################################################
            print "Doing ZL, BG estimation"
            tDirectory.cd()

            Histogram = Observable+"_mTLess30_OS_ZL"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig  +3
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "DYJetsAll"
            NameOut= "ZL"+str(TauScaleOut[tscale])
            NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)


            for bb in range(0,len(BinCateg)-1):
                NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)[0])
#                NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,True)[1])

            if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
            myOut.Write()

            #######################################  Filling Reducible BG ##########
            print "Doing ZJ, BG estimation"
            tDirectory.cd()
            Histogram = Observable+"_mTLess30_OS_ZJ"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig + 4
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "DYJetsAll"
            NameOut= "ZJ"+str(TauScaleOut[tscale])
            NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)


            for bb in range(0,len(BinCateg)-1):
                NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)[0])
#                NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,True)[1])

            if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
            myOut.Write()

            #        #######################################  Filling Reducible BG ##########
            print "Doing ZTT, BG estimation"
            tDirectory.cd()
            Histogram = Observable+"_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig + 5
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            normalTTEmbedded= NormTable[tscale].GetBinContent(XLoc,lenghtSig  +7)    #Get the Noralization
            Name= "Embedded"+ channel
            NameOut= "ZTT"+str(TauScaleOut[tscale])
            NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)


            for bb in range(0,len(BinCateg)-1):
                NewHIST.SetBinContent(bb,_Return_Value_Embedded(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False,normal,normalTTEmbedded)[0])
#                NewHIST.SetBinError(bb,_Return_Value_Embedded(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False,normal,normalTTEmbedded)[1])

#            if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral()) #No Need to resacale it again

            myOut.Write()
            #        #######################################  Filling Reducible BG ##########
            print "Doing W, BG estimation"
            tDirectory.cd()
            Histogram = Observable+"_mTLess30_OS"
            XLoc= icat + len(category)*chl + 1
            YLoc= lenghtSig + 6
            normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name='WJetsAll'
            NameOut= "W"+str(TauScaleOut[tscale])
            NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)


            for bb in range(0,len(BinCateg)-1):
                NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)[0])
#                NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,True)[1])

            if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())

            myOut.Write()
            #        #######################################  Filling Reducible BG QCD ##########
            if tscale ==1:
                print "Doing QCD, BG estimation"
                Histogram = "_QCDShapeNormTotal"
                normal = retrunQCDNormalization(channel, Histogram,NameCat)
                print NameCat, "    Here is Normal QCD   ----------  ",normal
                Name='Data'
                tDirectory.cd()
                NewHIST =TH1F("QCD","",len(BinCateg)-1,BinCateg)

                if NameCat=="_btag": NameCat="_btagLoose"
                for bb in range(0,len(BinCateg)-1):
                    NewHIST.SetBinContent(bb,_Return_Value_QCD(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)[0])
    #                NewHIST.SetBinError(bb,_Return_Value_QCD(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,True)[1])

                if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
                myOut.Write()

                #        #######################################  Filling Data ##########
            if tscale ==1:
                print "Doing Data estimation"
                tDirectory.cd()
                Histogram = Observable+"_mTLess30_OS"
                XLoc= icat + len(category)*chl + 1
                YLoc= lenghtSig + 8
                normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
                Name='Data'
                NewHIST =TH1F("data_obs","",len(BinCateg)-1,BinCateg)


                for bb in range(0,len(BinCateg)-1):
                    NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,True)[0])
                    NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,True)[1])

                if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
                myOut.Write()

    ###################################### Filling Signal ZH and WH ########
            print "Doing SM HIggs Background"
            for HiggsBG in range(len(SMHiggs_BackGround)):

                tDirectory.cd()
                Histogram = Observable+"_mTLess30_OS"
                XLoc= icat + len(category)*chl + 1
                YLoc= lenghtSig + 9 + HiggsBG
                normal = NormTable[tscale].GetBinContent(XLoc,YLoc)    #Get the Noralization
                Name= str(SMHiggs_BackGround[HiggsBG])
                NameOut= Name+str(TauScaleOut[tscale])
                NewHIST =TH1F(NameOut,"",len(BinCateg)-1,BinCateg)

                for bb in range(0,len(BinCateg)-1):
                    NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,NameCat, Histogram, TauScale[tscale],CoMEnergy,False)[0])
#                    NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,NameCat,Histogram, TauScale[tscale],CoMEnergy,False)[1])

                if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
                myOut.Write()



        

            
if __name__ == "__main__":

#    MakeTheHistogram("muTau","_VisibleMass","_8TeV",0)
#    MakeTheHistogram("eleTau","_VisibleMass","_8TeV",1)
    MakeTheHistogram("mutau","_SVMass","_8TeV",0)
#    MakeTheHistogram("etau","_SVMass","_8TeV",1)

