/*
 * File:   zh_Tree.h
 * Author: abdollah
 *
 * Created on February 6, 2013, 1:56 PM
 */

#ifndef ZH_TREE_H
#define	ZH_TREE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "myevent.h"
#include "myobject.h"
#include "Leptons_IdIso.h"
#include "zh_Auxiliary.h"
#include "Leptons_PreSelection.h"
#include "zh_Functions.h"
#include "LumiReweightingStandAlone.h"
//#include "LumiReweighting.h"
#include "htt_Trigger.h"



//#################################################################################################
//############## initializing the PU correction                                    ###############
//#################################################################################################

//reweight::LumiReWeighting* LumiWeights_12 = new reweight::LumiReWeighting("interface/HTTRootFiles/MC_Summer12_PU_S10-600bins.root", "interface/HTTRootFiles/Data_Pileup_2012_ReReco-600bins.root", "pileup", "pileup");
reweight::LumiReWeighting* LumiWeights_12 = new reweight::LumiReWeighting("interface/HTTRootFiles/MC_Summer12_PU_S10-600bins.root", "interface/HTTRootFiles/Data_Pileup_2012_ReRecoPixel-600bins.root", "pileup", "pileup");
reweight::LumiReWeighting* LumiWeights_11 = new reweight::LumiReWeighting("interface/HTTRootFiles/MC_Fall11_PU_S6-500bins.root", "interface/HTTRootFiles/Data_Pileup_2011_HCP-500bins.root", "pileup", "pileup");
//reweight::LumiReWeighting* LumiWeights_12 = new reweight::LumiReWeighting("interface/Summer12_PU.root", "interface/dataPileUpHistogram_True_2012.root", "mcPU", "pileup"); //changed to be sync with HTT
//reweight::LumiReWeighting* LumiWeights_11 = new reweight::LumiReWeighting("interface/Fall11_PU.root", "interface/dataPileUpHistogram_True_2011.root", "mcPU", "pileup");  //changed to be sync with HTT
//    LumiWeights_11 = new reweight::LumiReWeighting("interface/Fall11_PU_observed.root", "interface/dataPileUpHistogram_Observed_2011.root", "mcPU", "pileup"); // Last Bug found in 25 Nov


int Channel = 0;
int Run = 0;
int Lumi = 0;
int Event = 0;
//unsigned int Channel = 0;
//unsigned int Run = 0;
//unsigned int Lumi = 0;
//unsigned int Event = 0;
float mvis = 0;
float met, metphi, mvamet, mvametphi, mvametNoRecoil, mvametphiNoRecoil;
float l1M, l1Px, l1Py, l1Pz, l1E, l1Pt, l1Phi, l1Eta, l1Eta_SC, l1Charge, l1_muIso, l1_eleIso, l1_eleMVANonTrg, l1_eleNumHit, l1_tauIsoMVA2raw, l1_d0, l1_dZ_in = -10;
float l2M, l2Px, l2Py, l2Pz, l2E, l2Pt, l2Phi, l2Eta, l2Charge, l2_muIso, l2_eleIso, l2_eleMVANonTrg, l2_eleNumHit, l2_tauIsoMVA2raw, byCombinedIsolationDeltaBetaCorrRaw3Hits_2 = -10;
float l2_RefJetPt, l2_RefJetEta, l2_RefJetPhi = -10;
bool l1_muId_Loose, l1_muId_Tight, l1_eleId_Loose, l1_eleId_Tight, l2_muId_Loose, l2_eleId_Loose, l2_muId_Tight, l2_eleId_Tight;
bool l2_discriminationByMuonMVALoose, l2_discriminationByMuonMVAMedium, l2_discriminationByMuonMVATight;
float l2_discriminationByMuonMVAraw;

bool l1_tauIsoL, l1_tauIsoM, l1_tauIsoT, l1_tauRejMuL, l1_tauRejMuM, l1_tauRejMuT, l1_tauRejEleL, l1_tauRejEleM, l1_tauRejEleMVA;
bool l1_tauIso3HitL, l1_tauIso3HitM, l1_tauIso3HitT, l1_tauRejMu2L, l1_tauRejMu2M, l1_tauRejMu2T, l1_tauRejEleMVA3L, l1_tauRejEleMVA3M, l1_tauRejEleMVA3T;
bool l1_tauIsoVL, l1_tauIsoMVA2L, l1_tauIsoMVA2M, l1_tauIsoMVA2T;
bool l2_tauIsoL, l2_tauIsoM, l2_tauIsoT, l2_tauRejMuL, l2_tauRejMuM, l2_tauRejMuT, l2_tauRejEleL, l2_tauRejEleM, l2_tauRejEleMVA;
bool l2_tauIso3HitL, l2_tauIso3HitM, l2_tauIso3HitT, l2_tauRejMu3L, l2_tauRejMu2L, l2_tauRejMu2M, l2_tauRejMu3T, l2_tauRejMu2T, l2_tauRejEleMVA3L, l2_tauRejEleMVA3M, l2_tauRejEleMVA3T;
bool l2_tauIsoVL, l2_tauIsoMVA2L, l2_tauIsoMVA2M, l2_tauIsoMVA2T, l2_DecayModeFinding;
int l2_DecayMode;

//float mvacov00_mutau, mvacov01_mutau, mvacov10_mutau, mvacov11_mutau;
//float mvacov00_etau, mvacov01_etau, mvacov10_etau, mvacov11_etau;
float mvacov00, mvacov01, mvacov10, mvacov11;
float metcov00, metcov01, metcov10, metcov11;
float eff_Correction, pu_Weight, pu_Weight_old;
int num_PV, mcdata;
int npu;
//float npu;
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

float bpt_2;
float beta_2;
float bphi_2;
float bdiscriminant_2;

float loosebpt_1;
float loosebeta_1;
float loosebphi_1;
float loosebdiscriminant_1;

float loosebpt_2;
float loosebeta_2;
float loosebphi_2;
float loosebdiscriminant_2;

float mjj;
float jdeta;
float jdphi;

float jetpt;
float dijetphi;
int zCategory = -10;
double SVMass, SVMassUnc;
double SVMassUp, SVMassUncUp;
double SVMassDown, SVMassUncDown;
float embedWeight = 1;


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

//bool Trigger_LepTau12;
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
float gen_Higgs_pt = -10; //changed 23Sep;

bool l1_ConversionVeto;
float l1_dxy_PV;
float l1_dz_PV;
float l2_dxy_PV;
float l2_dz_PV;
int num_gen_jets;
int l1_Index, l2_Index;

void fillTree(unsigned int chnl, TTree * Run_Tree, myevent *m, std::string is_data_mc, std::string FinalState, myobject obj1, myobject obj2) {



    //    if (FinalState == "mutau") Channel = 1;
    //    else if (FinalState == "eltau") Channel = 2;
    //    else Channel = 3;
    Channel = chnl;

    if (is_data_mc == "mc11") mcdata = 1;
    if (is_data_mc == "data11") mcdata = 2;
    if (is_data_mc == "mc12") mcdata = 3;
    if (is_data_mc == "data12") mcdata = 4;
    if (is_data_mc == "embeddata12") mcdata = 5;
    if (is_data_mc == "embedmc12") mcdata = 6;
    bool isdata = (mcdata == 2 || mcdata == 4 || mcdata == 5);
    bool is2012 = (mcdata == 3 || mcdata == 4 || mcdata == 5 || mcdata == 6);

    //*********************************************************************************************
    //****************************    PileUp re weighting    ***************************************
    //*********************************************************************************************
    float num_PU = 1;
    float PU_Weightold = 1;

    if (mcdata == 3) {
        num_PU = m->PUInfo_true;
        PU_Weightold = LumiWeights_12->weight(num_PU);
    }
    if (mcdata == 1) {
        //                num_PU = m->PUInfo; // Last Bug found in 25 Nov
        num_PU = m->PUInfo_true;
        PU_Weightold = LumiWeights_11->weight(num_PU);
    }
    //*********************************************************************************************


    Run = m->runNumber;
    Lumi = m->lumiNumber;
    Event = m->eventNumber;
    embedWeight = m->embeddingWeight;


    //  ########## ########## ########## ########## ########## ##########
    //  MET Information
    //  ########## ########## ########## ########## ########## ##########
    vector<myobject> MVAMetRecoil_mutau;
    vector<myobject> MVAMetRecoil_etau;
    if (isdata) {
        MVAMetRecoil_mutau = m->PairMet_mutau;
        MVAMetRecoil_etau = m->PairMet_etau;
    } else {
        MVAMetRecoil_mutau = m->PairRecoilMet_mutau;
        MVAMetRecoil_etau = m->PairRecoilMet_etau;
    }
    vector<myobject> MVAMetNORecoil_mutau = m->PairMet_mutau;
    vector<myobject> MVAMetNORecoil_etau = m->PairMet_etau;


    l1_Index = obj1.gen_index;
    l2_Index = obj2.gen_index;
    int pairIndex = obj1.gen_index * 10 + obj2.gen_index;


    if (FinalState == "mutau") {
        mvamet = MVAMetRecoil_mutau[pairIndex].pt;
        mvametphi = MVAMetRecoil_mutau[pairIndex].phi;
        mvametNoRecoil = MVAMetNORecoil_mutau[pairIndex].pt;
        mvametphiNoRecoil = MVAMetNORecoil_mutau[pairIndex].phi;

        mt_1 = TMass(obj1, MVAMetRecoil_mutau[pairIndex]);
        mt_2 = TMass(obj2, MVAMetRecoil_mutau[pairIndex]);
    }
    if (FinalState == "eltau") {
        mvamet = MVAMetRecoil_etau[pairIndex].pt;
        mvametphi = MVAMetRecoil_etau[pairIndex].phi;
        mvametNoRecoil = MVAMetNORecoil_etau[pairIndex].pt;
        mvametphiNoRecoil = MVAMetNORecoil_etau[pairIndex].phi;
        mt_1 = TMass(obj1, MVAMetRecoil_etau[pairIndex]);
        mt_2 = TMass(obj2, MVAMetRecoil_etau[pairIndex]);
    }
    mvacov00 = m->MVAMet_sigMatrix_00;
    mvacov01 = m->MVAMet_sigMatrix_01;
    mvacov10 = m->MVAMet_sigMatrix_10;
    mvacov11 = m->MVAMet_sigMatrix_11;
    //PFMet
    vector<myobject> PFMet = m->RecPFMet;
    met = PFMet.front().pt;
    metphi = PFMet.front().phi;
    metcov00 = m->MET_sigMatrix_00;
    metcov01 = m->MET_sigMatrix_01;
    metcov10 = m->MET_sigMatrix_10;
    metcov11 = m->MET_sigMatrix_11;

    //  ########## ########## ########## ########## ########## ##########
    //  Jet Information
    //  ########## ########## ########## ########## ########## ##########
    vector<myobject> JETS = GoodJet30(m, obj1, obj2);
    vector<myobject> BJETS = GoodbJet20(m, obj1, obj2, isdata, is2012);
    vector<myobject> BLooseJETS = GoodLoosebJet20(m, obj1, obj2);
    vector<myobject> BJETSNoCor = GoodbJet20NoCor(m, obj1, obj2);

    njetpt20 = GoodJet20(m).size();
    njets = JETS.size();
    nbtag = BJETS.size();
    nbtagLoose = BLooseJETS.size();
    nbtagNoCor = BJETSNoCor.size();

    jpt_1 = (JETS.size() > 0 ? JETS[0].pt : -1000);
    jeta_1 = (JETS.size() > 0 ? JETS[0].eta : -1000);
    jphi_1 = (JETS.size() > 0 ? JETS[0].phi : -1000);
    jE_1 = (JETS.size() > 0 ? JETS[0].E : -1000);
    jpass_1 = (JETS.size() > 0 ? JETS[0].puJetIdLoose : -1000);

    jpt_2 = (JETS.size() > 1 ? JETS[1].pt : -1000);
    jeta_2 = (JETS.size() > 1 ? JETS[1].eta : -1000);
    jphi_2 = (JETS.size() > 1 ? JETS[1].phi : -1000);
    jE_2 = (JETS.size() > 1 ? JETS[1].E : -1000);
    jpass_2 = (JETS.size() > 1 ? JETS[1].puJetIdLoose : -1000);

    bpt_1 = (BJETS.size() > 0 ? BJETS[0].pt : -1000);
    beta_1 = (BJETS.size() > 0 ? BJETS[0].eta : -1000);
    bphi_1 = (BJETS.size() > 0 ? BJETS[0].phi : -1000);
    bdiscriminant_1 = (BJETS.size() > 0 ? BJETS[0].bDiscriminatiors_CSV : -1000);

    bpt_2 = (BJETS.size() > 1 ? BJETS[1].pt : -1000);
    beta_2 = (BJETS.size() > 1 ? BJETS[1].eta : -1000);
    bphi_2 = (BJETS.size() > 1 ? BJETS[1].phi : -1000);
    bdiscriminant_2 = (BJETS.size() > 1 ? BJETS[1].bDiscriminatiors_CSV : -1000);

    loosebpt_1 = (BLooseJETS.size() > 0 ? BLooseJETS[0].pt : -1000);
    loosebeta_1 = (BLooseJETS.size() > 0 ? BLooseJETS[0].eta : -1000);
    loosebphi_1 = (BLooseJETS.size() > 0 ? BLooseJETS[0].phi : -1000);
    loosebdiscriminant_1 = (BLooseJETS.size() > 0 ? BLooseJETS[0].bDiscriminatiors_CSV : -1000);

    loosebpt_2 = (BLooseJETS.size() > 1 ? BLooseJETS[1].pt : -1000);
    loosebeta_2 = (BLooseJETS.size() > 1 ? BLooseJETS[1].eta : -1000);
    loosebphi_2 = (BLooseJETS.size() > 1 ? BLooseJETS[1].phi : -1000);
    loosebdiscriminant_2 = (BLooseJETS.size() > 1 ? BLooseJETS[1].bDiscriminatiors_CSV : -1000);

    mjj = (JETS.size() > 1 ? InvarMass_2(JETS[0], JETS[1]) : -1000);
    jdeta = (JETS.size() > 1 ? fabs(JETS[0].eta - JETS[1].eta) : -1000);
    jdphi = (JETS.size() > 1 ? deltaPhi(JETS[0], JETS[1]) : -1000);
    TLorentzVector LorJet1;
    TLorentzVector LorJet2;
    TLorentzVector LorJetTot;
    if (JETS.size() > 1) {
        LorJet1.SetPtEtaPhiE(JETS[0].pt, JETS[0].eta, JETS[0].phi, JETS[0].E);
        LorJet2.SetPtEtaPhiE(JETS[1].pt, JETS[1].eta, JETS[1].phi, JETS[1].E);
        LorJetTot = LorJet1 + LorJet2;
    }
    jetpt = (JETS.size() > 1 ? LorJetTot.Pt() : -1000);
    dijetphi = (JETS.size() > 1 ? LorJetTot.Phi() : -1000);






    //  ########## ########## ########## ########## ########## ##########
    //  Lepton Information
    //  ########## ########## ########## ########## ########## ##########
    l1M = obj1.mass;
    l1Px = obj1.px;
    l1Py = obj1.py;
    l1Pz = obj1.pz;
    l1E = obj1.E;
    l1Pt = obj1.pt;
    l1Phi = obj1.phi;
    l1Eta = obj1.eta;
    l1Eta_SC = obj1.eta_SC;
    l1_muId_Loose = Id_Mu_Loose(obj1);
    l1_muId_Tight = Id_Mu_Tight(obj1);
    l1_muIso = Iso_Mu_dBeta(obj1);
    l1_eleId_Loose = EleMVANonTrigId_Loose(obj1);
    l1_eleId_Tight = EleMVANonTrigId_Tight(obj1);
    l1_eleIso = Iso_Ele_dBeta(obj1);
    l1_eleMVANonTrg = obj1.Id_mvaNonTrg;
    l1_eleNumHit = obj1.numHitEleInner;
    l1Charge = obj1.charge;
    l1_CloseJetPt = Find_Closet_Jet(obj1, m)[0];
    l1_CloseJetEta = Find_Closet_Jet(obj1, m)[1];
    l1_CloseJetPhi = Find_Closet_Jet(obj1, m)[2];
    l1_d0 = obj1.d0;
    l1_dZ_in = obj1.dZ_in; //the impact parameter in the transverse plane
    l1_ConversionVeto = obj1.passConversionVeto;
    l1_dxy_PV = obj1.dxy_PV; //the impact parameter in the transverse plane
    l1_dz_PV = obj1.dz_PV; //the impact parameter in the transverse plane

    l1_trgMatche_Ele20Tau20 = obj1.hasTrgObject_Ele20Tau20;
    l1_trgMatche_Mu17Tau20 = obj1.hasTrgObject_Mu17Tau20;
    l1_trgMatche_Mu18Tau25 = obj1.hasTrgObject_Mu18Tau25;
    l1_trgMatche_Mu24 = obj1.hasTrgObject_Mu24;
    //  ########## ########## ########## ########## ########## ##########
    //  Tau Information
    //  ########## ########## ########## ########## ########## ##########
    l2M = obj2.mass;
    l2Px = obj2.px;
    l2Py = obj2.py;
    l2E = obj2.E;
    l2Pt = obj2.pt * 1.01; // Due to tau ES Correction for ALL DM ?????
    l2Phi = obj2.phi;
    l2Pz = obj2.pz;
    l2Eta = obj2.eta;

    l2_DecayMode = obj2.decayMode;
    l2_DecayModeFinding = obj2.discriminationByDecayModeFinding;
    l2_DecayModeFindingNewDMs = obj2.discriminationByDecayModeFindingNewDMs;
    l2_DecayModeFindingOldDMs = obj2.discriminationByDecayModeFindingOldDMs;

    l2_tauIsoMVAraw3newDMwLTraw = obj2.byIsolationMVA3newDMwLTraw;
    l2_tauIsoMVAraw3newDMwoLTraw = obj2.byIsolationMVA3newDMwoLTraw;
    l2_tauIsoMVAraw3oldDMwLTraw = obj2.byIsolationMVA3oldDMwLTraw;
    l2_tauIsoMVAraw3oldDMwoLTraw = obj2.byIsolationMVA3oldDMwoLTraw;

    l2_tauIso3HitL = obj2.byLooseCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIso3HitM = obj2.byMediumCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIso3HitT = obj2.byTightCombinedIsolationDeltaBetaCorr3Hits;
    byCombinedIsolationDeltaBetaCorrRaw3Hits_2 = obj2.byRawCombinedIsolationDeltaBetaCorr3Hits;

    l2_LoosetauIsoMVA3newDMwLT = obj2.byLooseIsolationMVA3newDMwLT;
    l2_MediumtauIsoMVA3newDMwLT = obj2.byMediumIsolationMVA3newDMwLT;
    l2_TighttauIsoMVA3newDMwLT = obj2.byTightIsolationMVA3newDMwLT;
    l2_LoosetauIsoMVA3oldDMwLT = obj2.byLooseIsolationMVA3oldDMwLT;
    l2_MediumtauIsoMVA3oldDMwLT = obj2.byMediumIsolationMVA3oldDMwLT;
    l2_TighttauIsoMVA3oldDMwLT = obj2.byTightIsolationMVA3oldDMwLT;
    l2_LoosetauIsoMVA3newDMwoLT = obj2.byLooseIsolationMVA3newDMwoLT;
    l2_MediumtauIsoMVA3newDMwoLT = obj2.byMediumIsolationMVA3newDMwoLT;
    l2_TighttauIsoMVA3newDMwoLT = obj2.byTightIsolationMVA3newDMwoLT;
    l2_LoosetauIsoMVA3oldDMwoLT = obj2.byLooseIsolationMVA3oldDMwoLT;
    l2_MediumtauIsoMVA3oldDMwoLT = obj2.byMediumIsolationMVA3oldDMwoLT;
    l2_TighttauIsoMVA3oldDMwoLT = obj2.byTightIsolationMVA3oldDMwoLT;


    l2_tauRejMu3L = obj2.discriminationByMuonLoose3;
    l2_tauRejMu2M = obj2.discriminationByMuonMedium2; // it is 2???
    l2_tauRejMu3T = obj2.discriminationByMuonTight3;
    l2_discriminationByMuonMVALoose = obj2.discriminationByMuonMVALoose;
    l2_discriminationByMuonMVAMedium = obj2.discriminationByMuonMVAMedium;
    l2_discriminationByMuonMVATight = obj2.discriminationByMuonMVATight;
    l2_discriminationByMuonMVAraw = obj2.discriminationByMuonMVAraw;

    l2_tauRejEleL = obj2.discriminationByElectronLoose;
    l2_tauRejEleM = obj2.discriminationByElectronMedium;
    l2_tauRejEleMVA = obj2.discriminationByMVA5rawElectronRejection;
    l2_tauRejEleMVA3L = obj2.discriminationByElectronMVA5Loose;
    l2_tauRejEleMVA3M = obj2.discriminationByElectronMVA5Medium;
    l2_tauRejEleMVA3T = obj2.discriminationByElectronMVA5Tight;

    l2Charge = obj2.charge;
    l2_CloseJetPt = Find_Closet_Jet(obj2, m)[0];
    l2_CloseJetEta = Find_Closet_Jet(obj2, m)[1];
    l2_CloseJetPhi = Find_Closet_Jet(obj2, m)[2];
    l2_RefJetPt = obj2.jetPt;
    l2_RefJetEta = obj2.jetEta;
    l2_RefJetPhi = obj2.jetPhi;
    l2_trgMatche_Ele20Tau20 = obj2.hasTrgObject_Ele20Tau20;
    l2_trgMatche_Mu17Tau20 = obj2.hasTrgObject_Mu17Tau20;
    l2_trgMatche_Mu18Tau25 = obj2.hasTrgObject_Mu18Tau25;

    l2_dxy_PV = obj2.dxy_PV; //the impact parameter in the transverse plane
    l2_dz_PV = obj2.dz_PV; //the impact parameter in the transverse plane

    //  ########## ########## ########## ########## ########## ##########
    //  Other Information
    //  ########## ########## ########## ########## ########## ##########
    rho = m->Rho;
    eff_Correction = getCorrFactor(FinalState, is_data_mc, obj1, obj2, obj2);
    vector<myobject> Vertex = m->Vertex;
    num_PV = Vertex.size();
    pu_Weight_old = PU_Weightold;
    pu_Weight = m->PU_Weight;
    cout <<pu_Weight_old << "   vs   " << pu_Weight  << "  del="<<pu_Weight_old-pu_Weight<<"\n";
    npu = m->PUInfo_true;
    mvis = InvarMass_2(obj1, obj2);


    idweight_1 = getCorrIdIsoLep(FinalState, is_data_mc, obj2);
    trigweight_1 = getCorrTriggerLep(FinalState, is_data_mc, obj1);
    trigweight_2 = getCorrTriggerTau(FinalState, is_data_mc, obj2);
    //    zCategory = 0;
    zCategory = ZCategory(m, obj1, obj2);
    //    gen_Higgs_pt = get_gen_Higgs_pt(m);

    //  ########## ########## ########## ########## ########## ##########
    //  Trigger
    //  ########## ########## ########## ########## ########## ##########

    //    Trigger_LepTau12 = Trigger_12(m);
    Trigger_MuTau12 = Trigger_MuTau_12(m);
    Trigger_EleTau12 = Trigger_EleTau_12(m);
    Trigger_SingleMu12 = Trigger_SingleMu_12(m);
    Trigger_SingleEle12 = Trigger_SingleEle_12(m);
    Trigger_SingleJet12 = Trigger_SingleJet_12(m);


    //  ########## ########## ########## ########## ########## ##########
    //  GEN Info
    //  ########## ########## ########## ########## ########## ##########

    int genParticleStatus3 = 0;

    vector<myGenobject> genPar = m->RecGenParticle;
    for (int gg = 0; gg < genPar.size(); gg++) {

        // HIggs Pt
        if (genPar[gg].status == 3 && (fabs(genPar[gg].pdgId) == 25 || fabs(genPar[gg].pdgId) == 35 || fabs(genPar[gg].pdgId) == 36)) gen_Higgs_pt = genPar[gg].pt;

        // NumGen Jets
        if (fabs(genPar[gg].status) == 3) genParticleStatus3++;

    }



    num_gen_jets = genParticleStatus3 - 9;





    Run_Tree->Fill();
}



#endif	/* ZH_TREE_H */

