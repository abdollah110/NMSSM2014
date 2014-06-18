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

def Weight(mX, CoMEnergy):
    if CoMEnergy == '_8TeV':
        if mX == 'DYJetsToLL':  return 0.242
        if mX == 'DY1JetsToLL':  return  0.128
        if mX == 'DY2JetsToLL':  return 0.0914
        if mX == 'DY3JetsToLL':  return 0.0109
        if mX == 'DY4JetsToLL':  return  0.00540

        if mX == 'WJetsToLNu':  return 6.149
        if mX == 'W1JetsToLNu':  return  0.306
        if mX == 'W2JetsToLNu':  return 0.153
        if mX == 'W3JetsToLNu':  return 0.0436
        if mX == 'W4JetsToLNu':  return  0.0597

#    if CoMEnergy == '_7TeV':
#        if mX == 90:  return  [0.007537, 0.0001432]
#        if mX == 95:  return  [0.006451, 0.0002748]
#        if mX == 100: return  [0.005532, 0.0005474]
#        if mX == 105: return  [0.004723, 0.001039]
#        if mX == 110: return  [0.004000, 0.001780]  #[0.003815, .00177]
#        if mX == 115: return  [0.003326, 0.002773] #[0.003173 , .00275]
#        if mX == 120: return  [0.002707, 0.003954]#[0.002580, .00395]
#        if mX == 125: return  [0.002139, 0.005278] #[0.002032, .00522]
#        if mX == 130: return  [0.001628, 0.006517] #[0.001538 , .00645]
#        if mX == 135: return  [0.001186, 0.007552] #[0.001120, .00748]
#        if mX == 140: return  [0.0008254, 0.008333] #[0.000777, .00822]
#        if mX == 145: return  [0.0005451, 0.008821] #[0.000509, .00873]
#        if mX == 150: return  [0.0003330, 0.009051]
#        if mX == 155: return  [0.0001739, 0.009111]
#        if mX == 160: return  [0.00005767, 0.009071]
#        if mX == 'ZZ4L':        return 0.106
#        if mX == 'Data':        return 1
#        if mX == 'WZ3L':        return 1.057  # need to be change
#        if mX == 'TT2L2Nu':     return 23.64  # need to be change
#        if mX == 'GGToZZ2L2L':  return 0.00348
#        if mX == 'GGToZZ4L':    return 0.00174
#        if mX == 'TTZJets':     return 0.139 * 0.106 * 0.5  # in 7 TeV: /TTZTo2Lminus2Nu_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1
#        if mX == 'WZ3L':        return 0
#        if mX == 'WZJets3L':    return 0.868
#        if mX == 'DYJets':      return 3048.


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
Embedded = ['EmbeddedMuTau', 'EmbeddedETau']
Data = ['Data']




#Histogram = "VisibleMass_"
#category = ["_inclusive"]
category = ["_inclusive", "_nobtag", "_btag"]
#channel = ["MuTau", "ETau"]
channel = ["MuTau"]
#
#signal = ['zhtt', 'zhww']
#mass = [90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160]
#BackGround = ['ZZ4L', 'GGToZZ2L2L', 'TTZJets', 'Data']
##BackGround = ['ZZ4L', 'Data', 'GGToZZ2L2L', 'TTZJets']
##BackGround = ['ZZ4L', 'Data', 'GGToZZ2L2L','TTZJets','WZJets3L','WZ3L','TT2L2Nu']
##BackGround = ['ZZ4L',  'GGToZZ2L2L','Data','TTZJets','TT2L2Nu','DYJets']
lenghtSig = len(signal) * len(mass) +1
lenghtVV = len(DiBoson_BackGround) +1
lenghtTop = len(Top_BackGround) +1
lenghtZL = len(Z_BackGround) + 1
lenghtZJ = len(Z_BackGround) + 1
low_bin = 0
high_bin = 1000
digit = 3
verbos_ = True


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

def getHistoNorm_W_Z(PostFix,CoMEnergy,Name,chan,cat,Histogram):
    myfile = TFile(InputFileLocation + Name +CoMEnergy+ '.root')
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

    

def make2DTable(PostFix,CoMEnergy):
    myOut = TFile("Yield"+CoMEnergy+PostFix+".root", 'RECREATE')
    FullResults  = TH2F('FullResults', 'FullResults', 6, 0, 6, 80, 0, 80)
    FullError  = TH2F('FullError', 'FullError', 6, 0, 6, 80, 0, 80)

    for categ in range(len(category)):
        for chl in range(len(channel)):
            print "starting category and channel", category[categ], channel[chl]
            ###################################### Filling Signal ZH and WH ########
            for sig in range(len(signal)):
                for m in range(len(mass)):#    for m in range(110, 145, 5):

                    Histogram = "_visibleMass"
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

#        #######################################  Filling Reducible BG ##########
            print "Doing VV BG estimation"
            for BG_VV in range(len(DiBoson_BackGround)):

                Histogram = "_visibleMass"
                XLoc= categ + 3*chl + 1
                YLoc= lenghtSig + BG_VV + 1
                Name= str(DiBoson_BackGround[BG_VV])
                
                value = getHistoNorm(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0] * XSection(Name, CoMEnergy)
                FullResults.SetBinContent(XLoc,YLoc , value)
                FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                valueEr = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * XSection(Name, CoMEnergy)
                FullError.SetBinContent(XLoc , YLoc, valueEr)
                FullError.GetYaxis().SetBinLabel(YLoc, Name)
                if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr
                
            
#        #######################################  Filling Reducible BG ##########
            print "Doing TOP and Single BG estimation"
            for BG_T in range(len(Top_BackGround)):

                Histogram = "_visibleMass"
                XLoc= categ + 3*chl + 1
                YLoc= lenghtSig + lenghtVV+  BG_T + 1
                Name= str(Top_BackGround[BG_T])

                value = getHistoNorm(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0] * XSection(Name, CoMEnergy)
                FullResults.SetBinContent(XLoc,YLoc , value)
                FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                valueEr = getHistoNorm(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * XSection(Name, CoMEnergy)
                FullError.SetBinContent(XLoc , YLoc, valueEr)
                FullError.GetYaxis().SetBinLabel(YLoc, Name)
                if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

#        #######################################  Filling Reducible BG ##########
            print "Doing ZL, BG estimation"
            for BG_ZL in range(len(Z_BackGround)):
                
                Histogram = "_visibleMass_ZL"
                XLoc= categ + 3*chl + 1
                YLoc= lenghtSig + lenghtVV+ lenghtTop +BG_ZL +1
                Name= str(Z_BackGround[BG_ZL])


                value = getHistoNorm_W_Z(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0] * Weight(Name, CoMEnergy)
                FullResults.SetBinContent(XLoc,YLoc , value)
                FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                valueEr = getHistoNorm_W_Z(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * Weight(Name, CoMEnergy)
                FullError.SetBinContent(XLoc , YLoc, valueEr)
                FullError.GetYaxis().SetBinLabel(YLoc, Name)
                if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr

#        #######################################  Filling Reducible BG ##########
            print "Doing ZJ, BG estimation"
            for BG_ZJ in range(len(Z_BackGround)):

                Histogram = "_visibleMass_ZJ"
                XLoc= categ + 3*chl + 1
                YLoc= lenghtSig + lenghtVV+ lenghtTop +lenghtZL+ BG_ZJ+ 1
                Name= str(Z_BackGround[BG_ZJ])


                value = getHistoNorm_W_Z(PostFix,CoMEnergy, Name,channel[chl],category[categ],Histogram)[0] * Weight(Name, CoMEnergy)
                FullResults.SetBinContent(XLoc,YLoc , value)
                FullResults.GetYaxis().SetBinLabel(YLoc, Name)

                valueEr = getHistoNorm_W_Z(PostFix,CoMEnergy,Name ,channel[chl],category[categ],Histogram)[1] * Weight(Name, CoMEnergy)
                FullError.SetBinContent(XLoc , YLoc, valueEr)
                FullError.GetYaxis().SetBinLabel(YLoc, Name)
                if (verbos_): print "Same processed was=", Name, " coordinate was=",XLoc,YLoc, "  and the value is=",value ,"+/-", valueEr
            print "FullResults.GetBinContent(2,2)= ", FullResults.GetBinContent(2,2)

                
#        #######################################  Filling Reducible BG ##########
#        myfile = TFile('Reducible'+CoMEnergy+'.root')
#        Histo = myfile.Get('histo_Reducible')
#        value = Histo.GetBinContent(chl + 1)
#        value = round(value, digit)
#        FullResults.SetBinContent(chl + 1, lenghtSig  + 2, value)
#        FullResults.Fill(9, lenghtSig + 1, value)
#        FullResults.GetYaxis().SetBinLabel(lenghtSig  + 2, 'Reducible')
#        #Error
#        HistoEr = myfile.Get('histo_ReducibleEr')
#        valueEr = HistoEr.GetBinContent(chl + 1)
#        valueEr = round(valueEr, digit)
#        FullError.SetBinContent(chl + 1, lenghtSig  + 2, valueEr)
#        FullError.Fill(9, lenghtSig + 1, valueEr)
#        FullError.GetYaxis().SetBinLabel(lenghtSig  + 2, 'ReducibleEr')
#        #######################################   Filling BG and Data  #########
#        for bg in range (len(BackGround)):
#            myfile = TFile(OriginRootDir + str(BackGround[bg]) + CoMEnergy+'.root')
#            Histo = myfile.Get(str(channel[chl])) # to get Total number of events
#            myfileSub = TFile(SubRootDir + str(BackGround[bg]) + CoMEnergy+'.root')
#            HistoSubB = myfileSub.Get(Histogram+str(channel[chl])+"_pp"+PostFix)
#            value = 0
#            if (HistoSubB): value = HistoSubB.Integral(low_bin,high_bin) * luminosity(CoMEnergy) * XSection(BackGround[bg], CoMEnergy) / Histo.GetBinContent(1)
#            value = round(value, digit)
#            if BackGround[bg] == 'Data':
#                if (HistoSubB): value = HistoSubB.Integral(low_bin,high_bin) # as some data pomits are above 300
#            FullResults.SetBinContent(chl + 1, lenghtSig + bg + 3, value)
#            FullResults.Fill(9, lenghtSig + bg + 2, value)
#            FullResults.GetYaxis().SetBinLabel(lenghtSig  + bg + 3, str(BackGround[bg]))
#            ## Do Error
#            valueEr = 0
#            if (HistoSubB): valueEr = math.sqrt(HistoSubB.Integral(low_bin,high_bin)) * luminosity(CoMEnergy) * XSection(BackGround[bg], CoMEnergy) / Histo.GetBinContent(1)
#            valueEr = round(valueEr, digit)
#            FullError.SetBinContent(chl + 1, lenghtSig + bg + 3, valueEr)
#            FullError.Fill(9, lenghtSig + bg + 2, valueEr)
#            FullError.GetYaxis().SetBinLabel(lenghtSig  + bg + 3, str(BackGround[bg]))
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
    make2DTable("", "_8TeV")
#    make2DTable("_Up", "_8TeV")
#    make2DTable("_Down", "_8TeV")

