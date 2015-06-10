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
from ROOT import gSystem
from ROOT import TGraphErrors
import numpy as n


mass = [30,  35, 40, 45, 50, 55,  60, 65]

num=len(mass)
x = n.zeros(num, dtype=float)
y = n.zeros(num, dtype=float)
ex = n.zeros(num, dtype=float)
ey = n.zeros(num, dtype=float)
#gr= TGraph(n,x,y)


for m in range(len(mass)):
    LOFile = TFile("OutPythia6/out_bba1GenFil_"+str(mass[m])+"_8TeV.root")
    LOHisto_Incl=LOFile.Get("mutau_SVMass_mTLess30_OS_inclusive")
    LOHisto_btag=LOFile.Get("mutau_SVMass_mTLess30_OS_btag")

    NLOFile = TFile("OutPythia8/out_bba1GenFil_"+str(mass[m])+"_8TeV.root")
    NLOHisto_Incl=NLOFile.Get("mutau_SVMass_mTLess30_OS_inclusive")
    NLOHisto_btag=NLOFile.Get("mutau_SVMass_mTLess30_OS_btag")

    print "Mass is  ", str(mass[m])
    print LOHisto_Incl.Integral()    , " v.s " ,  NLOHisto_Incl.Integral(),   "  ratio=", LOHisto_Incl.Integral() / NLOHisto_Incl.Integral()
    print LOHisto_btag.Integral()    , " v.s " ,  NLOHisto_btag.Integral(),   "  ratio=", LOHisto_btag.Integral() / NLOHisto_btag.Integral()
    print NLOHisto_btag.GetEntries()
    print "\n"

    num= NLOHisto_btag.Integral()
    denum=LOHisto_btag.Integral()
    numEr=NLOHisto_btag.Integral()/pow(NLOHisto_btag.GetEntries(),0.5)
    denumEr=LOHisto_btag.Integral()/pow(LOHisto_btag.GetEntries(),0.5)
    
    
    x[m]= mass[m]
    y[m]= LOHisto_btag.Integral()/NLOHisto_btag.Integral()
    
    ex[m]=2.5
#    ey[m]=num/denum * 1.0/pow(denum,2) * (( numEr*denum  )+(num*denumEr))
    ey[m]=.2

#    gr.SetPoint(m,mass[m],y[m])

print "----->" , x[2], y[2],ex[2],ey[2]


c1 = TCanvas("c1","A Simple Graph Example",200,10,700,500);
#gr= TGraphErrors(num,x,y,ex,ey)
gr= TGraph(num,x,y)

gr.SetMarkerColor(4);
gr.SetMarkerStyle(21);
gr.Draw("AP")
c1.SaveAs("tgraph.pdf")



