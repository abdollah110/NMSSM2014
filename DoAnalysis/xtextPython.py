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


#inclusiveBinningP = array([0,50,100,300,500,1000])
#inclusiveBinning = py_object([0,50,100,300,500,1000])
#inclusiveBinningP= pointer(inclusiveBinning)

#import array
#inclusiveBinningP = array.array('d', [0,10,30,50,70,100,200,300,500,1000])
#
#
##def _Return_Value_Signal(bb,Name):
##    theFile = TFile("OutFiles/out_Data_8TeV.root")
##    Histo = theFile.Get("MuTau_visibleMass_mTLess30_OS_inclusive")
##    binCont = 0
##    binErr = 0
##    if Histo:
##        hist_rebinned = Histo.Rebin(len(inclusiveBinningP)-1,"rebinned",inclusiveBinningP)
###        MMM=Histo.Rebin(2,"REBINED",inclusiveBinningP)
###        Histo.Rebin(1000)
###        Histo.Rebin(len(inclusiveBinning),inclusiveBinning)
###        Histo.Rebin(31,"REBINED",inclusiveBinningP)
###        Histo.Rebin(30,"REBINED",inclusiveBinningP)
##        binCont = hist_rebinned.GetBinContent(bb)
##        print bb, binCont
##    theFile.Close()
##    return binCont
#
##print theHist.Integral()
#theFile = TFile("OutFiles/out_Data_8TeV.root")
#Histo = theFile.Get("MuTau_visibleMass_mTLess30_OS_inclusive")
##NewFile = TFile("XOUT.root", "RECREATE")
##NewHist2=TH1F("RebinHisto","",2,inclusiveBinningP)
##NewHist2=TH1F("RebinHisto","",1,0,1000)
#hist_rebinned = Histo.Rebin(len(inclusiveBinningP)-1,"rebinned",inclusiveBinningP)
##hist_rebinned = Histo.Rebin(2,"HI",pointer([0,200,1000]))
##Histo.Draw()
#hist_rebinned.Draw()
##for bb in range(1,3):
##            NewHist2.SetBinContent(bb,_Return_Value_Signal(bb,"MuTau_visibleMass_mTLess30_OS_inclusive"))
##NewFile.Write()
#canvas.SaveAs("XXX.pdf")


category=["Tau_inclusive","Tau_nobtag","Tau_btag"]
Sample=["VV","TT","ZL","ZJ","ZTT","W" ,"QCD","data_obs",]

for cat in category:
    for sample in Sample:
        Tfile_Abdollah= TFile("TotalRootForLimit_MuTau_8TeV.root")
        Tfile_Christian= TFile("htt_mt.inputs-mssm-8TeV-0.root")
        TH1F_Abdollah = Tfile_Abdollah.Get("Mu"+cat+"/"+sample)
        TH1F_Christian = Tfile_Christian.Get("mu"+cat+"/"+sample)
        print "mu"+cat+"/"+sample, TH1F_Abdollah.Integral(),TH1F_Christian.Integral(), " ====ratio is==== ",  (TH1F_Christian.Integral() - TH1F_Abdollah.Integral())/TH1F_Christian.Integral()
    print "\n"
print "\n\n\n"

for cat in category:
    for sample in Sample:
        Tfile_Abdollah= TFile("TotalRootForLimit_EleTau_8TeV.root")
        Tfile_Christian= TFile("htt_et.inputs-mssm-8TeV-0.root")
        TH1F_Abdollah = Tfile_Abdollah.Get("Ele"+cat+"/"+sample)
        TH1F_Christian = Tfile_Christian.Get("ele"+cat+"/"+sample)
        print "ele"+cat+"/"+sample, TH1F_Abdollah.Integral(),TH1F_Christian.Integral(), " ====ratio is==== ",  (TH1F_Christian.Integral() - TH1F_Abdollah.Integral())/TH1F_Christian.Integral()
    print "\n"
print "\n\n\n"    
