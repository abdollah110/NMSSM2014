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
InputFileLocation = '../FileROOT/nmssmROOTFiles/'
SubRootDir = 'OutFiles/'

def luminosity(CoMEnergy):
    if CoMEnergy == '_8TeV': return  19712 #19242
    if CoMEnergy == '_7TeV': return  4982

def XSection(mX, CoMEnergy):
    if CoMEnergy == '_8TeV':
        if mX == '25':      return  0.0216
        if mX == '30':      return  0.0315
        if mX == '35':      return  0.0428
        if mX == '40':      return  0.0607
        if mX == '45':      return  0.0771
        if mX == '50':      return  0.1014
        if mX == '55':      return  0.1100
        if mX == '60':      return  0.1439
        if mX == '65':      return  0.1636
        if mX == '70':      return  0.1819
        if mX == '75':      return  0.2019
        if mX == '80':      return  0.2178

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


        if mX == 'ggH_SM125':      return 1.23
        if mX == 'qqH_SM125':      return 0.100
        if mX == 'VH_SM125':      return 0.077
        if mX == 'TTEmbedded':      return 5.887

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



signal = ['bba1GenFil_']
mass = [25,30,  35, 40, 45, 50, 55,  60, 65, 70, 75, 80]
W_BackGround = ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
Z_BackGround = ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL']
Top_BackGround = ['TTJets_FullLeptMGDecays','TTJets_SemiLeptMGDecays',  'TTJets_HadronicMGDecays', 'Tbar_tW', 'T_tW']
#Top_BackGround = ['TTJets_FullLeptMGDecays', 'TTJets_HadronicMGDecays', 'TTJets_MassiveBinDECAY', 'TTJets_SemiLeptMGDecays']
#SingleTop_BackGround = ['Tbar_s', 'Tbar_t', 'Tbar_tW', 'T_s', 'T_t', 'T_tW']
DiBoson_BackGround = [ 'WWJetsTo2L2Nu',  'WZJetsTo2L2Q', 'WZJetsTo3LNu',  'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
#DiBoson_BackGround = ['WW', 'WWJetsTo2L2Nu', 'WZ', 'WZJetsTo2L2Q', 'WZJetsTo3LNu', 'ZZ', 'ZZJetsTo2L2Nu', 'ZZJetsTo2L2Q','ZZJetsTo4L' ]
Embedded = ['Embeddedmutau', 'Embeddedetau']
DYJets = ['DYJetsAll']
WJets = ['WJetsAll']
Data = ['Data']
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']




#Histogram = "VisibleMass_"
#category = ["_inclusive"]
#category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
category = ["_inclusive", "_btag", "_btagLoose", "_btagLowdR", "_btagMediumdR", "_btagHighdR"]
#category = ["_inclusive", "_btag", "_btagLoose"]
channel = ["mutau", "etau"]
#channel = ["MuTau"]
lenghtSig = len(signal) * len(mass) +1
lenghtVV = len(DiBoson_BackGround) +1
lenghtTop = len(Top_BackGround) +1
lenghtZL = len(Z_BackGround) + 1
lenghtZJ = len(Z_BackGround) + 1
lenghtZTT = len(Z_BackGround) + 1
low_bin = 0
high_bin = 300
digit = 1
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

#embedToDYWeight= getEmbedToDYWeight(PostFix,CoMEnergy,channel[chl],Observable+"_mTLess30_OS")
def getEmbedToDYWeight(PostFix,CoMEnergy,chan,Histogram):

    #  Get Normalization from DY Sample in Inclusive
    DY_Files = TFile(SubRootDir + "out_DYJetsAll"+CoMEnergy+".root")
    DY_Histo=DY_Files.Get(chan+Histogram+ "_ZTT_inclusive")
    Normalization_DY= DY_Histo.Integral()*luminosity(CoMEnergy)
    print "Normalization_DY= ", Normalization_DY


    #  Get Normalization from TTEmbedded Sample in Inclusive
    EmbedTT_Files = TFile(SubRootDir + "out_TTEmbedded"+chan+CoMEnergy+".root")
    EmbedTT_Histo=EmbedTT_Files.Get(chan+Histogram+ "_inclusive")
    OriginFile_EmbedTT = TFile(InputFileLocation + "TTEmbedded"+chan+CoMEnergy+".root")
    HistoTotal = OriginFile_EmbedTT.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
    Normalization_EmbedTT= (EmbedTT_Histo.Integral()*luminosity(CoMEnergy) * XSection("TTEmbedded", CoMEnergy))/HistoTotal.Integral()
    print "Normalization_EmbedTT= " , Normalization_EmbedTT
    OriginFile_EmbedTT.Close()

    #  Get Normalization from Embedded Data Sample in Inclusive
    EmbedData_Files = TFile(SubRootDir + "out_Embedded"+chan+CoMEnergy+".root")
    EmbedData_Histo=EmbedData_Files.Get(chan+Histogram+ "_inclusive")
    
    return (Normalization_DY+ Normalization_EmbedTT)/(EmbedData_Histo.Integral()* luminosity(CoMEnergy))

def getWExtraPol(PostFix,CoMEnergy,Name,chan,cat,HistogramNum,HistogramDenum ):
    myfileSub = TFile(SubRootDir + "out_"+Name+CoMEnergy+ '.root')
#    if cat=="_btag": cat = "_btagLoose"
    HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
    HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )
#    if not HistoNum or not HistoDenum:  #FIXME   I should find why WJets do not have statics for btag or no
#        cat = "_inclusive"
#        HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
#        HistoDenum = myfileSub.Get(chan+HistogramDenum+ cat+PostFix )

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
    FullResults  = TH2F('FullResults', 'FullResults', 15, 0, 15, 25, 0, 25)
    FullError  = TH2F('FullError', 'FullError', 15, 0, 15, 25, 0, 25)

    for categ in range(len(category)):
        for chl in range(len(channel)):
            print "\n##################################################################################################"
            print "starting category and channel", category[categ], channel[chl]
            print "##################################################################################################\n"
            ##################################################################################################
            #   Signal Estimation
            ##################################################################################################
            print "\nDoing Signal estimation"
            for sig in range(len(signal)):
                for m in range(len(mass)):#    for m in range(110, 145, 5):

                    Histogram = Observable+"_mTLess30_OS"
                    XLoc= categ + len(category)*chl + 1
                    YLoc= sig * len(mass) + m + 1
                    Name= str(signal[sig]) +str(mass[m])
#                    Name= str(signal[sig]) + "_"+str(mass[m])

                    value = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0] * XSection(str(mass[m]), CoMEnergy)
                    FullResults.SetBinContent(XLoc,YLoc , value)
                    FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                    valueEr = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * XSection(str(mass[m]), CoMEnergy)
                    FullError.SetBinContent(XLoc , YLoc, valueEr)
                    FullError.GetYaxis().SetBinLabel(YLoc, Name)
                    if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr
             ##################################################################################################
            #   VV Estimation
            ##################################################################################################
            print "\nDoing VV, BG estimation"

            DYIndex = ""
            Name= "VVAll"
            YLoc= lenghtSig +1
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex    # This is for signal selection
            HistomTHigh70OS = Observable+"_mTHigher70_OS"+DYIndex   # This is for W Normalization

            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, Name)
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            VV_mTHighOS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTHigh70OS)[0]
            ##################################################################################################
            #   TT Estimation
            ##################################################################################################
            print "\nDoing TT, BG estimation"

            DYIndex = ""
            Name= "TTAll"
            YLoc= lenghtSig  +2
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistomTHigh70OS = Observable+"_mTHigher70_OS"+DYIndex

            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, Name)
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            TT_mTHighOS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTHigh70OS)[0]
            ##################################################################################################
            #   ZL Estimation
            ##################################################################################################
            print "\nDoing ZL, BG estimation"
#            for BG_ZL in range(len(Z_BackGround)):
                
            DYIndex = "_ZL"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 3
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistomTHigh70OS = Observable+"_mTHigher70_OS"+DYIndex

            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, "ZL")
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            ZL_mTHighOS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTHigh70OS)[0]
#            ####  ZL Scale  UP
#            Value_ZL_MassScaleUp = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoZLScaleUp)[0]
#            FullResults.SetBinContent(XLoc,lenghtSig+13 , Value_ZL_MassScaleUp)
#            FullResults.GetYaxis().SetBinLabel(lenghtSig+13, "ZL_ScaleUp")
#
#            ####  ZL Scale  DOWN
#            Value_ZL_MassScaleDown = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoZLScaleDown)[0]
#            FullResults.SetBinContent(XLoc,lenghtSig+13 , Value_ZL_MassScaleDown)
#            FullResults.GetYaxis().SetBinLabel(lenghtSig+14, "ZL_ScaleDown")
            ##################################################################################################
            #   ZJ Estimation
            #################################################################################################
            print "\nDoing ZJ, BG estimation"

            DYIndex = "_ZJ"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 4
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistomTHigh70OS = Observable+"_mTHigher70_OS"+DYIndex

            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, "ZJ")
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

            ZJ_mTHighOS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTHigh70OS)[0]
        ##################################################################################################
        #   ZTT Estimation
        ##################################################################################################
            print "\nDoing ZTT, BG estimation"
#            for BG_ZTT in range(len(Z_BackGround)):

            DYIndex = "_ZTT"
            Name= "Embedded"+channel[chl]
            YLoc= lenghtSig + 5
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"
            HistomTHigh70OS = Observable+"_mTHigher70_OS"

            embedToDYWeight= getEmbedToDYWeight(PostFix,CoMEnergy,channel[chl],Histogram)

            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0]  * embedToDYWeight
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1]  * embedToDYWeight
            FullError.SetBinContent(XLoc , YLoc, valueEr)
            FullError.GetYaxis().SetBinLabel(YLoc, "ZTT")
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr, "  embedToDYWeight=",embedToDYWeight

            ZTT_mTHighOS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTHigh70OS)[0]* embedToDYWeight
#        ##################################################################################################
#        #   ZTT Estimation
#        ##################################################################################################
#            print "\nDoing ZTT, BG estimation"
##            for BG_ZTT in range(len(Z_BackGround)):
#
#            DYIndex = "_ZTT"
#            Name= "DYJetsAll"
#            YLoc= lenghtSig + 5
#            ## Similar To ALL ##
#            XLoc= categ + len(category)*chl + 1
#            XLocQCD= categ + 3*(chl+2) + 1
#            Histogram = Observable+"_mTLess30_OS"+DYIndex
#            HistomTHigh70OS = Observable+"_mTHigher70_OS"+DYIndex
#            HistomTHigh70SS = Observable+"_mTHigher70_SS"+DYIndex
#            HistomTLess30SS = Observable+"_mTLess30_SS"+DYIndex
#            HistoQCDShapeSS = Observable+"_QCDshape_SS"+DYIndex
#
#
#            EmbedEff = getEmbeddedWeight(PostFix,CoMEnergy, "Embedded",channel[chl],category[categ],Observable+"_mTLess30_OS")
#            print "categ=", categ   , " and  EmbedEff=", EmbedEff
#            value = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],"_inclusive",Histogram)[0]  * EmbedEff
#            FullResults.SetBinContent(XLoc,YLoc , value)
#            FullResults.GetYaxis().SetBinLabel(YLoc, Name)
#
#            valueEr = getHistoNorm_BG(PostFix,CoMEnergy,Name ,channel[chl],"_inclusive",Histogram)[1]  * EmbedEff
#            FullError.SetBinContent(XLoc , YLoc, valueEr)
#            FullError.GetYaxis().SetBinLabel(YLoc, "ZTT")
#            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr, "  embedEff=",EmbedEff
#
#            ZTT_mTHighOS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTHigh70OS)[0]
#            ZTT_mTHighSS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTHigh70SS)[0]
#            ZTT_mTLowSS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistomTLess30SS)[0]
#            ZTT_ShapeQCDSS = getHistoNorm_BG(PostFix,CoMEnergy, Name,channel[chl],category[categ],HistoQCDShapeSS)[0]
##            FullResults.SetBinContent(XLocQCD,YLoc , ZTT_ShapeQCDSS)
            
        ##################################################################################################
        #   W Estimation
        ##################################################################################################
            print "\nDoing W BG estimation in", channel[chl],   "   and  ", category[categ]

            numeratorW=Observable+"_mTLess30_OS"
            denumeratorW=Observable+"_mTHigher70_OS"
            Histogram =  Observable+"_mTHigher70_OS"
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 6
            W_mcName= "WJetsAll"
            Name='Data'

            ExtraPolationFactorFinal = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorW,denumeratorW)
            WNormInSideBandData=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
            value =(WNormInSideBandData - (VV_mTHighOS + TT_mTHighOS +ZL_mTHighOS + ZJ_mTHighOS + ZTT_mTHighOS )) * ExtraPolationFactorFinal

            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, "W")
            if (verbos_): print "WNormInSideBandData= ", WNormInSideBandData, "   VV_mTHighOS=", VV_mTHighOS, "   TT_mTHighOS=", TT_mTHighOS,  "   ZL_mTHighOS=",ZL_mTHighOS, "   ZJ_mTHighOS=",ZJ_mTHighOS,"   ZTT_mTHighOS=",ZTT_mTHighOS
            if (verbos_): print "Final W Value=", value, "   ExtraPolationFactorFinal=", ExtraPolationFactorFinal
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value 
            
#        ##################################################################################################
#        #   W Estimation for QCD Normalization
#        ##################################################################################################
#            print "\nDoing W BG estimation for QCD Normalization"
#
#            numeratorW=Observable+"_mTLess30_SS"
#            denumeratorW=Observable+"_mTHigher70_SS"
#            Histogram = Observable+"_mTHigher70_SS"
#            W_mcName= "WJetsAll"
#            XLoc= categ + len(category)*chl + 1
#            YLoc= lenghtSig + 6
#            Name='Data'
#
#            ExtraPolationFactorFinal = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorW,denumeratorW)
#            WNormInSideBandDataForQCD=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
#            WNormInQCD =(WNormInSideBandDataForQCD - (VV_mTHighSS + TT_mTHighSS +ZL_mTHighSS + ZJ_mTHighSS + ZTT_mTHighSS )) * ExtraPolationFactorFinal
#            if (verbos_): print "WNormInSideBandDataForQCD= ", WNormInSideBandDataForQCD, "   VV_mTHighSS=", VV_mTHighSS, "   TT_mTHighSS=", TT_mTHighSS,  "   ZL_mTHighSS=",ZL_mTHighSS, "   ZJ_mTHighSS=",ZJ_mTHighSS,"   ZTT_mTHighSS=",ZTT_mTHighSS
#            if (verbos_): print "Final WNormInQCD=", WNormInQCD, "   ExtraPolationFactorFinal=", ExtraPolationFactorFinal
#
#        ##################################################################################################
#        #   W Estimation for QCD Shape
#        ##################################################################################################
#            print "\nDoing W BG estimation for QCD Shape"
#
##            numeratorW=Observable+"_QCDshape_mTLess30_OS"
##            denumeratorW=Observable+"_QCDshape_mTHigher70_OS"
#            numeratorW=Observable+"_mTLess30_SS"
#            denumeratorW=Observable+"_mTHigher70_SS"
#            Histogram = Observable+"_QCDshape_SS"
#            W_mcName= "WJetsAll"
#            XLoc= categ + len(category)*chl + 1
#            XLocQCD= categ + 3*(chl+2) + 1
#            YLoc= lenghtSig + 6
#            Name='Data'
#
#            ExtraPolationFactorFinal = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorW,denumeratorW)
#            WNormInSideBandDataForQCDShape=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
#            W_ShapeQCDSS =(WNormInSideBandDataForQCDShape - (VV_ShapeQCDSS + TT_ShapeQCDSS +ZL_ShapeQCDSS + ZJ_ShapeQCDSS + ZTT_ShapeQCDSS )) * ExtraPolationFactorFinal
##            FullResults.SetBinContent(XLocQCD,YLoc , W_ShapeQCDSS)
#
#        ##################################################################################################
#        #   QCD Estimation
#        ##################################################################################################
#            print "Starting QCD  estimation"
#            Histogram = Observable+"_mTLess30_SS"
#            XLoc= categ + len(category)*chl + 1
#            YLoc= lenghtSig + 7
#            Name='Data'
#            QCDNormBare=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
#            print "WNormInSideBandData= ", WNormInSideBandData
#            FinalQCDNorm =(QCDNormBare - (VV_mTLowSS + TT_mTLowSS +ZL_mTLowSS + ZJ_mTLowSS + ZTT_mTLowSS + WNormInQCD)) * QCDScaleFactor
#            print "FinalQCDNorm =", FinalQCDNorm
#            FullResults.SetBinContent(XLoc,YLoc , FinalQCDNorm)
#            FullResults.GetYaxis().SetBinLabel(YLoc, "QCD")
#            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

        ##################################################################################################
        #   TTEmbedded Estimation
        ##################################################################################################
            print "Starting TTEmbedded  estimation"
            Histogram = Observable+"_mTLess30_OS"
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 7
            Name="TTEmbedded"+channel[chl]
            TTEmbedNorm=getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0] * XSection("TTEmbedded", CoMEnergy)
            print "TTEmbedded= ", TTEmbedNorm
            FullResults.SetBinContent(XLoc,YLoc , TTEmbedNorm)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",TTEmbedNorm 
        ##################################################################################################
        #   Data Estimation
        ##################################################################################################
            print "Starting Data  estimation"
            Histogram = Observable+"_mTLess30_OS"
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 8
            Name='Data'
            DataNorm=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0]
            print "Data= ", DataNorm
            FullResults.SetBinContent(XLoc,YLoc , DataNorm)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)
            if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",DataNorm
        ##################################################################################################
        #   SM Higgs  Estimation
        ##################################################################################################
            print "\nDoing Signal estimation"
            for HiggsBG in range(len(SMHiggs_BackGround)):

                Histogram = Observable+"_mTLess30_OS"
                XLoc= categ + len(category)*chl + 1
                YLoc= lenghtSig + 9 + HiggsBG
                Name= str(SMHiggs_BackGround[HiggsBG])

                value = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[0] * XSection(Name, CoMEnergy)
                FullResults.SetBinContent(XLoc,YLoc , value)
                FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                valueEr = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * XSection(Name, CoMEnergy)
                FullError.SetBinContent(XLoc , YLoc, valueEr)
                FullError.GetYaxis().SetBinLabel(YLoc, Name)
                if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr


#        ########################################################################
            FullResults.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
            FullError.GetXaxis().SetBinLabel(categ + len(category)*chl + 1,  channel[chl]+category[categ])
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
    make2DTable("_TMass","", "_8TeV")
    make2DTable("_TMass","Up", "_8TeV")
    make2DTable("_TMass","Down", "_8TeV")
#    make2DTable("_SVMass","", "_8TeV")
#    make2DTable("_SVMass","Up", "_8TeV")
#    make2DTable("_SVMass","Down", "_8TeV")

