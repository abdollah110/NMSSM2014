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





unsigned int Channel = 0;
unsigned int Run = 0;
unsigned int Lumi = 0;
unsigned int Event = 0;
float IMass = 0;
float ZMass = 0;
float HMass = 0;
float met, metPhi, pfmet, pfmetPhi;
float l1M, l1Px, l1Py, l1Pz, l1E, l1Pt, l1Phi, l1Eta, l1Charge, l1_muIso, l1_eleIso, l1_eleMVANonTrg, l1_eleNumHit, l1_tauIsoMVA2raw = -10;
float l2M, l2Px, l2Py, l2Pz, l2E, l2Pt, l2Phi, l2Eta, l2Charge, l2_muIso, l2_eleIso, l2_eleMVANonTrg, l2_eleNumHit, l2_tauIsoMVA2raw = -10;
float l1_RefJetPt, l1_RefJetEta, l1_RefJetPhi = -10;
float l2_RefJetPt, l2_RefJetEta, l2_RefJetPhi = -10;
bool l1_muId_Loose, l1_muId_Tight, l1_eleId_Loose, l1_eleId_Tight, l2_muId_Loose, l2_eleId_Loose, l2_muId_Tight, l2_eleId_Tight;
bool l1_tauIsoL, l1_tauIsoM, l1_tauIsoT, l1_tauRejMuL, l1_tauRejMuM, l1_tauRejMuT, l1_tauRejEleL, l1_tauRejEleM, l1_tauRejEleMVA;
bool l1_tauIso3HitL, l1_tauIso3HitM, l1_tauIso3HitT, l1_tauRejMu2L, l1_tauRejMu2M, l1_tauRejMu2T, l1_tauRejEleMVA3L, l1_tauRejEleMVA3M, l1_tauRejEleMVA3T;
bool l1_tauIsoVL, l1_tauIsoMVA2L, l1_tauIsoMVA2M, l1_tauIsoMVA2T;
bool l2_tauIsoL, l2_tauIsoM, l2_tauIsoT, l2_tauRejMuL, l2_tauRejMuM, l2_tauRejMuT, l2_tauRejEleL, l2_tauRejEleM, l2_tauRejEleMVA;
bool l2_tauIso3HitL, l2_tauIso3HitM, l2_tauIso3HitT, l2_tauRejMu2L, l2_tauRejMu2M, l2_tauRejMu2T, l2_tauRejEleMVA3L, l2_tauRejEleMVA3M, l2_tauRejEleMVA3T;
bool l2_tauIsoVL, l2_tauIsoMVA2L, l2_tauIsoMVA2M, l2_tauIsoMVA2T;

float covMet11, covMet12, covMet21, covMet22;
float pfcovMet11, pfcovMet12, pfcovMet21, pfcovMet22;
float eff_Correction, pu_Weight;
int num_PV, num_bjet, num_goodjet;
int mu_Size, BareMuon_Size, electron_Size, BareElectron_Size, tau_Size, BareTau_Size;
int mu_partTight_Size, ele_partTight_Size;
float l1_CloseJetPt, l2_CloseJetPt;
float l1_CloseJetEta, l2_CloseJetEta;
float l1_CloseJetPhi, l2_CloseJetPhi;

void fillTree(TTree * Run_Tree, myevent *m, float cor_eff, float PU_Weight, int channel, myobject obj1, myobject obj2) {



    vector<myobject> Met = m->RecMVAMet;
    vector<myobject> PFMet = m->RecPFMet;
    Channel = channel;
    Run = m->runNumber;
    Lumi = m->lumiNumber;
    Event = m->eventNumber;
    ZMass = InvarMass_2(obj1, obj2);
    covMet11 = m->MVAMet_sigMatrix_00;
    covMet12 = m->MVAMet_sigMatrix_01;
    covMet21 = m->MVAMet_sigMatrix_10;
    covMet22 = m->MVAMet_sigMatrix_11;
    pfcovMet11 = m->MET_sigMatrix_00;
    pfcovMet12 = m->MET_sigMatrix_01;
    pfcovMet21 = m->MET_sigMatrix_10;
    pfcovMet22 = m->MET_sigMatrix_11;
    met = Met.front().pt;
    pfmet = PFMet.front().pt;
    metPhi = Met.front().phi;
    pfmetPhi = PFMet.front().phi;
    eff_Correction = cor_eff;
    mu_Size = myCleanLepton(m, "mu").size();
    BareMuon_Size = myCleanBareLepton(m, "mu").size();
    electron_Size = myCleanLepton(m, "ele").size();
    BareElectron_Size = myCleanBareLepton(m, "ele").size();
    tau_Size = myCleanLepton(m, "tau").size();
    BareTau_Size = myCleanBareLepton(m, "tau").size();
    mu_partTight_Size = LeptonSubSet(m, "mu_tight_partly").size();
    ele_partTight_Size = LeptonSubSet(m, "ele_tight_partly").size();
    vector<myobject> Vertex = m->Vertex;
    num_PV = Vertex.size();
    num_bjet = bjet_Multiplicity(m);
    pu_Weight = PU_Weight;
    num_goodjet = GoodJet(m).size();

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
    l1_tauIsoMVA2raw = obj1.byIsolationMVA2raw;
    l1_tauIsoVL = obj1.byVLooseCombinedIsolationDeltaBetaCorr;
    l1_tauIsoL = obj1.byLooseCombinedIsolationDeltaBetaCorr;
    l1_tauIso3HitL = obj1.byLooseCombinedIsolationDeltaBetaCorr3Hits;
    l1_tauIsoMVA2L = obj1.byLooseIsolationMVA2;
    l1_tauIsoM = obj1.byMediumCombinedIsolationDeltaBetaCorr;
    l1_tauIso3HitM = obj1.byMediumCombinedIsolationDeltaBetaCorr3Hits;
    l1_tauIsoMVA2M = obj1.byMediumIsolationMVA2;
    l1_tauIsoT = obj1.byTightCombinedIsolationDeltaBetaCorr;
    l1_tauIso3HitT = obj1.byTightCombinedIsolationDeltaBetaCorr3Hits;
    l1_tauIsoMVA2T = obj1.byTightIsolationMVA2;
    l1_tauRejMuL = obj1.discriminationByMuonLoose;
    l1_tauRejMu2L = obj1.discriminationByMuonLoose2;
    l1_tauRejMuM = obj1.discriminationByMuonMedium;
    l1_tauRejMu2M = obj1.discriminationByMuonMedium2;
    l1_tauRejMuT = obj1.discriminationByMuonTight;
    l1_tauRejMu2T = obj1.discriminationByMuonTight2;
    l1_tauRejEleL = obj1.discriminationByElectronLoose;
    l1_tauRejEleM = obj1.discriminationByElectronMedium;
    l1_tauRejEleMVA = obj1.discriminationByElectronMVA5Loose;
    l1_tauRejEleMVA3L = obj1.discriminationByElectronMVA5Loose;
    l1_tauRejEleMVA3M = obj1.discriminationByElectronMVA5Medium;
    l1_tauRejEleMVA3T = obj1.discriminationByElectronMVA5Tight;
    //    l1_tauRejEleMVA3T = obj1.discriminationByElectronMVA3Tight;
    l1Charge = obj1.charge;
    l1_CloseJetPt = Find_Closet_Jet(obj1, m)[0];
    l1_CloseJetEta = Find_Closet_Jet(obj1, m)[1];
    l1_CloseJetPhi = Find_Closet_Jet(obj1, m)[2];
    l1_RefJetPt = obj1.jetPt;
    l1_RefJetEta = obj1.jetEta;
    l1_RefJetPhi = obj1.jetPhi;

    l2M = obj2.mass;
    l2Px = obj2.px;
    l2Py = obj2.py;
    l2E = obj2.E;
    l2Pt = obj2.pt;
    l2Phi = obj2.phi;
    l2Pz = obj2.pz;
    l2Eta = obj2.eta;
    l2_muId_Loose = Id_Mu_Loose(obj2);
    l2_muId_Tight = Id_Mu_Tight(obj2);
    l2_muIso = Iso_Mu_dBeta(obj2);
    l2_eleId_Loose = EleMVANonTrigId_Loose(obj2);
    l2_eleId_Tight = EleMVANonTrigId_Tight(obj2);
    l2_eleIso = Iso_Ele_dBeta(obj2);
    l2_eleMVANonTrg = obj2.Id_mvaNonTrg;
    l2_eleNumHit = obj2.numHitEleInner;
    l2_tauIsoMVA2raw = obj2.byIsolationMVA2raw;
    l2_tauIsoVL = obj2.byVLooseCombinedIsolationDeltaBetaCorr;
    l2_tauIsoL = obj2.byLooseCombinedIsolationDeltaBetaCorr;
    l2_tauIso3HitL = obj2.byLooseCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIsoMVA2L = obj2.byLooseIsolationMVA2;
    l2_tauIsoM = obj2.byMediumCombinedIsolationDeltaBetaCorr;
    l2_tauIso3HitM = obj2.byMediumCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIsoMVA2M = obj2.byMediumIsolationMVA2;
    l2_tauIsoT = obj2.byTightCombinedIsolationDeltaBetaCorr;
    l2_tauIso3HitT = obj2.byTightCombinedIsolationDeltaBetaCorr3Hits;
    l2_tauIsoMVA2T = obj2.byTightIsolationMVA2;
    l2_tauRejMuL = obj2.discriminationByMuonLoose;
    l2_tauRejMu2L = obj2.discriminationByMuonLoose2;
    l2_tauRejMuM = obj2.discriminationByMuonMedium;
    l2_tauRejMu2M = obj2.discriminationByMuonMedium2;
    l2_tauRejMuT = obj2.discriminationByMuonTight;
    l2_tauRejMu2T = obj2.discriminationByMuonTight2;
    l2_tauRejEleL = obj2.discriminationByElectronLoose;
    l2_tauRejEleM = obj2.discriminationByElectronMedium;
    l2_tauRejEleMVA = obj2.discriminationByElectronMVA5Loose;
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




    Run_Tree->Fill();
}



#endif	/* ZH_TREE_H */

