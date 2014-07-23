/*
 * File:   zh_Tree.h
 * Author: abdollah
 *
 * Created on February 6, 2013, 1:56 PM
 */

#ifndef TR_TREE_H
#define	TR_TREE_H

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "math.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TInterpreter.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFile.h"
#include <map>
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TFrame.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <utility>

//needed to make the executable
#include "makeHisto.h"
//#include "../interface/zh_Auxiliary.h"
#include "myHelper.h"


int Channel = 0;
int Run = 0;
int Lumi = 0;
int Event = 0;
float embedWeight =1;
//unsigned int Channel = 0;
//unsigned int Run = 0;
//unsigned int Lumi = 0;
//unsigned int Event = 0;
float mvis = 0;
float met, metphi, mvamet, mvametphi, mvametNoRecoil, mvametphiNoRecoil;
float l1M, l1Px, l1Py, l1Pz, l1E, l1Pt, l1Phi, l1Eta, l1Charge, l1_muIso, l1_eleIso, l1_eleMVANonTrg, l1_eleNumHit, l1_tauIsoMVA2raw, l1_dZ_in, l1_d0 = -10;
float l2M, l2Px, l2Py, l2Pz, l2E, l2Pt, l2Phi, l2Eta, l2Charge, l2_muIso, l2_eleIso, l2_eleMVANonTrg, l2_eleNumHit, l2_tauIsoMVA2raw, byCombinedIsolationDeltaBetaCorrRaw3Hits_2 = -10;
float l2_RefJetPt, l2_RefJetEta, l2_RefJetPhi = -10;
bool l1_muId_Loose, l1_muId_Tight, l1_eleId_Loose, l1_eleId_Tight, l2_muId_Loose, l2_eleId_Loose, l2_muId_Tight, l2_eleId_Tight;
bool l1_tauIsoL, l1_tauIsoM, l1_tauIsoT, l1_tauRejMuL, l1_tauRejMuM, l1_tauRejMuT, l1_tauRejEleL, l1_tauRejEleM, l1_tauRejEleMVA;
bool l1_tauIso3HitL, l1_tauIso3HitM, l1_tauIso3HitT, l1_tauRejMu2L, l1_tauRejMu2M, l1_tauRejMu2T, l1_tauRejEleMVA3L, l1_tauRejEleMVA3M, l1_tauRejEleMVA3T;
bool l1_tauIsoVL, l1_tauIsoMVA2L, l1_tauIsoMVA2M, l1_tauIsoMVA2T;
bool l2_tauIsoL, l2_tauIsoM, l2_tauIsoT, l2_tauRejMuL, l2_tauRejMuM, l2_tauRejMuT, l2_tauRejEleL, l2_tauRejEleM;
float l2_tauRejEleMVA;
bool l2_tauIso3HitL, l2_tauIso3HitM, l2_tauIso3HitT, l2_tauRejMu3L,l2_tauRejMu2L, l2_tauRejMu2M, l2_tauRejMu3T, l2_tauRejMu2T, l2_tauRejEleMVA3L, l2_tauRejEleMVA3M, l2_tauRejEleMVA3T;
bool l2_tauIsoVL, l2_tauIsoMVA2L, l2_tauIsoMVA2M, l2_tauIsoMVA2T;
//float mvacov00_mutau, mvacov01_mutau, mvacov10_mutau, mvacov11_mutau;
//float mvacov00_etau, mvacov01_etau, mvacov10_etau, mvacov11_etau;
float mvacov00, mvacov01, mvacov10, mvacov11;
float metcov00, metcov01, metcov10, metcov11;
float eff_Correction, pu_Weight;
int num_PV, npu, mcdata;
int mu_Size, BareMuon_Size, electron_Size, BareElectron_Size, tau_Size, BareTau_Size;
float l1_CloseJetPt, l2_CloseJetPt;
float l1_CloseJetEta, l2_CloseJetEta;
float l1_CloseJetPhi, l2_CloseJetPhi;


float mt_1;
float mt_2;
float idweight_1;
float trigweight_1;
float trigweight_2;
float rho;



int njets;
int njetpt20;
int nbtag, nbtagLoose, nbtagNoCor;

float jpt_1;
float jeta_1;
float jphi_1;
float jE_1;
bool jpass_1;

float jpt_2;
float jeta_2;
float jphi_2;
float jE_2;
bool jpass_2;

float bpt_1;
float beta_1;
float bphi_1;
float bdiscriminant_1;
float bpt;
float beta;
float bphi;
float bdiscriminant;

float bpt_2;
float beta_2;
float bphi_2;
float bdiscriminant_2;

float loosebpt_1;
float loosebeta_1;
float loosebphi_1;
float loosebdiscriminant_1;
float loosebpt;
float loosebeta;
float loosebphi;
float loosebdiscriminant;

float loosebpt_2;
float loosebeta_2;
float loosebphi_2;
float loosebdiscriminant_2;

float mjj;
float jdeta;
float jdphi;

float jetpt;
float dijetphi;
bool l2_DecayModeFinding;
int l2_DecayMode;
int zCategory = -10;

double SVMass, SVMassUnc;
double SVMassUp, SVMassUncUp;
double SVMassDown, SVMassUncDown;



bool l2_LoosetauIsoMVA3newDMwLT;
bool l2_MediumtauIsoMVA3newDMwLT;
bool l2_TighttauIsoMVA3newDMwLT;
bool l2_LoosetauIsoMVA3oldDMwLT;
bool l2_MediumtauIsoMVA3oldDMwLT;
bool l2_TighttauIsoMVA3oldDMwLT;
bool l2_LoosetauIsoMVA3newDMwoLT;
bool l2_MediumtauIsoMVA3newDMwoLT;
bool l2_TighttauIsoMVA3newDMwoLT;
bool l2_LoosetauIsoMVA3oldDMwoLT;
bool l2_MediumtauIsoMVA3oldDMwoLT;
bool l2_TighttauIsoMVA3oldDMwoLT;

bool l2_DecayModeFindingNewDMs;
bool l2_DecayModeFindingOldDMs;

float l2_tauIsoMVAraw3newDMwLTraw;
float l2_tauIsoMVAraw3newDMwoLTraw;
float l2_tauIsoMVAraw3oldDMwLTraw;
float l2_tauIsoMVAraw3oldDMwoLTraw;

bool Trigger_LepTau12;
bool Trigger_MuTau12;
bool Trigger_EleTau12;
bool Trigger_SingleMu12;
bool Trigger_SingleEle12;
bool Trigger_SingleJet12;

bool l1_trgMatche_Ele20Tau20;
bool l1_trgMatche_Mu17Tau20;
bool l1_trgMatche_Mu18Tau25;
bool l1_trgMatche_Mu24;
bool l2_trgMatche_Ele20Tau20;
bool l2_trgMatche_Mu17Tau20;
bool l2_trgMatche_Mu18Tau25;
float gen_Higgs_pt;

bool l1_ConversionVeto;
float l1_dxy_PV;
float l1_dz_PV;
float l2_dxy_PV;
float l2_dz_PV;
int num_gen_jets;

//void fillTreeN(TTree* BG_Tree, int Channel, int subChannel, float HMass, double SVMass, int trRun, int trLumi, int trEvent, float l3Pt, float l3Eta, float l3_CloseJetPt, float l3_CloseJetEta, float l4Pt, float l4Eta, float l4_CloseJetPt, float l4_CloseJetEta, float met, float metPhi, float covMet11, float covMet12, float covMet21, float covMet22, float l3M, float l3Px, float l3Py, float l3Pz, float l4M, float l4Px, float l4Py, float l4Pz, float eff_Correction, float pu_Weight) {
//
//    Channel_ = Channel - 90;
//    subChannel_ = subChannel;
//    HMass_ = HMass;
//    SVMass_ = SVMass;
//    l3Pt_ = l3Pt;
//    l3Eta_ = l3Eta;
//    l3_CloseJetPt_ = l3_CloseJetPt;
//    l3_CloseJetEta_ = l3_CloseJetEta;
//    l4Pt_ = l4Pt;
//    l4Eta_ = l4Eta;
//    l4_CloseJetPt_ = l4_CloseJetPt;
//    l4_CloseJetEta_ = l4_CloseJetEta;
//    Run_ = trRun;
//    Lumi_ = trLumi;
//    Event_ = trEvent;
//
//    met_ = met;
//    metPhi_ = metPhi;
//    covMet11_ = covMet11;
//    covMet12_ = covMet12;
//    covMet21_ = covMet21;
//    covMet22_ = covMet22;
//    l3M_ = l3M;
//    l3Px_ = l3Px;
//    l3Py_ = l3Py;
//    l3Pz_ = l3Pz;
//    l4M_ = l4M;
//    l4Px_ = l4Px;
//    l4Py_ = l4Py;
//    l4Pz_ = l4Pz;
//    eff_Correction_ = eff_Correction;
//    pu_Weight_ = pu_Weight;
//
//    BG_Tree->Fill();
//}


#endif	/* TR_TREE_H */

