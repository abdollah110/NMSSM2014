/* 
 * File:   myHelper.h
 * Author: abdollahmohammadi
 *
 * Created on March 3, 2013, 11:59 AM
 */

#ifndef MYHELPER_H
#define	MYHELPER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include <TLorentzVector.h>
//#include "../interface/zh_Auxiliary.h"



//    if (is_data_mc == "mc11") mcdata = 1;
//    if (is_data_mc == "data11") mcdata = 2;
//    if (is_data_mc == "mc12") mcdata = 3;
//    if (is_data_mc == "data12") mcdata = 4;
//    if (is_data_mc == "embed12") mcdata = 5;
//
//    cout<<l2_DecayMode
//        Tau decay mode 	abs(eta) < 1.5 	abs(eta) > 1.5
//Single hadron 	0.87 	0.96
//Hadron + Strips 	1.06 	1.00
//Three Hadrons 	1.02 	1.06


//-----------------------------------------------------------------------------
// CV: recipe for top quark Pt reweighting taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopPtReweighting

float compTopPtWeight(float topPt) {
    const float a = 0.156;
    const float b = -0.00137;
    return TMath::Exp(a + b * topPt);
}

float compTopPtWeight(float top1Pt, float top2Pt) {
    //std::cout << "<compTopPtWeight>:" << std::endl;
    float topPtWeight2 = compTopPtWeight(top1Pt) * compTopPtWeight(top2Pt);
    //std::cout << " top1Pt = " << top1Pt << ", top2Pt = " << top2Pt << ": topPtWeight2 = " << topPtWeight2 << std::endl;
    return ( topPtWeight2 > 0.) ? TMath::Sqrt(topPtWeight2) : 0.;
}
//-----------------------------------------------------------------------------

std::vector<float> HPtReWeight(float H_Pt, std::string Mass, TFile * inputFile) {
    std::vector<float> HiggsPtRW;
    HiggsPtRW.clear();

    //    TFile * inputFile = new TFile("mssmHiggsPtReweightGluGlu_mhmod+_POWHEG.root", "r");

    std::string DirName = "A_mA" + Mass + "_mu200";
    std::string nominalName = "mssmHiggsPtReweight_A_mA" + Mass + "_mu200_central";
    std::string UpName = "mssmHiggsPtReweight_A_mA" + Mass + "_mu200_tanBetaHigh";
    std::string DownName = "mssmHiggsPtReweight_A_mA" + Mass + "_mu200_tanBetaLow";

    TDirectory * tDir = (TDirectory *) inputFile->Get(DirName.c_str());
    TH1F *HpT = (TH1F*) tDir->Get(nominalName.c_str());
    TH1F *HpT_Up = (TH1F*) tDir->Get(UpName.c_str());
    TH1F *HpT_Down = (TH1F*) tDir->Get(DownName.c_str());


    int whichBin = 1 + int(H_Pt) / 5;
    if (whichBin > 50) whichBin = 50;
    if (!HpT_Down->GetBinContent(whichBin) || !HpT->GetBinContent(whichBin) || !HpT_Up->GetBinContent(whichBin)) cout << "No Historam exsit\n";
    //    HiggsPtRW.push_back(1);
    //    HiggsPtRW.push_back(1);
    //    HiggsPtRW.push_back(1);
    HiggsPtRW.push_back(HpT_Down->GetBinContent(whichBin) / HpT->GetBinContent(whichBin));
    HiggsPtRW.push_back(HpT->GetBinContent(whichBin));
    HiggsPtRW.push_back(HpT_Up->GetBinContent(whichBin) / HpT->GetBinContent(whichBin));

    //    cout << H_Pt << "   whcih bin= " << whichBin << "   nominal weight"<< HpT->GetBinContent(whichBin) <<   "   "<<  HpT_Down->GetBinContent(whichBin) / HpT->GetBinContent(whichBin)  << "  " << HpT_Up->GetBinContent(whichBin) / HpT->GetBinContent(whichBin)<<  "\n";
    //    inputFile->Close();
    return HiggsPtRW;






    //
    //    [23/09/14 10:16:52] Cécile Caillol: poidssup = poidssup * (1+ highMSSM*0.2*l1Pt_*0.001 +highMSSM*0.2*l2Pt_*0.001);
    //[23/09/14 10:25:34] Cécile Caillol:     TFile *f_HpT = new TFile("From_Christian/mssmHiggsPtReweightGluGlu_mhmod+.root","r");
    //    TDirectory *d_HpT = (TDirectory*) f_HpT->Get("A_mA120_mu200");
    //    TH1F *HpT = (TH1F*) d_HpT->Get("mssmHiggsPtReweight_A_mA120_mu200_central");
    //    TH1F *HpT_Up = (TH1F*) d_HpT->Get("mssmHiggsPtReweight_A_mA120_mu200_tanBetaHigh");
    //    TH1F *HpT_Down = (TH1F*) d_HpT->Get("mssmHiggsPtReweight_A_mA120_mu200_tanBetaLow");
    //    if (dataType=="GluGluH"){
    //           int binnb=1+int(gen_Higgs_pt)/5;
    //           if (binnb>50) binnb=50;
    //           cout<<HpT->GetBinContent(binnb)<<endl;
    //           poids = poids * HpT->GetBinContent(binnb);
    //           poidsUp = HpT_Up->GetBinContent(binnb)/HpT->GetBinContent(binnb);
    //           poidsDown = HpT_Down->GetBinContent(binnb)/HpT->GetBinContent(binnb);
    //        }
}

float TauESWeight(int mcdata, int DM, float eta) {
    if (mcdata == 2 || mcdata == 4)
        return 1;
    else {
        if (DM == 0 && fabs(eta) < 1.5) return 0.87;
        if (DM == 0 && fabs(eta) > 1.5) return 0.96;
        if (DM == 1 && fabs(eta) < 1.5) return 1.06;
        if (DM == 1 && fabs(eta) > 1.5) return 1.00;
        if (DM == 10 && fabs(eta) < 1.5) return 1.02;
        if (DM == 10 && fabs(eta) > 1.5) return 1.06;
    }
    return 1;
}

float TMass_F(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    return sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2));
}

bool WZ_Rej_B(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    bool b_met = met < 20;
    bool b_tmass = sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2)) < 30;
    //    if (b_met && b_tmass)
    if (b_tmass)
        return true;
    else
        return false;
}

bool justTmass(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    bool b_tmass = sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2)) < 30;
    if (b_tmass)
        return true;
    else
        return false;
}

float deltaPhi(float a, float b) {
    float result = a - b;
    while (result > M_PI) result -= 2 * M_PI;
    while (result <= -M_PI) result += 2 * M_PI;
    return fabs(result);
}

float dR(float l1eta, float l1phi, float l2eta, float l2phi) {
    float deta = l1eta - l2eta;
    float dphi = deltaPhi(l1phi, l2phi);
    return sqrt(deta * deta + dphi * dphi);
}

bool NewOverLap(float l1eta, float l1phi, float l2eta, float l2phi, float l3eta, float l3phi, float l4eta, float l4phi) {
    bool over_12 = dR(l1eta, l1phi, l2eta, l2phi) > 0.3;
    bool over_13 = dR(l1eta, l1phi, l3eta, l3phi) > 0.3;
    bool over_14 = dR(l1eta, l1phi, l4eta, l4phi) > 0.3;
    bool over_23 = dR(l2eta, l2phi, l3eta, l3phi) > 0.3;
    bool over_24 = dR(l2eta, l2phi, l4eta, l4phi) > 0.3;
    bool over_34 = dR(l3eta, l3phi, l4eta, l4phi) > 0.3;

    if (over_12 && over_13 && over_14 && over_23 && over_24 && over_34)
        return true;
    else
        return false;
}

float TMass_F(float et1, float et2, float px1, float px2, float py1, float py2) {
    return sqrt(pow(et1 + et2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2));

}

float InvarMass_F(float e1, float e2, float px1, float px2, float py1, float py2, float pz1, float pz2) {
    return sqrt(pow(e1 + e2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2) - pow(pz1 + pz2, 2));
}

#endif	/* MYHELPER_H */

