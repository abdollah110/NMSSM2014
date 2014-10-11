#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

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
    FinalState = ['mmet_tot', 'mmmt_tot', 'mmme_tot', 'eett_tot', 'eemt_tot', 'eeet_tot', 'eeem_tot']
    for Sample in TextSamples.readlines():

        print "sample is :    ",  Sample
        Tag = Sample[:-6]

        #Writing the tree
        outName = "File_SVMASS_" + Tag + ".root"
        outNameTFile = TFile(str(outName), "RECREATE")
        outTree = TTree('Tree_SVMASS', 'tree')
        # This is how a Tree is written
        svmass = n.zeros(1, dtype=float)
        outTree.Branch('svmass', svmass, 'svmass/D')
        # This is how a Tree is written
        svmassUnc = n.zeros(1, dtype=float)
        outTree.Branch('svmassUnc', svmassUnc, 'svmassUnc/D')

        File_mmet = ROOT.TFile.Open(Tag + "mmet_tot.root", "READ")
        File_mmmt = ROOT.TFile.Open(Tag + "mmmt_tot.root", "READ")
        File_mmme = ROOT.TFile.Open(Tag + "mmme_tot.root", "READ")
        File_eett = ROOT.TFile.Open(Tag + "eett_tot.root", "READ")
        File_eemt = ROOT.TFile.Open(Tag + "eemt_tot.root", "READ")
        File_eeet = ROOT.TFile.Open(Tag + "eeet_tot.root", "READ")
        File_eeem = ROOT.TFile.Open(Tag + "eeem_tot.root", "READ")
        FileCollection = [File_mmet, File_mmmt, File_mmme, File_eett, File_eemt, File_eeet, File_eeem]

        File_mmtt = ROOT.TFile.Open(Tag + "mmtt_tot.root", "READ")
        firstTr = File_mmtt.Get("Mass_tree")
        for entry in xrange(firstTr.GetEntries()):
            if (entry % 10000 == 0): print "Entry is : ", entry
            firstTr.GetEntry(entry)
            total = 0.
            totalUnc = 0.
            total += firstTr.SVmass
            totalUnc += firstTr.SVmassUnc
            for MyTfile in FileCollection:
                MyTree = MyTfile.Get("Mass_tree")
                MyTree.GetEntry(entry)
                total += MyTree.SVmass
                totalUnc += MyTree.SVmassUnc
                svmass[0]  = total
                svmassUnc[0]  = totalUnc
            outTree.Fill()

        File_eeem.Close()
        outNameTFile.cd()
        outTree.Write()
        outNameTFile.Close()







