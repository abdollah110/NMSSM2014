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
DIR_ROOT = 'outRoot_V3/'

signal = ['ggh', 'bbh']
mass = [80,90,  100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]
W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']
Top_BackGround = ['TTJets_FullLeptMGDecays','TTJets_SemiLeptMGDecays',  'TTJets_HadronicMGDecays', 'Tbar_tW', 'T_tW']
DiBoson_BackGround = [ 'WWJetsTo2L2Nu',  'WZJetsTo2L2Q', 'WZJetsTo3LNu',  'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']
Embedded = ['EmbeddedmuTau', 'EmbeddedeleTau']
Data = ['Data']






lenghtSig = len(signal) * len(mass)
#Histogram = "VisibleMass_"
#category_ = ["_inclusive"]
category_ = ["_inclusive", "_nobtag", "_btag"]
#category_ = ["_inclusive", "_nobtag"]
#channel = ["MuTau", "ETau"]
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

Binning_NoBTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500])
Binning_BTag = array.array("d",[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500])

def _Return_Value_Signal(bb,Name, channel,cat,Histo,PostFix,CoMEnergy,changeHistoName ):
    myfile = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    if cat=="_btag" and changeHistoName : cat = "_btagLoose" #; print "___________+++++++++++++++++++", str(channel)+str(Histo) + str(cat)
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

def MakeTheHistogram(channel,Observable,CoMEnergy,chl):
    myOut = TFile("TotalRootForLimit_"+channel + CoMEnergy+".root" , 'RECREATE')
    Table_File = TFile("Yield"+CoMEnergy+""+".root")
    Table_Hist = Table_File.Get('FullResults')
#    #ScaleUp
#    Table_FileUp = TFile("Yield"+CoMEnergy+"_Up"+".root")
#    Table_HistUp = Table_FileUp.Get('FullResults')
#    #ScaleDown
#    Table_FileDown = TFile("Yield"+CoMEnergy+"_Down"+".root")
#    Table_HistDown = Table_FileDown.Get('FullResults')

    

    
    categ=-1
    for category in category_:
        categ =categ +1
        print "starting category and channel", category, channel
        if category=="_nobtag" or category=="_inclusive"  : BinCateg = Binning_NoBTag
        if category=="_btag" or   category == "_btagLoose": BinCateg = Binning_BTag
        tDirectory= myOut.mkdir(channel + str(category))
        tDirectory.cd()
        ###################################### Filling Signal ZH and WH ########
        for sig in range(len(signal)):
            for m in range(len(mass)):#    for m in range(110, 145, 5):
   
                tDirectory.cd()
                Histogram = Observable+"_mTLess30_OS"
                XLoc= categ + 3*chl + 1
                YLoc= sig * len(mass) + m + 1
                normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
                Name= str(signal[sig]) + "_"+str(mass[m])
                NewHIST =TH1F(Name,"",len(BinCateg)-1,BinCateg)
                
                for bb in range(0,len(BinCateg)-1):
                    NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,False)[0])
                    NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,False)[1])

                if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
                myOut.Write()

           ################################################
#            #  Filling VV
#            ################################################
        print "Doing VV, BG estimation"
        tDirectory.cd()

        Histogram = Observable+"_mTLess30_OS"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig  +1
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name= "VVAll"
        NewHIST =TH1F("VV","",len(BinCateg)-1,BinCateg)


        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
            NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
        myOut.Write()
            ################################################
            #  Filling TOP
            ################################################
        print "Doing TOP, BG estimation"
        tDirectory.cd()

        Histogram = Observable+"_mTLess30_OS"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig  +2
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name= "TTAll"
        NewHIST =TH1F("TT","",len(BinCateg)-1,BinCateg)


        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
            NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
        myOut.Write()
            ################################################
        print "Doing ZL, BG estimation"
        tDirectory.cd()

        Histogram = Observable+"_mTLess30_OS_ZL"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig  +3
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name= "DYJetsAll"
        NewHIST =TH1F("ZL","",len(BinCateg)-1,BinCateg)


        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
            NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
        myOut.Write()

        #######################################  Filling Reducible BG ##########
        print "Doing ZJ, BG estimation"
        tDirectory.cd()
        Histogram = Observable+"_mTLess30_OS_ZJ"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig + 4
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name= "DYJetsAll"
        NewHIST =TH1F("ZJ","",len(BinCateg)-1,BinCateg)


        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
            NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
        myOut.Write()

        #        #######################################  Filling Reducible BG ##########
        print "Doing ZTT, BG estimation"
        tDirectory.cd()
        Histogram = Observable+"_mTLess30_OS"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig + 5
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name= "Embedded"+ channel
        NewHIST =TH1F("ZTT","",len(BinCateg)-1,BinCateg)


        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,False)[0])
            NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,False)[1])

        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())

        myOut.Write()
        #        #######################################  Filling Reducible BG ##########
        print "Doing W, BG estimation"
        tDirectory.cd()
        Histogram = Observable+"_mTLess30_OS"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig + 6
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name='WJetsAll'
        NewHIST =TH1F("W","",len(BinCateg)-1,BinCateg)


        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
            NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())

        myOut.Write()
        #        #######################################  Filling Reducible BG QCD ##########
        print "Doing QCD, BG estimation"

        Histogram = Observable+"_QCDshape_SS"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig + 7
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name='Data'
#        if category=="_btag"  : category = "_btagLoose"
        if category=="_btag"  : category = "_inclusive"




        XLocQCD= categ + 3*(chl+2) + 1
        YLoc= lenghtSig +  1
        file_VV = TFile(SubRootDir + "out_VVAll"+ CoMEnergy+ '.root')
        Histo_VV =  file_VV.Get(channel+Histogram + category+ "")
        Histo_VV.Scale(Table_Hist.GetBinContent(XLocQCD,YLoc))

        XLocQCD= categ + 3*(chl+2) + 1
        YLoc= lenghtSig + 2
        file_TT = TFile(SubRootDir + "out_TTAll"+  CoMEnergy+ '.root')
        Histo_TT =  file_TT.Get(channel+Histogram + category+ "")
        Histo_TT.Scale(Table_Hist.GetBinContent(XLocQCD,YLoc))

        XLocQCD= categ + 3*(chl+2) + 1
        YLoc= lenghtSig + 3
        file_ZL = TFile(SubRootDir + "out_DYJetsAll"+  CoMEnergy+ '.root')
        Histo_ZL =  file_ZL.Get(channel+Observable+"_QCDshape_SS_ZL" + category+ "")
        Histo_ZL.Scale(Table_Hist.GetBinContent(XLocQCD,YLoc))

        XLocQCD= categ + 3*(chl+2) + 1
        YLoc= lenghtSig + 4
        file_ZJ = TFile(SubRootDir + "out_DYJetsAll"+  CoMEnergy+ '.root')
        Histo_ZJ =  file_ZJ.Get(channel+Observable+"_QCDshape_SS_ZJ" + category+ "")
        Histo_ZJ.Scale(Table_Hist.GetBinContent(XLocQCD,YLoc))

        XLocQCD= categ + 3*(chl+2) + 1
        YLoc= lenghtSig + 5
        file_ZTT = TFile(SubRootDir + "out_DYJetsAll"+  CoMEnergy+ '.root')
        Histo_ZTT =  file_ZTT.Get(channel+Observable+"_QCDshape_SS_ZTT" + category+ "")
        Histo_ZTT.Scale(Table_Hist.GetBinContent(XLocQCD,YLoc))

#        if category=="_btagLoose"  : category = "_nobtag"
        if category=="_btagLoose"  : category = "_inclusive"
        XLocQCD= categ + 3*(chl+2) + 1
        YLoc= lenghtSig + 6
        file_W = TFile(SubRootDir + "out_WJetsAll"+  CoMEnergy+ '.root')
        Histo_W =  file_W.Get(channel+Observable+"_QCDshape_SS" + category+ "")
        Histo_W.Scale(Table_Hist.GetBinContent(XLocQCD,YLoc))

        file_QCD = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
        Histo_QCD =  file_QCD.Get(channel+Histogram + category+ "")


        print "Histo_VV= ", Histo_VV.Integral()
        Histo_VV.Add(Histo_TT)
        print "Histo_VV= ", Histo_VV.Integral()
        Histo_VV.Add(Histo_ZL)
        print "Histo_VV= ", Histo_VV.Integral()
        Histo_VV.Add(Histo_ZJ)
        print "Histo_VV= ", Histo_VV.Integral()
        Histo_VV.Add(Histo_ZTT)
        print "Histo_VV= ", Histo_VV.Integral()
        Histo_VV.Add(Histo_W)
        print "Histo_VV= ", Histo_VV.Integral()
        Histo_VV.Scale(-1)
        print "Histo_VV= ", Histo_VV.Integral()
        Histo_QCD.Add(Histo_VV)
        print "Histo_QCD= ", Histo_QCD.Integral()


        if category=="_nobtag" or category=="_inclusive"  : Histo_QCDN= Histo_QCD.Rebin(len(Binning_NoBTag)-1,"",Binning_NoBTag)
        if category=="_btag" or   category == "_btagLoose": Histo_QCDN = Histo_QCD.Rebin(len(Binning_BTag)-1,"",Binning_BTag)

        tDirectory.cd()
        NewHIST =TH1F("QCD","",len(BinCateg)-1,BinCateg)
        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,Histo_QCDN.GetBinContent(bb))
            NewHIST.SetBinError(bb,Histo_QCDN.GetBinError(bb))
        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
        myOut.Write()

        #        #######################################  Filling Data ##########
        print "Doing Data estimation"
        tDirectory.cd()
        Histogram = Observable+"_mTLess30_OS"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig + 8
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name='Data'
        NewHIST =TH1F("data_obs","",len(BinCateg)-1,BinCateg)


        for bb in range(0,len(BinCateg)-1):
            NewHIST.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
            NewHIST.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

        if NewHIST.Integral(): NewHIST.Scale(normal/NewHIST.Integral())
        myOut.Write()

            
if __name__ == "__main__":

    MakeTheHistogram("muTau","_SVMass","_8TeV",0)
    MakeTheHistogram("eleTau","_SVMass","_8TeV",1)
#    MakeTheHistogram("_inclusive","MuTau","_8TeV",0,0)
#    MakeTheHistogram("_nobtag","MuTau","_8TeV",1,0)
#    MakeTheHistogram("_btag","MuTau","_8TeV",2,0)
#    MakeTheHistogram("VisibleMass_","_8TeV")
#    MakeTheHistogram("SVMass_","_7TeV")
#    MakeTheHistogram("VisibleMass_","_7TeV")
            