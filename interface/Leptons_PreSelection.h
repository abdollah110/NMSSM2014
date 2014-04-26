/*
 * File:   LooseMuon.h
 * Author: abdollah
 *
 * Created on February 11, 2011, 4:13 PM
 */

#ifndef _GOODLEPTONS_H
#define	_GOODLEPTONS_H


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
#include "myevent.h"
#include "LinkDef.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include "myobject.h"
#include "Leptons_IdIso.h"
#include "zh_Auxiliary.h"



using namespace std;

struct myobject_grt {

    bool operator ()(myobject const& a, myobject const& b) const {
        return (a.pt > b.pt);
    }
};

struct myobject_sort_Phi {

    bool operator ()(myobject const& a, myobject const& b) const {
        return (a.phi > b.phi);
    }
};

struct myobject_sort_TauIsolation {

    bool operator ()(myobject const& a, myobject const& b) const {
        return (a.byRawCombinedIsolationDeltaBetaCorr3Hits < b.byRawCombinedIsolationDeltaBetaCorr3Hits);
    }
};

struct myobject_sort_BTagging {

    bool operator ()(myobject const& a, myobject const& b) const {
        return (a.bDiscriminatiors_CSV > b.bDiscriminatiors_CSV);
    }
};



float looseIsolation = 0.30;
//******************************************************************************************
//**********************   MUON   **********************************************************
//******************************************************************************************

vector<myobject> GoodMuon10GeV(myevent *m) {

    vector<myobject> looseMuon;
    looseMuon.clear();
    vector<myobject> muon = m->PreSelectedMuons;
    for (int i = 0; i < muon.size(); i++) {
        float muPt = muon[i].pt;
        //        float muEta = muon[i].eta;
        //        if (muPt > 10 && TMath::Abs(muEta) < 2.4 && Id_Mu_Loose(muon[i]) && Iso_Mu_dBeta(muon[i]) < looseIsolation)
        if (muPt > 10)
            looseMuon.push_back(muon[i]);
    }
    sort(looseMuon.begin(), looseMuon.end(), myobject_grt());
    return looseMuon;
}

vector<myobject> GoodMuon(myevent *m) {

    vector<myobject> looseMuon;
    looseMuon.clear();
    vector<myobject> muon = m->PreSelectedMuons;
    for (int i = 0; i < muon.size(); i++) {
        float muPt = muon[i].pt;
        float muEta = muon[i].eta;
        if (muPt > 10 && TMath::Abs(muEta) < 2.4 && Id_Mu_Loose(muon[i]) && Iso_Mu_dBeta(muon[i]) < looseIsolation)
            looseMuon.push_back(muon[i]);
    }
    sort(looseMuon.begin(), looseMuon.end(), myobject_grt());
    return looseMuon;
}

vector<myobject> NoIdIsoMuon(myevent *m) {

    vector<myobject> bareMuon;
    bareMuon.clear();
    vector<myobject> muon = m->PreSelectedMuons;
    for (int i = 0; i < muon.size(); i++) {
        float muPt = muon[i].pt;
        float muEta = muon[i].eta;
        bool muGlobal = muon[i].isGlobalMuon;
        bool muTracker = muon[i].isTrackerMuon;
        if (muPt > 10 && TMath::Abs(muEta) < 2.4 && (muGlobal || muTracker))
            bareMuon.push_back(muon[i]);
    }
    sort(bareMuon.begin(), bareMuon.end(), myobject_grt());
    return bareMuon;
}
//******************************************************************************************
//***********************   Electron   *****************************************************
//******************************************************************************************

vector<myobject> GoodElectron10GeV(myevent *m) {

    vector<myobject> goodElectron;
    goodElectron.clear();
    vector<myobject> electron = m->PreSelectedElectrons;
    for (int i = 0; i < electron.size(); i++) {
        float elePt = electron[i].pt;
        //        float eleEta = electron[i].eta_SC;
        //        if (elePt > 10 && TMath::Abs(eleEta) < 2.5 && EleMVANonTrigId_Loose(electron[i]) && Iso_Ele_dBeta(electron[i]) < looseIsolation)
        if (elePt > 10)
            goodElectron.push_back(electron[i]);
    }
    sort(goodElectron.begin(), goodElectron.end(), myobject_grt());
    return goodElectron;
}

vector<myobject> GoodElectron(myevent *m) {

    vector<myobject> goodElectron;
    goodElectron.clear();
    vector<myobject> electron = m->PreSelectedElectrons;
    for (int i = 0; i < electron.size(); i++) {
        float elePt = electron[i].pt;
        float eleEta = electron[i].eta_SC;
        if (elePt > 10 && TMath::Abs(eleEta) < 2.5 && EleMVANonTrigId_Loose(electron[i]) && Iso_Ele_dBeta(electron[i]) < looseIsolation)
            goodElectron.push_back(electron[i]);
    }
    sort(goodElectron.begin(), goodElectron.end(), myobject_grt());
    return goodElectron;
}

vector<myobject> NoIdIsoElectron(myevent *m) {

    vector<myobject> bareElectron;
    bareElectron.clear();
    vector<myobject> electron = m->PreSelectedElectrons;
    for (int i = 0; i < electron.size(); i++) {
        float elePt = electron[i].pt;
        float eleEta = electron[i].eta_SC;
        if (elePt > 10 && TMath::Abs(eleEta) < 2.5)
            bareElectron.push_back(electron[i]);
    }
    sort(bareElectron.begin(), bareElectron.end(), myobject_grt());
    return bareElectron;
}
//******************************************************************************************
//*********************   TAU   ************************************************************
//******************************************************************************************

vector<myobject> GoodTau20GeV(myevent *m) {

    vector<myobject> goodHPSTau;
    vector<myobject> tau = m->PreSelectedHPSTaus;
    for (int i = 0; i < tau.size(); i++) {

        float tauPt = tau[i].pt;
        //        float tauEta = tau[i].eta;

        if (tauPt > 20)
            goodHPSTau.push_back(tau[i]);
    }
    //        if (tauPt > 15 && TMath::Abs(tauEta) < 2.3 &&
    //                tau[i].discriminationByDecayModeFinding && tau[i].byLooseCombinedIsolationDeltaBetaCorr3Hits &&
    //                tau[i].discriminationByElectronLoose && tau[i].discriminationByMuonLoose2)
    //            goodHPSTau.push_back(tau[i]);
    //    }

    sort(goodHPSTau.begin(), goodHPSTau.end(), myobject_sort_TauIsolation());
    return goodHPSTau;
}

vector<myobject> GoodTau(myevent *m) {

    vector<myobject> goodHPSTau;
    vector<myobject> tau = GoodTau20GeV(m);
    for (int i = 0; i < tau.size(); i++) {

        if (tau[i].pt > 15 && TMath::Abs(tau[i].eta) < 2.3 &&
                tau[i].discriminationByDecayModeFinding && tau[i].byLooseCombinedIsolationDeltaBetaCorr3Hits)
            goodHPSTau.push_back(tau[i]);
    }

    sort(goodHPSTau.begin(), goodHPSTau.end(), myobject_sort_TauIsolation());
    return goodHPSTau;
}
//vector<myobject> GoodTau(myevent *m) {
//
//    vector<myobject> goodHPSTau;
//    vector<myobject> tau = m->PreSelectedHPSTaus;
//    for (int i = 0; i < tau.size(); i++) {
//
//        float tauPt = tau[i].pt;
//        float tauEta = tau[i].eta;
//
//        if (tauPt > 15 && TMath::Abs(tauEta) < 2.3 &&
//                tau[i].discriminationByDecayModeFinding && tau[i].byLooseCombinedIsolationDeltaBetaCorr3Hits &&
//                tau[i].discriminationByElectronLoose && tau[i].discriminationByMuonLoose2)
//            goodHPSTau.push_back(tau[i]);
//    }
//
//    sort(goodHPSTau.begin(), goodHPSTau.end(), myobject_grt());
//    return goodHPSTau;
//}

vector<myobject> NoIsoTau(myevent *m) {

    vector<myobject> goodHPSTau;
    vector<myobject> tau = m->PreSelectedHPSTaus;
    for (int i = 0; i < tau.size(); i++) {
        float tauPt = tau[i].pt;
        float tauEta = tau[i].eta;
        if (tauPt > 5 && TMath::Abs(tauEta) < 2.3 &&
                tau[i].discriminationByDecayModeFinding
                //              &&   tau[i].discriminationByElectronLoose && tau[i].discriminationByMuonLoose //changed in 19April
                )
            goodHPSTau.push_back(tau[i]);
    }
    sort(goodHPSTau.begin(), goodHPSTau.end(), myobject_grt());
    return goodHPSTau;
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

bool NonOverLapWithMuEle(myevent *m, myobject const& a) {
    vector<myobject> mu_ = GoodMuon(m);
    vector<myobject> ele_ = GoodElectron(m);
    vector<myobject> tau_ = GoodTau20GeV(m); // Loose HPS
    bool Nooverlap = true;

    for (int p = 0; p < mu_.size(); p++) {
        if (deltaR(a, mu_[p]) < 0.4)
            Nooverlap = false;
    }
    for (int q = 0; q < ele_.size(); q++) {
        if (deltaR(a, ele_[q]) < 0.4)
            Nooverlap = false;
    }
    for (int r = 0; r < tau_.size(); r++) {
        if (deltaR(a, tau_[r]) < 0.4)
            Nooverlap = false;
    }
    return Nooverlap;
}

vector<myobject> GoodJet(myevent *m) {

    vector<myobject> goodJet;
    vector<myobject> Jet = m->RecPFJetsAK5;

    for (int i = 0; i < Jet.size(); i++) {
        if (Jet[i].pt > 0 && TMath::Abs(Jet[i].eta) < 4.7) {
            if (NonOverLapWithMuEle(m, Jet[i])) goodJet.push_back(Jet[i]);
        }
    }
    sort(goodJet.begin(), goodJet.end(), myobject_grt());
    return goodJet;
}

vector<myobject> GoodJet20(myevent *m) {

    vector<myobject> goodJet;
    vector<myobject> Jet = m->RecPFJetsAK5;

    for (int i = 0; i < Jet.size(); i++) {
        if (Jet[i].pt > 20 && TMath::Abs(Jet[i].eta) < 4.7) {
            if (NonOverLapWithMuEle(m, Jet[i])) goodJet.push_back(Jet[i]);
        }
    }
    sort(goodJet.begin(), goodJet.end(), myobject_grt());
    return goodJet;
}

vector<myobject> GoodbJet20(myevent *m) {

    vector<myobject> goodbJet;
    vector<myobject> jet = GoodJet20(m);

    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
    for (int k = 0; k < jet.size(); k++) {
        if (jet[k].pt > 20 && TMath::Abs(jet[k].eta) < 2.4 && jet[k].bDiscriminatiors_CSV > 0.679) {
            if (NonOverLapWithMuEle(m, jet[k])) goodbJet.push_back(jet[k]);
        }
    }
    sort(goodbJet.begin(), goodbJet.end(), myobject_sort_BTagging());
    return goodbJet;
}

//*********************************************************************************************
//****************************    Removing OverLaps  for GoodLeptons ***************************************
//*********************************************************************************************
//remove overlap Muon and electron

vector <myobject> myCleanLepton(myevent *m, string lep) {

    vector<myobject> mu_;
    vector<myobject> electron_;
    vector<myobject> tau_;

    mu_.clear();
    electron_.clear();
    tau_.clear();

    vector<myobject> Muon_Vector = GoodMuon(m);
    vector<myobject> Electron_Vector = GoodElectron(m);
    vector<myobject> Tau_Vector = GoodTau20GeV(m);

    //#####################  CleanMuon
    if (lep == "mu") {
        for (int a = 0; a < Muon_Vector.size(); a++) {
            mu_.push_back(Muon_Vector[a]);
        }
        return mu_;
    }
    //#####################  CleanElectron
    if (lep == "ele") {
        for (int a = 0; a < Electron_Vector.size(); a++) {
            bool Keep_obj = true;
            for (int b = 0; b < Muon_Vector.size(); b++) {
                if (deltaR(Electron_Vector[a], Muon_Vector[b]) < 0.1)
                    Keep_obj = false;
            }
            if (Keep_obj == true)
                electron_.push_back(Electron_Vector[a]);
        }
        return electron_;
    }
    //#####################  CleanTau
    if (lep == "tau") {
        for (int a = 0; a < Tau_Vector.size(); a++) {
            bool Keep_obj = true;
            for (int b = 0; b < Electron_Vector.size(); b++) {
                if (deltaR(Tau_Vector[a], Electron_Vector[b]) < 0.1)
                    Keep_obj = false;
            }
            for (int c = 0; c < Muon_Vector.size(); c++) {
                if (deltaR(Tau_Vector[a], Muon_Vector[c]) < 0.1)
                    Keep_obj = false;
            }
            if (Keep_obj == true)
                tau_.push_back(Tau_Vector[a]);
        }
        return tau_;
    }
    //###################################
}

vector <myobject> myCleanBareLepton(myevent *m, string lep) {

    vector<myobject> mu_;
    vector<myobject> electron_;
    vector<myobject> tau_;

    mu_.clear();
    electron_.clear();
    tau_.clear();

    vector<myobject> Muon_Vector = NoIdIsoMuon(m);
    vector<myobject> Electron_Vector = NoIdIsoElectron(m);
    vector<myobject> Tau_Vector = NoIsoTau(m);
    //Good Vector
    vector<myobject> goodmu_ = myCleanLepton(m, "mu");
    vector<myobject> goodelectron_ = myCleanLepton(m, "ele");

    //#####################  CleanMuon
    if (lep == "mu") {
        for (int a = 0; a < Muon_Vector.size(); a++) {
            mu_.push_back(Muon_Vector[a]);
        }
        return mu_;
    }
    //#####################  CleanElectron
    if (lep == "ele") {
        for (int a = 0; a < Electron_Vector.size(); a++) {
            bool Keep_obj = true;
            for (int b = 0; b < goodmu_.size(); b++) {
                if (deltaR(Electron_Vector[a], goodmu_[b]) < 0.1)
                    Keep_obj = false;
            }
            if (Keep_obj == true)
                electron_.push_back(Electron_Vector[a]);
        }
        return electron_;
    }
    //#####################  CleanTau
    if (lep == "tau") {
        for (int a = 0; a < Tau_Vector.size(); a++) {
            bool Keep_obj = true;
            for (int b = 0; b < goodelectron_.size(); b++) {
                if (deltaR(Tau_Vector[a], goodelectron_[b]) < 0.1)
                    Keep_obj = false;
            }
            for (int c = 0; c < goodmu_.size(); c++) {
                if (deltaR(Tau_Vector[a], goodmu_[c]) < 0.1)
                    Keep_obj = false;
            }
            if (Keep_obj == true)
                tau_.push_back(Tau_Vector[a]);
        }
        return tau_;
    }
    //###################################
}

//******************************************************************************************
//******************************************************************************************

vector <myobject> LeptonSubSet(myevent *m, string lep) {

    //    vector<myobject> mu_Debug_L;
    vector<myobject> mu_Debug_T;
    //    mu_Debug_L.clear();
    mu_Debug_T.clear();
    //    vector<myobject> ele_Debug_L;
    vector<myobject> ele_Debug_T;
    //    ele_Debug_L.clear();
    ele_Debug_T.clear();

    vector<myobject> Muon_Vector = myCleanLepton(m, "mu");
    vector<myobject> Electron_Vector = myCleanLepton(m, "ele");



    for (int b = 0; b < Muon_Vector.size(); b++) {
        //        if (Iso_Mu_dBeta(Muon_Vector[b]) > 0.30 && Iso_Mu_dBeta(Muon_Vector[b]) < looseIsolation)
        //            mu_Debug_L.push_back(Muon_Vector[b]);
        if (Iso_Mu_dBeta(Muon_Vector[b]) > 0.15 && Iso_Mu_dBeta(Muon_Vector[b]) < looseIsolation)
            mu_Debug_T.push_back(Muon_Vector[b]);
    }

    for (int a = 0; a < Electron_Vector.size(); a++) {
        //        if (Iso_Ele_dBeta(Electron_Vector[a]) > 0.30 && Iso_Ele_dBeta(Electron_Vector[a]) < looseIsolation)
        //            ele_Debug_L.push_back(Electron_Vector[a]);
        if (Iso_Ele_dBeta(Electron_Vector[a]) > 0.10 && Iso_Ele_dBeta(Electron_Vector[a]) < looseIsolation)
            ele_Debug_T.push_back(Electron_Vector[a]);
    }

    //    if (lep == "mu_loose_partly") return mu_Debug_L;
    if (lep == "mu_tight_partly") return mu_Debug_T;
    //    if (lep == "ele_loose_partly") return ele_Debug_L;
    if (lep == "ele_tight_partly") return ele_Debug_T;


}

bool Multi_Lepton_Veto(std::string channel, myevent * m) {

    vector<myobject> mu_ = GoodMuon10GeV(m);
    vector<myobject> electron_ = GoodElectron10GeV(m);


    bool ThisIsNoExtraLepton = true;
    if (channel == "MM") {
        for (int i = 0; i < mu_.size(); i++) {
            for (int j = i + 1; j < mu_.size(); j++) {
                bool DiMu_Pt = mu_[i].pt > 15 && mu_[j].pt > 15;
                bool DiMu_Eta = fabs(mu_[i].eta) < 2.4 && fabs(mu_[j].eta) < 2.4;
                bool DiMu_Id = mu_[i].isGlobalMuon && mu_[j].isGlobalMuon && mu_[i].isPFMuon && mu_[j].isPFMuon && mu_[i].isTrackerMuon && mu_[j].isTrackerMuon;
                bool DiMu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.3 && Iso_Mu_dBeta(mu_[j]) < 0.3;
                bool DiMu_dZ = mu_[i].dZ_in < 0.2 && mu_[j].dZ_in < 0.2;
                bool DiMu_charge = mu_[i].charge * mu_[j].charge < 0;

                if (DiMu_Pt && DiMu_Eta && DiMu_Id && DiMu_Iso && DiMu_dZ && DiMu_charge)
                    ThisIsNoExtraLepton = false;

            }
        }
    }

    if (channel == "MMM") {
        for (int i = 0; i < mu_.size(); i++) {
            for (int j = i + 1; j < mu_.size(); j++) {
                for (int k = j + 1; k < mu_.size(); k++) {
                    bool DiMu_Pt = mu_[i].pt > 15 && mu_[j].pt > 15;
                    bool DiMu_Eta = fabs(mu_[i].eta) < 2.4 && fabs(mu_[j].eta) < 2.4;
                    bool DiMu_Id = mu_[i].isGlobalMuon && mu_[j].isGlobalMuon && mu_[i].isPFMuon && mu_[j].isPFMuon && mu_[i].isTrackerMuon && mu_[j].isTrackerMuon;
                    bool DiMu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.3 && Iso_Mu_dBeta(mu_[j]) < 0.3;
                    bool DiMu_dZ = mu_[i].dZ_in < 0.2 && mu_[j].dZ_in < 0.2;
                    bool DiMu_charge = mu_[i].charge * mu_[j].charge < 0;

                    bool ThirdMu_Pt = mu_[k].pt > 10;
                    bool ThirdMu_Eta = fabs(mu_[k].eta) < 2.4;
                    bool ThirdMu_Id = Id_Mu_Tight(mu_[k]);
                    bool ThirdMu_Iso = Iso_Mu_dBeta(mu_[k]) < 0.3;

                    if (DiMu_Pt && DiMu_Eta && DiMu_Id && DiMu_Iso && DiMu_dZ && DiMu_charge && ThirdMu_Pt && ThirdMu_Eta && ThirdMu_Id && ThirdMu_Iso)
                        ThisIsNoExtraLepton = false;
                }
            }
        }
    }

    if (channel == "MME") {
        for (int i = 0; i < mu_.size(); i++) {
            for (int j = i + 1; j < mu_.size(); j++) {
                for (int k = 0; k < electron_.size(); k++) {
                    bool DiMu_Pt = mu_[i].pt > 15 && mu_[j].pt > 15;
                    bool DiMu_Eta = fabs(mu_[i].eta) < 2.4 && fabs(mu_[j].eta) < 2.4;
                    bool DiMu_Id = mu_[i].isGlobalMuon && mu_[j].isGlobalMuon && mu_[i].isPFMuon && mu_[j].isPFMuon && mu_[i].isTrackerMuon && mu_[j].isTrackerMuon;
                    bool DiMu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.3 && Iso_Mu_dBeta(mu_[j]) < 0.3;
                    bool DiMu_dZ = mu_[i].dZ_in < 0.2 && mu_[j].dZ_in < 0.2;
                    bool DiMu_charge = mu_[i].charge * mu_[j].charge < 0;

                    bool ThirdEl_Pt = electron_[k].pt > 10;
                    bool ThirdEl_Eta = fabs(electron_[k].eta) < 2.5;
                    bool ThirdEl_Id = EleMVANonTrigId_Loose(electron_[k]);
                    bool ThirdEle_Iso = Iso_Ele_dBeta(electron_[k]) < 0.3;

                    if (DiMu_Pt && DiMu_Eta && DiMu_Id && DiMu_Iso && DiMu_dZ && DiMu_charge && ThirdEl_Pt && ThirdEl_Eta && ThirdEl_Id && ThirdEle_Iso)
                        ThisIsNoExtraLepton = false;
                }
            }
        }
    }

    if (channel == "EE") {
        for (int i = 0; i < electron_.size(); i++) {
            for (int j = i + 1; j < electron_.size(); j++) {
                bool DiEl_Pt = electron_[i].pt > 15 && electron_[j].pt > 15;
                bool DiEl_Eta = fabs(electron_[i].eta) < 2.5 && fabs(electron_[j].eta) < 2.5;
                bool DiEl_Id = EleLooseForEtauVeto(electron_[i]) && EleLooseForEtauVeto(electron_[j]);
                bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.3 && Iso_Ele_dBeta(electron_[j]) < 0.3;
                bool DiEl_dZ = electron_[i].dZ_in < 0.2 && electron_[j].dZ_in < 0.2;
                bool DiEl_charge = electron_[i].charge * electron_[j].charge < 0;

                if (DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && DiEl_dZ && DiEl_charge)
                    ThisIsNoExtraLepton = false;
            }
        }
    }

    if (channel == "EEE") {
        for (int i = 0; i < electron_.size(); i++) {
            for (int j = i + 1; j < electron_.size(); j++) {
                for (int k = j + 1; k < electron_.size(); k++) {
                    bool DiEl_Pt = electron_[i].pt > 15 && electron_[j].pt > 15;
                    bool DiEl_Eta = fabs(electron_[i].eta) < 2.5 && fabs(electron_[j].eta) < 2.5;
                    bool DiEl_Id = EleLooseForEtauVeto(electron_[i]) && EleLooseForEtauVeto(electron_[j]);
                    bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.3 && Iso_Ele_dBeta(electron_[j]) < 0.3;
                    bool DiEl_dZ = electron_[i].dZ_in < 0.2 && electron_[j].dZ_in < 0.2;
                    bool DiEl_charge = electron_[i].charge * electron_[j].charge < 0;


                    bool ThirdEl_Pt = electron_[k].pt > 10;
                    bool ThirdEl_Eta = fabs(electron_[k].eta) < 2.5;
                    bool ThirdEl_Id = EleMVANonTrigId_Loose(electron_[k]);
                    bool ThirdEle_Iso = Iso_Ele_dBeta(electron_[k]) < 0.3;


                    if (DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && DiEl_dZ && DiEl_charge && ThirdEl_Pt && ThirdEl_Eta && ThirdEl_Id && ThirdEle_Iso)
                        ThisIsNoExtraLepton = false;
                }
            }
        }
    }

    if (channel == "EEM") {
        for (int i = 0; i < electron_.size(); i++) {
            for (int j = i + 1; j < electron_.size(); j++) {
                for (int k = 0; k < mu_.size(); k++) {
                    bool DiEl_Pt = electron_[i].pt > 15 && electron_[j].pt > 15;
                    bool DiEl_Eta = fabs(electron_[i].eta) < 2.5 && fabs(electron_[j].eta) < 2.5;
                    bool DiEl_Id = EleLooseForEtauVeto(electron_[i]) && EleLooseForEtauVeto(electron_[j]);
                    bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.3 && Iso_Ele_dBeta(electron_[j]) < 0.3;
                    bool DiEl_dZ = electron_[i].dZ_in < 0.2 && electron_[j].dZ_in < 0.2;
                    bool DiEl_charge = electron_[i].charge * electron_[j].charge < 0;


                    bool ThirdMu_Pt = mu_[k].pt > 10;
                    bool ThirdMu_Eta = fabs(mu_[k].eta) < 2.4;
                    bool ThirdMu_Id = Id_Mu_Tight(mu_[k]);
                    bool ThirdMu_Iso = Iso_Mu_dBeta(mu_[k]) < 0.3;


                    if (DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && DiEl_dZ && DiEl_charge && ThirdMu_Pt && ThirdMu_Eta && ThirdMu_Id && ThirdMu_Iso)
                        ThisIsNoExtraLepton = false;
                }
            }
        }
    }



    return ThisIsNoExtraLepton;



}



#endif	/* _GOODMUON_H */




//
//
//skimOutput_14_1_FHP.root
//skimOutput_1_1_vtr.root
//skimOutput_6_1_dbC.root
//skimOutput_15_1_hGi.root
//skimOutput_11_1_Qay.root
//skimOutput_26_1_oPQ.root
//skimOutput_123_1_k1C.root
//skimOutput_149_1_Brf.root
//skimOutput_138_1_m7N.root
//skimOutput_108_1_Kdt.root
//skimOutput_174_1_l0o.root
//skimOutput_135_1_d0X.root
//skimOutput_147_1_5P1.root
//skimOutput_197_1_9uM.root
//skimOutput_189_1_3jh.root
//skimOutput_55_1_dEV.root
//skimOutput_205_1_snV.root
//skimOutput_20_1_15p.root
//skimOutput_196_1_WPR.root
//skimOutput_124_1_LDG.root
//skimOutput_167_1_kex.root
//skimOutput_251_1_lio.root
//skimOutput_42_1_v3J.root
//skimOutput_27_1_UIP.root
//skimOutput_49_1_TNI.root
//skimOutput_316_1_AeL.root
//skimOutput_303_1_xjg.root
//skimOutput_330_1_RoS.root
//skimOutput_248_1_1l1.root
//skimOutput_237_1_wsA.root
//skimOutput_155_1_EMZ.root
//skimOutput_311_1_NKp.root
//skimOutput_231_1_LpL.root
//skimOutput_353_1_gjq.root
//skimOutput_481_1_O6d.root
//
//
//
//
//
//15535192026 10:03 skimOutput_14_1_aNY.root
//15495146126 10:08 skimOutput_15_1_b6b.root
//15327723326 10:10 skimOutput_6_1_ZSj.root
//14284175626 10:15 skimOutput_11_1_WkA.root
//15179004126 10:49 skimOutput_1_1_2Ko.root
//13336873226 12:09 skimOutput_174_1_JzW.root
//13561739726 12:12 skimOutput_189_1_nc3.root
//17910126226 12:12 skimOutput_147_1_4Ws.root
//15283154426 12:13 skimOutput_149_1_ZfO.root
//14576044426 12:14 skimOutput_197_1_2HB.root
//15998551926 12:16 skimOutput_124_1_LBb.root
//15075548126 12:17 skimOutput_138_1_S83.root
//14974771226 12:22 skimOutput_108_1_OBv.root
//15999781026 12:25 skimOutput_123_1_rD7.root