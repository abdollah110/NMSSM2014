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
from ROOT import TH2F
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gSystem

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
#ROOT.gROOT.ProcessLine('.x rootlogon.C')

################################################
HWWasSignal = False
################################################
n_bin = 15
low_bin = 0
high_bin = 300
reb_ = high_bin / n_bin
DIR_ROOT = 'outRoot_V3/'
channel = ["mmtt", "mmet", "mmmt", "mmme", "eett", "eemt", "eeet", "eeem"]
signal = ['zhtt','zhww']
signalname = ['ZH_htt','ZH_hww']
mass = [90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160]
#BackGround = ['ZZ4L', 'GGToZZ2L2L']
#BackGroundname = ['ZZ', 'GGToZZ2L2L']
BackGround = ['ZZ4L', 'GGToZZ2L2L', 'TTZJets', 'Data']
BackGroundname = ['ZZ', 'GGToZZ2L2L',"TTZ",'data_obs']
lenghtSig = len(signal) * len(mass)

def _Return_Value_Signal(bb,signal, mass, channel,histoName,PostFix,CoMEnergy ):
    myfile = TFile(DIR_ROOT + str(signal) + str(mass) +CoMEnergy+ '.root')
    #  ####################   This is to use zhww @ 125 GeV as Signal/BG  #  ####################
    if (HWWasSignal and signal=="zhww"): myfile = TFile(DIR_ROOT + str(signal) + "125" +CoMEnergy+ '.root')
    #  ####################   #  ####################   #  ####################   #  ############
    Histo =  myfile.Get(histoName + str(channel)+"_pp"+ PostFix)
    binCont = 0
    binErr = 0
    if Histo:
        Histo.Rebin(reb_)
        binCont = Histo.GetBinContent(bb)
        binErr = Histo.GetBinError(bb)
    myfile.Close()
    return binCont , binErr

def _Return_Value_BG(bb,backG, channel,histoName,PostFix,CoMEnergy ):
    myfile = TFile(DIR_ROOT + str(backG)  +CoMEnergy+ '.root')
    Histo =  myfile.Get(histoName + str(channel)+"_pp"+ PostFix)
    binCont = 0
    binErr = 0
    if Histo:
        Histo.Rebin(reb_)
        binCont = Histo.GetBinContent(bb)
        binErr = Histo.GetBinError(bb)
    myfile.Close()
    return binCont , binErr

def MakeTheHistogram(histoName,CoMEnergy):
    myOut = TFile("TotalRootForLimit_"+histoName+CoMEnergy+".root" , 'RECREATE')
    Table_File = TFile("Yield"+CoMEnergy+""+".root")
    Table_Hist = Table_File.Get('FullResults')
    #ScaleUp
    Table_FileUp = TFile("Yield"+CoMEnergy+"_Up"+".root")
    Table_HistUp = Table_FileUp.Get('FullResults')
    #ScaleDown
    Table_FileDown = TFile("Yield"+CoMEnergy+"_Down"+".root")
    Table_HistDown = Table_FileDown.Get('FullResults')



    for chl in range(len(channel)):
        print channel[chl]
        if (channel[chl]=="mmtt" or channel[chl]=="eett") : scaleNameUp = "_CMS_scale_t_lltt"+CoMEnergy+"Up" ; scaleNameDown = "_CMS_scale_t_lltt"+CoMEnergy+"Down"
        if (channel[chl]=="mmmt" or channel[chl]=="eemt") : scaleNameUp = "_CMS_scale_t_llmt"+CoMEnergy+"Up" ; scaleNameDown = "_CMS_scale_t_llmt"+CoMEnergy+"Down"
        if (channel[chl]=="mmet" or channel[chl]=="eeet") : scaleNameUp = "_CMS_scale_t_llet"+CoMEnergy+"Up" ; scaleNameDown = "_CMS_scale_t_llet"+CoMEnergy+"Down"
        if (channel[chl]=="mmme" or channel[chl]=="eeem") : scaleNameUp = "_CMS_scale_t_llem"+CoMEnergy+"Up" ; scaleNameDown = "_CMS_scale_t_llem"+CoMEnergy+"Down"
        MMM= myOut.mkdir(str(channel[chl])+'_zh')
        MMM.cd()
        ###################################### Filling Signal ZH and WH ########
        for sig in range(len(signal)):
            for m in range(len(mass)):#    for m in range(110, 145, 5):
                ################################################
                #Norm
                MMM.cd()
                NewHist =TH1F(str(signalname[sig]) + str(mass[m]),"",n_bin,low_bin,high_bin)
                

                for bb in range(0,n_bin):
                    NewHist.SetBinContent(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl], histoName, "",CoMEnergy)[0])
                    NewHist.SetBinError(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl],histoName, "",CoMEnergy)[1])

                normal = Table_Hist.GetBinContent(chl + 1, sig * len(mass) + m + 1)
                if (HWWasSignal and sig==1): normal = Table_Hist.GetBinContent(chl + 1, sig * len(mass) + 7 + 1) #7 is due to 125 GeV
                if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
                ################################################
                #ScaleUp
                MMM.cd()
                NewHistUp =TH1F(str(signalname[sig]) + str(mass[m])+scaleNameUp,"",n_bin,low_bin,high_bin)
                for bb in range(0,n_bin):
                    NewHistUp.SetBinContent(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl], histoName, "_Up",CoMEnergy)[0])
                    NewHistUp.SetBinError(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl],histoName, "_Up",CoMEnergy)[1])

                normalUp = Table_HistUp.GetBinContent(chl + 1, sig * len(mass) + m + 1)
                if (HWWasSignal and sig==1): normalUp = Table_HistUp.GetBinContent(chl + 1, sig * len(mass) + 7 + 1) #7 is due to 125 GeV
                if NewHistUp.Integral(): NewHistUp.Scale(normalUp/NewHistUp.Integral())
                ################################################
                #ScaleDown
                MMM.cd()
                NewHistDown =TH1F(str(signalname[sig]) + str(mass[m])+scaleNameDown,"",n_bin,low_bin,high_bin)
                for bb in range(0,n_bin):
                    NewHistDown.SetBinContent(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl], histoName, "_Down",CoMEnergy)[0])
                    NewHistDown.SetBinError(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl],histoName, "_Down",CoMEnergy)[1])

                normalDown = Table_HistDown.GetBinContent(chl + 1, sig * len(mass) + m + 1)
                if (HWWasSignal and sig==1): normalDown = Table_HistDown.GetBinContent(chl + 1, sig * len(mass) + 7 + 1) #7 is due to 125 GeV
                if NewHistDown.Integral(): NewHistDown.Scale(normalDown/NewHistDown.Integral())
                myOut.Write()

        ###################################### Filling Reducible ########
        MMM.cd()
        NewHist =TH1F('Zjets',"",n_bin,low_bin,high_bin)
        myfile = TFile(DIR_ROOT + 'Data'+CoMEnergy+'.root')
#        Histo =  myfile.Get("VisibleMass_Shape_"+ channel[chl])  ## BUG found in 21 July
        Histo =  myfile.Get(histoName+"Shape_"+ channel[chl])
        Histo.Rebin(reb_)
        for bb in range(0,n_bin):
            NewHist.SetBinContent(bb,Histo.GetBinContent(bb))
            NewHist.SetBinError(bb,Histo.GetBinError(bb))
        normal = Table_Hist.GetBinContent(chl + 1, lenghtSig  + 2)
        NewHist.Scale(normal/NewHist.Integral())
        myOut.Write()
        ###################################### Filling ZZ and Data ########
        for bg in range (len(BackGround)):
            #  Norm   #####################################
            MMM.cd()
            NewHist =TH1F(BackGroundname[bg],"",n_bin,low_bin,high_bin)
            for bb in range(0,n_bin):
                    NewHist.SetBinContent(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl], histoName, "",CoMEnergy)[0])
                    NewHist.SetBinError(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl],histoName, "",CoMEnergy)[1])

            normal = Table_Hist.GetBinContent(chl + 1, lenghtSig + bg + 3)
            if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
            #  ScaleUp   #####################################
            MMM.cd()
            NewHistUp =TH1F(BackGroundname[bg]+scaleNameUp,"",n_bin,low_bin,high_bin)
            for bb in range(0,n_bin):
                    NewHistUp.SetBinContent(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl], histoName, "_Up",CoMEnergy)[0])
                    NewHistUp.SetBinError(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl],histoName, "_Up",CoMEnergy)[1])

            normalUp = Table_HistUp.GetBinContent(chl + 1, lenghtSig + bg + 3)
            if NewHistUp.Integral(): NewHistUp.Scale(normalUp/NewHistUp.Integral())
            #  ScaleDown   #####################################
            MMM.cd()
            NewHistDown =TH1F(BackGroundname[bg]+scaleNameDown,"",n_bin,low_bin,high_bin)
            for bb in range(0,n_bin):
                    NewHistDown.SetBinContent(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl], histoName, "_Down",CoMEnergy)[0])
                    NewHistDown.SetBinError(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl],histoName, "_Down",CoMEnergy)[1])

            normalDown = Table_HistDown.GetBinContent(chl + 1, lenghtSig + bg + 3)
            if NewHistDown.Integral(): NewHistDown.Scale(normalDown/NewHistDown.Integral())
            myOut.Write()

            
if __name__ == "__main__":
    MakeTheHistogram("SVMass_","_8TeV")
    MakeTheHistogram("VisibleMass_","_8TeV")
    MakeTheHistogram("SVMass_","_7TeV")
    MakeTheHistogram("VisibleMass_","_7TeV")
            