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



//#################################################################################################
//############## initializing the PU correction                                    ###############
//#################################################################################################

reweight::LumiReWeighting* LumiWeights_12 = new reweight::LumiReWeighting("interface/Summer12_PU.root", "interface/dataPileUpHistogram_True_2012.root", "mcPU", "pileup");
reweight::LumiReWeighting* LumiWeights_11 = new reweight::LumiReWeighting("interface/Fall11_PU.root", "interface/dataPileUpHistogram_True_2011.root", "mcPU", "pileup");
//    LumiWeights_11 = new reweight::LumiReWeighting("interface/Fall11_PU_observed.root", "interface/dataPileUpHistogram_Observed_2011.root", "mcPU", "pileup"); // Last Bug found in 25 Nov


unsigned int Channel = 0;
unsigned int Run = 0;
unsigned int Lumi = 0;
unsigned int Event = 0;
float IMass = 0;
float mvis = 0;
float HMass = 0;
float met, metphi, mvamet, mvametphi;
float l1M, l1Px, l1Py, l1Pz, l1E, l1Pt, l1Phi, l1Eta, l1Charge, l1_muIso, l1_eleIso, l1_eleMVANonTrg, l1_eleNumHit, l1_tauIsoMVA2raw = -10;
float l2M, l2Px, l2Py, l2Pz, l2E, l2Pt, l2Phi, l2Eta, l2Charge, l2_muIso, l2_eleIso, l2_eleMVANonTrg, l2_eleNumHit, l2_tauIsoMVA2raw, byCombinedIsolationDeltaBetaCorrRaw3Hits_2 = -10;
float l2_RefJetPt, l2_RefJetEta, l2_RefJetPhi = -10;
bool l1_muId_Loose, l1_muId_Tight, l1_eleId_Loose, l1_eleId_Tight, l2_muId_Loose, l2_eleId_Loose, l2_muId_Tight, l2_eleId_Tight;
bool l1_tauIsoL, l1_tauIsoM, l1_tauIsoT, l1_tauRejMuL, l1_tauRejMuM, l1_tauRejMuT, l1_tauRejEleL, l1_tauRejEleM, l1_tauRejEleMVA;
bool l1_tauIso3HitL, l1_tauIso3HitM, l1_tauIso3HitT, l1_tauRejMu2L, l1_tauRejMu2M, l1_tauRejMu2T, l1_tauRejEleMVA3L, l1_tauRejEleMVA3M, l1_tauRejEleMVA3T;
bool l1_tauIsoVL, l1_tauIsoMVA2L, l1_tauIsoMVA2M, l1_tauIsoMVA2T;
bool l2_tauIsoL, l2_tauIsoM, l2_tauIsoT, l2_tauRejMuL, l2_tauRejMuM, l2_tauRejMuT, l2_tauRejEleL, l2_tauRejEleM, l2_tauRejEleMVA;
bool l2_tauIso3HitL, l2_tauIso3HitM, l2_tauIso3HitT, l2_tauRejMu2L, l2_tauRejMu2M, l2_tauRejMu2T, l2_tauRejEleMVA3L, l2_tauRejEleMVA3M, l2_tauRejEleMVA3T;
bool l2_tauIsoVL, l2_tauIsoMVA2L, l2_tauIsoMVA2M, l2_tauIsoMVA2T;

float mvacov00, mvacov01, mvacov10, mvacov11;
float metcov00, metcov01, metcov10, metcov11;
float eff_Correction, pu_Weight;
int num_PV, num_bjet, num_goodjet, npu, mcdata;
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
int nbtag;

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

float bpt;
float beta;
float bphi;
float bdiscriminant;

float bpt_2;
float beta_2;
float bphi_2;
float bdiscriminant_2;

float mjj;
float jdeta;
float jdphi;

float jetpt;
float dijetphi;

void fillTree(std::string ThisChannel, TTree * Run_Tree, myevent *m, std::string is_data_mc, std::string is_mt_et, myobject obj1, myobject obj2) {



    Channel = (ThisChannel == "mutau" ? 1 : 2);
    if (is_data_mc == "mc11") mcdata = 1;
    if (is_data_mc == "data11") mcdata = 2;
    if (is_data_mc == "mc12") mcdata = 3;
    if (is_data_mc == "data12") mcdata = 4;

    //*********************************************************************************************
    //****************************    PileUp re weighting    ***************************************
    //*********************************************************************************************
    int num_PU = 1;
    float PU_Weight = 1;

    if (mcdata == 3) {
        num_PU = m->PUInfo_true;
        PU_Weight = LumiWeights_12->weight(num_PU);
    }
    if (mcdata == 1) {
        //                num_PU = m->PUInfo; // Last Bug found in 25 Nov
        num_PU = m->PUInfo_true;
        PU_Weight = LumiWeights_11->weight(num_PU);
    }
    //*********************************************************************************************



    Run = m->runNumber;
    Lumi = m->lumiNumber;
    Event = m->eventNumber;



    //  ########## ########## ########## ########## ########## ##########
    //  MET Information
    //  ########## ########## ########## ########## ########## ##########
    vector<myobject> MVAMet = m->RecMVAMet;
    vector<myobject> PFMet = m->RecPFMet;
    mvacov00 = m->MVAMet_sigMatrix_00;
    mvacov01 = m->MVAMet_sigMatrix_01;
    mvacov10 = m->MVAMet_sigMatrix_10;
    mvacov11 = m->MVAMet_sigMatrix_11;
    metcov00 = m->MET_sigMatrix_00;
    metcov01 = m->MET_sigMatrix_01;
    metcov10 = m->MET_sigMatrix_10;
    metcov11 = m->MET_sigMatrix_11;

    mvamet = MVAMet.front().pt;
    mvametphi = MVAMet.front().phi;
    met = PFMet.front().pt;
    metphi = PFMet.front().phi;



    //  ########## ########## ########## ########## ########## ##########
    //  Jet Information
    //  ########## ########## ########## ########## ########## ##########
    vector<myobject> JETS = GoodJet30(m, obj1, obj2);
    vector<myobject> BJETS = GoodbJet20(m, obj1, obj2);

    njetpt20 = GoodJet20(m).size();
    njets = JETS.size();
    nbtag = BJETS.size();

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

    bpt = (BJETS.size() > 0 ? BJETS[0].pt : -1000);
    beta = (BJETS.size() > 0 ? BJETS[0].eta : -1000);
    bphi = (BJETS.size() > 0 ? BJETS[0].phi : -1000);
    bdiscriminant = (BJETS.size() > 0 ? BJETS[0].bDiscriminatiors_CSV : -1000);

    bpt_2 = (BJETS.size() > 1 ? BJETS[1].pt : -1000);
    beta_2 = (BJETS.size() > 1 ? BJETS[1].eta : -1000);
    bphi_2 = (BJETS.size() > 1 ? BJETS[1].phi : -1000);
    bdiscriminant_2 = (BJETS.size() > 1 ? BJETS[1].bDiscriminatiors_CSV : -1000);

    mjj = (JETS.size() > 1 ? InvarMass_2(JETS[0], JETS[1]) : -1000);
    jdeta = (JETS.size() > 1 ? JETS[0].eta - JETS[1].eta : -1000);
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

    //  ########## ########## ########## ########## ########## ##########
    //  Tau Information
    //  ########## ########## ########## ########## ########## ##########
    l2M = obj2.mass;
    l2Px = obj2.px;
    l2Py = obj2.py;
    l2E = obj2.E;
    l2Pt = obj2.pt;
    l2Phi = obj2.phi;
    l2Pz = obj2.pz;
    l2Eta = obj2.eta;
    l2_tauIsoMVA2raw = obj2.byIsolationMVA3oldDMwoLTraw;
    byCombinedIsolationDeltaBetaCorrRaw3Hits_2 = obj2.byRawCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIsoVL = obj2.byVLooseCombinedIsolationDeltaBetaCorr;
    l2_tauIsoL = obj2.byLooseCombinedIsolationDeltaBetaCorr;
    l2_tauIso3HitL = obj2.byLooseCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIsoMVA2L = obj2.byLooseIsolationMVA3oldDMwoLT;
    l2_tauIsoM = obj2.byMediumCombinedIsolationDeltaBetaCorr;
    l2_tauIso3HitM = obj2.byMediumCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIsoMVA2M = obj2.byMediumIsolationMVA3oldDMwoLT;
    l2_tauIsoT = obj2.byTightCombinedIsolationDeltaBetaCorr;
    l2_tauIso3HitT = obj2.byTightCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIsoMVA2T = obj2.byTightIsolationMVA3oldDMwoLT;
    l2_tauRejMuL = obj2.discriminationByMuonLoose;
    l2_tauRejMu2L = obj2.discriminationByMuonLoose2;
    l2_tauRejMuM = obj2.discriminationByMuonMedium;
    l2_tauRejMu2M = obj2.discriminationByMuonMedium2;
    l2_tauRejMuT = obj2.discriminationByMuonTight;
    l2_tauRejMu2T = obj2.discriminationByMuonTight2;
    l2_tauRejEleL = obj2.discriminationByElectronLoose;
    l2_tauRejEleM = obj2.discriminationByElectronMedium;
    l2_tauRejEleMVA = obj2.discriminationByMVA5rawElectronRejection;
    l2_tauRejEleMVA3L = obj2.discriminationByElectronMVA5Loose;
    l2_tauRejEleMVA3M = obj2.discriminationByElectronMVA5Medium;
    l2_tauRejEleMVA3T = obj2.discriminationByElectronMVA5Tight;
    //    l2_tauRejEleMVA = obj2.discriminationByElectronMVA;
    //    l2_tauRejEleMVA3L = obj2.discriminationByElectronMVA3Loose;
    //    l2_tauRejEleMVA3M = obj2.discriminationByElectronMVA3Medium;
    //    l2_tauRejEleMVA3T = obj2.discriminationByElectronMVA3Tight;
    l2Charge = obj2.charge;
    l2_CloseJetPt = Find_Closet_Jet(obj2, m)[0];
    l2_CloseJetEta = Find_Closet_Jet(obj2, m)[1];
    l2_CloseJetPhi = Find_Closet_Jet(obj2, m)[2];
    l2_RefJetPt = obj2.jetPt;
    l2_RefJetEta = obj2.jetEta;
    l2_RefJetPhi = obj2.jetPhi;

    //  ########## ########## ########## ########## ########## ##########
    //  Other Information
    //  ########## ########## ########## ########## ########## ##########
    rho = m->Rho;
    eff_Correction = getCorrFactor(ThisChannel, is_data_mc, obj1, obj2, obj2);
    mu_Size = myCleanLepton(m, "mu").size();
    BareMuon_Size = myCleanBareLepton(m, "mu").size();
    electron_Size = myCleanLepton(m, "ele").size();
    BareElectron_Size = myCleanBareLepton(m, "ele").size();
    tau_Size = myCleanLepton(m, "tau").size();
    BareTau_Size = myCleanBareLepton(m, "tau").size();
    vector<myobject> Vertex = m->Vertex;
    num_PV = Vertex.size();
    pu_Weight = PU_Weight;
    npu = m->PUInfo_true;
    mvis = InvarMass_2(obj1, obj2);
    mt_1 = TMass(obj1, MVAMet.front());
    mt_2 = TMass(obj2, MVAMet.front());

    idweight_1 = getCorrIdIsoLep(ThisChannel, is_data_mc, obj2);
    trigweight_1 = getCorrTriggerLep(ThisChannel, is_data_mc, obj1);
    trigweight_2 = getCorrTriggerTau(ThisChannel, is_data_mc, obj2);

    Run_Tree->Fill();
}



#endif	/* ZH_TREE_H */

