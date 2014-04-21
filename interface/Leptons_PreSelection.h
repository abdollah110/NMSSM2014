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



float looseIsolation = 0.30;
//******************************************************************************************
//**********************   MUON   **********************************************************
//******************************************************************************************

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

vector<myobject> GoodTau(myevent *m) {

    vector<myobject> goodHPSTau;
    vector<myobject> tau = m->PreSelectedHPSTaus;
    for (int i = 0; i < tau.size(); i++) {

        float tauPt = tau[i].pt;
        float tauEta = tau[i].eta;

        if (tauPt > 15 && TMath::Abs(tauEta) < 2.3 &&
                tau[i].discriminationByDecayModeFinding && tau[i].byLooseCombinedIsolationDeltaBetaCorr3Hits &&
                tau[i].discriminationByElectronLoose && tau[i].discriminationByMuonLoose2)
            goodHPSTau.push_back(tau[i]);
    }

    sort(goodHPSTau.begin(), goodHPSTau.end(), myobject_grt());
    return goodHPSTau;
}

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

vector<myobject> GoodJet(myevent *m) {

    vector<myobject> goodJet;
    vector<myobject> Jet = m->RecPFJetsAK5;
    for (int i = 0; i < Jet.size(); i++) {
        float JetPt = Jet[i].pt;
        float JetEta = Jet[i].eta;
        if (JetPt > 10 && TMath::Abs(JetEta) < 5.0)
            goodJet.push_back(Jet[i]);
    }
    sort(goodJet.begin(), goodJet.end(), myobject_grt());
    return goodJet;
}

//inline float deltaPhi(myobject const& a, myobject const& b) {
//    float result = a.phi - b.phi;
//    while (result > M_PI) result -= 2 * M_PI;
//    while (result <= -M_PI) result += 2 * M_PI;
//    return TMath::Abs(result);
//}
//
//inline float deltaR2(myobject const& a, myobject const& b) {
//    float deta = a.eta - b.eta;
//    float dphi = deltaPhi(a, b);
//    return deta * deta + dphi*dphi;
//}
////inline float deltaR2(float eta1, float phi1, float eta2, float phi2) {
////    float deta = eta1 - eta2;
////    float dphi = deltaPhi(phi1, phi2);
////    return deta * deta + dphi*dphi;
////}
//
//inline float deltaR(myobject const& a, myobject const& b) {
//    return sqrt(deltaR2(a, b));
//}

int bjet_Multiplicity(myevent *m) {
    vector<myobject> mu_ = GoodMuon(m);
    vector<myobject> ele_ = GoodElectron(m);
    vector<myobject> tau_ = GoodTau(m); // Loose HPS
    vector<myobject> jet = GoodJet(m);

    //    jet = m->RecPFJetsAK5;
    int num_bj = 0;
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
    for (int k = 0; k < jet.size(); k++) {
        //        if (jet[k].pt > 20 && TMath::Abs(jet[k].eta) < 2.4 && jet[k].bDiscriminatiors_CSV > 0.898) { //changed on 19April
        if (jet[k].pt > 20 && TMath::Abs(jet[k].eta) < 2.4 && jet[k].bDiscriminatiors_CSV > 0.679) {
            bool Nooverlap = true;

            for (int p = 0; p < mu_.size(); p++) {
                if (deltaR(jet[k], mu_[p]) < 0.4)
                    Nooverlap = false;
            }
            for (int q = 0; q < ele_.size(); q++) {
                if (deltaR(jet[k], ele_[q]) < 0.4)
                    Nooverlap = false;
            }
            /*
            for (int r = 0; r < tau_.size(); r++) {
                       if (deltaR(jet[k], tau_[r]) < 0.4)
                           Nooverlap = false;
                   }
             */
            if (Nooverlap) num_bj++;
        }

    }

    return num_bj;
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
    vector<myobject> Tau_Vector = GoodTau(m);

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


#endif	/* _GOODMUON_H */

