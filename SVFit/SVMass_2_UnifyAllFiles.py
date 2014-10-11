#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
# Once the jobs are done we need to add all final states (8) and 3 different energy scale togheter
# to just one file. Here are the step
# 1) we need the same text files  "textSample.txt" including the names of all samples
# 2) a directory with all outPut of root files from submission
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
    AllRootDir = "Step1_ROOT/"
#    FinalState = ['et_tot', 'mmmt_tot', 'mmme_tot', 'eett_tot', 'eemt_tot', 'eeet_tot', 'eeem_tot']
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

        MassScale= [svmass,svmassUp,svmassDown]
        MassScaleUnc= [svmassUnc,svmassUncUp,svmassUncDown]

#        for scale in range(len(ScaleValue)):
        File_et0 = ROOT.TFile.Open(AllRootDir+Tag + "et_tot" +str(ScaleValue[0])+".root", "READ")
        File_mt1 = ROOT.TFile.Open(AllRootDir+Tag + "mt_tot" +str(ScaleValue[1])+".root", "READ")
        File_et1 = ROOT.TFile.Open(AllRootDir+Tag + "et_tot" +str(ScaleValue[1])+".root", "READ")
        File_mt2 = ROOT.TFile.Open(AllRootDir+Tag + "mt_tot" +str(ScaleValue[2])+".root", "READ")
        File_et2 = ROOT.TFile.Open(AllRootDir+Tag + "et_tot" +str(ScaleValue[2])+".root", "READ")
        FileCollection = [File_et0, File_mt1,File_et1 ,File_mt2,File_et2]

        File_mmtt = ROOT.TFile.Open(AllRootDir+Tag + "mt_tot" +str(ScaleValue[0])+".root", "READ")
        print AllRootDir+Tag + "mt_tot" +str(ScaleValue[0])+".root"
        firstTr = File_mmtt.Get("Mass_tree")
        for entry in xrange(firstTr.GetEntries()):
            if (entry % 10000 == 0): print "Entry is : ", entry
            firstTr.GetEntry(entry)
            total0 = 0.
            totalUnc0 = 0.
            total1 = 0.
            totalUnc1 = 0.
            total2 = 0.
            totalUnc2 = 0.
            total0 += firstTr.SVmass
            totalUnc0 += firstTr.SVmassUnc
            for MyTfile in range(len(FileCollection)):
                MyTree = FileCollection[MyTfile].Get("Mass_tree")
                MyTree.GetEntry(entry)
                if(MyTfile < 1):
                    total0 += MyTree.SVmass
                    totalUnc0 += MyTree.SVmassUnc
                    MassScale[0][0]  = total0
                    MassScaleUnc[0][0]  = totalUnc0
                if(MyTfile >= 1 and MyTfile < 3 ):
                    total1 += MyTree.SVmass
                    totalUnc1 += MyTree.SVmassUnc
                    MassScale[1][0]  = total1
                    MassScaleUnc[1][0]  = totalUnc1
                if(MyTfile >= 4 ):
                    total2 += MyTree.SVmass
                    totalUnc2 += MyTree.SVmassUnc
                    MassScale[2][0]  = total2
                    MassScaleUnc[2][0]  = totalUnc2
            outTree.Fill()

        File_et2.Close()
        File_mt2.Close()
        File_mt1.Close()
        File_et1.Close()
        File_et0.Close()
        File_mt0.Close()
#        File_mmtt0.Close()
        outNameTFile.cd()
        outTree.Write()
        outNameTFile.Close()







