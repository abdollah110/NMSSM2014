import math

import ROOT
from ROOT import Double
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TH2F
from ROOT import TGraph
from ROOT import TNtuple
from ROOT import TProfile
from ROOT import gBenchmark
from ROOT import gROOT
from ROOT import gRandom
from ROOT import gStyle
from ROOT import TLine
from ROOT import TText
from ROOT import gSystem
from ROOT import TGraphErrors
import numpy as n


mass = [30,  35, 40, 45, 50, 55, 65]

cont=len(mass)
x = n.zeros(cont, dtype=float)
y = n.zeros(cont, dtype=float)
ex = n.zeros(cont, dtype=float)
ey = n.zeros(cont, dtype=float)

categ=["_inclusive","_btag"]

for cat in categ:
    for m in range(len(mass)):
        LOFile = TFile("OutPythia6/out_bba1GenFil_"+str(mass[m])+"_8TeV.root")
        LOHisto_btag=LOFile.Get("mutau_SVMass_mTLess30_OS"+cat)
        
        NLOFile = TFile("OutPythia8/out_bba1GenFil_"+str(mass[m])+"_8TeV.root")
        NLOHisto_btag=NLOFile.Get("mutau_SVMass_mTLess30_OS"+cat)

        OrgFile= TFile("../FileROOT/NLORootsLooseCoupling/BBHToTauTau_yb2_M-"+str(mass[m])+".root")
        total=OrgFile.Get("TotalEventsNumber").Integral()
#        totalWeight=OrgFile.Get("TotalEventsNumberFABS").Integral()
        totalWeight=OrgFile.Get("TotalEventsNumberWeight").Integral()
#        weightONSample=total/totalWeight
        weightONSample=1
        print "Mass is  ", str(mass[m]), "-------> weightONSample is  ", weightONSample
       
        print "Yield= ", LOHisto_btag.Integral(), " number of events=",LOHisto_btag.GetEntries()
        print "Yield= ",NLOHisto_btag.Integral()*weightONSample, " number of events=",NLOHisto_btag.GetEntries()
        print "Ratio=",   NLOHisto_btag.Integral()*weightONSample / LOHisto_btag.Integral()
#        print NLOHisto_btag.GetEntries()
#        print "\n"
#        
        num= NLOHisto_btag.Integral()*weightONSample
        denum=LOHisto_btag.Integral()
        numEr=NLOHisto_btag.Integral()*weightONSample/pow(NLOHisto_btag.GetEntries(),0.5)
        denumEr=LOHisto_btag.Integral()/pow(LOHisto_btag.GetEntries(),0.5)
        
        
        x[m]= mass[m]
        y[m]= num/denum
        
        ex[m]=2.5
        ey[m]= 1.0/pow(denum,2) * (( numEr*denum  )+(num*denumEr))
#        ey[m]=  y[m] * ((numEr/num) + (denumEr/denum))





    c1 = TCanvas("c1","A Simple Graph Example",200,10,700,500);
    gr= TGraphErrors(cont,x,y,ex,ey)
    #gr= TGraph(num,x,y)
    gr.SetTitle()
    gr.GetYaxis().SetRangeUser(0,5)
    gr.GetXaxis().SetTitle("m_{A} [GeV]")
    gr.GetYaxis().SetTitle("NLO/LO acceptance x efficiency ")
    gr.GetXaxis().SetTitleSize(0.05)
    gr.GetYaxis().SetTitleSize(0.05)
    gr.GetXaxis().SetTitleOffset(0.8)
    gr.SetMarkerColor(4);
    gr.SetMarkerStyle(21);
    gr.Draw("AP")
    L=TLine(24,1,71,1)
    L.SetLineColor(2)
#    L.SetLineWidth(1.2)
    L.Draw()
    T=TText(55,4.0,"CMS Simulation")
    T2=TText(35,4.0,"MuTau"+str(cat))
    T.Draw()
    T2.Draw()
    c1.SaveAs("MuTau_tgraph"+cat+".pdf")








for cat in categ:
    for m in range(len(mass)):
        
        if cat=="_btag":
            emuPythia6 = [2.53717E-05,3.0668E-05,4.84691E-05,6.18132E-05,0.00010763,0.000123721,0.00018274,0.000219057]
            emuPythia8 = [3.01625E-05,3.40908E-05,6.33333E-05,9.07331E-05,0.000113333,0.00013,0.000166667,0.000295786]
            emuPythia6_cont=[234,215,239,239,313,226,378,406]
            emuPythia8_cont=[9,10,19,27,34,39,25,83]

        if cat=="_inclusive":
            emuPythia6 = [6.85252E-05,0.000101561,0.000193471,0.000350189,0.000640965,0.000939406,0.001362333,0.00181558]
            emuPythia8 = [7.70819E-05,0.000122727,0.000196667,0.000329328,0.000546667,0.000996667,0.001273333,0.001774718]
            emuPythia6_cont=[632,712,954,1354,1864,1716,2818,3365]
            emuPythia8_cont=[23,36,59,98,164,299,196,498]
    
    
        num= emuPythia8[m]
        denum=emuPythia6[m]
        numEr=emuPythia8[m]/pow(emuPythia8_cont[m],0.5)
        denumEr=emuPythia6[m]/pow(emuPythia6_cont[m],0.5)
        
        
        x[m]= mass[m]
        y[m]= num/denum
        
        ex[m]=2.5
        ey[m]= 1.0/pow(denum,2) * (( numEr*denum  )+(num*denumEr))
    
    
    
    c1 = TCanvas("c1","A Simple Graph Example",200,10,700,500);
    gr= TGraphErrors(cont,x,y,ex,ey)
    #gr= TGraph(num,x,y)
    gr.SetTitle("")
    gr.GetYaxis().SetRangeUser(0,2)
    gr.GetXaxis().SetTitle("m_{A} [GeV]")
    gr.GetYaxis().SetTitle("NLO/LO acceptance x efficiency ")
    gr.GetXaxis().SetTitleSize(0.05)
    gr.GetYaxis().SetTitleSize(0.05)
    gr.GetXaxis().SetTitleOffset(0.8)
    gr.SetMarkerColor(4);
    gr.SetMarkerStyle(21);
    gr.Draw("AP")
    L=TLine(24,1,71,1)
    L.SetLineColor(2)
#    L.SetLineWidth(1.2)
    L.Draw()
    T=TText(55,1.85,"CMS Simulation")
    T2=TText(35,1.85,"EMu"+str(cat))
    T2.Draw()
    T.Draw()
    c1.SaveAs("Emu_tgraph"+cat+".pdf")




