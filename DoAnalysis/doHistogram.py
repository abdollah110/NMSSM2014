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
SubRootDir = 'OutFiles/'

n_bin = 50
low_bin = 0
high_bin = 1000
reb_ = high_bin / n_bin
DIR_ROOT = 'outRoot_V3/'

signal = ['ggh', 'bbh']
mass = [80,90,  100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]
W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']
Top_BackGround = ['TTJets_FullLeptMGDecays','TTJets_SemiLeptMGDecays',  'TTJets_HadronicMGDecays', 'Tbar_tW', 'T_tW']
DiBoson_BackGround = [ 'WWJetsTo2L2Nu',  'WZJetsTo2L2Q', 'WZJetsTo3LNu',  'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']
Embedded = ['EmbeddedMuTau', 'EmbeddedETau']
Data = ['Data']






lenghtSig = len(signal) * len(mass)
#Histogram = "VisibleMass_"
#category_ = ["_inclusive"]
category_ = ["_inclusive", "_nobtag", "_btag"]
#channel = ["MuTau", "ETau"]
channel = ["MuTau"]
lenghtSig = len(signal) * len(mass) +1
lenghtVV = len(DiBoson_BackGround) +1
lenghtTop = len(Top_BackGround) +1
lenghtZL = len(Z_BackGround) + 1
lenghtZJ = len(Z_BackGround) + 1
lenghtZTT = len(Z_BackGround) + 1
low_bin = 0
high_bin = 1000
digit = 3
verbos_ = True
QCDScaleFactor = 1.06


def _Return_Value_Signal(bb,Name, channel,cat,Histo,PostFix,CoMEnergy,changeHistoName ):
    myfile = TFile(SubRootDir + "out_"+ Name +CoMEnergy+ '.root')
    if cat=="_btag" and changeHistoName : cat = "_btagLoose" #; print "___________+++++++++++++++++++", str(channel)+str(Histo) + str(cat)
    Histo =  myfile.Get(channel+Histo + cat+ PostFix)
#    print "___________+++++++++++++++++++", str(channel)+str(Histo) + str(cat)
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
def MakeTheHistogram(channel,CoMEnergy,chl):
    myOut = TFile("TotalRootForLimit_"+channel + CoMEnergy+".root" , 'RECREATE')
    Table_File = TFile("Yield"+CoMEnergy+""+".root")
    Table_Hist = Table_File.Get('FullResults')
#    #ScaleUp
#    Table_FileUp = TFile("Yield"+CoMEnergy+"_Up"+".root")
#    Table_HistUp = Table_FileUp.Get('FullResults')
#    #ScaleDown
#    Table_FileDown = TFile("Yield"+CoMEnergy+"_Down"+".root")
#    Table_HistDown = Table_FileDown.Get('FullResults')

    

    
    categ=-1
    for category in category_:
        categ =categ +1
        print "starting category and channel", category, channel
#        tDirectory= myOut.mkdir(channel + str(category))
#        myOut.rmdir(channel + str(category))
        tDirectory= myOut.mkdir(channel + str(category))
        tDirectory.cd()
        ###################################### Filling Signal ZH and WH ########
        for sig in range(len(signal)):
            for m in range(len(mass)):#    for m in range(110, 145, 5):
                ################################################
                #Norm
                tDirectory.cd()

                Histogram = "_visibleMass_mTLess30_OS"
                XLoc= categ + 3*chl + 1
                YLoc= sig * len(mass) + m + 1
                normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization

                Name= str(signal[sig]) + "_"+str(mass[m])
                NewHist =TH1F(Name,"",n_bin,low_bin,high_bin)

                for bb in range(0,n_bin):
                    NewHist.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,False)[0])
                    NewHist.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,False)[1])

                print "NOrmal is= " , normal
                if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
                myOut.Write()
                ################################################
                #  Filling VV
                ################################################
        tDirectory.cd()
        NewHist2 =TH1F("VV","",n_bin,low_bin,high_bin)
        print "Doing VV BG estimation"
        for BG_VV in range(len(DiBoson_BackGround)):

            Histogram = "_visibleMass_mTLess30_OS"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + BG_VV + 1
            normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= str(DiBoson_BackGround[BG_VV])
            NewHist =TH1F(Name,"",n_bin,low_bin,high_bin)

            for bb in range(0,n_bin):
                NewHist.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,False)[0])
                NewHist.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,False)[1])

            print "NOrmal is= " , normal
            if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
            NewHist2.Add(NewHist)

        myOut.Write()
            ################################################
            #  Filling Top
            ################################################
        tDirectory.cd()
        NewHist2 =TH1F("TT","",n_bin,low_bin,high_bin)
        print "Doing TOP and Single BG estimation"
        for BG_T in range(len(Top_BackGround)):

            Histogram = "_visibleMass_mTLess30_OS"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + lenghtVV+  BG_T + 1
            normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= str(DiBoson_BackGround[BG_T])
            NewHist =TH1F(Name,"",n_bin,low_bin,high_bin)

            for bb in range(0,n_bin):
                NewHist.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,False)[0])
                NewHist.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,False)[1])

            print "NOrmal is= " , normal
            if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
            NewHist2.Add(NewHist)

        myOut.Write()

            ################################################
            #  Filling ZL
            ################################################
        print "Doing ZL, BG estimation"
        tDirectory.cd()
        NewHist2 =TH1F("ZL","",n_bin,low_bin,high_bin)
        for BG_ZL in range(len(Z_BackGround)):

            Histogram = "_visibleMass_mTLess30_OS_ZL"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + lenghtVV+ lenghtTop +BG_ZL +1
            normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= str(Z_BackGround[BG_ZL])

            NewHist =TH1F(Name,"",n_bin,low_bin,high_bin)

            for bb in range(0,n_bin):
                NewHist.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
                NewHist.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

            print "NOrmal is= " , normal
            if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
            NewHist2.Add(NewHist)

        myOut.Write()

        #######################################  Filling Reducible BG ##########
        print "Doing ZJ, BG estimation"
        tDirectory.cd()
        NewHist2 =TH1F("ZJ","",n_bin,low_bin,high_bin)
        for BG_ZJ in range(len(Z_BackGround)):

            Histogram = "_visibleMass_mTLess30_OS_ZJ"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + lenghtVV+ lenghtTop +lenghtZL+ BG_ZJ+ 1
            normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= str(Z_BackGround[BG_ZJ])

            NewHist =TH1F(Name,"",n_bin,low_bin,high_bin)

            for bb in range(0,n_bin):
                NewHist.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
                NewHist.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

            print "NOrmal is= " , normal
            if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
            NewHist2.Add(NewHist)

        myOut.Write()



        #        #######################################  Filling Reducible BG ##########
        print "Doing ZTT, BG estimation"
        tDirectory.cd()
        NewHist2 =TH1F("ZTT","",n_bin,low_bin,high_bin)
        for BG_ZTT in range(len(Z_BackGround)):

            Histogram = "_visibleMass_mTLess30_OS"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + lenghtVV+ lenghtTop +lenghtZL+ lenghtZJ+ BG_ZTT+ 1
            normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
            Name= "EmbeddedMuTau"

            NewHist =TH1F(Name+str(BG_ZTT),"",n_bin,low_bin,high_bin)

            for bb in range(0,n_bin):
                NewHist.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,False)[0])
                NewHist.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,False)[1])

            print "NOrmal is= " , normal
            if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
            NewHist2.Add(NewHist)

        myOut.Write()
        #        #######################################  Filling Reducible BG ##########
        #        #######################################  Filling Reducible BG ##########
        print "Doing W, BG estimation"
        tDirectory.cd()
        NewHist2 =TH1F("W","",n_bin,low_bin,high_bin)
        Histogram = "_visibleMass_mTLess30_OS"
        XLoc= categ + 3*chl + 1
        YLoc= lenghtSig + lenghtVV+ lenghtTop +lenghtZL+lenghtZJ+lenghtZTT+0+1
        normal = Table_Hist.GetBinContent(XLoc,YLoc)    #Get the Noralization
        Name='WJetsToLNu'

        NewHist =TH1F(Name,"",n_bin,low_bin,high_bin)

        for bb in range(0,n_bin):
            NewHist.SetBinContent(bb,_Return_Value_Signal(bb,Name, channel,category, Histogram, "",CoMEnergy,True)[0])
            NewHist.SetBinError(bb,_Return_Value_Signal(bb,Name, channel,category,Histogram, "",CoMEnergy,True)[1])

        print "NOrmal is= " , normal
        if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
        NewHist2.Add(NewHist)

        myOut.Write()
        #        #######################################  Filling Reducible BG ##########



            

#            value = getHistoNorm(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0] * XSection(Name, CoMEnergy)
#            FullResults.SetBinContent(XLoc,YLoc , value)
#            FullResults.GetYaxis().SetBinLabel(YLoc, Name)
#
#            valueEr = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * XSection(Name, CoMEnergy)
#            FullError.SetBinContent(XLoc , YLoc, valueEr)
#            FullError.GetYaxis().SetBinLabel(YLoc, Name)
#            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr
#            VV_NormForWSub += getHistoNorm(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorm)[0] * XSection(Name, CoMEnergy)
#            VV_NormForWforQCD += getHistoNorm(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorminQCD)[0] * XSection(Name, CoMEnergy)
#            VV_NormForQCD += getHistoNorm(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCD)[0] * XSection(Name, CoMEnergy)
#                #ScaleUp
#                MMM.cd()
#                NewHistUp =TH1F(str(signalname[sig]) + str(mass[m])+scaleNameUp,"",n_bin,low_bin,high_bin)
#                for bb in range(0,n_bin):
#                    NewHistUp.SetBinContent(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl], histoName, "_Up",CoMEnergy)[0])
#                    NewHistUp.SetBinError(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl],histoName, "_Up",CoMEnergy)[1])
#
#                normalUp = Table_HistUp.GetBinContent(chl + 1, sig * len(mass) + m + 1)
#                if (HWWasSignal and sig==1): normalUp = Table_HistUp.GetBinContent(chl + 1, sig * len(mass) + 7 + 1) #7 is due to 125 GeV
#                if NewHistUp.Integral(): NewHistUp.Scale(normalUp/NewHistUp.Integral())
#                ################################################
#                #ScaleDown
#                MMM.cd()
#                NewHistDown =TH1F(str(signalname[sig]) + str(mass[m])+scaleNameDown,"",n_bin,low_bin,high_bin)
#                for bb in range(0,n_bin):
#                    NewHistDown.SetBinContent(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl], histoName, "_Down",CoMEnergy)[0])
#                    NewHistDown.SetBinError(bb,_Return_Value_Signal(bb,signal[sig], mass[m], channel[chl],histoName, "_Down",CoMEnergy)[1])
#
#                normalDown = Table_HistDown.GetBinContent(chl + 1, sig * len(mass) + m + 1)
#                if (HWWasSignal and sig==1): normalDown = Table_HistDown.GetBinContent(chl + 1, sig * len(mass) + 7 + 1) #7 is due to 125 GeV
#                if NewHistDown.Integral(): NewHistDown.Scale(normalDown/NewHistDown.Integral())
#                myOut.Write()

#        ###################################### Filling Reducible ########
#        MMM.cd()
#        NewHist =TH1F('Zjets',"",n_bin,low_bin,high_bin)
#        myfile = TFile(DIR_ROOT + 'Data'+CoMEnergy+'.root')
##        Histo =  myfile.Get("VisibleMass_Shape_"+ channel[chl])  ## BUG found in 21 July
#        Histo =  myfile.Get(histoName+"Shape_"+ channel[chl])
#        Histo.Rebin(reb_)
#        for bb in range(0,n_bin):
#            NewHist.SetBinContent(bb,Histo.GetBinContent(bb))
#            NewHist.SetBinError(bb,Histo.GetBinError(bb))
#        normal = Table_Hist.GetBinContent(chl + 1, lenghtSig  + 2)
#        NewHist.Scale(normal/NewHist.Integral())
#        myOut.Write()
#        ###################################### Filling ZZ and Data ########
#        for bg in range (len(BackGround)):
#            #  Norm   #####################################
#            MMM.cd()
#            NewHist =TH1F(BackGroundname[bg],"",n_bin,low_bin,high_bin)
#            for bb in range(0,n_bin):
#                    NewHist.SetBinContent(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl], histoName, "",CoMEnergy,True)[0])
#                    NewHist.SetBinError(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl],histoName, "",CoMEnergy,True)[1])
#
#            normal = Table_Hist.GetBinContent(chl + 1, lenghtSig + bg + 3)
#            if NewHist.Integral(): NewHist.Scale(normal/NewHist.Integral())
#            #  ScaleUp   #####################################
#            MMM.cd()
#            NewHistUp =TH1F(BackGroundname[bg]+scaleNameUp,"",n_bin,low_bin,high_bin)
#            for bb in range(0,n_bin):
#                    NewHistUp.SetBinContent(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl], histoName, "_Up",CoMEnergy)[0])
#                    NewHistUp.SetBinError(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl],histoName, "_Up",CoMEnergy)[1])
#
#            normalUp = Table_HistUp.GetBinContent(chl + 1, lenghtSig + bg + 3)
#            if NewHistUp.Integral(): NewHistUp.Scale(normalUp/NewHistUp.Integral())
#            #  ScaleDown   #####################################
#            MMM.cd()
#            NewHistDown =TH1F(BackGroundname[bg]+scaleNameDown,"",n_bin,low_bin,high_bin)
#            for bb in range(0,n_bin):
#                    NewHistDown.SetBinContent(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl], histoName, "_Down",CoMEnergy)[0])
#                    NewHistDown.SetBinError(bb,_Return_Value_BG(bb,BackGround[bg], channel[chl],histoName, "_Down",CoMEnergy)[1])
#
#            normalDown = Table_HistDown.GetBinContent(chl + 1, lenghtSig + bg + 3)
#            if NewHistDown.Integral(): NewHistDown.Scale(normalDown/NewHistDown.Integral())
#            myOut.Write()

            
if __name__ == "__main__":

    MakeTheHistogram("MuTau","_8TeV",0)
#    MakeTheHistogram("_inclusive","MuTau","_8TeV",0,0)
#    MakeTheHistogram("_nobtag","MuTau","_8TeV",1,0)
#    MakeTheHistogram("_btag","MuTau","_8TeV",2,0)
#    MakeTheHistogram("VisibleMass_","_8TeV")
#    MakeTheHistogram("SVMass_","_7TeV")
#    MakeTheHistogram("VisibleMass_","_7TeV")
            