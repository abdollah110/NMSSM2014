
#include <THStack.h>
 *hstack() {



       TFile* input = new TFile("TotalRootForLimit_muTau_8TeV.root");
    //cout<<"1";

    const char* dataset;
    //if(std::string(input).find("7TeV")!=std::string::npos){dataset = "CMS Preliminary,  ZH#rightarrowll#tau#tau, 4.9 fb^{-1} at 7 TeV";}
    //if(std::string(input).find("8TeV")!=std::string::npos){dataset = "CMS Preliminary,  ZH#rightarrowll#tau#tau, 19.4 fb^{-1} at 8 TeV";}
//    if (period == 7)
//        dataset = "CMS Preliminary,  ZH#rightarrowllLL, 5.0 fb^{-1} at 7 TeV";
//    else if (period == 8)
//        dataset = "CMS Preliminary,  ZH#rightarrowllLL, 19.8 fb^{-1} at 8 TeV";
//    else
//        dataset = "CMS Preliminary,  ZH#rightarrowllLL, 5.0 fb^{-1} at 7 TeV, 19.8 fb^{-1} at 8 TeV";
    /*
      mass plot before and after fit
     */
    THStack * hs= new THStack("hs", "test stacked histograms");
    TCanvas *cst = new TCanvas("cst","stacked hists",10,10,700,700);
    TH1F* data = (TH1F*) input->Get("muTau_btag/data_obs");
    TH1F* ZH_htt = (TH1F*) input->Get("muTau_btag/QCD");
    ZH_htt->SetFillColor(3);
    ZH_htt->SetMarkerStyle(21);
    ZH_htt->SetLineColor(kRed);
//    hs->Add(ZH_htt);
    ZH_htt->Draw("h");
//    TH1F* Zjets = (TH1F*) input->Get("muTau_btag/ZTT");
//    Zjets->SetFillColor(4);
//    Zjets->SetMarkerStyle(21);
//    Zjets->SetLineColor(kBlue);
////    hs->Add(Zjets);
//    Zjets->Draw("same");
//    TH1F* ZZ = (TH1F*) input->Get("muTau_btag/W");
//    ZZ->SetFillColor(5);
//    ZZ->SetMarkerStyle(21);
//    ZZ->SetLineColor(kGreen);
//    ZZ->Draw("same");
//    hs->Add(ZZ);

    
//
//THStack *hs = new THStack("hs","Stacked 1D histograms");
//   //create three 1-d histograms
//   TH1F *h1st = new TH1F("h1st","test hstack",100,-4,4);
//   h1st->FillRandom("gaus",20000);
//   h1st->SetFillColor(kRed);
//   h1st->SetMarkerStyle(21);
//   h1st->SetMarkerColor(kRed);
//   hs->Add(h1st);
//   TH1F *h2st = new TH1F("h2st","test hstack",100,-4,4);
//   h2st->FillRandom("gaus",15000);
//   h2st->SetFillColor(kBlue);
//   h2st->SetMarkerStyle(21);
//   h2st->SetMarkerColor(kBlue);
//   hs->Add(h2st);
//   TH1F *h3st = new TH1F("h3st","test hstack",100,-4,4);
//   h3st->FillRandom("gaus",10000);
//   h3st->SetFillColor(kGreen);
//   h3st->SetMarkerStyle(21);
//   h3st->SetMarkerColor(kGreen);
//   hs->Add(h3st);
//
   
//   cst->SetFillColor(41);
//   cst->Divide(2,2);
//   // in top left pad, draw the stack with defaults
//   cst->cd(1);
//   hs->SetFillColor(7);
//   hs->Draw();
   // in top right pad, draw the stack in non-stack mode 
   // and errors option
//   cst->cd(2);
//   gPad->SetGrid();
//   hs->Draw("nostack,e1p");
//   //in bottom left, draw in stack mode with "lego1" option
//   cst->cd(3);
//   gPad->SetFrameFillColor(17);
//   gPad->SetTheta(3.77);
//   gPad->SetPhi(2.9);
//   hs->Draw("lego1");
//
//   cst->cd(4);
//   //create two 2-D histograms and draw them in stack mode
//   gPad->SetFrameFillColor(17);
//   THStack *a = new THStack("a","Stacked 2D histograms");
//   TF2 *f1 = new TF2("f1",
//      "xygaus + xygaus(5) + xylandau(10)",-4,4,-4,4);
//   Double_t params[] = {130,-1.4,1.8,1.5,1, 150,2,0.5,-2,0.5,
//      3600,-2,0.7,-3,0.3};
//   f1->SetParameters(params);
//   TH2F *h2sta = new TH2F("h2sta","h2sta",20,-4,4,20,-4,4);
//   h2sta->SetFillColor(38);
//   h2sta->FillRandom("f1",4000);
//   TF2 *f2 = new TF2("f2","xygaus + xygaus(5)",-4,4,-4,4);
//   Double_t params[] = {100,-1.4,1.9,1.1,2, 80,2,0.7,-2,0.5};
//   f2->SetParameters(params);
//   TH2F *h2stb = new TH2F("h2stb","h2stb",20,-4,4,20,-4,4);
//   h2stb->SetFillColor(46);
//   h2stb->FillRandom("f2",3000);
//   a->Add(h2sta);
//   a->Add(h2stb);
//   a->Draw();
//   return cst;
}

