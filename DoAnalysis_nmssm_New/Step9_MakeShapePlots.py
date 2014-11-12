#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
#http:#root.cern.ch/viewvc/trunk/tutorials/pyroot/hsimple.py?revision=20881&view=markup
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
from ROOT import TLegend
from ROOT import TStyle
from ROOT import TPaveText
from ctypes import *
import ROOT as r
import array

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)



def SetStyle():

      HttStyle = TStyle("Htt-Style","The Perfect Style for Plots -)")
      gStyle = HttStyle

      # Canvas
      HttStyle.SetCanvasColor     (0)
      HttStyle.SetCanvasBorderSize(10)
      HttStyle.SetCanvasBorderMode(0)
      HttStyle.SetCanvasDefH      (700)
      HttStyle.SetCanvasDefW      (700)
      HttStyle.SetCanvasDefX      (100)
      HttStyle.SetCanvasDefY      (100)

      # color palette for 2D temperature plots
#      HttStyle.SetPalette(1,0)

      # Pads
      HttStyle.SetPadColor       (0)
      HttStyle.SetPadBorderSize  (10)
      HttStyle.SetPadBorderMode  (0)
      HttStyle.SetPadBottomMargin(0.13)
      HttStyle.SetPadTopMargin   (0.08)
      HttStyle.SetPadLeftMargin  (0.15)
      HttStyle.SetPadRightMargin (0.05)
      HttStyle.SetPadGridX       (0)
      HttStyle.SetPadGridY       (0)
      HttStyle.SetPadTickX       (1)
      HttStyle.SetPadTickY       (1)

      # Frames
      HttStyle.SetLineWidth(3)
      HttStyle.SetFrameFillStyle ( 0)
      HttStyle.SetFrameFillColor ( 0)
      HttStyle.SetFrameLineColor ( 1)
      HttStyle.SetFrameLineStyle ( 0)
      HttStyle.SetFrameLineWidth ( 2)
      HttStyle.SetFrameBorderSize(10)
      HttStyle.SetFrameBorderMode( 0)

      # Histograms
      HttStyle.SetHistFillColor(2)
      HttStyle.SetHistFillStyle(0)
      HttStyle.SetHistLineColor(1)
      HttStyle.SetHistLineStyle(0)
      HttStyle.SetHistLineWidth(3)
      HttStyle.SetNdivisions(505, "X")

      # Functions
      HttStyle.SetFuncColor(1)
      HttStyle.SetFuncStyle(0)
      HttStyle.SetFuncWidth(2)

      # Various
      HttStyle.SetMarkerStyle(20)
      HttStyle.SetMarkerColor(1)
      HttStyle.SetMarkerSize (1.1)

      HttStyle.SetTitleBorderSize(0)
      HttStyle.SetTitleFillColor (0)
      HttStyle.SetTitleX         (0.2)

      HttStyle.SetTitleSize  (0.055,"X")
      HttStyle.SetTitleOffset(1.200,"X")
      HttStyle.SetLabelOffset(0.005,"X")
      HttStyle.SetLabelSize  (0.040,"X")
      HttStyle.SetLabelFont  (42   ,"X")

      HttStyle.SetStripDecimals(0)
      HttStyle.SetLineStyleString(11,"20 10")

      HttStyle.SetTitleSize  (0.055,"Y")
      HttStyle.SetTitleOffset(1.600,"Y")
      HttStyle.SetLabelOffset(0.010,"Y")
      HttStyle.SetLabelSize  (0.040,"Y")
      HttStyle.SetLabelFont  (42   ,"Y")

      HttStyle.SetTextSize   (0.055)
      HttStyle.SetTextFont   (42)

      HttStyle.SetStatFont   (42)
      HttStyle.SetTitleFont  (42)
      HttStyle.SetTitleFont  (42,"X")
      HttStyle.SetTitleFont  (42,"Y")

      HttStyle.SetOptStat(0)

      return 



def MakeCanvas(name, title,  dX,  dY):

      canvas = TCanvas(name, title, 0, 0, dX, dY)
#      gStyle=TStyle()
#      gStyle.SetOptStat(0)
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





def MakePlost(HistoName):
    BinCateg = array.array("d",[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300])
    SetStyle()
    canv = MakeCanvas("canv", "histograms", 600, 600)

    File_W_Cor=TFile("QCDFinalFile_WithOSSSCorrection.root")
    inclusive_mt_W_Cor=File_W_Cor.Get(HistoName)
    inclusive_mt_W_Cor_=inclusive_mt_W_Cor.Rebin(len(BinCateg)-1,"",BinCateg)
    inclusive_mt_W_Cor_.GetXaxis().SetRangeUser(0,200)
    inclusive_mt_W_Cor_.GetXaxis().SetTitle("m_{#tau#tau} [GeV]")
    inclusive_mt_W_Cor_.SetMarkerStyle(21)
    inclusive_mt_W_Cor_.SetMarkerColor(2)
    inclusive_mt_W_Cor_.SetLineColor(2)
#    inclusive_mt_W_Cor_.Draw("PE")
    inclusive_mt_W_Cor_.DrawNormalized("PE")
    print inclusive_mt_W_Cor_.Integral()

#    File_No_Cor=TFile("QCDFinalFile_NoOSSSCorrection.root")
    File_No_Cor=TFile("QCDFinalFile_XXX.root")
    inclusive_mt_No_Cor=File_No_Cor.Get(HistoName)
    inclusive_mt_No_Cor_=inclusive_mt_No_Cor.Rebin(len(BinCateg)-1,"",BinCateg)
    inclusive_mt_No_Cor_.SetMarkerStyle(24)
    inclusive_mt_No_Cor_.SetMarkerColor(4)
    inclusive_mt_No_Cor_.SetLineColor(4)
#    inclusive_mt_No_Cor_.Draw("SAMEPE")
    inclusive_mt_No_Cor_.DrawNormalized("SAMEPE")
    print inclusive_mt_No_Cor_.Integral()


    Leg=TLegend(0.62, 0.58, 0.92, 0.79)
    Leg.SetFillStyle (0)
    Leg.SetFillColor (0)
    Leg.SetBorderSize(0)
    Leg.AddEntry(inclusive_mt_No_Cor_,"No Correction", "lp")
    Leg.AddEntry(inclusive_mt_W_Cor_,"OS/SS Correction", "lp")
    Leg.Draw()

    fitInfo  =TPaveText(.20,0.9,.50,0.99, "NDC");
    fitInfo.SetBorderSize(   0 );
    fitInfo.SetFillStyle(    0 );
    fitInfo.SetTextAlign(   12 );
    fitInfo.SetTextSize ( 0.025 );
    fitInfo.SetTextColor(    1 );
    fitInfo.SetTextFont (   62 );
    fitInfo.AddText(HistoName)
    fitInfo.Draw()

#    canv.SaveAs("comparePlot_"+HistoName+".pdf")
    canv.SaveAs("comparePlot_"+HistoName+"_Normalized.pdf")

MakePlost("mutau_QCDShapeNormTotal_btag")
MakePlost("mutau_QCDShapeNormTotal_inclusive")
MakePlost("mutau_QCDShapeNormTotal_nobtag")
MakePlost("mutau_QCDShapeNormTotal_btagLoose")
MakePlost("etau_QCDShapeNormTotal_btag")
MakePlost("etau_QCDShapeNormTotal_inclusive")
MakePlost("etau_QCDShapeNormTotal_nobtag")
MakePlost("etau_QCDShapeNormTotal_btagLoose")
