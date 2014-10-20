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
from ROOT import TH1
from ROOT import TF1
from ROOT import TH2F
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gStyle
from ROOT import gSystem
import array

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
InputFileLocation = '../FileROOT/MSSMROOTFiles/'
SubRootDir = 'OutFiles/'
verbosity_ = True

def luminosity(CoMEnergy):
    if CoMEnergy == '_8TeV': return  19712 #19242
    if CoMEnergy == '_7TeV': return  4982


category_ = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#category = ["_inclusive"]
#category = ["_inclusive", "_nobtag", "_btag"]
category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
categoryM = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#channel = ["mutau"]
channel = ["mutau", "etau"]
channelDirectory = ["muTau", "eleTau"]

lenghtSig = 0
low_bin = 0
high_bin = 15000
digit = 1
verbos_ = True
QCDScaleFactor = 1.06
Binning_PT = array.array("d",[0,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300])

def doRatio2D(num, denum, marColor):
    ratio = ROOT.TGraphAsymmErrors(num, denum, "")
    ratio.SetLineColor(marColor)
    ratio.SetLineWidth(2)
    return ratio

def getHistoNorm_BG(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral() * luminosity(CoMEnergy)
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral()) * luminosity(CoMEnergy)
        valueEr = round(valueEr, digit)
    return value, valueEr

def getHistoShape_BG(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    NewFile=TFile("Extra/XXXout_"+Name +CoMEnergy+chan+Histogram+ cat+PostFix+".root","RECREATE")
    NewFile.WriteObject(HistoSub,"XXX")
    return NewFile

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
        value = HistoNum.Integral()/ HistoDenum.Integral()
    else:
        value =0
    return value

def getHistoIntegral(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral()
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral())
        valueEr = round(valueEr, digit)
    return value, valueEr



#############################################################################
#   Estimate Norm and Shape of Backgorunds
#############################################################################
def GetNorm_BackGround(BackGround,PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange):

    if BackGround=="VV" : DYIndex= "" ;  Name= "VVAll"
    if BackGround=="TT" : DYIndex= "" ;  Name= "TTAll"
    if BackGround=="ZL" : DYIndex= "_ZL" ;  Name= "DYJetsAll"
    if BackGround=="ZJ" : DYIndex= "_ZJ" ;  Name= "DYJetsAll"
    if BackGround=="ZTT" : DYIndex= "_ZTT" ;  Name= "DYJetsAll"

    Name_Histo=HistoName+etaRange+DYIndex
    BG_EstimateNorm= getHistoNorm_BG(PostFix,CoMEnergy,Name, channelName,categoryName,Name_Histo)[0]
    if verbosity_:  print '--------   Normalization for QCD is ---------= ', BG_EstimateNorm
    return BG_EstimateNorm

def GetShape_BackGround(BackGround,PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange):

    if BackGround=="VV" : DYIndex= "" ;  Name= "VVAll"
    if BackGround=="TT" : DYIndex= "" ;  Name= "TTAll"
    if BackGround=="ZL" : DYIndex= "_ZL" ;  Name= "DYJetsAll"
    if BackGround=="ZJ" : DYIndex= "_ZJ" ;  Name= "DYJetsAll"
    if BackGround=="ZTT" : DYIndex= "_ZTT" ;  Name= "DYJetsAll"

    Name_Histo=HistoName+etaRange+DYIndex
    BG_EstimateShape= getHistoShape_BG(PostFix,CoMEnergy,Name, channelName,categoryName,Name_Histo)
#    print "test for Step1 TT_ForqcdShape  estimate  _______", BG_EstimateShape.Integral()

    return BG_EstimateShape

#############################################################################
#   Estimate Norm and Shape of  W Backgorunds
#############################################################################
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

    W_EstimateNorm=(DatainHighMT - (VVinHighMT+TTinHighMT+ZLinHighMT+ZJinHighMT+ZTTinHighMT))* ExtraPolFactor
    return W_EstimateNorm

def GetShape_W(PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange):

    DYIndex= ""
    Name= "WJetsAll"
    Name_Histo=HistoName+etaRange+DYIndex
    W_EstimateShape= getHistoShape_BG(PostFix,CoMEnergy,Name, channelName,categoryName,Name_Histo)
    return W_EstimateShape

#############################################################################
#   Estimate Norm and Shape of  QCD Backgorunds
#############################################################################
def GetNorm_QCD(PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange):


    VV_ForqcdNorm=GetNorm_BackGround("VV",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    TT_ForqcdNorm=GetNorm_BackGround("TT",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZL_ForqcdNorm=GetNorm_BackGround("ZL",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZJ_ForqcdNorm=GetNorm_BackGround("ZJ",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZTT_ForqcdNorm=GetNorm_BackGround("ZTT",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    W_ForqcdNorm=GetNorm_W(PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    Data_ForqcdNorm=getHistoIntegral(PostFix,CoMEnergy, "Data",channelName,categoryName,HistoName+etaRange)[0]

    QCD_EstimateNorm=(Data_ForqcdNorm - (W_ForqcdNorm+VV_ForqcdNorm+TT_ForqcdNorm+ZL_ForqcdNorm+ZJ_ForqcdNorm+ZTT_ForqcdNorm))
    return QCD_EstimateNorm


def GetShape_QCD(PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange):
    
    #Normalization for different background
    VV_ForqcdNorm=GetNorm_BackGround("VV",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    TT_ForqcdNorm=GetNorm_BackGround("TT",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZL_ForqcdNorm=GetNorm_BackGround("ZL",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZJ_ForqcdNorm=GetNorm_BackGround("ZJ",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZTT_ForqcdNorm=GetNorm_BackGround("ZTT",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    W_ForqcdNorm=GetNorm_W(PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)

    
    #Shaoe for different background Normalizaed to their corresponding Normalization
    VV_ForqcdShape=GetShape_BackGround("VV",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    VV_ForqcdShapeHisto=VV_ForqcdShape.Get("XXX")
    if (VV_ForqcdShapeHisto) : print "  &&&&&&&&&&&&&&&&&&&&&&&&&   test for VV_ForqcdShape  estimate", VV_ForqcdShapeHisto.Integral() ; VV_ForqcdShapeHisto.Scale(VV_ForqcdNorm/VV_ForqcdShapeHisto.Integral())

    TT_ForqcdShape=GetShape_BackGround("TT",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    TT_ForqcdShapeHisto=TT_ForqcdShape.Get("XXX")
    if (TT_ForqcdShapeHisto) : print "  &&&&&&&&&&&&&&&&&&&&&&&&&   test for TT_ForqcdShape  estimate", TT_ForqcdShapeHisto.Integral(); TT_ForqcdShapeHisto.Scale(TT_ForqcdNorm/TT_ForqcdShapeHisto.Integral())

    ZL_ForqcdShape=GetShape_BackGround("ZL",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZL_ForqcdShapeHisto=ZL_ForqcdShape.Get("XXX")
    if (ZL_ForqcdShapeHisto) : print "  &&&&&&&&&&&&&&&&&&&&&&&&&   test for ZL_ForqcdShape  estimate", ZL_ForqcdShapeHisto.Integral(); ZL_ForqcdShapeHisto.Scale(ZL_ForqcdNorm/ZL_ForqcdShapeHisto.Integral())

    ZJ_ForqcdShape=GetShape_BackGround("ZJ",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZJ_ForqcdShapeHisto=ZJ_ForqcdShape.Get("XXX")
    if (ZJ_ForqcdShapeHisto) : print "  &&&&&&&&&&&&&&&&&&&&&&&&&   test for ZJ_ForqcdShape  estimate", ZJ_ForqcdShapeHisto.Integral(); ZJ_ForqcdShapeHisto.Scale(ZJ_ForqcdNorm/ZJ_ForqcdShapeHisto.Integral())

    ZTT_ForqcdShape=GetShape_BackGround("ZTT",PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    ZTT_ForqcdShapeHisto=ZTT_ForqcdShape.Get("XXX")
    if (ZTT_ForqcdShapeHisto) : print "  &&&&&&&&&&&&&&&&&&&&&&&&&   test for ZTT_ForqcdShape  estimate", ZTT_ForqcdShapeHisto.Integral(); ZTT_ForqcdShapeHisto.Scale(ZTT_ForqcdNorm/ZTT_ForqcdShapeHisto.Integral())

    W_ForqcdShape=GetShape_W(PostFix,CoMEnergy,channelName,categoryName,HistoName,etaRange)
    W_ForqcdShapeHisto=W_ForqcdShape.Get("XXX")
    if (W_ForqcdShapeHisto) : print "  &&&&&&&&&&&&&&&&&&&&&&&&&   test for W_ForqcdShape  estimate", W_ForqcdShapeHisto.Integral(); W_ForqcdShapeHisto.Scale(W_ForqcdNorm/W_ForqcdShapeHisto.Integral())

    Data_ForqcdShape=getHistoShape_BG(PostFix,CoMEnergy, "Data",channelName,categoryName,HistoName+etaRange)
    Data_ForqcdShapeHisto=Data_ForqcdShape.Get("XXX")

    
#    if VV_ForqcdNorm and VV_ForqcdShape: VV_ForqcdShape.Scale(VV_ForqcdNorm/VV_ForqcdShape.Integral())
#    if TT_ForqcdNorm and TT_ForqcdShape: TT_ForqcdShape.Scale(TT_ForqcdNorm/TT_ForqcdShape.Integral())
#    if ZL_ForqcdNorm and ZL_ForqcdShape: ZL_ForqcdShape.Scale(ZL_ForqcdNorm/ZL_ForqcdShape.Integral())
#    if ZJ_ForqcdNorm and ZJ_ForqcdShape: ZJ_ForqcdShape.Scale(ZJ_ForqcdNorm/ZJ_ForqcdShape.Integral())
#    if ZTT_ForqcdNorm and ZTT_ForqcdShape: ZTT_ForqcdShape.Scale(ZTT_ForqcdNorm/ZTT_ForqcdShape.Integral())
#    if W_ForqcdNorm and W_ForqcdShape: W_ForqcdShape.Scale(W_ForqcdNorm/W_ForqcdShape.Integral())

    if verbosity_ and Data_ForqcdShapeHisto: print "Data_ForqcdShape Integral is= " , Data_ForqcdShapeHisto.Integral()
    if verbosity_ and TT_ForqcdShapeHisto: print  "TT_ForqcdShape Integral is= ",TT_ForqcdShapeHisto.Integral(); Data_ForqcdShapeHisto.Add(TT_ForqcdShapeHisto,-1)
    if verbosity_ and VV_ForqcdShapeHisto: print "VV_ForqcdShape Integral is= ", VV_ForqcdShapeHisto.Integral(); Data_ForqcdShapeHisto.Add(VV_ForqcdShapeHisto,-1)
    if verbosity_ and W_ForqcdShapeHisto: print "W_ForqcdShape Integral is= ", W_ForqcdShapeHisto.Integral(); Data_ForqcdShapeHisto.Add(W_ForqcdShapeHisto,-1)
    if verbosity_ and ZL_ForqcdShapeHisto: print  "ZL_ForqcdShape Integral is= ",ZL_ForqcdShapeHisto.Integral(); Data_ForqcdShapeHisto.Add(ZL_ForqcdShapeHisto,-1)
    if verbosity_ and ZJ_ForqcdShapeHisto: print   "ZJ_ForqcdShape Integral is= ",ZJ_ForqcdShapeHisto.Integral(); Data_ForqcdShapeHisto.Add(ZJ_ForqcdShapeHisto,-1)
    if verbosity_ and ZTT_ForqcdShapeHisto: print "ZTT_ForqcdShape Integral is= ",ZTT_ForqcdShapeHisto.Integral(); Data_ForqcdShapeHisto.Add(ZTT_ForqcdShapeHisto,-1)
    
    NewShapeForQCD=TFile("Extra/XXX"+PostFix+CoMEnergy+channelName+categoryName+HistoName+etaRange+".root","RECREATE")
    NewShapeForQCD.WriteObject(Data_ForqcdShapeHisto,"XXX")
    return NewShapeForQCD
    




def MakeTheHistogram(PostFix,channel,Observable,CoMEnergy,chl,etaRange):
    #################### Name Of the output file
    myOut = TFile("QCDTotalRootForLimit_"+channel + etaRange+CoMEnergy+".root" , 'RECREATE')

    categ=-1
    for categoryName in category:
        categ =categ +1
        print "starting category and channel", categoryName, channel
#        if category=="_nobtag" or category=="_inclusive"  : BinCateg = Binning_NoBTag
#        if category=="_btag" or   category == "_btagLoose": BinCateg = Binning_BTag
        tDirectory= myOut.mkdir(channelDirectory[chl] + str(categoryName))
        tDirectory.cd()
        print "No Error"
#############################################################################################################
#        #######################################  Filling Reducible BG QCD ##########
        print "Doing QCD, BG estimation"

#        HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+etaRange
#        HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+etaRange
#        HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+etaRange
#        HistoQCDShapeLowMTSSRelax = "_QCDshape2D_mTLess30_SS_RelaxIso"+ etaRange
#        HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange
#
#        Name='Data'
#        if category=="_btag"  : category = "_btagLoose"   #FIXME
#        if category=="_btag"  : category = "_inclusive"
#        file_QCD = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')

        Shape_QCDSSIso=GetShape_QCD(PostFix,CoMEnergy,channel,categoryName,"_TauPt_mTLess30_SS",etaRange)
        Histo_QCDSSIso=Shape_QCDSSIso.Get("XXX")
        Histo_QCDNumerator= Histo_QCDSSIso.Rebin(len(Binning_PT)-1,"",Binning_PT)


        Shape_QCDSSRelax=GetShape_QCD(PostFix,CoMEnergy,channel,categoryName,"_TauPt_mTLess30_SS_RelaxIso",etaRange)
        Histo_QCDSSRelax=Shape_QCDSSRelax.Get("XXX")
        Histo_QCDDeNumerator= Histo_QCDSSRelax.Rebin(len(Binning_PT)-1,"",Binning_PT)


        Shape_QCDOSRelax=GetShape_QCD(PostFix,CoMEnergy,channel,categoryName,"_TauPt_mTLess30_OS_RelaxIso",etaRange)
        Histo_QCDOSRelax=Shape_QCDOSRelax.Get("XXX")
        Histo_QCDControlRegion= Histo_QCDOSRelax.Rebin(len(Binning_PT)-1,"",Binning_PT)

        print "@@@@@@@@@@@@@@@@@@@@@@@ check if the problem is from here"
        Shape_QCDQCDShape2DSSRelax=GetShape_QCD(PostFix,CoMEnergy,channel,categoryName,"_QCDshape2D_mTLess30_SS_RelaxIso", etaRange)
        Histo_QCDQCDShape2DSSRelax=Shape_QCDQCDShape2DSSRelax.Get("XXX")


#        tDirectory.cd()
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

        tDirectory.WriteObject(HistoNum,"QCDNumerator")
        tDirectory.WriteObject(HistoDeNum,"QCDDenumerator")
        tDirectory.WriteObject(NewHIST_ControlRegion,"QCDControlRegion")
        
        for aa in range(1,Histo_QCDOSRelax.GetNbinsX()):
            binValue = Histo_QCDOSRelax.GetBinContent(aa)
            if binValue < 0: binValue=0
            NewHIST_ControlRegionFinBin.SetBinContent(aa,binValue)

        tDirectory.WriteObject(NewHIST_ControlRegionFinBin,"NewHIST_ControlRegionFinBin")
        
        for aa in range(1,Histo_QCDQCDShape2DSSRelax.GetNbinsX()):
            for bb in range(1,Histo_QCDQCDShape2DSSRelax.GetNbinsY()):
                binValue = Histo_QCDQCDShape2DSSRelax.GetBinContent(aa,bb)
                if binValue < 0: binValue=0
                NewHIST_ControlRegionQCDShape2D.SetBinContent(aa,bb,binValue)

        tDirectory.WriteObject(NewHIST_ControlRegionQCDShape2D,"NewHIST_ControlRegionQCDShape2D")


    myOut.Close()

       


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
            
            
            
#
##############################################################################################################
###   Calculating the Fake Rate and applying Fake Rate
##############################################################################################################
#
#def fitFunc_Exp3Par(x,par):
#    return par[0] + par[1] + ((par[2] * x[0]))
#def Func_Exp3Par(x,par0,par1,par2):
#    return par0 + par1 + ((par2 * x))
#
## This fake rate is used for all Categories and channels but different for different eta range
#def MakeFakeRateHisto(CoMEnergy,etaRange):
#    NewFile = TFile("QCDTotalRootForLimit_"+ "mutau"+ etaRange+CoMEnergy+".root")
#    HistoNum = NewFile.Get("muTau_inclusive/QCDNumerator")
#    HistoDeNum = NewFile.Get("muTau_inclusive/QCDDenumerator")
#    print "0000000 ", NewFile.GetName(),  HistoDeNum.Integral()
#    for i in range(HistoDeNum.GetNbinsX()):
#            print HistoDeNum.GetBinContent(i+1) , HistoNum.GetBinContent(i+1)
#            ## CAVEAT   Here I have set the denum Value equal to Num Value in case it is smaller
#            if HistoDeNum.GetBinContent(i+1) < HistoNum.GetBinContent(i+1):
#                HistoDeNum.SetBinContent(i+1, 0)
#                HistoNum.SetBinContent(i+1, 0)
##                HistoDeNum.SetBinContent(i+1, HistoNum.GetBinContent(i+1))
#    FRHisto=doRatio2D(HistoNum, HistoDeNum, 3)
#    canvas =TCanvas("canvas", "", 700, 500)
#    theFit=TF1("theFit", fitFunc_Exp3Par, 20, 200, 3)
#    theFit.SetParameter(0, 0.6)
#    theFit.SetParameter(1, 0.18)
#    theFit.SetParameter(2, -0.2)
#    FRHisto.Fit(theFit, "R0","")
#    FRHisto.Draw("PAE")
#    FitParam=theFit.GetParameters()
#    theFit.Draw("SAME")
#    canvas.SaveAs("fitResults_"+"mutau"+CoMEnergy+etaRange+".pdf")
#    return  FitParam[0],FitParam[1],FitParam[2]
#
#def ReturnScaledReadyHisto(CoMEnergy,etaRange,categ,chl):
#    myOut = TFile("YieldShapeQCD"+CoMEnergy+".root", 'RECREATE')
#
#    fitParameters=MakeFakeRateHisto(CoMEnergy,etaRange)  # same for muTau and eTau
#    fitpar0= fitParameters[0]
#    fitpar1= fitParameters[1]
#    fitpar2= fitParameters[2]
#
#    MainRootFile = TFile("QCDTotalRootForLimit_"+channel[chl] + etaRange + CoMEnergy+".root")
#    HistoCR = MainRootFile.Get(channel[chl]+category[categ]+"/NewHIST_ControlRegionQCDShape2D")
#
#    myOut.cd()
#    templateShape =TH1F("QCDShapeNorm","",1500,0,1500)
#
#    for bb in range(1500):
#
#        NormInPtBin=0
#        for ss in range(300):
#            NormInPtBin += Func_Exp3Par(ss+0.5,fitpar0,fitpar1,fitpar2)*HistoCR.GetBinContent(bb+1,ss+1)
#        templateShape.SetBinContent(bb,NormInPtBin)
#
#
#    XLoc= categ + len(category)*chl + 1
#
#    FileNorm = TFile("Yield"+etaRange+CoMEnergy+".root")
#    normHistio=FileNorm.Get("FullResultsSSIsoQCDNorm")
#    NormQCDMC=0
#    for i in range(6):
#        print "Backgrounds to be subtracted from data", i, normHistio.GetBinContent(XLoc,i+1)
#        NormQCDMC +=normHistio.GetBinContent(XLoc,i+1)
#    FinalQCDEstimate=(normHistio.GetBinContent(XLoc,7)-NormQCDMC) * QCDScaleFactor
#    templateShape.Scale(FinalQCDEstimate/templateShape.Integral())
#    myOut.Write()
#    return myOut
#
#def GetFinalQCDShapeNorm():
#    FinalFile = TFile("QCDFinalFile.root", "RECREATE")
#
#    for categ in range(len(category)):
##        for chl in range(1):
#        for chl in range(len(channel)):
#            getFileBar=ReturnScaledReadyHisto("_8TeV","_Bar",categ,chl)
#            getFileCen=ReturnScaledReadyHisto("_8TeV","_Cen",categ,chl)
#            getFileEnd=ReturnScaledReadyHisto("_8TeV","_End",categ,chl)
#
#            HistoBar=getFileBar.Get("QCDShapeNorm")
#            HistoCen=getFileCen.Get("QCDShapeNorm")
#            HistoEnd=getFileEnd.Get("QCDShapeNorm")
#
#            FinalFile.cd()
#            QCDShapeTotal =TH1F(channel[chl]+"_QCDShapeNormTotal"+category[categ],"",1500,0,1500)
#            for bb in range(1500):
#                QCDShapeTotal.SetBinContent(bb, HistoBar.GetBinContent(bb)+HistoCen.GetBinContent(bb)+HistoEnd.GetBinContent(bb))
#
#            FinalFile.Write()
#
#
#





if __name__ == "__main__":
#    GetShape_QCD()
    MakeTheHistogram("","mutau","_SVMass","_8TeV",0,"_Bar")
    MakeTheHistogram("","mutau","_SVMass","_8TeV",0,"_Cen")
    MakeTheHistogram("","mutau","_SVMass","_8TeV",0,"_End")
    MakeTheHistogram("","etau","_SVMass","_8TeV",1,"_Bar")
    MakeTheHistogram("","etau","_SVMass","_8TeV",1,"_Cen")
    MakeTheHistogram("","etau","_SVMass","_8TeV",1,"_End")
    GetFinalQCDShapeNorm()


    

#if __name__ == "__main__":
#     print "Doing QCD, BG estimation"
#
#    HistoTauPtLowMTSSIso = "_TauPt_mTLess30_SS"+etaRange
#    HistoTauPtLowMTSSRelax = "_TauPt_mTLess30_SS_RelaxIso"+etaRange
#    HistoTauPtLowMTOSRelax = "_TauPt_mTLess30_OS_RelaxIso"+etaRange
#    HistoQCDShapeLowMTSSRelax = "_QCDshape2D_mTLess30_SS_RelaxIso"+ etaRange
#    HistoQCDNormLowMTSSIso = "_QCDNorm_mTLess30_SS"+ etaRange
#
#
#
#
#    make2DTable("_SVMass","", "_8TeV", "_Bar")
#    make2DTable("_SVMass","", "_8TeV", "_Cen")
#    make2DTable("_SVMass","", "_8TeV", "_End")

