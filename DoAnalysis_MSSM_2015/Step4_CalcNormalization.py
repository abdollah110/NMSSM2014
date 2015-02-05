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
#InputFileLocation = '../FileROOT/MSSMROOTFiles/'
SubRootDir = 'OutFiles/'

UseTauPolarOff = True

#def luminosity(CoMEnergy):
#    if CoMEnergy == '_8TeV': return  19710 #19242
#    if CoMEnergy == '_7TeV': return  4982
#
#def XSection(mX, CoMEnergy):
#    if CoMEnergy == '_8TeV':
#        if mX == 'signal':      return 1.
#
#        if mX == 'WWJetsTo2L2Nu': return 5.824
#        if mX == 'WZJetsTo2L2Q':      return 2.207 
#        if mX == 'WZJetsTo3LNu':      return 1.058 
#        if mX == 'ZZJetsTo2L2Nu':      return 0.716 
#        if mX == 'ZZJetsTo2L2Q':      return 2.502 
#        if mX == 'ZZJetsTo4L':      return  	0.181
#
#        if mX == 'TTJets_FullLeptMGDecays':      return 26.1975
#        if mX == 'TTJets_SemiLeptMGDecays':      return 109.281
#        if mX == 'TTJets_HadronicMGDecays':      return 114.0215
#        if mX == 'Tbar_tW':      return 11.1
#        if mX == 'T_tW':      return 11.1
#
#
#        if mX == 'ggH_SM125':      return 1.23
#        if mX == 'qqH_SM125':      return 0.100
#        if mX == 'VH_SM125':      return 0.077
#        if mX == 'TTEmbeddedmutau':      return (26.197  * 771693/ 12011428)  # FIXME   This hsould be fixed
#        if mX == 'TTEmbeddedetau':      return (26.197  * 718441/ 12011428)  # FIXME   This hsould be fixed
##        if mX == 'TTEmbeddedmutau':      return (5.887  * 771693/ 12011428)  # FIXME   This hsould be fixed
##        if mX == 'TTEmbeddedetau':      return (5.887  * 718441/ 12011428)  # FIXME   This hsould be fixed
#        if mX == 'tauPolarOff': return 2950 *1.187694915
#
##        if mX == 'DYJetsToLL':  return 0.242 * 0.001
##        if mX == 'DY1JetsToLL':  return  0.128* 0.001
##        if mX == 'DY2JetsToLL':  return 0.0914* 0.001
##        if mX == 'DY3JetsToLL':  return 0.0109* 0.001
##        if mX == 'DY4JetsToLL':  return  0.00540* 0.001
##
##        if mX == 'WJetsToLNu':  return 6.149* 0.001
##        if mX == 'W1JetsToLNu':  return  0.306* 0.001
##        if mX == 'W2JetsToLNu':  return 0.153* 0.001
##        if mX == 'W3JetsToLNu':  return 0.0436* 0.001
#        if mX == 'W4JetsToLNu':  return  0.0597* 0.001



signal = ['ggH', 'bbH']
mass = [80,90,  100, 110,  120, 130, 140,  160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000]
SMHiggs_BackGround = ['ggH_SM125', 'qqH_SM125', 'VH_SM125']

category = ["_btag_high"]
#category = ["_inclusive", "_nobtag", "_btag", "_btagLoose"]
#category =["_inclusive", "_nobtag_low", "_nobtag_medium", "_nobtag_high", "_btag_low", "_btag_high", "_btagLoose_low", "_btagLoose_high"]
#category = ["_inclusive",  "_nobtag","_btag"]
#channel = ["mutau", "etau"]
#channel = ["etau"]
channel = ["mutau"]
lenghtSig = len(signal) * len(mass) +1

digit = 3
verbos_ = False


#def getHistoIntegral(PostFix,CoMEnergy,Name,chan,cat,Histogram):
#    myfile = TFile(InputFileLocation + Name +CoMEnergy+ '.root')
#    HistoDenum = myfile.Get("TotalEventsNumber")  # to get Total number of events  "MuTau_Multiplicity" + index[icat]
#    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
#    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
#    value = 10E-7
#    if (HistoSub):
#        value = HistoSub.Integral() * luminosity(CoMEnergy) / HistoDenum.GetBinContent(1)
#        value = round(value, digit)
#    return value
#
#def getHistoIntegral(PostFix,CoMEnergy,Name,chan,cat,Histogram):
#    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
#    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
#    value = 10E-7
#    if (HistoSub):
#        value = HistoSub.Integral() * luminosity(CoMEnergy)
#        value = round(value, digit)
#    return value

#def getEmbeddedWeight(PostFix,CoMEnergy,Name,chan,cat,Histogram):
#    myfileSub = TFile(SubRootDir + "out_"+Name+chan +CoMEnergy+ '.root') #need chan due to embedded name include MuTau
#    HistoInclusive = myfileSub.Get(chan+Histogram+ "_inclusive"+PostFix )
#    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
#    value = HistoSub.Integral()/ HistoInclusive.Integral()
#    return value

def getEmbedToDYWeight(PostFix,CoMEnergy,chan,HistogramNoMT):
    # Here we have used ZTT for all DY and Embedded data and Embed MC     
    
    # Get Normalization from DY Sample in Inclusive
    if not UseTauPolarOff:
        DY_Files = TFile(SubRootDir + "out_DYJetsAll"+CoMEnergy+".root")
        DY_Histo=DY_Files.Get(chan+HistogramNoMT+ "_inclusive"+PostFix)
        Normalization_DY= DY_Histo.Integral()
    else:
        DY_Files = TFile(SubRootDir + "out_DYJetsToLL_PolarOff"+CoMEnergy+".root")
        DY_Histo=DY_Files.Get(chan+HistogramNoMT+ "_inclusive"+PostFix)
        Normalization_DY= DY_Histo.Integral()


    #  Get Normalization from Embedded Data Sample in Inclusive
    EmbedData_Files = TFile(SubRootDir + "out_Embedded"+chan+CoMEnergy+".root")
    EmbedData_Histo=EmbedData_Files.Get(chan+HistogramNoMT+ "_inclusive")
    Normalization_EmbedData= EmbedData_Histo.Integral()

#    HistogramNoMT=HistogramNoMT.replace("_ZTT","") # There is no _ZTT for TTEmbedded #FIXME   maight be resoved by new ntuples
    #  Get Normalization from TTEmbedded Sample in Inclusive
    EmbedTT_Files = TFile(SubRootDir + "out_TTEmbedded"+chan+CoMEnergy+".root")
    EmbedTT_Histo=EmbedTT_Files.Get(chan+HistogramNoMT+ "_inclusive"+PostFix)
    Normalization_EmbedTT= EmbedTT_Histo.Integral()

    print "PostFix is ", PostFix
    print "---------->  ", chan+HistogramNoMT+ "_inclusive"
    print "DY MC Incluvive= ", Normalization_DY, "   #ofevents=(",DY_Histo.GetEntries(),")"
    print "TTEmbedded MC Incluvive= ", Normalization_EmbedTT, "   #ofevents=(",EmbedTT_Histo.GetEntries(),")"
    print "Embed Data Incluvive= ", Normalization_EmbedData, "   #ofevents=(",EmbedData_Histo.GetEntries(),")"
    print "ExtraPOl Factor= ", (Normalization_DY)/(Normalization_EmbedData- Normalization_EmbedTT)

    return (Normalization_DY)/(Normalization_EmbedData- Normalization_EmbedTT)

def getWExtraPol(PostFix,CoMEnergy,Name,chan,cat,HistogramNum,HistogramDenum ):
    myfileSub = TFile(SubRootDir + "out_"+Name+CoMEnergy+ '.root')
    HistoNum = myfileSub.Get(chan+HistogramNum+ cat+PostFix )
    HistoDenum = myfileSub.Get(chan+HistogramDenum+ "_inclusive"+PostFix )
#    print "----------------------------" , chan+HistogramNum+ cat+PostFix
#    print "W(from MC, signal region in category i) = ", HistoNum.GetName(), HistoNum.Integral()
#    print "W(from MC, control region in inclusive)=  ", HistoDenum.GetName(), HistoDenum.Integral()
#    print " scaleFactor =  ", HistoNum.Integral()/ HistoDenum.Integral()
    value = HistoNum.Integral()/ HistoDenum.Integral()
    return value

def getHistoIntegral(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    if (HistoSub):
        value = HistoSub.Integral()
        value = round(value, digit)
    return value

def getHistoEntry(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfileSub = TFile(SubRootDir + "out_"+Name +CoMEnergy+ '.root')
    HistoSub = myfileSub.Get(chan+Histogram+ cat+PostFix )
    value = 10E-7
    if (HistoSub):
        value = HistoSub.GetEntries()
        value = round(value, digit)
    return value
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################
##################################################################################################


def make2DTable(Observable,PostFix,CoMEnergy):
    myOut = TFile("Yield"+CoMEnergy+PostFix+".root", 'RECREATE')
    FullResults  = TH2F('FullResults', 'FullResults', 16, 0, 16, 55, 0, 55)

    for categ in range(len(category)):
        for chl in range(len(channel)):
            print "\n##################################################################################################"
            print "starting category and channel", category[categ], channel[chl]
            print "##################################################################################################\n"
            ##################################################################################################
            #   Signal Estimation
            ##################################################################################################
            print "\nDoing Signal estimation in ", category[categ], channel[chl]
            for sig in range(len(signal)):
                for m in range(len(mass)):#    for m in range(110, 145, 5):

                    Histogram = Observable+"_mTLess30_OS"
                    XLoc= categ + len(category)*chl + 1
                    YLoc= sig * len(mass) + m + 1
                    Name= str(signal[sig]) +str(mass[m])
#                    Name= str(signal[sig]) + "_"+str(mass[m])

                    value = getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)
                    FullResults.SetBinContent(XLoc,YLoc , value)
                    FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                    if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value 
             ##################################################################################################
            #   VV Estimation
            ##################################################################################################
            print "\nDoing VV, BG estimation  in ", category[categ], channel[chl]

            DYIndex = ""
            Name= "VVAll"
            YLoc= lenghtSig +1
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex    # This is for signal selection
            HistomTHigh70 = Observable+"_mTHigher70_OS"+DYIndex   # This is for W Normalization

            value = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value 

            VV_mTHighOS = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],"_inclusive",HistomTHigh70)
            ##################################################################################################
            #   TT Estimation
            ##################################################################################################
            print "\nDoing TT, BG estimation  in ", category[categ], channel[chl]

            DYIndex = ""
            Name= "TTAll"
            YLoc= lenghtSig  +2
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistomTHigh70 = Observable+"_mTHigher70_OS"+DYIndex

            value = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value 

            TT_mTHighOS = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],"_inclusive",HistomTHigh70)
            ##################################################################################################
            #   ZL Estimation
            ##################################################################################################
            print "\nDoing ZL, BG estimation  in ", category[categ], channel[chl]
#            for BG_ZL in range(len(Z_BackGround)):
                
            DYIndex = "_ZL"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 3
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistomTHigh70 = Observable+"_mTHigher70_OS"+DYIndex

            value = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value 

            ZL_mTHighOS = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],"_inclusive",HistomTHigh70)
            ##################################################################################################
            #   ZJ Estimation
            #################################################################################################
            print "\nDoing ZJ, BG estimation in ", category[categ], channel[chl]

            DYIndex = "_ZJ"
            Name= "DYJetsAll"
            YLoc= lenghtSig + 4
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistomTHigh70 = Observable+"_mTHigher70_OS"+DYIndex

            value = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value 

            ZJ_mTHighOS = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],"_inclusive",HistomTHigh70)
        ##################################################################################################
        #   ZTT Estimation
        ##################################################################################################
            print "\nDoing ZTT, BG estimation in ", category[categ], channel[chl]
#            for BG_ZTT in range(len(Z_BackGround)):

            DYIndex = "_ZTT"
            Name= "Embedded"+channel[chl]
            NameDY= "DYJetsAll"
            YLoc= lenghtSig + 5
            ## Similar To ALL ##
            XLoc= categ + len(category)*chl + 1
#            Histogram = Observable+"_mTLess30_OS"  changing the embedding technique FIXME FIXED
            HistogramNoMT = Observable+"_OS"+DYIndex
            Histogram = Observable+"_mTLess30_OS"+DYIndex
            HistomTHigh70 = Observable+"_mTHigher70_OS"+DYIndex
            
            print "------->",channel[chl],category[categ]
            embedToDYWeight= getEmbedToDYWeight(PostFix,CoMEnergy,channel[chl],HistogramNoMT)
            #print "       ------------   extraPol Embeede w8=======", embedToDYWeight

            value = getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)  * embedToDYWeight
            print   "-------> ZTauTau Value", getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram), "   #ofevents=(",getHistoEntry(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram),")"
            # print    "@@@@@@@@@@@  Test for ZTT in "+Name+channel[chl]+category[categ]+Histogram +" ==> NUmber of events in embeeded data=", getHistoIntegral(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram) , "  embed weight= ", embedToDYWeight,   "  Final ZTT Yield= ", value
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value , "  embedToDYWeight=",embedToDYWeight

            ZTT_mTHighOS = getHistoIntegral(PostFix,CoMEnergy, NameDY,channel[chl],"_inclusive",HistomTHigh70)
            
        ##################################################################################################
        #   W Estimation
        ##################################################################################################
            print "\nDoing W BG estimation in ", category[categ], channel[chl]

            numeratorW=Observable+"_mTLess30_OS"
            denumeratorW=Observable+"_mTHigher70_OS"
            HistogramContReg =Observable+"_mTHigher70_OS"
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 6
            W_mcName= "WJetsAll"
            Name='Data'

            WNormInSideBandData=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],"_inclusive",HistogramContReg)
            ExtraPolationFactorFinal = getWExtraPol(PostFix,CoMEnergy,W_mcName ,channel[chl],category[categ],numeratorW,denumeratorW)
            value =(WNormInSideBandData - (VV_mTHighOS + TT_mTHighOS +ZL_mTHighOS + ZJ_mTHighOS + ZTT_mTHighOS )) * ExtraPolationFactorFinal

            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, "W")
            if (verbos_): print "WNormInSideBandData= ", WNormInSideBandData, "   VV_mTHighOS=", VV_mTHighOS, "   TT_mTHighOS=", TT_mTHighOS,  "   ZL_mTHighOS=",ZL_mTHighOS, "   ZJ_mTHighOS=",ZJ_mTHighOS,"   ZTT_mTHighOS=",ZTT_mTHighOS
            if (verbos_): print "Final W Value is =", (WNormInSideBandData - (VV_mTHighOS + TT_mTHighOS +ZL_mTHighOS + ZJ_mTHighOS + ZTT_mTHighOS )), " time x ExtraPolationFactorFinal=", ExtraPolationFactorFinal, "  =", value
            
        ##################################################################################################
        #   TTEmbedded Estimation
        ##################################################################################################
            print "\nDoing TTEmbedded  estimation in ", category[categ], channel[chl]
            
            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 7
            Name="TTEmbedded"+channel[chl]
            Histogram = Observable+"_mTLess30_OS"
# FIXME added december 3   resolve the issue
            value=embedToDYWeight * getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)
            print "------EmbedTT test", value, "   #ofevents=(",getHistoEntry(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram),")"
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value
        ##################################################################################################
        #   Data Estimation
        ##################################################################################################
            print "\nDoing Data  estimation in ", category[categ], channel[chl]

            XLoc= categ + len(category)*chl + 1
            YLoc= lenghtSig + 8
            Name='Data'
            Histogram = Observable+"_mTLess30_OS"

            value=getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)
            FullResults.SetBinContent(XLoc,YLoc , value)
            FullResults.GetYaxis().SetBinLabel(YLoc, Name)

            if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value
        ##################################################################################################
        #   SM Higgs  Estimation
        ##################################################################################################
            print "\nDoing Signal estimation in ", category[categ], channel[chl]
            for HiggsBG in range(len(SMHiggs_BackGround)):

                Histogram = Observable+"_mTLess30_OS"
                XLoc= categ + len(category)*chl + 1
                YLoc= lenghtSig + 9 + HiggsBG
                Name= str(SMHiggs_BackGround[HiggsBG])

                value = getHistoIntegral(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)
                FullResults.SetBinContent(XLoc,YLoc , value)
                FullResults.GetYaxis().SetBinLabel(YLoc, Name)


                if (verbos_): print "Processing ...   =", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value 
#        ########################################################################
            FullResults.GetXaxis().SetBinLabel(categ + len(category)*chl + 1, channel[chl]+category[categ])
#        ########################################################################
    myOut.Write()
    myCanvas = TCanvas()
    gStyle.SetOptStat(0)
    FullResults.Draw('text')
    myCanvas.SaveAs("tableAll"+PostFix+CoMEnergy+".pdf")


if __name__ == "__main__":
#    make2DTable("_visibleMass","", "_8TeV")
#    make2DTable("_SVMass","", "_8TeV")
#    make2DTable("_SVMass","Up", "_8TeV")
    make2DTable("_SVMass","Down", "_8TeV")

