#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "abdollahmohammadi"
__date__ = "$Oct 31, 2013 12:02:55 PM$"

#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "abdollahmohammadi"
__date__ = "$Mar 8, 2013 3:00:22 PM$"

import math

import ROOT
from ROOT import Double
from ROOT import TAxis
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH2F
from ROOT import TLatex
from ROOT import TLegend
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gStyle
from ROOT import gSystem

def _PlotMaker(file1, histo1, file2, histo2, reB):

    canvas = TCanvas("canvas", "", 600, 600)
    gStyle.SetOptStat(0);
    gStyle.SetOptTitle(0);
    canvas.SetFillColor(0)
    canvas.SetTitle("qqqqqq")
    canvas.SetName("")
    canvas.SetBorderMode(0)
    canvas.SetBorderSize(2)
    canvas.SetFrameBorderMode(0)
    canvas.SetFrameLineWidth(2)
    canvas.SetFrameBorderMode(0)



    File_1 = TFile(file1, "OPEN")
    Histo_1 = File_1.Get(histo1)

    File_2 = TFile(file2, "OPEN")
    Histo_2 = File_2.Get(histo2)

    Histo_1.Rebin(reB)
    Histo_1.SetLineColor(1)
    Histo_1.SetLineWidth(3)
#    Histo_1.Normalized()
    Histo_1.GetXaxis().SetTitle(histo1)
#    Histo_1.GetYaxis().SetTitle("# of events")
#    print Histo_1.GetMaximum()
    Histo_1.GetYaxis().SetRangeUser(0,2*Histo_1.GetMaximum() );
    Histo_1.DrawNormalized()
    print  "entery of ", Histo_1.GetName(), "  is= ", Histo_1.Integral(),  "   =  ", Histo_1.Integral()/39996.

    if Histo_2:
        Histo_2.Rebin(reB)
        Histo_2.SetLineColor(2)
        Histo_2.SetLineWidth(3)

        Histo_2.DrawNormalized("same")
        print  "entery of ", Histo_2.GetName(), "  is= ", Histo_2.Integral(),  "   =  ", Histo_2.Integral()/40000.


    legend_ = TLegend(0.6, 0.6, 0.9, 0.9)
    legend_.SetFillColor(0)
    legend_.SetBorderSize(0)
    legend_.SetTextSize(.05)
    legend_.AddEntry(Histo_1, "Vegas", "lp")
    legend_.AddEntry(Histo_2, "Markov", "lp")
    legend_.Draw()
    

    ChannelName = histo1 + "_compare.pdf"
    canvas.SaveAs(ChannelName)


if __name__ == "__main__":
    _PlotMaker("ZZ4L_8TeV.root", "SVMass_mmtt_pp", "../DoWholeAnalysis/noCorrectedZZ.root", "SVMass_mmtt_pp", 20)
#    _PlotMaker("ZZ4L_8TeV.root", "SVMass_mmtt_pp", "../DoWholeAnalysis/outRoot_V3/ZZ4L_8TeV.root", "SVMass_mmtt_pp", 20)
#    _PlotMaker("ZZ4L_8TeV.root", "SVMass_mmet_pp", "../DoWholeAnalysis/outRoot_V3/ZZ4L_8TeV.root", "SVMass_mmet_pp", 20)

#    _PlotMaker("_testpythia6.root", "genJet_Pt", "_testpythia8.root", "genJet_Pt", 10)
#    _PlotMaker("_testpythia6.root", "genJet_Eta", "_testpythia8.root", "genJet_Eta", 10)
#    _PlotMaker("_testpythia6.root", "genJet_Phi", "_testpythia8.root", "genJet_Phi", 10)

#    _PlotMaker("_testpythia6.root", "gen_tau_Pt", "_testpythia8.root", "gen_tau_Pt", 10)
#    _PlotMaker("_testpythia6.root", "gen_tau_Eta", "_testpythia8.root", "gen_tau_Eta", 10)
#    _PlotMaker("_testpythia6.root", "gen_tau_Phi", "_testpythia8.root", "gen_tau_Phi", 10)
#
#    _PlotMaker("_testpythia6.root", "gen_electron_Pt", "_testpythia8.root", "gen_electron_Pt", 10)
#    _PlotMaker("_testpythia6.root", "gen_electron_Eta", "_testpythia8.root", "gen_electron_Eta", 10)
#    _PlotMaker("_testpythia6.root", "gen_electron_Phi", "_testpythia8.root", "gen_electron_Phi", 10)
#
#    _PlotMaker("_testpythia6.root", "gen_mu_Pt", "_testpythia8.root", "gen_mu_Pt", 10)
#    _PlotMaker("_testpythia6.root", "gen_mu_Eta", "_testpythia8.root", "gen_mu_Eta", 10)
#    _PlotMaker("_testpythia6.root", "gen_mu_Phi", "_testpythia8.root", "gen_mu_Phi", 10)
#
#    _PlotMaker("_testpythia6.root", "gen_Higgs_Pt", "_testpythia8.root", "gen_Higgs_Pt", 10)
#    _PlotMaker("_testpythia6.root", "gen_Higgs_Eta", "_testpythia8.root", "gen_Higgs_Eta", 10)
#    _PlotMaker("_testpythia6.root", "gen_Higgs_Phi", "_testpythia8.root", "gen_Higgs_Phi", 10)
#
#
#    _PlotMaker("_testpythia6.root", "gen_chargedPion_Pt", "_testpythia8.root", "gen_chargedPion_Pt", 10)
#    _PlotMaker("_testpythia6.root", "gen_chargedPion_Eta", "_testpythia8.root", "gen_chargedPion_Eta", 10)
#    _PlotMaker("_testpythia6.root", "gen_chargedPion_Phi", "_testpythia8.root", "gen_chargedPion_Phi", 10)
#
#
#    _PlotMaker("_testpythia6.root", "gen_Rho_Pt", "_testpythia8.root", "gen_Rho_Pt", 10)
#    _PlotMaker("_testpythia6.root", "gen_Rho_Eta", "_testpythia8.root", "gen_Rho_Eta", 10)
#    _PlotMaker("_testpythia6.root", "gen_Rho_Phi", "_testpythia8.root", "gen_Rho_Phi", 10)
#
#
#    _PlotMaker("_testpythia6.root", "gen_a1_Pt", "_testpythia8.root", "gen_a1_Pt", 10)
#    _PlotMaker("_testpythia6.root", "gen_a1_Eta", "_testpythia8.root", "gen_a1_Eta", 10)
#    _PlotMaker("_testpythia6.root", "gen_a1_Phi", "_testpythia8.root", "gen_a1_Phi", 10)
#
