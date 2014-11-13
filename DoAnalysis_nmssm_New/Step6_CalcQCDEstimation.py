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
InputFileLocation = '../FileROOT/nmssmROOTFiles/'
SubRootDir = 'OutFiles/'
verbosity_ = False
applyTauFR_Correction= True
applyOS_SS_Correction= True
apply106asScaleFactor = False
applyOSSSForQCDNorm= True

def MakeCanvas(name, title,  dX,  dY):

      canvas = TCanvas(name, title, 0, 0, dX, dY)
      canvas.SetFillColor      (0)
      canvas.SetBorderMode     (0)
      canvas.SetBorderSize     (10)
      canvas.SetLeftMargin     (0.18)
      canvas.SetRightMargin    (0.05)
      canvas.SetTopMargin      (0.08)
      canvas.SetBottomMargin   (0.15)
      canvas.SetFrameFillStyle (0)
      canvas.SetFrameLineStyle (0)
      canvas.SetFrameBorderMode(0)
      canvas.SetFrameBorderSize(10)
      canvas.SetFrameFillStyle (0)
      canvas.SetFrameLineStyle (0)
      canvas.SetFrameBorderMode(0)
      canvas.SetFrameBorderSize(10)
      canvas.SetLogy(0)

      return canvas


def luminosity(CoMEnergy):
    if CoMEnergy == '_8TeV': return  19712 #19242
    if CoMEnergy == '_7TeV': return  4982


#Bcategory = ["_inclusive","_btag"]
#Bcategory = ["_inclusive", "_nobtag", "_btag", "_btagLoose","_doublebtag"]
Bcategory = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#Bcategory = ["_inclusive"]
#Bcategory = [ "_btag"]
#channel = ["mutau"]
#channel = ["mutau"]
channel = ["mutau", "etau"]
channelDirectory = ["muTau", "eleTau"]
#POSTFIX=[""]
POSTFIX=["","Up","Down"]

MASS_BIN = 300
PT_BIN = 300
digit = 3
verbos_ = True
QCDScaleFactor = 1.06
#Binning_PT = array.array("d",[0,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300])
#Binning_PT = array.array("d",[0,20,25,30,35,40,45,50,60,70,80,90,100,120,140,160,180,200,250,300])
Binning_PT = array.array("d",[0,20,25,30,35,40,50,70,100,150,300])

def doRatio2D(num, denum, marColor):
    ratio = ROOT.TGraphAsymmErrors(num, denum, "")
#    ratio = num.Divide(denum)
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
#    NewFile=TFile("Extra/XXXout_"+Name +CoMEnergy+chan+Histogram+ cat+PostFix+".root","RECREATE")
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
    BG_EstimateNorm= getHistoNorm_BG(PostFix,CoMEnergy,Name, channelName,catName,Name_Histo)[0]
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
    if verbosity_:  print "###### QCD Estimate in", PostFix,CoMEnergy,channelName,catName,HistoName,etaRange, " ##### QCD_EstimateNorm= ", QCD_EstimateNorm,  "  = data - other BG", Data_ForqcdNorm ,"  W_ForqcdNorm=", W_ForqcdNorm,"  VV_ForqcdNorm=",VV_ForqcdNorm,"  TT_ForqcdNorm,",TT_ForqcdNorm,"  ZL_ForqcdNorm=",ZL_ForqcdNorm,"  ZJ_ForqcdNorm=",ZJ_ForqcdNorm,"  ZTT_ForqcdNorm=",ZTT_ForqcdNorm
    return QCD_EstimateNorm


def GetShape_QCD(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange):

    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
    catLooseName=catName
    if catName=="_btag": catLooseName="_btagLoose"
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
    if (ZL_ForqcdShapeHisto) :
        ZL_ForqcdShapeHisto.Scale(ZL_ForqcdNorm/ZL_ForqcdShapeHisto.Integral())
    ZJ_ForqcdShape=GetShape_BackGround("ZJ",PostFix,CoMEnergy,channelName,catLooseName,HistoName,etaRange)
    ZJ_ForqcdShapeHisto=ZJ_ForqcdShape.Get("XXX")
    if (ZJ_ForqcdShapeHisto) :
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
    QCDNorm= GetNorm_QCD(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange)
    Data_ForqcdShapeHisto.Scale(QCDNorm/Data_ForqcdShapeHisto.Integral())

    NewShapeForQCD=TFile("Extra/XXX.root","RECREATE")
    NewShapeForQCD.WriteObject(Data_ForqcdShapeHisto,"XXX")
    return NewShapeForQCD

#############################################################################################################
##   Calculating the Fake Rate and OS Over SS Fake Rate and applying Fake Rate
#############################################################################################################

def fitFunc_Exp3Par(x,par):
    return par[0] + (par[1] * x[0])
def Func_Exp3Par(x,par0,par1):
    return par0 +  (par1 * x)

# This fake rate is used for all Categories and channels but different for different eta range
def Make_Tau_FakeRate(PostFix,CoMEnergy,catName,channelName,etaRange):

    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
    catName= "_inclusive"
    channelName="mutau"
    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo

    ShapeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_TauPt_LepAntiIso_mTLess30_SS",etaRange)
    HistoNum=ShapeNum.Get("XXX")
    HistoNum= HistoNum.Rebin(len(Binning_PT)-1,"",Binning_PT)
    HistoNum.Scale(1000/HistoNum.Integral())

    ShapeDeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_TauPt_LepAntiIso_mTLess30_SS_RelaxIso",etaRange)
    HistoDeNum=ShapeDeNum.Get("XXX")
    HistoDeNum= HistoDeNum.Rebin(len(Binning_PT)-1,"",Binning_PT)
    HistoDeNum.Scale(1000/HistoDeNum.Integral())

    HistoNum.Divide(HistoDeNum)

    canv = MakeCanvas("canv", "histograms", 600, 600)
    HistoNum.SetMinimum(0.5)
    HistoNum.GetXaxis().SetRangeUser(0,150)
    HistoNum.GetYaxis().SetRangeUser(0,2)
    HistoNum.SetMarkerStyle(20)
    theFit=TF1("theFit", fitFunc_Exp3Par, 20, 150, 2)
    theFit.SetParameter(0, 0.6)
    theFit.SetParameter(1, 0.18)
    HistoNum.Fit(theFit, "R0","")
    HistoNum.Draw("E1")
    theFit.SetLineWidth(3)
    theFit.SetLineColor(2)
    FitParam=theFit.GetParameters()
    FitParamEre=theFit.GetParErrors()
    theFit.Draw("SAME")
    fitInfo  =TPaveText(.20,0.7,.60,0.9, "NDC");
    fitInfo.SetBorderSize(   0 );
    fitInfo.SetFillStyle(    0 );
    fitInfo.SetTextAlign(   12 );
    fitInfo.SetTextSize ( 0.03 );
    fitInfo.SetTextColor(    1 );
    fitInfo.SetTextFont (   62 );
    fitInfo.AddText("Linear Fit=  " + str(round(FitParam[0],3))+" + "+str(round(FitParam[1],9))+"x")
    fitInfo.AddText("Par0=" + str(round(FitParam[0],3)) + " #pm " + str(round(FitParamEre[0],3)) + " ,  Par1=" + str(round(FitParam[1],7)) + " #pm " + str(round(FitParamEre[1],7)))
    fitInfo.SetTextColor(    2 );
    fitInfo.AddText("Chis quare=  " + str(round(theFit.GetChisquare(),2)))
    fitInfo.Draw()
    canv.SaveAs("fitResults_Mass100_TauFR"+catName+channelName+etaRange+".pdf")

    return  FitParam[0],FitParam[1]


# This fake rate is used for all Categories and channels but different for different eta range
def Make_OS_over_SS_FakeRate(PostFix,CoMEnergy,catName,channelName,etaRange):

    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
    if catName=="_btag": catName="_btagLoose"
    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo

    ShapeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_TauPt_LepAntiIso_mTLess30_OS_RelaxIso","")
    HistoNum=ShapeNum.Get("XXX")
    HistoNum= HistoNum.Rebin(len(Binning_PT)-1,"",Binning_PT)

    ShapeDeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_TauPt_LepAntiIso_mTLess30_SS_RelaxIso","")
    HistoDeNum=ShapeDeNum.Get("XXX")
    HistoDeNum= HistoDeNum.Rebin(len(Binning_PT)-1,"",Binning_PT)

    HistoNum.Divide(HistoDeNum)

    canv = MakeCanvas("canv", "histograms", 600, 600)
    HistoNum.SetMinimum(0.5)
    HistoNum.GetXaxis().SetRangeUser(0,150)
    HistoNum.GetYaxis().SetRangeUser(0,2)
    HistoNum.SetMarkerStyle(20)
    theFit=TF1("theFit", fitFunc_Exp3Par, 20, 150, 2)
    theFit.SetParameter(0, 0.6)
    theFit.SetParameter(1, 0.18)
    HistoNum.Fit(theFit, "R0","")
    HistoNum.Draw("E1")
    theFit.SetLineWidth(3)
    theFit.SetLineColor(3)
    FitParam=theFit.GetParameters()
    FitParamEre=theFit.GetParErrors()
    theFit.Draw("SAME")
    fitInfo  =TPaveText(.20,0.7,.60,0.9, "NDC");
    fitInfo.SetBorderSize(   0 );
    fitInfo.SetFillStyle(    0 );
    fitInfo.SetTextAlign(   12 );
    fitInfo.SetTextSize ( 0.03 );
    fitInfo.SetTextColor(    1 );
    fitInfo.SetTextFont (   62 );
    fitInfo.AddText("Linear Fit=  " + str(round(FitParam[0],3))+" + "+str(round(FitParam[1],9))+"x")
    fitInfo.AddText("Par0=" + str(round(FitParam[0],3)) + " #pm " + str(round(FitParamEre[0],3)) + " ,  Par1=" + str(round(FitParam[1],7)) + " #pm " + str(round(FitParamEre[1],7)))
    fitInfo.SetTextColor(    2 );
    fitInfo.AddText("Chis quare=  " + str(round(theFit.GetChisquare(),2)))
    fitInfo.Draw()
    canv.SaveAs("fitResults_Mass100_OSSS"+catName+channelName+".pdf")

    return  FitParam[0],FitParam[1]


#############################################################################################################
##   Calculating the Fake Rate and applying Fake Rate  On Shape and Normalization
#############################################################################################################
def ApplyCorrectionOnQCDShape(Observable,CoMEnergy, etaRange, catName, channelName, PostFix):

    fitParametersOSSS = Make_OS_over_SS_FakeRate("",CoMEnergy,catName,channelName,etaRange)  # same for muTau and eTau
    fitparOSSS0 = fitParametersOSSS[0]
    fitparOSSS1 = fitParametersOSSS[1]

    fitParameterstauFR = Make_Tau_FakeRate("",CoMEnergy,catName,channelName,etaRange)  # same for muTau and eTau
    fitpartauFR0 = fitParameterstauFR[0]
    fitpartauFR1 = fitParameterstauFR[1]


    # FIXME replace the observable to SDOBservable
    nowObs= Observable.replace("_","_2D")
    QCDShape_File=GetShape_QCD("",CoMEnergy,channelName,catName,nowObs+"Pt_LepAntiIso_mTLess30_SS_RelaxIso", etaRange)
    QCDShape_Hist=QCDShape_File.Get("XXX")

    myOut = TFile("Extra/XXX.root","RECREATE")
    templateShape = TH1F("XXX", "", MASS_BIN, 0, MASS_BIN)

    for bb in range(MASS_BIN):
        NormInPtBin = 0
        for ss in range(PT_BIN):
            fakeCorrection= 1
            if applyTauFR_Correction: fakeCorrection= fakeCorrection * Func_Exp3Par(ss + 0.5, fitpartauFR0, fitpartauFR1)
            if applyOS_SS_Correction: fakeCorrection= fakeCorrection * Func_Exp3Par(ss + 0.5, fitparOSSS0, fitparOSSS1)
            if PostFix == "":   FakeRate = fakeCorrection
            if PostFix == "Down":   FakeRate = 1
            if PostFix == "Up":   FakeRate = pow(fakeCorrection, 2)
            NormInPtBin += FakeRate * QCDShape_Hist.GetBinContent(bb + 1, ss + 1)
            if NormInPtBin < 0 : NormInPtBin=0
        templateShape.SetBinContent(bb, NormInPtBin)


#    FinalQCDEstimate=GetNorm_QCD("",CoMEnergy,channelName,catName,"_SVMass_mTLess30_SS",etaRange)
#    templateShape.Scale(FinalQCDEstimate/templateShape.Integral())

    myOut.WriteObject(templateShape,"XXX")
    return myOut

def ApplyCorrectionOnQCDNormalization(Observable,CoMEnergy, etaRange, catName, channelName, PostFix):

    fitParametersOSSS = Make_OS_over_SS_FakeRate(PostFix,CoMEnergy,catName,channelName,etaRange)  # same for muTau and eTau
    fitparOSSS0 = fitParametersOSSS[0]
    fitparOSSS1 = fitParametersOSSS[1]


    # FIXME replace the observable to SDOBservable
    nowObs= Observable.replace("_","_2D")
    QCDNorm_File=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,nowObs+"Pt_mTLess30_SS", "")
    QCDNorm_Hist=QCDNorm_File.Get("XXX")


    NormInPtBin = 0
    for bb in range(MASS_BIN):
        for ss in range(PT_BIN):
            fakeCorrection=  Func_Exp3Par(ss + 0.5, fitparOSSS0, fitparOSSS1)
            if PostFix == "":   FakeRate = fakeCorrection
            if PostFix == "Down":   FakeRate = 1
            if PostFix == "Up":   FakeRate = pow(fakeCorrection, 2)
            NormInPtBin += FakeRate * QCDNorm_Hist.GetBinContent(bb + 1, ss + 1)

    return NormInPtBin


#############################################################################################################
##   Finalizaoing and Creating the ROOT File inclusing QCD shape and norm
#############################################################################################################

def GetFinalQCDShapeNorm(Observable,CoMEnergy):

    FinalFile = TFile("QCDFinalFile.root", "RECREATE")

    for catName in Bcategory:
        for channelName in channel:
            for PostFix in POSTFIX:

                getFileBar=ApplyCorrectionOnQCDShape(Observable,"_8TeV","_Bar",catName,channelName,PostFix)
                getFileCen=ApplyCorrectionOnQCDShape(Observable,"_8TeV","_Cen",catName,channelName,PostFix)
                getFileEnd=ApplyCorrectionOnQCDShape(Observable,"_8TeV","_End",catName,channelName,PostFix)

                HistoBar=getFileBar.Get("XXX")
                HistoCen=getFileCen.Get("XXX")
                HistoEnd=getFileEnd.Get("XXX")

                FinalFile.cd()
                QCDShapeTotal =TH1F(channelName+"_QCDShapeNormTotal"+catName+PostFix,"",MASS_BIN,0,MASS_BIN)
                for bb in range(MASS_BIN):
                    QCDShapeTotal.SetBinContent(bb, HistoBar.GetBinContent(bb)+HistoCen.GetBinContent(bb)+HistoEnd.GetBinContent(bb))



                if applyOSSSForQCDNorm: NewFinalQCDEstimate=ApplyCorrectionOnQCDNormalization(Observable,CoMEnergy, "", catName, channelName, "")
                if apply106asScaleFactor: NewFinalQCDEstimate=GetNorm_QCD("",CoMEnergy,channelName,catName,Observable+"_mTLess30_SS","")* QCDScaleFactor

                QCDShapeTotal.Scale(NewFinalQCDEstimate/QCDShapeTotal.Integral())

                FinalFile.Write()


#############################################################################################################
##   Run the Jobs
#############################################################################################################
if __name__ == "__main__":
    GetFinalQCDShapeNorm("_SVMass","_8TeV")



