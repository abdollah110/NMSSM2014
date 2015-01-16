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
from ROOT import TLine
import array

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
#InputFileLocation = '../FileROOT/nmssmROOTFiles/'
SubRootDir = 'OutFiles/'
verbosity_ = False
applyTauFR_Correction= True
applyOS_SS_Correction= True
apply106asScaleFactor = False
applyOSSSForQCDNorm= True
GetThePreValues = True

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


#def luminosity(CoMEnergy):
#    if CoMEnergy == '_8TeV': return  19712 #19242
#    if CoMEnergy == '_7TeV': return  4982


Bcategory = ["_inclusive"]
#Bcategory = ["_inclusive",  "_btag", "_btagLoose"]
#Bcategory = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#Bcategory = ["_inclusive"]
#Bcategory = [ "_btag"]
#channel = ["mutau"]
channel = ["mutau"]
#channel = ["mutau", "etau"]
channelDirectory = ["muTau"]
#channelDirectory = ["muTau", "eleTau"]
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

#def getHistoNorm_BG(PostFix,CoMEnergy,Name,chan,cat,Histogram):
#    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
#    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
#    value = 10E-7
#    valueEr = 10E-7
#    if (HistoSub):
#        value = HistoSub.Integral() * luminosity(CoMEnergy)
#        value = round(value, digit)
#        valueEr = math.sqrt(HistoSub.Integral()) * luminosity(CoMEnergy)
#        valueEr = round(valueEr, digit)
#    return value, valueEr

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

    QCD_EstimateNorm=(Data_ForqcdNorm - (VV_ForqcdNorm+TT_ForqcdNorm+ZL_ForqcdNorm+ZJ_ForqcdNorm+ZTT_ForqcdNorm))
#    QCD_EstimateNorm=(Data_ForqcdNorm - (W_ForqcdNorm+VV_ForqcdNorm+TT_ForqcdNorm+ZL_ForqcdNorm+ZJ_ForqcdNorm+ZTT_ForqcdNorm))
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
#    if  W_ForqcdShapeHisto:
#        Data_ForqcdShapeHisto.Add(W_ForqcdShapeHisto,-1)

    #################### Return QCD Shape with Proper Normalization
    QCDNorm= GetNorm_QCD("",CoMEnergy,channelName,catName,HistoName,etaRange)
#    QCDNorm= GetNorm_QCD(PostFix,CoMEnergy,channelName,catName,HistoName,etaRange) FIXME I just saif postFox == ""
    Data_ForqcdShapeHisto.Scale(QCDNorm/Data_ForqcdShapeHisto.Integral())

    NewShapeForQCD=TFile("Extra/XXX.root","RECREATE")
    NewShapeForQCD.WriteObject(Data_ForqcdShapeHisto,"XXX")
    return NewShapeForQCD

#############################################################################################################
##   Calculating the Fake Rate and OS Over SS Fake Rate and applying Fake Rate
#############################################################################################################

#def fitFunc_Linear2Par(x,par):
#    return par[0] + (par[1] * x[0])
#def Func_Linear2Par(x,par0,par1):
#    return par0 +  (par1 * x)
#
#def fitFunc_Exp3Par( x,  par0,  par1,  par2) :
#    return par0 + par1 * math.exp(par2 * x)
#def Func_Exp3Par( x,  par) :
#    return par[0] + par[1]*math.exp(par[2] * x[0])

def Func_Ploynomial(X,p):
    xVar=X[0]-p[4]
    return p[0]+p[1]*xVar+p[2]*xVar*xVar+p[3]*xVar*xVar*xVar


#p0 =  0.787452;
#    p1 = -0.146412;
#        p2 = -0.0276803;
#            p3 = -0.0824184;
#                X = (pt-149.832)/100;  #//(x-meanPt)/100
#                    correction = p0+p1*X+p2*X*X+p3*X*X*X;
#
#




# This fake rate is used for all Categories and channels but different for different eta range
def Make_Tau_FakeRate(PostFix,CoMEnergy,catName,channelName,etaRange):

    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
    catName= "_inclusive"
    channelName="mutau"
    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo

    ShapeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_TauPt_mTHigher70_OS","")
    HistoNum=ShapeNum.Get("XXX")
    HistoNum= HistoNum.Rebin(10)
#    HistoNum.Scale(1000/HistoNum.Integral())

    ShapeDeNum=GetShape_W(PostFix,CoMEnergy,channelName,catName,"_TauPt_mTHigher70_OS","")
    HistoDeNum=ShapeDeNum.Get("XXX")
    HistoDeNum= HistoDeNum.Rebin(10)
#    HistoDeNum.Scale(1000/HistoDeNum.Integral())

    HistoNum.Divide(HistoDeNum)
    
    #######   Make Historam
    canv = MakeCanvas("canv", "histograms", 600, 600)

    HistoNum.SetTitle("")
    HistoNum.SetStats(0)
    HistoNum.SetMinimum(0.5)
    HistoNum.GetXaxis().SetRangeUser(0,200)
    HistoNum.GetYaxis().SetRangeUser(0,2)
    HistoNum.SetMarkerStyle(20)
    HistoNum.GetXaxis().SetTitle("P_{T}(#tau_{had}) [GeV]")
    HistoNum.GetYaxis().SetTitle("Data / WJet MC")
    HistoNum.GetYaxis().SetLabelSize(.025)
    HistoNum.GetXaxis().SetLabelSize(.025)
    HistoNum.Draw("E1")
    
    #######   Fit Polynomial
    theFit=TF1("theFit", Func_Ploynomial, 20, 150, 5)
    theFit.SetParameter(0, 0.787452)
    theFit.SetParameter(1, -0.146412)
    theFit.SetParameter(2, -0.0276803)
    theFit.SetParameter(3, -0.0824184)
    theFit.SetParameter(4, 149.832)
    HistoNum.Fit(theFit, "R0","")
    theFit.SetLineWidth(3)
    theFit.SetLineColor(2)
    FitParam=theFit.GetParameters()
    FitParamEre=theFit.GetParErrors()
    theFit.Draw("SAME")

    #######   Flat Line Till 200 GeV
    line2 = TLine(149,0.31,200,0.31)
    line2.SetLineWidth(3)
    line2.SetLineColor(2)
    line2.SetLineStyle(2)
    line2.Draw("SAME")
    
    #######   Flat Line at 1
    line = TLine(0,1,200,1)
    line.SetLineWidth(2)
    line.SetLineColor(30)
    line.Draw("SAME")
    

    
    
    fitInfo  =TPaveText(.20,0.7,.60,0.9, "NDC");
    fitInfo.SetBorderSize(   0 );
    fitInfo.SetFillStyle(    0 );
    fitInfo.SetTextAlign(   12 );
    fitInfo.SetTextSize ( 0.03 );
    fitInfo.SetTextColor(    1 );
    fitInfo.SetTextFont (   62 );
    fitInfo.AddText("m_{T} > 50 GeV")
    fitInfo.AddText("p_{T} < 150: Polynomial Fit (a+bx+cx^{2}+dx^{3})")
    fitInfo.AddText("p_{T} > 150: Flat Line")
#    fitInfo.SetTextColor(    2 );
#    fitInfo.AddText("Chis quare=  " + str(round(theFit.GetChisquare(),2)))
    fitInfo.Draw()
    canv.SaveAs("dataOverMC_WJet_tauPt_FR.pdf")

    return  FitParam[0],FitParam[1]


## This fake rate is used for all Categories and channels but different for different eta range
#def Make_OS_over_SS_FakeRate(PostFix,CoMEnergy,catName,channelName,etaRange):
#
#    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
#    if catName=="_btag": catName="_btagLoose"
#    ##  ooooooooooooooooooooooooooo   Bcategory change name  ooooooooooooooooooooooooooo
#
##    ShapeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_TauPt_LepAntiIso_mTLess30_OS_RelaxIso","")
#    ShapeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_SVMass_LepAntiIso_mTLess30_OS_RelaxIso","")
#    HistoNum=ShapeNum.Get("XXX")
#    HistoNum= HistoNum.Rebin(len(Binning_OSSS)-1,"",Binning_OSSS)
#
##    ShapeDeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_TauPt_LepAntiIso_mTLess30_SS_RelaxIso","")
#    ShapeDeNum=GetShape_QCD(PostFix,CoMEnergy,channelName,catName,"_SVMass_LepAntiIso_mTLess30_SS_RelaxIso","")
#    HistoDeNum=ShapeDeNum.Get("XXX")
#    HistoDeNum= HistoDeNum.Rebin(len(Binning_OSSS)-1,"",Binning_OSSS)
#
#    HistoNum.Divide(HistoDeNum)
#
#    canv = MakeCanvas("canv", "histograms", 600, 600)
#    HistoNum.SetMinimum(0.5)
#    HistoNum.SetTitle("")
#    HistoNum.GetXaxis().SetRangeUser(0,150)
#    HistoNum.GetYaxis().SetRangeUser(0.5,2)
#    HistoNum.GetYaxis().SetTitle("OS/SS ratio")
#    HistoNum.GetXaxis().SetTitle("M_{#tau#tau} [GeV]")
#    HistoNum.SetMarkerStyle(20)
##    theFit=TF1("theFit", fitFunc_Linear2Par, 0, 60, 2)
#    nPar = 3 # number of parameters in the fit
#    theFit=TF1("theFit", Func_Exp3Par, 10, 150,nPar)
#    theFit.SetParameter(0, .2)
#    theFit.SetParameter(1, 50)
#    theFit.SetParLimits(1, 1, 50)
#    theFit.SetParameter(2, -2)
#    HistoNum.Fit("theFit", "R0")
##    HistoNum.Fit(theFit, "R0","")
#    HistoNum.Draw("E1")
#    theFit.SetLineWidth(3)
#    theFit.SetLineColor(3)
#    FitParam=theFit.GetParameters()
#    FitParamEre=theFit.GetParErrors()
#    theFit.Draw("SAME")
#    fitInfo  =TPaveText(.20,0.7,.60,0.9, "NDC");
#    fitInfo.SetBorderSize(   0 );
#    fitInfo.SetFillStyle(    0 );
#    fitInfo.SetTextAlign(   12 );
#    fitInfo.SetTextSize ( 0.03 );
#    fitInfo.SetTextColor(    1 );
#    fitInfo.SetTextFont (   62 );
#    fitInfo.AddText("" + str(round(FitParam[0],2))+" + "+str(round(FitParam[1],3))+"*Exp(" +str(round(FitParam[2],3))+"* X)")
##    fitInfo.AddText("Par0=" + str(round(FitParam[0],3)) + " #pm " + str(round(FitParamEre[0],3)) + " ,  Par1=" + str(round(FitParam[1],7)) + " #pm " + str(round(FitParamEre[1],7)))
##    fitInfo.SetTextColor(    2 );
##    fitInfo.AddText("Chis quare=  " + str(round(theFit.GetChisquare(),2)))
#    fitInfo.Draw()
#    canv.SaveAs("fitResults_Mass100_OSSS"+catName+channelName+".pdf")
#
#    return  FitParam[0],FitParam[1], FitParam[2] 
#
#
##############################################################################################################
###   Calculating the Fake Rate and applying Fake Rate  On Shape and Normalization
##############################################################################################################
#def ApplyCorrectionOnQCDShape(Observable,CoMEnergy, etaRange, catName, channelName, shift_TauFR, shift_OSSS):
#
#    
#    if applyOS_SS_Correction:
#        if GetThePreValues:   
#            if catName== "_inclusive":
#                fitparOSSS0 = 1.13
#                fitparOSSS1 = 1.00
#                fitparOSSS2 = -0.087
#            elif  catName== "_btag" or catName== "_btagLoose":
#                fitparOSSS0 = 1.10
#                fitparOSSS1 = 1.418
#                fitparOSSS2 = -0.11
#            else:
#                print "No Category to be used"
#        else:
#            fitParametersOSSS = Make_OS_over_SS_FakeRate("",CoMEnergy,catName,channelName,etaRange)  # same for muTau and eTau
#            fitparOSSS0 = fitParametersOSSS[0]
#            fitparOSSS1 = fitParametersOSSS[1]
#            fitparOSSS2 = fitParametersOSSS[2]
#    
#    if applyTauFR_Correction:
#        fitParameterstauFR = Make_Tau_FakeRate("",CoMEnergy,catName,channelName,etaRange)  # same for muTau and eTau
#        fitpartauFR0 = fitParameterstauFR[0]
#        fitpartauFR1 = fitParameterstauFR[1]
#
#
#    # FIXME replace the observable to SDOBservable
#    nowObs= Observable.replace("_","_2D")
#    QCDShape_File=GetShape_QCD("",CoMEnergy,channelName,catName,nowObs+"Pt_LepAntiIso_mTLess30_SS_RelaxIso", etaRange)
#    QCDShape_Hist=QCDShape_File.Get("XXX")
#
#    myOut = TFile("Extra/XXX.root","RECREATE")
#    templateShape = TH1F("XXX", "", MASS_BIN, 0, MASS_BIN)
#
#    for bb in range(MASS_BIN):
#        NormInPtBin = 0
#        for ss in range(PT_BIN):
#            
#            fakeCorrection_TauFR = Func_Linear2Par(ss + 0.5, fitpartauFR0, fitpartauFR1)
#            eachBinValue = QCDShape_Hist.GetBinContent(bb + 1, ss + 1)
#            
#            if shift_TauFR == "":     NormInPtBin +=  eachBinValue * fakeCorrection_TauFR
#            if shift_TauFR == "Down": NormInPtBin +=  eachBinValue * 1
#            if shift_TauFR == "Up":   NormInPtBin +=  eachBinValue * pow(fakeCorrection_TauFR, 2)
#            if NormInPtBin < 0 : NormInPtBin=0
#            
#        
#        fakeCorrection_OSSS= fitFunc_Exp3Par(bb + 0.5, fitparOSSS0, fitparOSSS1,fitparOSSS2);
#        
#        if shift_OSSS == "":     finalNorm = NormInPtBin * fakeCorrection_OSSS
#        if shift_OSSS == "Down": finalNorm = NormInPtBin * 1
#        if shift_OSSS == "Up":   finalNorm = NormInPtBin * pow(fakeCorrection_OSSS, 2)
#        if finalNorm < 0  : finalNorm =0
#        
#        templateShape.SetBinContent(bb, finalNorm)
#
#    myOut.WriteObject(templateShape,"XXX")
#    return myOut
#
#def ApplyCorrectionOnQCDNormalization(Observable,CoMEnergy, etaRange, catName, channelName, PostFix):
#
#    if applyOS_SS_Correction:
#        if GetThePreValues:   
#            if catName== "_inclusive":
#                fitparOSSS0 = 1.13
#                fitparOSSS1 = 1.00
#                fitparOSSS2 = -0.087
#            elif  catName== "_btag" or catName== "_btagLoose":
#                fitparOSSS0 = 1.10
#                fitparOSSS1 = 1.418
#                fitparOSSS2 = -0.11
#            else:
#                print "No Category to be used"
#        else:
#            fitParametersOSSS = Make_OS_over_SS_FakeRate("",CoMEnergy,catName,channelName,etaRange)  # same for muTau and eTau
#            fitparOSSS0 = fitParametersOSSS[0]
#            fitparOSSS1 = fitParametersOSSS[1]
#            fitparOSSS2 = fitParametersOSSS[2]
#
#
#    # FIXME replace the observable to SDOBservable
##    nowObs= Observable.replace("_","_2D")
##    QCDNorm_File=GetShape_QCD("",CoMEnergy,channelName,catName,nowObs+"Pt_mTLess30_SS", "")
#    QCDNorm_File=GetShape_QCD("",CoMEnergy,channelName,catName,Observable+"_mTLess30_SS", "")
#    
#    QCDNorm_Hist=QCDNorm_File.Get("XXX")
#
#
#    NormInPtBin = 0
#    for bb in range(MASS_BIN):
#        fakeCorrection=  fitFunc_Exp3Par(bb + 0.5, fitparOSSS0, fitparOSSS1,fitparOSSS2)
#        if PostFix == "":   FakeRate = fakeCorrection
#        if PostFix == "Down":   FakeRate = 1
#        if PostFix == "Up":   FakeRate = pow(fakeCorrection, 2)
#        NormInPtBin += FakeRate * QCDNorm_Hist.GetBinContent(bb + 1)
##    for bb in range(MASS_BIN):
##        for ss in range(PT_BIN):
##            fakeCorrection=  fitFunc_Exp3Par(ss + 0.5, fitparOSSS0, fitparOSSS1,fitparOSSS2)
##            if PostFix == "":   FakeRate = fakeCorrection
##            if PostFix == "Down":   FakeRate = 1
##            if PostFix == "Up":   FakeRate = pow(fakeCorrection, 2)
##            NormInPtBin += FakeRate * QCDNorm_Hist.GetBinContent(bb + 1, ss + 1)
#
#    return NormInPtBin
#
#
##############################################################################################################
###   Finalizaoing and Creating the ROOT File inclusing QCD shape and norm
##############################################################################################################
#CorrectionFR= ["","Down","Up"]
#CorrectionOSSS= ["","Down","Up"]
##POSTFIX=["","Up","Down"]
#def GetFinalQCDShapeNorm(Observable,CoMEnergy):
#
#    FinalFile = TFile("QCDFinalFile.root", "RECREATE")
#
#    for catName in Bcategory:
#        for channelName in channel:
#            for shiftFR in CorrectionFR:
#                for shiftOSSS in CorrectionOSSS:
#            
#                    getFileBar=ApplyCorrectionOnQCDShape(Observable,"_8TeV","_Bar",catName,channelName,shiftFR,shiftOSSS)
#                    getFileCen=ApplyCorrectionOnQCDShape(Observable,"_8TeV","_Cen",catName,channelName,shiftFR,shiftOSSS)
#                    getFileEnd=ApplyCorrectionOnQCDShape(Observable,"_8TeV","_End",catName,channelName,shiftFR,shiftOSSS)
#
#                    HistoBar=getFileBar.Get("XXX")
#                    HistoCen=getFileCen.Get("XXX")
#                    HistoEnd=getFileEnd.Get("XXX")
#
#                    QCDShapeTotal= HistoBar.Clone()
#                    QCDShapeTotal.Add(HistoCen)
#                    QCDShapeTotal.Add(HistoEnd)
#
#        #                if apply106asScaleFactor: NewFinalQCDEstimate=GetNorm_QCD("",CoMEnergy,channelName,catName,Observable+"_mTLess30_SS","")* QCDScaleFactor
#
##                    if applyOSSSForQCDNorm and applyTauFR_Correction: NewFinalQCDEstimate=ApplyCorrectionOnQCDNormalization(Observable,CoMEnergy, "", catName, channelName, shiftOSSS)  # FIXME  Just same Normalization for all systematics
#                    if applyOSSSForQCDNorm and applyTauFR_Correction: NewFinalQCDEstimate=ApplyCorrectionOnQCDNormalization(Observable,CoMEnergy, "", catName, channelName, "")
#
#
#                    QCDShapeTotal.Scale(NewFinalQCDEstimate/QCDShapeTotal.Integral())
#
#                    FinalFile.WriteObject(QCDShapeTotal,channelName+"_QCDShapeNormTotal"+shiftFR+"FR"+shiftOSSS+"OSSS"+catName)
#
#
#############################################################################################################
##   Run the Jobs
#############################################################################################################
if __name__ == "__main__":
#    GetFinalQCDShapeNorm("_SVMass","_8TeV")
    Make_Tau_FakeRate("","_8TeV","_inclusive","mutau","")



