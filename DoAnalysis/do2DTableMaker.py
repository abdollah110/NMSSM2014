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
from ROOT import gStyle
from ROOT import gSystem

gROOT.Reset()
import os
ROOT.gROOT.SetBatch(True)
InputFileLocation = '../FileROOT/NewROOTFiles/'
#OriginRootDir = '../ROOTFiles_V3/ROOTDebugScale_7TeV/'
SubRootDir = 'OutFiles/'

def luminosity(CoMEnergy):
    if CoMEnergy == '_8TeV': return  19712 #19242
    if CoMEnergy == '_7TeV': return  4982

def XSection(mX, CoMEnergy):
    if CoMEnergy == '_8TeV':
        if mX == 90:  return [0.009186, 0.0001740]  # first argument is ZH_HTT and second is WH_ZH_TTH_HWW_Leptonic
        if mX == 95:  return [0.007884, 0.0003351]
        if mX == 100: return [0.006775, 0.0006694]
        if mX == 105: return [0.005794, 0.001268]
        if mX == 110: return [0.004918, 0.002178]    #  [.00471, .00206]  ????
        if mX == 115: return [0.004102, 0.003407]  # [.00392, .00336]
        if mX == 120: return [0.003349, 0.004866] # [.00319, .00479]
        if mX == 125: return [0.002651, 0.006503]  # [.00251, .00640]
        if mX == 130: return [0.002021, 0.008042]  #[.00191, .00789]
        if mX == 135: return [0.001478, 0.009342]  #[.00139, .00917]
        if mX == 140: return [0.001030, 0.01032]  #[.00097, .0102] # changed 0.0101 to 0.0102
        if mX == 145: return [0.000681, 0.01095]   #[.00064, .0107] # Changed from 0.0140 to 0.0107
        if mX == 150: return [0.000417, 0.01126]
        if mX == 155: return [0.000218, 0.01136]
        if mX == 160: return [0.000073, 0.01133]
        if mX == 'ZZ4L':        return 0.130
        if mX == 'Data':        return 1
        if mX == 'TT2L2Nu':     return 23.64
        if mX == 'GGToZZ2L2L':  return 0.01203
        if mX == 'GGToZZ4L':    return 0.0048
        if mX == 'TTZJets':     return 0.208
        if mX == 'WZ3L':        return 0
        if mX == 'WZJets3L':    return 1.057
        if mX == 'DYJets':      return 3504.

        if mX == 'signal':      return 1.

        if mX == 'WWJetsTo2L2Nu': return 5.824
        if mX == 'WZJetsTo2L2Q':      return 2.207 
        if mX == 'WZJetsTo3LNu':      return 1.058 
        if mX == 'ZZJetsTo2L2Nu':      return 0.716 
        if mX == 'ZZJetsTo2L2Q':      return 2.502 
        if mX == 'ZZJetsTo4L':      return  	0.181

        if mX == 'TTJets_FullLeptMGDecays':      return 26.1975
        if mX == 'TTJets_SemiLeptMGDecays':      return 109.281
        if mX == 'TTJets_HadronicMGDecays':      return 114.0215
        if mX == 'Tbar_tW':      return 11.1
        if mX == 'T_tW':      return 11.1

#        if mX == 'DYJetsToLL':  return 0.242 * 0.001
#        if mX == 'DY1JetsToLL':  return  0.128* 0.001
#        if mX == 'DY2JetsToLL':  return 0.0914* 0.001
#        if mX == 'DY3JetsToLL':  return 0.0109* 0.001
#        if mX == 'DY4JetsToLL':  return  0.00540* 0.001
#
#        if mX == 'WJetsToLNu':  return 6.149* 0.001
#        if mX == 'W1JetsToLNu':  return  0.306* 0.001
#        if mX == 'W2JetsToLNu':  return 0.153* 0.001
#        if mX == 'W3JetsToLNu':  return 0.0436* 0.001
#        if mX == 'W4JetsToLNu':  return  0.0597* 0.001



signal = ['ggh', 'bbh']
mass = [80,90,  100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]
W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']
Top_BackGround = ['TTJets_FullLeptMGDecays','TTJets_SemiLeptMGDecays',  'TTJets_HadronicMGDecays', 'Tbar_tW', 'T_tW']
#Top_BackGround = ['TTJets_FullLeptMGDecays', 'TTJets_HadronicMGDecays', 'TTJets_MassiveBinDECAY', 'TTJets_SemiLeptMGDecays']
#SingleTop_BackGround = ['Tbar_s', 'Tbar_t', 'Tbar_tW', 'T_s', 'T_t', 'T_tW']
DiBoson_BackGround = [ 'WWJetsTo2L2Nu',  'WZJetsTo2L2Q', 'WZJetsTo3LNu',  'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
#DiBoson_BackGround = ['WW', 'WWJetsTo2L2Nu', 'WZ', 'WZJetsTo2L2Q', 'WZJetsTo3LNu', 'ZZ', 'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']
Embedded = ['EmbeddedmuTau', 'EmbeddedeleTau']
DYJets = ['DYJetsAll']
WJets = ['WJetsAll']
Data = ['Data']




#Histogram = "VisibleMass_"
#category = ["_inclusive"]
category = ["_inclusive", "_nobtag", "_btag"]
channel = ["muTau", "eleTau"]
#channel = ["MuTau"]
lenghtSig = len(signal) * len(mass) +1
lenghtVV = len(DiBoson_BackGround) +1
lenghtTop = len(Top_BackGround) +1
lenghtZL = len(Z_BackGround) + 1
lenghtZJ = len(Z_BackGround) + 1
lenghtZTT = len(Z_BackGround) + 1
low_bin = 0
high_bin = 15000
digit = 3
verbos_ = True
QCDScaleFactor = 1.06


def getHistoNorm(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfile = TFile(InputFileLocation + Name +CoMEnergy+ '.root')
    HistoDenum = myfile.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral(low_bin,high_bin) * luminosity(CoMEnergy) / HistoDenum.GetBinContent(1)
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral(low_bin,high_bin)) * luminosity(CoMEnergy)  / HistoDenum.GetBinContent(1)
        valueEr = round(valueEr, digit)
    return value, valueEr

def getHistoNorm_BG(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral(low_bin,high_bin) * luminosity(CoMEnergy) 
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral(low_bin,high_bin)) * luminosity(CoMEnergy) 
        valueEr = round(valueEr, digit)
    return value, valueEr

def getEmbeddedWeight(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name+chan +CoMEnergy+ '.root') #need chan due to embedded name include MuTau
    HistoInclusive = myfileSub.Get(chan+Histogram+ "_inclusive"+PostFix )
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = HistoSub.Integral(low_bin,high_bin)/ HistoInclusive.Integral(low_bin,high_bin)
    return value

def getWExtraPol(PostFix,CoMEnergy,Name,chan,cat,HistogramNum,HistogramDenum ):
    myfileSub = TFile(SubRootDir + "out_"+Name+CoMEnergy+ '.root')
    if cat=="_btag": cat = "_btagLoose" 
    HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
    HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )
    if not HistoNum or not HistoDenum:  #FIXME   I should find why WJets do not have statics for btag or no 
        cat = "_inclusive"
        HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
        HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )

    value = HistoNum.Integral(low_bin,high_bin)/ HistoDenum.Integral(low_bin,high_bin)
    return value

def getHistoIntegral(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    valueEr = 10E-7
    if (HistoSub):
        value = HistoSub.Integral(low_bin,high_bin)
        value = round(value, digit)
        valueEr = math.sqrt(HistoSub.Integral(low_bin,high_bin))
        valueEr = round(valueEr, digit)
    return value, valueEr

    

def make2DTable(Observable,PostFix,CoMEnergy):
    myOut = TFile("Yield"+CoMEnergy+PostFix+".root", 'RECREATE')
    FullResults  = TH2F('FullResults', 'FullResults', 15, 0, 15, 80, 0, 80)
    FullError  = TH2F('FullError', 'FullError', 15, 0, 15, 80, 0, 80)

    for categ in range(len(category)):
        for chl in range(len(channel)):
            print "starting category and channel", category[categ], channel[chl]
            ##################################################################################################
            #   Signal Estimation
            ##################################################################################################
            for sig in range(len(signal)):
                for m in range(len(mass)):#    for m in range(110, 145, 5):

                    Histogram = Observable+"_mTLess30_OS"
                    XLoc= categ + 3*chl + 1
                    YLoc= sig * len(mass) + m + 1
                    Name= str(signal[sig]) + "_"+str(mass[m])

                    value = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0] * XSection("signal", CoMEnergy)
                    FullResults.SetBinContent(XLoc,YLoc , value)
                    FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                    valueEr = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * XSection("signal", CoMEnergy)
                    FullError.SetBinContent(XLoc , YLoc, valueEr)
                    FullError.GetYaxis().SetBinLabel(YLoc, Name)
                    if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr
             ##################################################################################################
            #   VV Estimation
            ##################################################################################################
            print "Doing VV, BG estimation"

            DYIndex = ""
            Name= "VVAll"
            YLoc= lenghtSig +1
            ## Similar To ALL ##
            XLoc= categ + 3*chl + 1
            XLocQCD= categ + 3*(chl+2) + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistogramForWNorm = Observable+"_mTHigher70_OS"+DYIndex
            HistogramForWNorminQCD = Observable+"_mTHigher70_SS"+DYIndex
            HistogramForQCDNorm = Observable+"_mTLess30_SS"+DYIndex
            HistogramForQCDShape = Observable+"_QCDshape_SS"+DYIndex

            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, Name)
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            VV_NormForWSub = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorm)[0]
            VV_NormForWforQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorminQCD)[0]
            VV_NormForQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDNorm)[0]
            VV_NormForQCDShape = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDShape)[0]
            FullResults.SetBinContent(XLocQCD,YLoc , VV_NormForQCDShape)


            ##################################################################################################
            #   TT Estimation
            ##################################################################################################
            print "Doing TT, BG estimation"

            DYIndex = ""
            Name= "TTAll"
            YLoc= lenghtSig  +2
            ## Similar To ALL ##
            XLoc= categ + 3*chl + 1
            XLocQCD= categ + 3*(chl+2) + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistogramForWNorm = Observable+"_mTHigher70_OS"+DYIndex
            HistogramForWNorminQCD = Observable+"_mTHigher70_SS"+DYIndex
            HistogramForQCDNorm = Observable+"_mTLess30_SS"+DYIndex
            HistogramForQCDShape = Observable+"_QCDshape_SS"+DYIndex

            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, Name)
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            TT_NormForWSub = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorm)[0]
            TT_NormForWforQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorminQCD)[0]
            TT_NormForQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDNorm)[0]
            TT_NormForQCDShape = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDShape)[0]
            FullResults.SetBinContent(XLocQCD,YLoc , TT_NormForQCDShape)

            ##################################################################################################
            #   ZL Estimation
            ##################################################################################################
            print "Doing ZL, BG estimation"
#            for BG_ZL in range(len(Z_BackGround)):
                
            DYIndex = "_ZL"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 3
            ## Similar To ALL ##
            XLoc= categ + 3*chl + 1
            XLocQCD= categ + 3*(chl+2) + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistogramForWNorm = Observable+"_mTHigher70_OS"+DYIndex
            HistogramForWNorminQCD = Observable+"_mTHigher70_SS"+DYIndex
            HistogramForQCDNorm = Observable+"_mTLess30_SS"+DYIndex
            HistogramForQCDShape = Observable+"_QCDshape_SS"+DYIndex


            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, "ZL")
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            ZL_NormForWSub = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorm)[0]
            ZL_NormForWforQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorminQCD)[0]
            ZL_NormForQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDNorm)[0]
            ZL_NormForQCDShape = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDShape)[0]
            FullResults.SetBinContent(XLocQCD,YLoc , ZL_NormForQCDShape)
            ##################################################################################################
            #   ZJ Estimation
            #################################################################################################
            print "Doing ZJ, BG estimation"

            DYIndex = "_ZJ"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 4
            ## Similar To ALL ##
            XLoc= categ + 3*chl + 1
            XLocQCD= categ + 3*(chl+2) + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistogramForWNorm = Observable+"_mTHigher70_OS"+DYIndex
            HistogramForWNorminQCD = Observable+"_mTHigher70_SS"+DYIndex
            HistogramForQCDNorm = Observable+"_mTLess30_SS"+DYIndex
            HistogramForQCDShape = Observable+"_QCDshape_SS"+DYIndex


            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, "ZJ")
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            ZJ_NormForWSub = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorm)[0]
            ZJ_NormForWforQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorminQCD)[0]
            ZJ_NormForQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDNorm)[0]
            ZJ_NormForQCDShape = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDShape)[0]
            FullResults.SetBinContent(XLocQCD,YLoc , ZJ_NormForQCDShape)
        ##################################################################################################
        #   ZTT Estimation
        ##################################################################################################
            print "Doing ZTT, BG estimation"
#            for BG_ZTT in range(len(Z_BackGround)):

            DYIndex = "_ZTT"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 5
            ## Similar To ALL ##
            XLoc= categ + 3*chl + 1
            XLocQCD= categ + 3*(chl+2) + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistogramForWNorm = Observable+"_mTHigher70_OS"+DYIndex
            HistogramForWNorminQCD = Observable+"_mTHigher70_SS"+DYIndex
            HistogramForQCDNorm = Observable+"_mTLess30_SS"+DYIndex
            HistogramForQCDShape = Observable+"_QCDshape_SS"+DYIndex

            EmbedEff = getEmbeddedWeight(PostFix,CoMEnergy, "Embedded",channel[chl],category[categ],Observable+"_mTLess30_OS")
            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],"_inclusive",Histogram)[0]  * EmbedEff
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],"_inclusive",Histogram)[1]  * EmbedEff
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, "ZTT")
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr, "  embedEff=",EmbedEff

            ZTT_NormForWSub = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorm)[0]
            ZTT_NormForWforQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForWNorminQCD)[0]
            ZTT_NormForQCD = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDNorm)[0]
            ZTT_NormForQCDShape = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistogramForQCDShape)[0]
            FullResults.SetBinContent(XLocQCD,YLoc , ZTT_NormForQCDShape)
        ##################################################################################################
        #   W Estimation
        ##################################################################################################
            print "Doing ExtraPolationFactor for W estimation"

            numeratorW=Observable+"_mTLess30_OS"
            denumeratorW=Observable+"_mTHigher70_OS"
            Histogram = Observable+"_mTHigher70_OS"
            W_mcName= "WJetsAll"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + 6
            Name='Data'

            ExtraPolationFactorFinal = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorW,denumeratorW)
            WNormInSideBandData=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
            value =(WNormInSideBandData - (VV_NormForWSub + TT_NormForWSub +ZL_NormForWSub + ZJ_NormForWSub + ZTT_NormForWSub )) * ExtraPolationFactorFinal

            print "WNormInSideBandData= ", WNormInSideBandData
            print "Final W Value=", value
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, "W")
            


        ##################################################################################################
        #   W Estimation for QCD Normalization
        ##################################################################################################
            print "Doing ExtraPolationFactor for W estimation for QCD Normalization"

            numeratorW=Observable+"_mTLess30_SS"
            denumeratorW=Observable+"_mTHigher70_SS"
            Histogram = Observable+"_mTHigher70_SS"
            W_mcName= "WJetsAll"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + 6
            Name='Data'

            ExtraPolationFactorFinal = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorW,denumeratorW)
            WNormInSideBandDataForQCD=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
            WNormInQCD =(WNormInSideBandDataForQCD - (VV_NormForWforQCD + TT_NormForWforQCD +ZL_NormForWforQCD + ZJ_NormForWforQCD + ZTT_NormForWforQCD )) * ExtraPolationFactorFinal

        ##################################################################################################
        #   W Estimation for QCD Shape
        ##################################################################################################
            print "Doing ExtraPolationFactor for W estimation for QCD Shape"

            numeratorW=Observable+"_mTLess30_QCDshape_OS"
            denumeratorW=Observable+"_mTHigher70_QCDshape_OS"
#            numeratorW=Observable+"_mTLess30_SS"
#            denumeratorW=Observable+"_mTHigher70_SS"
            Histogram = Observable+"_QCDshape_SS"
            W_mcName= "WJetsAll"
            XLoc= categ + 3*chl + 1
            XLocQCD= categ + 3*(chl+2) + 1
            YLoc= lenghtSig + 6
            Name='Data'

            ExtraPolationFactorFinal = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorW,denumeratorW)
            WNormInSideBandDataForQCDShape=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
            W_NormForQCDShape =(WNormInSideBandDataForQCDShape - (VV_NormForQCDShape + TT_NormForQCDShape +ZL_NormForQCDShape + ZJ_NormForQCDShape + ZTT_NormForQCDShape )) * ExtraPolationFactorFinal
            FullResults.SetBinContent(XLocQCD,YLoc , W_NormForQCDShape)

        ##################################################################################################
        #   QCD Estimation
        ##################################################################################################
            print "Starting QCD  estimation"
            Histogram = Observable+"_mTLess30_SS"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + 7
            Name='Data'
            QCDNormBare=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
            print "WNormInSideBandData= ", WNormInSideBandData
            FinalQCDNorm =(QCDNormBare - (VV_NormForQCD + TT_NormForQCD +ZL_NormForQCD + ZJ_NormForQCD + ZTT_NormForQCD + WNormInQCD)) * QCDScaleFactor
            print "FinalQCDNorm =", FinalQCDNorm
            FullResults.SetBinContent(XLoc,YLoc , FinalQCDNorm)
            FullResults.GetYaxis().SetBinLabel(YLoc, "QCD")

        ##################################################################################################
        #   Data Estimation
        ##################################################################################################
            print "Starting Data  estimation"
            Histogram = Observable+"_mTLess30_OS"
            XLoc= categ + 3*chl + 1
            YLoc= lenghtSig + 8
            Name='Data'
            DataNorm=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
            print "Data= ", DataNorm
            FullResults.SetBinContent(XLoc,YLoc , DataNorm)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)
            
            


#        ########################################################################
            FullResults.GetXaxis().SetBinLabel(categ + 3*chl + 1, channel[chl]+category[categ])
            FullError.GetXaxis().SetBinLabel(categ + 3*chl + 1,  channel[chl]+category[categ])
    myOut.Write()
    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResults.Draw('text')
    myCanvas.SaveAs("tableAll"+PostFix+CoMEnergy+".pdf")
    myCanvasEr = TCanvas()
    gStyle.SetOptStat(0)
    FullError.Draw('text')
    myCanvasEr.SaveAs("ErrorAll"+PostFix+CoMEnergy+".pdf")


if __name__ == "__main__":
#    make2DTable("_visibleMass","", "_8TeV")
    make2DTable("_SVMass","", "_8TeV")
#    make2DTable("_Up", "_8TeV")
#    make2DTable("_Down", "_8TeV")

#            ExPolFactorW_ = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[0]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW1 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[1]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW2 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[2]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW3 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[3]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW4 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[4]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#
#            Histogram = Observable+"_mTHigher70_OS"
#            WeightedEventsW_= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[0]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[0]), CoMEnergy)
#            WeightedEventsW1= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[1]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[1]), CoMEnergy)
#            WeightedEventsW2= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[2]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[2]), CoMEnergy)
#            WeightedEventsW3= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[3]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[3]), CoMEnergy)
#            WeightedEventsW4= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[4]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[4]), CoMEnergy)
#
#            ExtraPolationFactorNum = ExPolFactorW_ * WeightedEventsW_ + ExPolFactorW1 * WeightedEventsW1 + ExPolFactorW2 * WeightedEventsW2 + ExPolFactorW3 * WeightedEventsW3 + ExPolFactorW4 * WeightedEventsW4
#            ExtraPolationFactorDenum =  WeightedEventsW_ + WeightedEventsW1 + WeightedEventsW2 +  WeightedEventsW3 +  WeightedEventsW4
#            ExtraPolationFactorFinal = ExtraPolationFactorNum / ExtraPolationFactorDenum

#            if (verbos_):print "VV_NOrmalization ToTal = ", VV_NormForWSub
#            if (verbos_):print "TT_NOrmalization ToTal = ", TT_NormForWSub
#            if (verbos_):print "ZL_NOrmalization ToTal = ", ZL_NormForWSub
#            if (verbos_):print "ZJ_NOrmalization ToTal = ", ZJ_NormForWSub
#            if (verbos_):print "ZTT_NOrmalization ToTal = ", ZTT_NormForWSub



#
#            ExPolFactorW_ = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[0]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW1 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[1]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW2 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[2]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW3 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[3]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#            ExPolFactorW4 = getWExtraPol(PostFix,CoMEnergy,str(W_BackGround[4]) ,channel[chl],category[categ],numeratorW,denumeratorW)
#
#            WeightedEventsW_= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[0]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[0]), CoMEnergy)
#            WeightedEventsW1= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[1]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[1]), CoMEnergy)
#            WeightedEventsW2= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[2]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[2]), CoMEnergy)
#            WeightedEventsW3= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[3]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[3]), CoMEnergy)
#            WeightedEventsW4= getHistoNorm_BG(PostFix,CoMEnergy, str(W_BackGround[4]),channel[chl],category[categ],Histogram)[0] * XSection(str(W_BackGround[4]), CoMEnergy)
#
#            ExtraPolationFactorNum = ExPolFactorW_ * WeightedEventsW_ + ExPolFactorW1 * WeightedEventsW1 + ExPolFactorW2 * WeightedEventsW2 + ExPolFactorW3 * WeightedEventsW3 + ExPolFactorW4 * WeightedEventsW4
#            ExtraPolationFactorDenum =  WeightedEventsW_ + WeightedEventsW1 + WeightedEventsW2 +  WeightedEventsW3 +  WeightedEventsW4
#            ExtraPolationFactorFinal = ExtraPolationFactorNum / ExtraPolationFactorDenum
#            if (verbos_):print "******************ExPolFactorW_=", ExPolFactorW_
#            if (verbos_):print "******************ExPolFactorW1=", ExPolFactorW1
#            if (verbos_):print "******************ExPolFactorW2=", ExPolFactorW2
#            if (verbos_):print "******************ExPolFactorW3=", ExPolFactorW3
#            if (verbos_):print "******************ExPolFactorW3=", ExPolFactorW4
#            if (verbos_):print "******************ExtraPolationFactorFinal=", ExtraPolationFactorFinal

#            if (verbos_):print "VV_NOrmalization ToTal = ", VV_NormForWforQCD
#            if (verbos_):print "TT_NOrmalization ToTal = ", TT_NormForWforQCD
#            if (verbos_):print "ZL_NOrmalization ToTal = ", ZL_NormForWforQCD
#            if (verbos_):print "ZJ_NOrmalization ToTal = ", ZJ_NormForWforQCD
#            if (verbos_):print "ZTT_NOrmalization ToTal = ", ZTT_NormForWforQCD
