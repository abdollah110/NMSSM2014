#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="abdollahmohammadi"
__date__ ="$Jul 1, 2014 10:11:41 PM$"

import ROOT
from ROOT import TFile
from ROOT import TH1F
from ROOT import TCanvas
from ROOT import gStyle
from ctypes import *
import array


canvas = TCanvas("canvas", "", 600, 600)
gStyle.SetOptStat(0);
gStyle.SetOptTitle(0);
canvas.SetFillColor(0)
canvas.SetTitle("")
canvas.SetName("CMS Preliminary")
canvas.SetBorderMode(0)
canvas.SetBorderSize(2)
canvas.SetFrameBorderMode(0)
canvas.SetFrameLineWidth(2)
canvas.SetFrameBorderMode(0)
canvas.SetGridx(10)
canvas.SetGridy(10)

category=["Tau_inclusive","Tau_nobtag","Tau_btag"]
Sample=["VV","TT","ZL","ZJ","ZTT","W" ,"QCD","data_obs",]

for cat in category:
    for sample in Sample:
        Tfile_Abdollah= TFile("TotalRootForLimit_muTau_8TeV.root")
        Tfile_Christian= TFile("htt_mt.inputs-mssm-8TeV-0.root")
        TH1F_Abdollah = Tfile_Abdollah.Get("mu"+cat+"/"+sample)
        TH1F_Christian = Tfile_Christian.Get("mu"+cat+"/"+sample)
        print "mu"+cat+"/"+sample, TH1F_Abdollah.Integral(),TH1F_Christian.Integral(), " ====ratio is==== ",  (TH1F_Christian.Integral() - TH1F_Abdollah.Integral())/TH1F_Christian.Integral()
    print "\n"
print "\n\n\n"

for cat in category:
    for sample in Sample:
        Tfile_Abdollah= TFile("TotalRootForLimit_eleTau_8TeV.root")
        Tfile_Christian= TFile("htt_et.inputs-mssm-8TeV-0.root")
        TH1F_Abdollah = Tfile_Abdollah.Get("ele"+cat+"/"+sample)
        TH1F_Christian = Tfile_Christian.Get("ele"+cat+"/"+sample)
        print "ele"+cat+"/"+sample, TH1F_Abdollah.Integral(),TH1F_Christian.Integral(), " ====ratio is==== ",  (TH1F_Christian.Integral() - TH1F_Abdollah.Integral())/TH1F_Christian.Integral()
    print "\n"
print "\n\n\n"    
