#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
# Once the jobs are done we need to add all final states (8) and 3 different energy scale togheter
# to just one file. Here are the step
# 1) we need the same text files  "textSample.txt" including the names of all samples
# 2) a directory with all outPut of root files from submission  forLaptop
# 3) Run the current code: python SVMass_2_UnifyAllFiles.py

__author__ = "abdollahmohammadi"
__date__ = "$May 3, 2013 12:13:45 PM$"

from array import array
import math

import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH2F
from ROOT import TLatex
from ROOT import TLegend
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import TTree
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gStyle
from ROOT import gSystem
import numpy as n
import os


if __name__ == "__main__":
    TextSamples = open("textSamples.txt", "r")
    AllRootDir = "forLaptop/"
#    FinalState = ['mmet_tot', 'mmmt_tot', 'mmme_tot', 'eett_tot', 'eemt_tot', 'eeet_tot', 'eeem_tot']
    for Sample in TextSamples.readlines():

        print "sample is :    ",  Sample
        Tag = Sample[:-6]
        ScaleValue = [ 0, 0.03, -0.03,]
#        ScaleValue = [0]

        #Writing the tree
        outName = "File_SVMASS_" + Tag + ".root"
        outNameTFile = TFile(str(outName), "RECREATE")
        outTree = TTree('Tree_SVMASS', 'tree')
        # This is how a Tree is written
        svmass = n.zeros(1, dtype=float)
        outTree.Branch('svmass', svmass, 'svmass/D')
        svmassUp = n.zeros(1, dtype=float)
        outTree.Branch('svmassUp', svmassUp, 'svmassUp/D')
        svmassDown = n.zeros(1, dtype=float)
        outTree.Branch('svmassDown', svmassDown, 'svmassDown/D')
        # This is how a Tree is written
        svmassUnc = n.zeros(1, dtype=float)
        outTree.Branch('svmassUnc', svmassUnc, 'svmassUnc/D')
        svmassUncUp = n.zeros(1, dtype=float)
        outTree.Branch('svmassUncUp', svmassUncUp, 'svmassUncUp/D')
        svmassUncDown = n.zeros(1, dtype=float)
        outTree.Branch('svmassUncDown', svmassUncDown, 'svmassUncDown/D')
        # This is how a Tree is written
        Pt = n.zeros(1, dtype=float)
        outTree.Branch('Pt', Pt, 'Pt/D')
        PtUp = n.zeros(1, dtype=float)
        outTree.Branch('PtUp', PtUp, 'PtUp/D')
        PtDown = n.zeros(1, dtype=float)
        outTree.Branch('PtDown', PtDown, 'PtDown/D')
        # This is how a Tree is written
        Eta = n.zeros(1, dtype=float)
        outTree.Branch('Eta', Eta, 'Eta/D')
        EtaUp = n.zeros(1, dtype=float)
        outTree.Branch('EtaUp', EtaUp, 'EtaUp/D')
        EtaDown = n.zeros(1, dtype=float)
        outTree.Branch('EtaDown', EtaDown, 'EtaDown/D')
        # This is how a Tree is written
        Phi = n.zeros(1, dtype=float)
        outTree.Branch('Phi', Phi, 'Phi/D')
        PhiUp = n.zeros(1, dtype=float)
        outTree.Branch('PhiUp', PhiUp, 'PhiUp/D')
        PhiDown = n.zeros(1, dtype=float)
        outTree.Branch('PhiDown', PhiDown, 'PhiDown/D')

        MassScale= [svmass,svmassUp,svmassDown]
        MassScaleUnc= [svmassUnc,svmassUncUp,svmassUncDown]
        PtScale= [Pt,PtUp,PtDown]
        EtaScale= [Eta,EtaUp,EtaDown]
        PhiScale= [Phi,PhiUp,PhiDown]


#        for scale in range(len(ScaleValue)):
        File_mmet0 = ROOT.TFile.Open(AllRootDir+Tag + "mmet_tot" +str(ScaleValue[0])+".root", "READ")
        File_mmmt0 = ROOT.TFile.Open(AllRootDir+Tag + "mmmt_tot" +str(ScaleValue[0])+".root", "READ")
        File_mmme0 = ROOT.TFile.Open(AllRootDir+Tag + "mmme_tot" +str(ScaleValue[0])+".root", "READ")
        File_eett0 = ROOT.TFile.Open(AllRootDir+Tag + "eett_tot" +str(ScaleValue[0])+".root", "READ")
        File_eemt0 = ROOT.TFile.Open(AllRootDir+Tag + "eemt_tot" +str(ScaleValue[0])+".root", "READ")
        File_eeet0 = ROOT.TFile.Open(AllRootDir+Tag + "eeet_tot" +str(ScaleValue[0])+".root", "READ")
        File_eeem0 = ROOT.TFile.Open(AllRootDir+Tag + "eeem_tot" +str(ScaleValue[0])+".root", "READ")
        File_mmtt1 = ROOT.TFile.Open(AllRootDir+Tag + "mmtt_tot" +str(ScaleValue[1])+".root", "READ")
        File_mmet1 = ROOT.TFile.Open(AllRootDir+Tag + "mmet_tot" +str(ScaleValue[1])+".root", "READ")
        File_mmmt1 = ROOT.TFile.Open(AllRootDir+Tag + "mmmt_tot" +str(ScaleValue[1])+".root", "READ")
        File_mmme1 = ROOT.TFile.Open(AllRootDir+Tag + "mmme_tot" +str(ScaleValue[1])+".root", "READ")
        File_eett1 = ROOT.TFile.Open(AllRootDir+Tag + "eett_tot" +str(ScaleValue[1])+".root", "READ")
        File_eemt1 = ROOT.TFile.Open(AllRootDir+Tag + "eemt_tot" +str(ScaleValue[1])+".root", "READ")
        File_eeet1 = ROOT.TFile.Open(AllRootDir+Tag + "eeet_tot" +str(ScaleValue[1])+".root", "READ")
        File_eeem1 = ROOT.TFile.Open(AllRootDir+Tag + "eeem_tot" +str(ScaleValue[1])+".root", "READ")
        File_mmtt2 = ROOT.TFile.Open(AllRootDir+Tag + "mmtt_tot" +str(ScaleValue[2])+".root", "READ")
        File_mmet2 = ROOT.TFile.Open(AllRootDir+Tag + "mmet_tot" +str(ScaleValue[2])+".root", "READ")
        File_mmmt2 = ROOT.TFile.Open(AllRootDir+Tag + "mmmt_tot" +str(ScaleValue[2])+".root", "READ")
        File_mmme2 = ROOT.TFile.Open(AllRootDir+Tag + "mmme_tot" +str(ScaleValue[2])+".root", "READ")
        File_eett2 = ROOT.TFile.Open(AllRootDir+Tag + "eett_tot" +str(ScaleValue[2])+".root", "READ")
        File_eemt2 = ROOT.TFile.Open(AllRootDir+Tag + "eemt_tot" +str(ScaleValue[2])+".root", "READ")
        File_eeet2 = ROOT.TFile.Open(AllRootDir+Tag + "eeet_tot" +str(ScaleValue[2])+".root", "READ")
        File_eeem2 = ROOT.TFile.Open(AllRootDir+Tag + "eeem_tot" +str(ScaleValue[2])+".root", "READ")
        FileCollection = [File_mmet0,File_mmmt0, File_mmme0, File_eett0, File_eemt0, File_eeet0, File_eeem0, File_mmtt1,File_mmet1, File_mmmt1, File_mmme1, File_eett1, File_eemt1, File_eeet1, File_eeem1, File_mmtt2,File_mmet2, File_mmmt2, File_mmme2, File_eett2, File_eemt2, File_eeet2, File_eeem2]

        File_mmtt = ROOT.TFile.Open(AllRootDir+Tag + "mmtt_tot" +str(ScaleValue[0])+".root", "READ")
        print AllRootDir+Tag + "mmtt_tot" +str(ScaleValue[0])+".root"
        treeSVFit = File_mmtt.Get("Mass_tree")
        for entry in xrange(treeSVFit.GetEntries()):
            if (entry % 10000 == 0): print "Entry is : ", entry
            treeSVFit.GetEntry(entry)

            ##Initial Norm
            totMassNorm = 0.
            totMassUncNorm = 0.
            totPtNorm = 0.
            totEtaNorm = 0.
            totPhiNorm = 0.
            ##Initial Up
            totMassUp = 0.
            totMassUncUp = 0.
            totPtUp = 0.
            totEtaUp = 0.
            totPhiUp = 0.
            ##Initial Down
            totMassDown = 0.
            totMassUncDown = 0.
            totPtDown = 0.
            totEtaDown = 0.
            totPhiDown = 0.

            if (treeSVFit.SVmass > -5) : totMassNorm += treeSVFit.SVmass
            if (treeSVFit.SVmassUnc > -5) : totMassUncNorm += treeSVFit.SVmassUnc
            if (treeSVFit.PT > -5) : totPtNorm += treeSVFit.PT
            if (treeSVFit.ETA > -5) : totEtaNorm += treeSVFit.ETA
            if (treeSVFit.PHI > -5) : totPhiNorm += treeSVFit.PHI

            for MyTfile in range(len(FileCollection)):
                treeSVFitOthers = FileCollection[MyTfile].Get("Mass_tree")
                treeSVFitOthers.GetEntry(entry)

                if(MyTfile < 7):
                    if (treeSVFitOthers.SVmass > -5) : totMassNorm += treeSVFitOthers.SVmass
                    if (treeSVFitOthers.SVmassUnc > -5) : totMassUncNorm += treeSVFitOthers.SVmassUnc
                    if (treeSVFitOthers.PT > -5) : totPtNorm += treeSVFitOthers.PT
                    if (treeSVFitOthers.ETA > -5) : totEtaNorm += treeSVFitOthers.ETA
                    if (treeSVFitOthers.PHI > -5) : totPhiNorm += treeSVFitOthers.PHI
                    MassScale[0][0]  = totMassNorm
                    MassScaleUnc[0][0]  = totMassUncNorm
                    PtScale[0][0]  = totPtNorm
                    EtaScale[0][0]  = totEtaNorm
                    PhiScale[0][0]  = totPhiNorm

                if(MyTfile >= 7 and MyTfile < 15 ):
                    if (treeSVFitOthers.SVmass > -5) : totMassUp += treeSVFitOthers.SVmass
                    if (treeSVFitOthers.SVmassUnc > -5) : totMassUncUp += treeSVFitOthers.SVmassUnc
                    if (treeSVFitOthers.PT > -5) : totPtUp += treeSVFitOthers.PT
                    if (treeSVFitOthers.ETA > -5) : totEtaUp += treeSVFitOthers.ETA
                    if (treeSVFitOthers.PHI > -5) : totPhiUp += treeSVFitOthers.PHI
                    MassScale[1][0]  = totMassUp
                    MassScaleUnc[1][0]  = totMassUncUp
                    PtScale[1][0]  = totPtNorm
                    EtaScale[1][0]  = totEtaNorm
                    PhiScale[1][0]  = totPhiNorm

                if(MyTfile >= 15 ):
                    if (treeSVFitOthers.SVmass > -5) : totMassDown += treeSVFitOthers.SVmass
                    if (treeSVFitOthers.SVmassUnc > -5) : totMassUncDown += treeSVFitOthers.SVmassUnc
                    if (treeSVFitOthers.PT > -5) : totPtDown += treeSVFitOthers.PT
                    if (treeSVFitOthers.ETA > -5) : totEtaDown += treeSVFitOthers.ETA
                    if (treeSVFitOthers.PHI > -5) : totPhiDown += treeSVFitOthers.PHI
                    MassScale[2][0]  = totMassDown
                    MassScaleUnc[2][0]  = totMassUncDown
                    PtScale[2][0]  = totPtNorm
                    EtaScale[2][0]  = totEtaNorm
                    PhiScale[2][0]  = totPhiNorm
            outTree.Fill()

        File_eeem2.Close()
        File_eeem1.Close()
        File_eeem0.Close()
        File_eeet2.Close()
        File_eeet1.Close()
        File_eeet0.Close()
        File_eemt2.Close()
        File_eemt1.Close()
        File_eemt0.Close()
        File_eett2.Close()
        File_eett1.Close()
        File_eett0.Close()
        File_mmme2.Close()
        File_mmme1.Close()
        File_mmme0.Close()
        File_mmet2.Close()
        File_mmet1.Close()
        File_mmet0.Close()
        File_mmmt2.Close()
        File_mmmt1.Close()
        File_mmmt0.Close()
        File_mmtt2.Close()
        File_mmtt1.Close()
#        File_mmtt0.Close()
        outNameTFile.cd()
        outTree.Write()
        outNameTFile.Close()







