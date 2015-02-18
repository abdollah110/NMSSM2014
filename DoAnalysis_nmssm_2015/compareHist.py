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
from ROOT import TLegend
from ROOT import TPaveText
import array
gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)


Canvas= TCanvas("Can","Can",600,800)
File1 = TFile("QCDFinalFile_Default.root")
Hist1= File1.Get("mutau_QCDShapeNormTotalFROSSS_inclusive")
#Hist1= File1.Get("mutau_QCDShapeNormTotalFROSSS_btag")
File2 = TFile("QCDFinalFile.root")
Hist2= File2.Get("mutau_QCDShapeNormTotalFROSSS_inclusive")
#Hist2= File2.Get("mutau_QCDShapeNormTotalFROSSS_btag")


Hist1.Rebin(5)
Hist2.Rebin(5)



Hist1.SetTitle("")
Hist1.SetStats(0)
Hist1.GetXaxis().SetTitle("M_{#tau#tau} [GeV]")
Hist1.GetYaxis().SetTitle("# of events")


Hist1.SetMarkerStyle(20)
Hist1.SetMarkerColor(2)
Hist1.SetLineColor(2)
Hist1.Draw("E")


Hist2.SetMarkerColor(4)
Hist2.SetMarkerStyle(25)
Hist2.SetLineColor(4)
Hist2.Draw("Esame")

leg=TLegend(0.52, 0.78, 0.92, 0.89)
leg.SetFillStyle (0);
leg.SetFillColor (0);
leg.SetBorderSize(0);
leg.AddEntry(Hist1,"RelaxIsoTau_antiIsoMu","lp")
leg.AddEntry(Hist2,"RelaxIsoTau_IsoMu","lp")
leg.Draw()


lumi  = TPaveText(0.67, 0.61, 0.82, 0.79, "NDC");
lumi.SetBorderSize(   0 );
lumi.SetFillStyle(    0 );
lumi.SetTextAlign(   12 );
lumi.SetTextSize ( 0.03 );
lumi.SetTextColor(    1 );
lumi.SetTextFont (   62 );
#lumi.AddText("muTau-Btag");
lumi.AddText("muTau-Inclusive");
lumi.Draw();



Canvas.SaveAs("shapeComparison_QCD_Inclusive.pdf")
#Canvas.SaveAs("shapeComparison_QCD_btag.pdf")