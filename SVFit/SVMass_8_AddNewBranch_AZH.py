#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
# Here we need 3 files, first textSamples.txt, then svmass where the output of the last step is there
#(mass and uncertainty on the mass) combined and finall Results which the original root files are here
# the aim here is to add a massTree from svmass/XXX.root to Results/XXX.root

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
    OriginalFilesLoc = "/afs/cern.ch/work/a/abdollah/SVFit/NewRootCopy/"
    MassFileLoc = "AZH_File_SVMASS/"
#    OriginalFilesLoc = "../ROOTFiles/Results/"
#    MassFileLoc = "../ROOTFiles/svmasses/"
    FinalState = ['mmet_tot', 'mmmt_tot', 'mmme_tot', 'eett_tot', 'eemt_tot', 'eeet_tot', 'eeem_tot']
    for Sample in TextSamples.readlines():

        print "sample is :    ",  Sample
        Tag = Sample[:-6]
        outNameOrig = OriginalFilesLoc + Tag + ".root"
        outNameMass = MassFileLoc + "File_SVMASS_" + Tag + ".root"
        #        Reading the original
        FileOrig = ROOT.TFile.Open(outNameOrig, "UPDATE")
        addNewBranch2Tree = TTree('Mass_tree', 'Mass_tree')

#        Masses
        SVMass = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMass', SVMass, 'SVMass/D')
        SVMassUp = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMassUp', SVMassUp, 'SVMassUp/D')
        SVMassDown = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMassDown', SVMassDown, 'SVMassDown/D')
        # Uncertainties
        SVMassUnc = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMassUnc', SVMassUnc, 'SVMassUnc/D')
        SVMassUncUp = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMassUncUp', SVMassUncUp, 'SVMassUncUp/D')
        SVMassUncDown = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('SVMassUncDown', SVMassUncDown, 'SVMassUncDown/D')
#        Pt
        Pt = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('Pt', Pt, 'Pt/D')
        PtUp = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('PtUp', PtUp, 'PtUp/D')
        PtDown = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('PtDown', PtDown, 'PtDown/D')
#        Eta
        Eta = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('Eta', Eta, 'Eta/D')
        EtaUp = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('EtaUp', EtaUp, 'EtaUp/D')
        EtaDown = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('EtaDown', EtaDown, 'EtaDown/D')
#        Phi
        Phi = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('Phi', Phi, 'Phi/D')
        PhiUp = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('PhiUp', PhiUp, 'PhiUp/D')
        PhiDown = n.zeros(1, dtype=float)
        addNewBranch2Tree.Branch('PhiDown', PhiDown, 'PhiDown/D')


#        Reading the Mass
        print outNameMass
        FileMass = ROOT.TFile.Open(outNameMass, "READ")
        firstTr = FileMass.Get("Tree_SVMASS")

        for entry in xrange(firstTr.GetEntries()):
                if (entry % 20000 == 0): print "Entry is : ", entry
                firstTr.GetEntry(entry)
                SVMass[0] = firstTr.svmass
                SVMassUp[0] = firstTr.svmassUp
                SVMassDown[0] = firstTr.svmassDown
                SVMassUnc[0] = firstTr.svmassUnc
                SVMassUncUp[0] = firstTr.svmassUncUp
                SVMassUncDown[0] = firstTr.svmassUncDown
                Pt[0] = firstTr.Pt
                PtUp[0] = firstTr.PtUp
                PtDown[0] = firstTr.PtDown
                Eta[0] = firstTr.Eta
                EtaUp[0] = firstTr.EtaUp
                EtaDown[0] = firstTr.EtaDown
                Phi[0] = firstTr.Phi
                PhiUp[0] = firstTr.PhiUp
                PhiDown[0] = firstTr.PhiDown
                addNewBranch2Tree.Fill()
        FileMass.Close()
        FileOrig.cd()
        addNewBranch2Tree.Write()
        FileOrig.Close()

