#include <iostream>

#include <TH1F.h>
#include <TFile.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include <Rtypes.h>

#include <TMath.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAttLine.h>
#include <TPaveText.h>
#include <THStack.h>

#include "interface/HttStyles.h"
#include "src/HttStyles.cc"

float maximum(TH1F* h, bool LOG = false) {
    if (LOG) {
        if (h->GetMaximum() > 1000) {
            return 1000. * TMath::Nint(500 * h->GetMaximum() / 1000.);
        }
        if (h->GetMaximum() > 10) {
            return 10. * TMath::Nint(50 * h->GetMaximum() / 10.);
        }
        return 50 * h->GetMaximum();
    } else {
        if (h->GetMaximum() > 12) {
            return 10. * TMath::Nint((1.3 * h->GetMaximum() / 10.));
        }
        if (h->GetMaximum() > 1.2) {
            return TMath::Nint((1.6 * h->GetMaximum()));
        }
        return 1.6 * h->GetMaximum();
    }
}

//void draw_prefit(const char* inputname = "TotalRootForLimit_muTau_8TeV.root", const char* label = "Combined", int period = 8) {

void draw_prefit_Sample(std::string inputF, std::string channel, int MaxY, std::string xTitle, std::string nameHisto) {

    //    gStyle->SetOptStat(0);


    SetStyle();
    //    InitSubPad
    TCanvas *canv = MakeCanvas("canv", "histograms", 600, 600);
    float SIGNAL_SCALE = 10;
    bool scaled = true;
    bool log = false;

    TFile* input = new TFile(inputF.c_str());
    //cout<<"1";

    const char* dataset;


    //    std::string channel = "muTau_btag/";
    //    std::string channel = "eleTau_inclusive/";
    THStack hs("hs", "");
    TH1F* data = (TH1F*) input->Get((channel + "data_obs").c_str());
    TH1F* zero = (TH1F*) data->Clone("zero");


    TH1F* QCD = (TH1F*) input->Get((channel + "QCD").c_str());
    InitHist(QCD, "", "", TColor::GetColor(250, 202, 255), 1001);
    hs.Add(QCD);



    TH1F* W = (TH1F*) input->Get((channel + "W").c_str());
    InitHist(W, "", "", 46, 1001);

    TH1F* ZJ = (TH1F*) input->Get((channel + "ZJ").c_str());
//    InitHist(ZJ, "", "", TColor::GetColor(100, 182, 232), 1001);
    W->Add(ZJ);

    TH1F* ZL = (TH1F*) input->Get((channel + "ZL").c_str());
//    InitHist(ZL, "", "", TColor::GetColor(100, 182, 232), 1001);
    W->Add(ZL);


    TH1F* VV = (TH1F*) input->Get((channel + "VV").c_str());
//    InitHist(VV, "", "", TColor::GetColor(100, 182, 232), 1001);
    W->Add(VV);

    hs.Add(W);
    ////    TH1F* ZLL = (TH1F*) input->Get((channel +"ZLL");
    //    InitHist(ttbar, "", "", TColor::GetColor(155, 152, 204), 1001);

    TH1F* TT = (TH1F*) input->Get((channel + "TT").c_str());
    InitHist(TT, "", "", 9, 1001);
    hs.Add(TT);

    TH1F* ZTT = (TH1F*) input->Get((channel + "ZTT").c_str());
    InitHist(ZTT, "", "", TColor::GetColor(248, 206, 104), 1001);

    TH1F* ZTTLow = (TH1F*) input->Get((channel + "ZTT_lowMass").c_str());
    InitHist(ZTTLow, "", "", TColor::GetColor(248, 206, 104), 1001);

    ZTT->Add(ZTTLow);

    hs.Add(ZTT);
    

    InitData(data);

    TH1F* signal = (TH1F*) input->Get((channel + "bba150").c_str());
    signal->Scale(10);
    InitSignal(signal);
    //    signal->SetFillColor(kGreen);
    //    signal->SetLineColor(kGreen);
    hs.Add(signal);


    canv->cd();

    //    const char * MMM = xTitle.c_str();
    //    hs.GetXaxis()->SetLabelSize(9);
    zero->Scale(0);
    zero->GetXaxis()->SetRangeUser(0,60);
    zero->GetXaxis()->SetTitle(xTitle.c_str());
    zero->SetMaximum(MaxY);
    zero->Draw();

    hs.Draw("hsame");
    data->SetBinContent(1,0);
    data->SetBinContent(2,0);
    data->SetBinContent(3,0);
    data->SetBinContent(4,0);
    data->SetBinContent(5,0);
    data->SetBinContent(6,0);
    data->SetBinContent(7,0);
    data->SetBinContent(8,0);
    data->SetBinContent(9,0);
    data->SetBinContent(10,0);
    data->SetBinContent(11,0);
    data->SetBinContent(12,0);
    data->SetBinContent(13,0);
    data->Draw("PEsame");


    const char* dataset;
    dataset = "CMS Preliminary,  bba1#rightarrow#tau#tau, 19.7 fb^{-1} at 8 TeV";
    CMSPrelim(dataset, "", 0.17, 0.835);



    TLegend* leg = new TLegend(0.62, 0.58, 0.92, 0.89);
    SetLegendStyle(leg);
    leg->AddEntry(signal, TString::Format("a1(50 GeV)#rightarrow#tau#tau [XS= 10 bp]", SIGNAL_SCALE), "L");



    leg->AddEntry(data, "observed", "LP");
    leg->AddEntry(ZTT, "Z#rightarrow#tau#tau", "F");
    leg->AddEntry(TT, "t#bar{t}", "F");
    leg->AddEntry(W, "electroweak", "F");
    leg->AddEntry(QCD, "QCD", "F");
    leg->Draw();

//    canv->Print(TString::Format( (nameHisto+".pdf").c_str()));
//    canv->Print(TString::Format( (nameHisto+".root").c_str()));
    canv->Print(TString::Format( (nameHisto+"_Low_.pdf").c_str()));
    canv->Print(TString::Format( (nameHisto+"_Low_.root").c_str()));
}

void draw_prefit() {
    draw_prefit_Sample("TotalRootForLimit_etau_8TeV.root", "eleTau_nobtag/", 1000, "m_{#tau#tau}[GeV]","PLOT_eleTau_nobtag_m");
    draw_prefit_Sample("TotalRootForLimit_muTau_8TeV.root", "muTau_nobtag/", 4000, "m_{#tau#tau}[GeV]","PLOT_muTau_nobtag_m" );
    draw_prefit_Sample("TotalRootForLimit_etau_8TeV.root", "eleTau_btag/", 25, "m_{#tau#tau}[GeV]","PLOT_eleTau_btag_m");
    draw_prefit_Sample("TotalRootForLimit_muTau_8TeV.root", "muTau_btag/", 45, "m_{#tau#tau}[GeV]","PLOT_muTau_btag_m" );
//    draw_prefit_Sample("TotalRootForLimit_etau_8TeV.root", "eleTau_nobtag/", 18000, "m_{#tau#tau}[GeV]","PLOT_eleTau_nobtag_m");
//    draw_prefit_Sample("TotalRootForLimit_muTau_8TeV.root", "muTau_nobtag/", 40000, "m_{#tau#tau}[GeV]","PLOT_muTau_nobtag_m" );
//    draw_prefit_Sample("TotalRootForLimit_etau_8TeV.root", "eleTau_btag/", 250, "m_{#tau#tau}[GeV]","PLOT_eleTau_btag_m");
//    draw_prefit_Sample("TotalRootForLimit_muTau_8TeV.root", "muTau_btag/", 450, "m_{#tau#tau}[GeV]","PLOT_muTau_btag_m" );
};



