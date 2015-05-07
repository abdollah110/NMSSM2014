from ROOT import *
from ROOT import TFile
from ROOT import TH1F
from ROOT import TCanvas


c1 =  TCanvas("c1", "A Simple Graph Example",200,67,919,760);
c1.SetHighLightColor(2);
c1.Range(109.482,1.323741,991.482,14.34171);
c1.SetFillColor(0);
c1.SetBorderMode(0);
c1.SetBorderSize(2);

#c1.SetGridx();
#c1.SetGridy();
c1.SetFrameBorderMode(0);
gStyle.SetOptStat(0)

File_antiMuantiTau=TFile("QCDFinalFile.root")
File_MuantiTau=TFile("qcdFinalFile_IsoMuAntiIsoTau.root")
File_antiMuTau=TFile("qcdFinalFile-IsoTau.root")


rbin=5
Histo_antiMuantiTau=File_antiMuantiTau.Get("mutau_QCDShapeNormTotalFROSSS_btag")
Histo_antiMuantiTau.Rebin(rbin)

Histo_MuantiTau=File_MuantiTau.Get("mutau_QCDShapeNormTotalFROSSS_btag")
Histo_MuantiTau.Rebin(rbin)

Histo_antiMuTau=File_antiMuTau.Get("mutau_QCDShapeNormTotalFROSSS_btag")
Histo_antiMuTau.Rebin(rbin)

#c1.SetOptStat(0);
Histo_antiMuantiTau.GetYaxis().SetRangeUser(0,50)
Histo_antiMuantiTau.GetYaxis().SetTitle("Events")

Histo_antiMuantiTau.GetXaxis().SetRangeUser(0,180)
Histo_antiMuantiTau.GetXaxis().SetTitle("M_{#tau#tau} [GeV]")


Histo_antiMuantiTau.SetMarkerStyle(20)
Histo_antiMuantiTau.SetMarkerColor(2)
Histo_antiMuantiTau.SetLineColor(2)
Histo_antiMuantiTau.Draw()

Histo_MuantiTau.SetMarkerStyle(22)
Histo_MuantiTau.SetMarkerColor(3)
Histo_MuantiTau.SetLineColor(3)
Histo_MuantiTau.Draw("same")

Histo_antiMuTau.SetMarkerStyle(24)
Histo_antiMuTau.SetMarkerColor(4)
Histo_antiMuTau.SetLineColor(4)
Histo_antiMuTau.Draw("same")

leg=TLegend(.5500,.600,.90,.80)
leg.AddEntry(Histo_antiMuantiTau,"antiIsoLep-RelaxIsoTau","lpf")
leg.AddEntry(Histo_MuantiTau,"IsoLep-RelaxIsoTau","lpf")
leg.AddEntry(Histo_antiMuTau,"antiIsoLep-IsoTau","lpf")
leg.Draw()




tex =TLatex(10,47,"QCD Shape Comparison in Different Control Regions (#mu#tau)");
tex.SetTextSize(.035);
#tex.SetLineWidth(2);
tex.Draw();



c1.SaveAs("shapeQCD-Mu.pdf")