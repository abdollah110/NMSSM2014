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
from ROOT import TPaveText
import array
gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)

SubRootDir = 'OutFiles/'
############################################################
#  Switches
############################################################
verbosity_ = False
applyTauFR_Correction= True
applyOS_SS_Correction= True
apply106asScaleFactor = False
applyOSSSForQCDNorm= True
GetThePreValues = True
############################################################
Bcategory = ["_inclusive",  "_btag", "_btagLoose"]
#Bcategory = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#Bcategory = ["_inclusive"]
#Bcategory = [ "_btag"]
#channel = ["mutau"]
channel = ["mutau", "etau"]
POSTFIX=[""]
#POSTFIX=["","Up","Down"]

MASS_BIN = 200
PT_BIN = 200
digit = 3
verbos_ = True
QCDScaleFactor = 1.06
#Binning_PT = array.array("d",[0,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300])
#Binning_PT = array.array("d",[0,20,25,30,35,40,45,50,60,70,80,90,100,120,140,160,180,200,250,300])
Binning_OSSS = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,160,180,200])
Binning_PT = array.array("d",[0,20,25,30,35,40,50,70,100,150,200])

def doRatio2D(num, denum, marColor):
    ratio = ROOT.TGraphAsymmErrors(num, denum, "")
#    ratio = num.Divide(denum)
    ratio.SetLineColor(marColor)
    ratio.SetLineWidth(2)
    return ratio


def getHistoShape_BG(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    NewFile=TFile("Extra/XXX.root","RECREATE")
    NewFile.WriteObject(HistoSub,"XXX")
    return NewFile

def getWExtraPol(PostFix,CoMEnergy,Name,chan,cat,HistogramNum,HistogramDenum ):
    myfileSub = TFile(SubRootDir + "out_"+Name+CoMEnergy+ '.root')
    HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
    HistoDenum = myfileSub.Get(chan+HistogramDenum+ "_inclusive"+PostFix )
    extrapolationRatio = 0
    if HistoNum and HistoDenum:    extrapolationRatio= HistoNum.Integral()/HistoDenum.Integral()
    return extrapolationRatio

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
def GetNorm_BackGround(BackGround,PostFix,CoMEnergy,channelName,catName,HistoName,etaRange):

    if BackGround=="VV" : DYIndex= "" ;  Name= "VVAll"
    if BackGround=="TT" : DYIndex= "" ;  Name= "TTAll"
    if BackGround=="ZL" : DYIndex= "_ZL" ;  Name= "DYJetsAll"
    if BackGround=="ZJ" : DYIndex= "_ZJ" ;  Name= "DYJetsAll"
    if BackGround=="ZTT" : DYIndex= "_ZTT" ;  Name= "DYJetsAll"

    Name_Histo=HistoName+etaRange+DYIndex
    BG_EstimateNorm= getHistoIntegral(PostFix,CoMEnergy,Name, channelName,catName,Name_Histo)[0]
    if verbosity_:  print '--------   Contamination of ',BackGround,' in ',Name_Histo,'  is = ', BG_EstimateNorm
    return BG_EstimateNorm

def GetShape_BackGround(BackGround,PostFix,CoMEnergy,channelName,catName,HistoName,etaRange):

    if BackGround=="VV" : DYIndex= "" ;  Name= "VVAll"
    if BackGround=="TT" : DYIndex= "" ;  Name= "TTAll"
    if BackGround=="ZL" : DYIndex= "_ZL" ;  Name= "DYJetsAll"
    if BackGround=="ZJ" : DYIndex= "_ZJ" ;  Name= "DYJetsAll"
    if BackGround=="ZTT" : DYIndex= "_ZTT" ;  Name= "DYJetsAll"

    Name_Histo=HistoName+etaRange+DYIndex
    BG_EstimateShape= getHistoShape_BG(PostFix,CoMEnergy,Name, channelName,catName,Name_Histo)
    if verbosity_  and BG_EstimateShape.Get("XXX"):  print 'Integral of the existing Histogram in the passing File= ', BG_EstimateShape.Get("XXX").Integral()

    return BG_EstimateShape

#############################################################################
#   Estimate Norm and Shape of  W Backgorunds
#############################################################################
def GetNorm_W(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange):

    #Get the extrapolation factor from MC
    HistoForNumerator=HistoName
    HistoForDeNumerator=HistoForNumerator.replace("mTLess30_", "mTHigher70_")
    if verbosity_:  print HistoForNumerator, HistoForDeNumerator
    ExtraPolFactor=getWExtraPol(PostFix,CoMEnergy, "WJetsAll",channelName,catName,HistoForNumerator+etaRange,HistoForDeNumerator+etaRange)

    VVinHighMT=GetNorm_BackGround("VV",PostFix,CoMEnergy,channelName,"_inclusive",HistoForDeNumerator,etaRange)
    TTinHighMT=GetNorm_BackGround("TT",PostFix,CoMEnergy,channelName,"_inclusive",HistoForDeNumerator,etaRange)
    ZLinHighMT=GetNorm_BackGround("ZL",PostFix,CoMEnergy,channelName,"_inclusive",HistoForDeNumerator,etaRange)
    ZJinHighMT=GetNorm_BackGround("ZJ",PostFix,CoMEnergy,channelName,"_inclusive",HistoForDeNumerator,etaRange)
    ZTTinHighMT=GetNorm_BackGround("ZTT",PostFix,CoMEnergy,channelName,"_inclusive",HistoForDeNumerator,etaRange)
    DatainHighMT=getHistoIntegral(PostFix,CoMEnergy, "Data",channelName,"_inclusive",HistoForDeNumerator+etaRange)[0]

    W_EstimateNorm=(DatainHighMT - (VVinHighMT+TTinHighMT+ZLinHighMT+ZJinHighMT+ZTTinHighMT))* ExtraPolFactor
    return W_EstimateNorm

def GetShape_W(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange):

    DYIndex= ""
    Name= "WJetsAll"
    Name_Histo=HistoName+etaRange+DYIndex
    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
#    if catName=="_btag": catName="_btagLoose"
    W_EstimateShape= getHistoShape_BG(PostFix,CoMEnergy,Name, channelName,catName,Name_Histo)
    return W_EstimateShape

#############################################################################
#   Estimate Norm and Shape of  QCD Backgorunds
#############################################################################
def GetNorm_QCD(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange):

    VV_ForqcdNorm=GetNorm_BackGround("VV",PostFix,CoMEnergy,channelName,catName,HistoName,etaRange)
    TT_ForqcdNorm=GetNorm_BackGround("TT",PostFix,CoMEnergy,channelName,catName,HistoName,etaRange)
    ZL_ForqcdNorm=GetNorm_BackGround("ZL",PostFix,CoMEnergy,channelName,catName,HistoName,etaRange)
    ZJ_ForqcdNorm=GetNorm_BackGround("ZJ",PostFix,CoMEnergy,channelName,catName,HistoName,etaRange)
    ZTT_ForqcdNorm=GetNorm_BackGround("ZTT",PostFix,CoMEnergy,channelName,catName,HistoName,etaRange)
    W_ForqcdNorm=GetNorm_W(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange)
    Data_ForqcdNorm=getHistoIntegral(PostFix,CoMEnergy, "Data",channelName,catName,HistoName+etaRange)[0]

    QCD_EstimateNorm=(Data_ForqcdNorm - (W_ForqcdNorm+VV_ForqcdNorm+TT_ForqcdNorm+ZL_ForqcdNorm+ZJ_ForqcdNorm+ZTT_ForqcdNorm))
#    if verbosity_:
    print "###### QCD Estimate in", PostFix,CoMEnergy,channelName,catName,HistoName,etaRange, " ##### QCD_EstimateNorm= ", QCD_EstimateNorm,  "  = data - other BG", Data_ForqcdNorm ,"  W_ForqcdNorm=", W_ForqcdNorm,"  VV_ForqcdNorm=",VV_ForqcdNorm,"  TT_ForqcdNorm,",TT_ForqcdNorm,"  ZL_ForqcdNorm=",ZL_ForqcdNorm,"  ZJ_ForqcdNorm=",ZJ_ForqcdNorm,"  ZTT_ForqcdNorm=",ZTT_ForqcdNorm
    return QCD_EstimateNorm


def GetShape_QCD(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange):

    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
    catLooseName=catName
#    if catName=="_btag": catLooseName="_btagLoose"
    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo

    #Normalization for different background
    VV_ForqcdNorm=GetNorm_BackGround("VV",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    TT_ForqcdNorm=GetNorm_BackGround("TT",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    ZL_ForqcdNorm=GetNorm_BackGround("ZL",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    ZJ_ForqcdNorm=GetNorm_BackGround("ZJ",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    ZTT_ForqcdNorm=GetNorm_BackGround("ZTT",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    W_ForqcdNorm=GetNorm_W(PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)


    #Shape for different background Normalizaed to their corresponding Normalization
    VV_ForqcdShape=GetShape_BackGround("VV",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    VV_ForqcdShapeHisto=VV_ForqcdShape.Get("XXX")
    if (VV_ForqcdShapeHisto) :
     VV_ForqcdShapeHisto.Scale(VV_ForqcdNorm/VV_ForqcdShapeHisto.Integral())
    TT_ForqcdShape=GetShape_BackGround("TT",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    TT_ForqcdShapeHisto=TT_ForqcdShape.Get("XXX")
    if (TT_ForqcdShapeHisto) :
        TT_ForqcdShapeHisto.Scale(TT_ForqcdNorm/TT_ForqcdShapeHisto.Integral())
    ZL_ForqcdShape=GetShape_BackGround("ZL",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    ZL_ForqcdShapeHisto=ZL_ForqcdShape.Get("XXX")
    #if (ZL_ForqcdShapeHisto) :  FIXME
    #    ZL_ForqcdShapeHisto.Scale(ZL_ForqcdNorm/ZL_ForqcdShapeHisto.Integral())
    ZJ_ForqcdShape=GetShape_BackGround("ZJ",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    ZJ_ForqcdShapeHisto=ZJ_ForqcdShape.Get("XXX")
    if (ZJ_ForqcdShapeHisto and ZJ_ForqcdShapeHisto.Integral()) :
        ZJ_ForqcdShapeHisto.Scale(ZJ_ForqcdNorm/ZJ_ForqcdShapeHisto.Integral())
    ZTT_ForqcdShape=GetShape_BackGround("ZTT",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    ZTT_ForqcdShapeHisto=ZTT_ForqcdShape.Get("XXX")
    if (ZTT_ForqcdShapeHisto) :
        ZTT_ForqcdShapeHisto.Scale(ZTT_ForqcdNorm/ZTT_ForqcdShapeHisto.Integral())
    W_ForqcdShape=GetShape_W(PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    W_ForqcdShapeHisto=W_ForqcdShape.Get("XXX")
    if (W_ForqcdShapeHisto) :
        W_ForqcdShapeHisto.Scale(W_ForqcdNorm/W_ForqcdShapeHisto.Integral())

    #################### Get QCD Shape from Data
    Data_ForqcdShape=getHistoShape_BG(PostFix,CoMEnergy, "Data",channelName,catLooseName,HistoName+etaRange)
    Data_ForqcdShapeHisto=Data_ForqcdShape.Get("XXX")

    ################### Subtract All BG from QCD
    if  VV_ForqcdShapeHisto:
        Data_ForqcdShapeHisto.Add(VV_ForqcdShapeHisto,-1)
    if  TT_ForqcdShapeHisto:
        Data_ForqcdShapeHisto.Add(TT_ForqcdShapeHisto,-1)
    if  ZL_ForqcdShapeHisto:
        Data_ForqcdShapeHisto.Add(ZL_ForqcdShapeHisto,-1)
    if  ZJ_ForqcdShapeHisto:
        Data_ForqcdShapeHisto.Add(ZJ_ForqcdShapeHisto,-1)
    if  ZTT_ForqcdShapeHisto:
        Data_ForqcdShapeHisto.Add(ZTT_ForqcdShapeHisto,-1)
    if  W_ForqcdShapeHisto:
        Data_ForqcdShapeHisto.Add(W_ForqcdShapeHisto,-1)

    #################### Return QCD Shape with Proper Normalization
    QCDNorm= GetNorm_QCD("",CoMEnergy,channelName,catName,HistoName,etaRange)
#    QCDNorm= GetNorm_QCD(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange) FIXME I just saif postFox == ""
    Data_ForqcdShapeHisto.Scale(QCDNorm/Data_ForqcdShapeHisto.Integral())

    NewShapeForQCD=TFile("Extra/XXX.root","RECREATE")
    NewShapeForQCD.WriteObject(Data_ForqcdShapeHisto,"XXX")
    return NewShapeForQCD


#############################################################################################################
##   Finalizaoing and Creating the ROOT File inclusing QCD shape and norm
#############################################################################################################
#CorrectionFR= ["","Down","Up"]
#CorrectionOSSS= ["","Down","Up"]
CorrectionFR= [""]
CorrectionOSSS= [""]
#POSTFIX=["","Up","Down"]
def GetFinalQCDShapeNorm(Observable,CoMEnergy):

    FinalFile = TFile("QCDFinalFile_ControlPlots.root", "RECREATE")

    for catName in Bcategory:
        for channelName in channel:
            for shiftFR in CorrectionFR:
                for shiftOSSS in CorrectionOSSS:
                    

                    HistoNameShape=Observable+"_LepAntiIso_mTLess30_OS_RelaxIso"
                    QCDShape_File=GetShape_QCD("",CoMEnergy,channelName,catName,HistoNameShape,"")
                    QCDShapeTotal=QCDShape_File.Get("XXX")
                    
                    HistoNameNorm=Observable+"_mTLess30_SS"
                    NewFinalQCDEstimate=GetNorm_QCD("",CoMEnergy,channelName,catName,HistoNameNorm,"") * 1.10


                    QCDShapeTotal.Scale(NewFinalQCDEstimate/QCDShapeTotal.Integral())
                    FinalFile.WriteObject(QCDShapeTotal,channelName+"_QCDShapeNormTotal"+shiftFR+"FR"+shiftOSSS+"OSSS"+catName)


#############################################################################################################
##   Run the Jobs
#############################################################################################################
if __name__ == "__main__":
#    GetFinalQCDShapeNorm("_SVMass","_8TeV")
#    GetFinalQCDShapeNorm("_l1Pt","_8TeV")
#    GetFinalQCDShapeNorm("_l2Pt","_8TeV")
#    GetFinalQCDShapeNorm("_l1Eta","_8TeV")
#    GetFinalQCDShapeNorm("_l2Eta","_8TeV")
    GetFinalQCDShapeNorm("_MET","_8TeV")



