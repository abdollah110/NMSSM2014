#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
#http://root.cern.ch/viewvc/trunk/tutorials/pyroot/hsimple.py?revision=20881&view=markup
__author__ = "abdollahmohammadi"
__date__ = "$Feb 23, 2013 10:39:33 PM$"

from numbers import Integral
import math
import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TF1
from ROOT import TH2F
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gSystem
from ROOT import TGraph
from ROOT import TGraphAsymmErrors
from ctypes import *
from ROOT import gStyle
import array

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
#ROOT.gROOT.ProcessLine('.x rootlogon.C')
SubRootDir = 'OutFiles/'

n_bin = 50
low_bin = 0
high_bin = 300
reb_ = high_bin / n_bin
DIR_ROOT = 'outRoot/'

#signal = ['ggh', 'bbh']
signal = ['ggH', 'bbH']
mass = [80,90, 100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]
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
#category_ = ["_inclusive"]
#category_ = ["_inclusive", "_nobtag", "_btag"]
category_ = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#categoryM = ["_inclusive", "_nobtag", "_btag"]
categoryM = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#category_ = ["_inclusive", "_nobtag"]
channelDirectory = ["muTau", "eleTau"]
channel = ["mutau","etau"]
#lenghtSig = len(signal) * len(mass) +1
lenghtSig = 0
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

Binning_NoBTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300])
Binning_BTag = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300])
Binning_PT = array.array("d",[0,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300])
TauScale = ["Down", "", "Up"]
#TauScale = [ ""]

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

def doRatio2D(num, denum, marColor):
    ratio = ROOT.TGraphAsymmErrors(num, denum, "")
    ratio.SetLineColor(marColor)
    ratio.SetLineWidth(2)
    return ratio

def MakeTheHistogram(channel,Observable,CoMEnergy,chl,etaRange):
    #################### Name Of the output file
    myOut = TFile("QCDTotalRootForLimit_"+channel + etaRange+CoMEnergy+".root" , 'RECREATE')

    #################### Different Normalization Tables
    Table_File = TFile("Yield"+etaRange+CoMEnergy+".root")
    NormTableOSIso = Table_File.Get('FullResultsOSIso')
    NormTableSSIso = Table_File.Get('FullResultsSSIso')
    NormTableOSRelax = Table_File.Get('FullResultsOSRelax')
    NormTableSSRelax = Table_File.Get('FullResultsSSRelax')
    
    
    categ=-1
    for category in category_:
        categ =categ +1
        print "starting category and channel", category, channel
#        if category=="_nobtag" or category=="_inclusive"  : BinCateg = Binning_NoBTag
#        if category=="_btag" or   category == "_btagLoose": BinCateg = Binning_BTag
        tDirectory= myOut.mkdir(channelDirectory[chl] + str(category))
        tDirectory.cd()
       ################################################
       
        #        #######################################  Filling Reducible BG QCD ##########
        print "Doing QCD, BG estimation"

        HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+etaRange
        HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+etaRange
        HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+etaRange
        HistoQCDShapeLowMTSSRelax = "_QCDshape2D_mTLess30_SS_RelaxIso"+ etaRange 
        HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange

        DYIndex = ""
        XLocQCD= categ + len(category_)*chl + 1
        YLoc= lenghtSig + 2
        file_TT = TFile(SubRootDir + "out_TTAll"+  CoMEnergy+ '.root')
        Histo_TTSSIso =  file_TT.Get(channel+HistoTauPtLowMTSSIso + DYIndex + category+ "")
        Histo_TTSSRelax =  file_TT.Get(channel+HistoTauPtLowMTSSRelax + DYIndex + category+ "")
        Histo_TTOSRelax =  file_TT.Get(channel+HistoTauPtLowMTOSRelax + DYIndex + category+ "")
        Histo_TTQCDShape2DSSRelax =  file_TT.Get(channel+HistoQCDShapeLowMTSSRelax + DYIndex + category+ "")
        if Histo_TTSSIso:    Histo_TTSSIso.Scale(NormTableSSIso.GetBinContent(XLocQCD,YLoc)/Histo_TTSSIso.Integral())
        if Histo_TTSSRelax:  Histo_TTSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_TTSSRelax.Integral())
        if Histo_TTOSRelax:  Histo_TTOSRelax.Scale(NormTableOSRelax.GetBinContent(XLocQCD,YLoc)//Histo_TTOSRelax.Integral())
        if Histo_TTQCDShape2DSSRelax:  Histo_TTQCDShape2DSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_TTQCDShape2DSSRelax.Integral())
#        if verbos_: print  NormTableSSIso.GetBinContent(XLocQCD,YLoc), NormTableSSRelax.GetBinContent(XLocQCD,YLoc), NormTableOSRelax.GetBinContent(XLocQCD,YLoc)

        
        DYIndex = ""
        XLocQCD= categ + len(category_)*chl + 1
        YLoc= lenghtSig +  1
        file_VV = TFile(SubRootDir + "out_VVAll"+ CoMEnergy+ '.root')
        Histo_VVSSIso =  file_VV.Get(channel+HistoTauPtLowMTSSIso + DYIndex + category+ "")
        Histo_VVSSRelax =  file_VV.Get(channel+HistoTauPtLowMTSSRelax + DYIndex + category+ "")
        Histo_VVOSRelax =  file_VV.Get(channel+HistoTauPtLowMTOSRelax + DYIndex + category+ "")
        Histo_VVQCDShape2DSSRelax =  file_TT.Get(channel+HistoQCDShapeLowMTSSRelax + DYIndex + category+ "")
        if Histo_VVSSIso:    Histo_VVSSIso.Scale(NormTableSSIso.GetBinContent(XLocQCD,YLoc)/Histo_VVSSIso.Integral())
        if Histo_VVSSRelax:  Histo_VVSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_VVSSRelax.Integral())
        if Histo_VVOSRelax:  Histo_VVOSRelax.Scale(NormTableOSRelax.GetBinContent(XLocQCD,YLoc)//Histo_VVOSRelax.Integral())
        if Histo_VVQCDShape2DSSRelax and Histo_VVQCDShape2DSSRelax.Integral():
            Histo_VVQCDShape2DSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_VVQCDShape2DSSRelax.Integral())
#        if verbos_: print "VV integrals in categories: ", Histo_VVSSIso.Integral(), Histo_VVSSRelax.Integral(), Histo_VVOSRelax.Integral()



            
        DYIndex = "_ZL"
        XLocQCD= categ + len(category_)*chl + 1
        YLoc= lenghtSig + 3
        file_ZL = TFile(SubRootDir + "out_DYJetsAll"+  CoMEnergy+ '.root')
        Histo_ZLSSIso =  file_ZL.Get(channel+HistoTauPtLowMTSSIso + DYIndex + category+ "")
        Histo_ZLSSRelax =  file_ZL.Get(channel+HistoTauPtLowMTSSRelax + DYIndex + category+ "")
        Histo_ZLOSRelax =  file_ZL.Get(channel+HistoTauPtLowMTOSRelax + DYIndex + category+ "")
        Histo_ZLQCDShape2DSSRelax =  file_ZL.Get(channel+HistoQCDShapeLowMTSSRelax + DYIndex + category+ "")
        if Histo_ZLQCDShape2DSSRelax:  Histo_ZLQCDShape2DSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_ZLQCDShape2DSSRelax.Integral())
        if Histo_ZLSSIso:    Histo_ZLSSIso.Scale(NormTableSSIso.GetBinContent(XLocQCD,YLoc)/Histo_ZLSSIso.Integral())
        if Histo_ZLSSRelax:  Histo_ZLSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_ZLSSRelax.Integral())
        if Histo_ZLOSRelax:  Histo_ZLOSRelax.Scale(NormTableOSRelax.GetBinContent(XLocQCD,YLoc)//Histo_ZLOSRelax.Integral())
#        if verbos_: print NormTableSSIso.GetBinContent(XLocQCD,YLoc), NormTableSSRelax.GetBinContent(XLocQCD,YLoc), NormTableOSRelax.GetBinContent(XLocQCD,YLoc)
   

        DYIndex = "_ZJ"
        XLocQCD= categ + len(category_)*chl + 1
        YLoc= lenghtSig + 4
        file_ZJ = TFile(SubRootDir + "out_DYJetsAll"+  CoMEnergy+ '.root')
        Histo_ZJSSIso =  file_ZJ.Get(channel+HistoTauPtLowMTSSIso + DYIndex + category+ "")
        Histo_ZJSSRelax =  file_ZJ.Get(channel+HistoTauPtLowMTSSRelax + DYIndex + category+ "")
        Histo_ZJOSRelax =  file_ZJ.Get(channel+HistoTauPtLowMTOSRelax + DYIndex + category+ "")
        Histo_ZJQCDShape2DSSRelax =  file_ZJ.Get(channel+HistoQCDShapeLowMTSSRelax + DYIndex + category+ "")
        if Histo_ZJQCDShape2DSSRelax:  Histo_ZJQCDShape2DSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_ZJQCDShape2DSSRelax.Integral())
        if Histo_ZJSSIso:    Histo_ZJSSIso.Scale(NormTableSSIso.GetBinContent(XLocQCD,YLoc)/Histo_ZJSSIso.Integral())
        if Histo_ZJSSRelax:  Histo_ZJSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_ZJSSRelax.Integral())
        if Histo_ZJOSRelax:  Histo_ZJOSRelax.Scale(NormTableOSRelax.GetBinContent(XLocQCD,YLoc)//Histo_ZJOSRelax.Integral())
#        if verbos_: print NormTableSSIso.GetBinContent(XLocQCD,YLoc), NormTableSSRelax.GetBinContent(XLocQCD,YLoc), NormTableOSRelax.GetBinContent(XLocQCD,YLoc)
    
        DYIndex = "_ZTT"
        XLocQCD= categ + len(category_)*chl + 1
        YLoc= lenghtSig + 5
        file_ZTT = TFile(SubRootDir + "out_DYJetsAll"+  CoMEnergy+ '.root')
        Histo_ZTTSSIso =  file_ZTT.Get(channel+HistoTauPtLowMTSSIso + DYIndex + category+ "")
        Histo_ZTTSSRelax =  file_ZTT.Get(channel+HistoTauPtLowMTSSRelax + DYIndex + category+ "")
        Histo_ZTTOSRelax =  file_ZTT.Get(channel+HistoTauPtLowMTOSRelax + DYIndex + category+ "")
        Histo_ZTTQCDShape2DSSRelax =  file_ZTT.Get(channel+HistoQCDShapeLowMTSSRelax + DYIndex + category+ "")
        if Histo_ZTTQCDShape2DSSRelax:  Histo_ZTTQCDShape2DSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_ZTTQCDShape2DSSRelax.Integral())
        if Histo_ZTTSSIso:    Histo_ZTTSSIso.Scale(NormTableSSIso.GetBinContent(XLocQCD,YLoc)/Histo_ZTTSSIso.Integral())
        if Histo_ZTTSSRelax:  Histo_ZTTSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_ZTTSSRelax.Integral())
        if Histo_ZTTOSRelax:  Histo_ZTTOSRelax.Scale(NormTableOSRelax.GetBinContent(XLocQCD,YLoc)//Histo_ZTTOSRelax.Integral())
#        if verbos_: print "+++++++++++++++++", Histo_ZTTOSRelax.Integral(), NormTableSSIso.GetBinContent(XLocQCD,YLoc), NormTableSSRelax.GetBinContent(XLocQCD,YLoc), NormTableOSRelax.GetBinContent(XLocQCD,YLoc)
     

        DYIndex = ""
        XLocQCD= categ + len(category_)*chl + 1
        YLoc= lenghtSig + 6
        file_W = TFile(SubRootDir + "out_WJetsAll"+  CoMEnergy+ '.root')
        Histo_WSSIso =  file_W.Get(channel+HistoTauPtLowMTSSIso + DYIndex + category+ "")
        Histo_WSSRelax =  file_W.Get(channel+HistoTauPtLowMTSSRelax + DYIndex + category+ "")
        Histo_WOSRelax =  file_W.Get(channel+HistoTauPtLowMTOSRelax + DYIndex + category+ "")
        Histo_WQCDShape2DSSRelax =  file_W.Get(channel+HistoQCDShapeLowMTSSRelax + DYIndex + category+ "")
        if Histo_WQCDShape2DSSRelax:  Histo_WQCDShape2DSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_WQCDShape2DSSRelax.Integral())
        if Histo_WSSIso:    Histo_WSSIso.Scale(NormTableSSIso.GetBinContent(XLocQCD,YLoc)/Histo_WSSIso.Integral())
        if Histo_WSSRelax:  Histo_WSSRelax.Scale(NormTableSSRelax.GetBinContent(XLocQCD,YLoc)/Histo_WSSRelax.Integral())
        if Histo_WOSRelax:  Histo_WOSRelax.Scale(NormTableOSRelax.GetBinContent(XLocQCD,YLoc)//Histo_WOSRelax.Integral())
#        if verbos_: print NormTableSSIso.GetBinContent(XLocQCD,YLoc), NormTableSSRelax.GetBinContent(XLocQCD,YLoc), NormTableOSRelax.GetBinContent(XLocQCD,YLoc)


        Name='Data'
#        if category=="_btag"  : category = "_btagLoose"   #FIXME
#        if category=="_btag"  : category = "_inclusive"
        file_QCD = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')

        Histo_QCDSSIso =  file_QCD.Get(channel+HistoTauPtLowMTSSIso + category+ "")
#        print "test existance of a histogram", Histo_VVSSIso.Integral()
        if Histo_VVSSIso: Histo_TTSSIso.Add(Histo_VVSSIso)
        if Histo_ZLSSIso: Histo_TTSSIso.Add(Histo_ZLSSIso)
        if Histo_ZJSSIso: Histo_TTSSIso.Add(Histo_ZJSSIso)
        if Histo_ZTTSSIso: Histo_TTSSIso.Add(Histo_ZTTSSIso)
        if Histo_WSSIso: Histo_TTSSIso.Add(Histo_WSSIso)
        if Histo_TTSSIso:
            Histo_TTSSIso.Scale(-1)
            Histo_QCDSSIso.Add(Histo_TTSSIso)


        Histo_QCDSSRelax =  file_QCD.Get(channel+HistoTauPtLowMTSSRelax + category+ "")
        if Histo_VVSSRelax: Histo_TTSSRelax.Add(Histo_VVSSRelax)
        if Histo_ZLSSRelax: Histo_TTSSRelax.Add(Histo_ZLSSRelax)
        if Histo_ZJSSRelax: Histo_TTSSRelax.Add(Histo_ZJSSRelax)
        if Histo_ZTTSSRelax: Histo_TTSSRelax.Add(Histo_ZTTSSRelax)
        if Histo_WSSRelax: Histo_TTSSRelax.Add(Histo_WSSRelax)
        if Histo_TTSSRelax:
            if Histo_TTSSRelax: Histo_TTSSRelax.Scale(-1)
            Histo_QCDSSRelax.Add(Histo_TTSSRelax)


        Histo_QCDOSRelax =  file_QCD.Get(channel+HistoTauPtLowMTOSRelax + category+ "")
        if Histo_VVOSRelax: Histo_TTOSRelax.Add(Histo_VVOSRelax)
        if Histo_ZLOSRelax: Histo_TTOSRelax.Add(Histo_ZLOSRelax)
        if Histo_ZJOSRelax: Histo_TTOSRelax.Add(Histo_ZJOSRelax)
        if Histo_ZTTOSRelax: Histo_TTOSRelax.Add(Histo_ZTTOSRelax)
        if Histo_WOSRelax: Histo_TTOSRelax.Add(Histo_WOSRelax)
        if Histo_TTOSRelax:
            Histo_TTOSRelax.Scale(-1)
            Histo_QCDOSRelax.Add(Histo_TTOSRelax)

        
        Histo_QCDQCDShape2DSSRelax =  file_QCD.Get(channel+HistoQCDShapeLowMTSSRelax + category+ "")
        if Histo_VVQCDShape2DSSRelax: Histo_TTQCDShape2DSSRelax.Add(Histo_VVQCDShape2DSSRelax)
        if Histo_ZLQCDShape2DSSRelax: Histo_TTQCDShape2DSSRelax.Add(Histo_ZLQCDShape2DSSRelax)
        if Histo_ZJQCDShape2DSSRelax: Histo_TTQCDShape2DSSRelax.Add(Histo_ZJQCDShape2DSSRelax)
        if Histo_ZTTQCDShape2DSSRelax: Histo_TTQCDShape2DSSRelax.Add(Histo_ZTTQCDShape2DSSRelax)
        if Histo_WQCDShape2DSSRelax: Histo_TTQCDShape2DSSRelax.Add(Histo_WQCDShape2DSSRelax)
        if Histo_TTQCDShape2DSSRelax:
            Histo_TTQCDShape2DSSRelax.Scale(-1)
            Histo_QCDQCDShape2DSSRelax.Add(Histo_TTQCDShape2DSSRelax)

        
        Histo_QCDNumerator= Histo_QCDSSIso.Rebin(len(Binning_PT)-1,"",Binning_PT)
        Histo_QCDDeNumerator= Histo_QCDSSRelax.Rebin(len(Binning_PT)-1,"",Binning_PT)
        Histo_QCDControlRegion= Histo_QCDOSRelax.Rebin(len(Binning_PT)-1,"",Binning_PT)

        tDirectory.cd()
        HistoNum =TH1F("QCDNumerator","",len(Binning_PT)-1,Binning_PT)
        HistoDeNum =TH1F("QCDDenumerator","",len(Binning_PT)-1,Binning_PT)
        NewHIST_ControlRegion =TH1F("QCDControlRegion","",len(Binning_PT)-1,Binning_PT)
        NewHIST_ControlRegionFinBin =TH1F("NewHIST_ControlRegionFinBin","",300,0,300)
        NewHIST_ControlRegionQCDShape2D =TH2F("NewHIST_ControlRegionQCDShape2D","",1500,0,1500,300,0,300)

        for bb in range(1,len(Binning_PT)):
            ## CAVEAT   Here I have set th enegative bins in numerator and denumerator to 0
            binValueNum= Histo_QCDNumerator.GetBinContent(bb)
            if binValueNum < 0 : binValueNum =0
            HistoNum.SetBinContent(bb,binValueNum)

            binValueDeNum= Histo_QCDDeNumerator.GetBinContent(bb)
            if binValueDeNum < 0 : binValueDeNum =0
            HistoDeNum.SetBinContent(bb,binValueDeNum)
            
            NewHIST_ControlRegion.SetBinContent(bb,Histo_QCDControlRegion.GetBinContent(bb))
            
        for aa in range(1,Histo_QCDOSRelax.GetNbinsX()):
            binValue = Histo_QCDOSRelax.GetBinContent(aa)
            if binValue < 0: binValue=0
            NewHIST_ControlRegionFinBin.SetBinContent(aa,binValue)

        for aa in range(1,Histo_QCDQCDShape2DSSRelax.GetNbinsX()):
            for bb in range(1,Histo_QCDQCDShape2DSSRelax.GetNbinsY()):
                binValue = Histo_QCDQCDShape2DSSRelax.GetBinContent(aa,bb)
                if binValue < 0: binValue=0
                NewHIST_ControlRegionQCDShape2D.SetBinContent(aa,bb,binValue)


        
        myOut.Write()
#############################################################################################################
##   Calculating the Fake Rate and applying Fake Rate
#############################################################################################################

def fitFunc_Exp3Par(x,par):
    return par[0] + par[1] + ((par[2] * x[0]))
def Func_Exp3Par(x,par0,par1,par2):
    return par0 + par1 + ((par2 * x))

# This fake rate is used for all Categories and channels but different for different eta range
def MakeFakeRateHisto(CoMEnergy,etaRange):
    NewFile = TFile("QCDTotalRootForLimit_"+ "mutau"+ etaRange+CoMEnergy+".root")
    HistoNum = NewFile.Get("muTau_inclusive/QCDNumerator")
    HistoDeNum = NewFile.Get("muTau_inclusive/QCDDenumerator")
    for i in range(HistoDeNum.GetNbinsX()):
            print HistoDeNum.GetBinContent(i+1) , HistoNum.GetBinContent(i+1)
            ## CAVEAT   Here I have set the denum Value equal to Num Value in case it is smaller
            if HistoDeNum.GetBinContent(i+1) < HistoNum.GetBinContent(i+1):
                HistoDeNum.SetBinContent(i+1, 0)
                HistoNum.SetBinContent(i+1, 0)
#                HistoDeNum.SetBinContent(i+1, HistoNum.GetBinContent(i+1))
    FRHisto=doRatio2D(HistoNum, HistoDeNum, 3)
    canvas =TCanvas("canvas", "", 700, 500)
    theFit=TF1("theFit", fitFunc_Exp3Par, 20, 200, 3)
    theFit.SetParameter(0, 0.6)
    theFit.SetParameter(1, 0.18)
    theFit.SetParameter(2, -0.2)
    FRHisto.Fit(theFit, "R0","")
    FRHisto.Draw("PAE")
    FitParam=theFit.GetParameters()
    theFit.Draw("SAME")
    canvas.SaveAs("fitResults_"+"mutau"+CoMEnergy+etaRange+".pdf")
    return  FitParam[0],FitParam[1],FitParam[2]

def ReturnScaledReadyHisto(CoMEnergy,etaRange,categ,chl):
    myOut = TFile("YieldShapeQCD"+CoMEnergy+".root", 'RECREATE')
    
    fitParameters=MakeFakeRateHisto(CoMEnergy,etaRange)  # same for muTau and eTau
    fitpar0= fitParameters[0]
    fitpar1= fitParameters[1]
    fitpar2= fitParameters[2]

    MainRootFile = TFile("QCDTotalRootForLimit_"+channel[chl] + etaRange + CoMEnergy+".root")
    HistoCR = MainRootFile.Get(channelDirectory[chl]+categoryM[categ]+"/NewHIST_ControlRegionQCDShape2D")

    myOut.cd()
    templateShape =TH1F("QCDShapeNorm","",1500,0,1500)

    for bb in range(1500):

        NormInPtBin=0
        for ss in range(300):
            NormInPtBin += Func_Exp3Par(ss+0.5,fitpar0,fitpar1,fitpar2)*HistoCR.GetBinContent(bb+1,ss+1)
        templateShape.SetBinContent(bb,NormInPtBin)


    XLoc= categ + len(category_)*chl + 1

    FileNorm = TFile("Yield"+etaRange+CoMEnergy+".root")
    normHistio=FileNorm.Get("FullResultsSSIsoQCDNorm")
    NormQCDMC=0
    for i in range(6):
        print "Backgrounds to be subtracted from data", i, normHistio.GetBinContent(XLoc,i+1)
        NormQCDMC +=normHistio.GetBinContent(XLoc,i+1)
    FinalQCDEstimate=(normHistio.GetBinContent(XLoc,7)-NormQCDMC) * QCDScaleFactor
    templateShape.Scale(FinalQCDEstimate/templateShape.Integral())
    myOut.Write()
    return myOut

def GetFinalQCDShapeNorm():
    FinalFile = TFile("QCDFinalFile.root", "RECREATE")

    for categ in range(len(categoryM)):
#        for chl in range(1):
        for chl in range(len(channel)):
            getFileBar=ReturnScaledReadyHisto("_8TeV","_Bar",categ,chl)
            getFileCen=ReturnScaledReadyHisto("_8TeV","_Cen",categ,chl)
            getFileEnd=ReturnScaledReadyHisto("_8TeV","_End",categ,chl)

            HistoBar=getFileBar.Get("QCDShapeNorm")
            HistoCen=getFileCen.Get("QCDShapeNorm")
            HistoEnd=getFileEnd.Get("QCDShapeNorm")

            FinalFile.cd()
            QCDShapeTotal =TH1F(channel[chl]+"_QCDShapeNormTotal"+categoryM[categ],"",1500,0,1500)
            for bb in range(1500):
                QCDShapeTotal.SetBinContent(bb, HistoBar.GetBinContent(bb)+HistoCen.GetBinContent(bb)+HistoEnd.GetBinContent(bb))

            FinalFile.Write()
                

if __name__ == "__main__":

    MakeTheHistogram("mutau","_SVMass","_8TeV",0,"_Bar")
    MakeTheHistogram("mutau","_SVMass","_8TeV",0,"_Cen")
    MakeTheHistogram("mutau","_SVMass","_8TeV",0,"_End")
    MakeTheHistogram("etau","_SVMass","_8TeV",1,"_Bar")
    MakeTheHistogram("etau","_SVMass","_8TeV",1,"_Cen")
    MakeTheHistogram("etau","_SVMass","_8TeV",1,"_End")
    GetFinalQCDShapeNorm()
    
            