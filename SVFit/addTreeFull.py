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

    TextSamples = open("textSample.txt", "r")
    OriginalFilesLoc = "../ROOTFiles/Results/"
    MassFileLoc = "../ROOTFiles/svmasses/"
    FinalState = ['mmet_tot', 'mmmt_tot', 'mmme_tot', 'eett_tot', 'eemt_tot', 'eeet_tot', 'eeem_tot']
    for Sample in TextSamples.readlines():

        print "sample is :    ",  Sample
        Tag = Sample[:-6]
        outNameOrig = OriginalFilesLoc + Tag + ".root"
        outNameMass = MassFileLoc + "File_SVMASS_" + Tag + ".root"

#        Reading the original
        FileOrig = ROOT.TFile.Open(outNameOrig, "UPDATE")
        addNewBranch2Tree = TTree('Mass_tree', 'Mass_tree')
        SVMass = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMass', SVMass, 'SVMass/D')
        SVMassUnc = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMassUnc', SVMassUnc, 'SVMassUnc/D')

#        Reading the Mass
        FileMass = ROOT.TFile.Open(outNameMass, "READ")
        firstTr = FileMass.Get("Tree_SVMASS")

        for entry in xrange(firstTr.GetEntries()):
                if (entry % 20000 == 0): print "Entry is : ", entry
                firstTr.GetEntry(entry)
                SVMass[0] = firstTr.svmass
                SVMassUnc[0] = firstTr.svmassUnc
                addNewBranch2Tree.Fill()
        FileMass.Close()
        FileOrig.cd()
        addNewBranch2Tree.Write()
        FileOrig.Close()

