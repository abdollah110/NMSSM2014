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
#include "BtagSF.h"



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

//******************************************************************************************

bool NonOverLapWithAB(myobject const& obj1, myobject const& obj2, myobject const& jet) {
    bool Nooverlap = true;


    if (deltaR(obj1, jet) < 0.5 || deltaR(obj2, jet) < 0.5)
        Nooverlap = false;


    return Nooverlap;
}
//******************************************************************************************
//**********************   MUON   **********************************************************
//******************************************************************************************

vector<myobject> GoodMuon10GeV(myevent *m) {

    vector<myobject> looseMuon;
    looseMuon.clear();
    vector<myobject> muon = m->PreSelectedMuons;
    for (int i = 0; i < muon.size(); i++) {
        if (muon[i].pt > 10)
            looseMuon.push_back(muon[i]);
    }
    sort(looseMuon.begin(), looseMuon.end(), myobject_grt());
    return looseMuon;
}
//******************************************************************************************
//***********************   Electron   *****************************************************
//******************************************************************************************

vector<myobject> GoodElectron10GeV(myevent *m) {

    vector<myobject> goodElectron;
    goodElectron.clear();
    vector<myobject> electron = m->PreSelectedElectrons;
    for (int i = 0; i < electron.size(); i++) {
        if (electron[i].pt > 10)
            goodElectron.push_back(electron[i]);
    }
    sort(goodElectron.begin(), goodElectron.end(), myobject_grt());
    return goodElectron;
}
//******************************************************************************************
//*********************   TAU   ************************************************************
//******************************************************************************************

vector<myobject> GoodTau20GeV(myevent *m) {

    vector<myobject> goodHPSTau;
    vector<myobject> tau = m->PreSelectedHPSTausLT;
    for (int i = 0; i < tau.size(); i++) {
        if (tau[i].pt > 19)
            goodHPSTau.push_back(tau[i]);
    }
    sort(goodHPSTau.begin(), goodHPSTau.end(), myobject_sort_TauIsolation());
    return goodHPSTau;
}

//******************************************************************************************
//*********************   JET   ************************************************************
//******************************************************************************************

vector<myobject> GoodJet20(myevent *m) {

    vector<myobject> goodJet;
    vector<myobject> Jet = m->RecPFJetsAK5;

    for (int i = 0; i < Jet.size(); i++) {
        if (Jet[i].pt > 20 && Jet[i].jetId_loose > 0.5) {
            goodJet.push_back(Jet[i]);
        }
    }
    sort(goodJet.begin(), goodJet.end(), myobject_grt());
    return goodJet;
}

vector<myobject> GoodJet30(myevent *m, myobject const& a, myobject const& b) {

    vector<myobject> goodJet;
    vector<myobject> Jet = GoodJet20(m);

    for (int i = 0; i < Jet.size(); i++) {
        if (Jet[i].pt > 30 && TMath::Abs(Jet[i].eta) < 4.7 && Jet[i].puJetIdLoose > 0.5) {
            if (NonOverLapWithAB(a, b, Jet[i])) goodJet.push_back(Jet[i]);
        }
    }
    sort(goodJet.begin(), goodJet.end(), myobject_grt());
    return goodJet;
}

//******************************************************************************************
//*********************   B---JET   ************************************************************
//******************************************************************************************

vector<myobject> GoodbJet20(myevent *m, myobject const& a, myobject const& b, bool isdata, bool is2012) {

    vector<myobject> goodbJet;
    vector<myobject> jet = GoodJet20(m);
    bool isBtagged = false;
    BtagSF isBJ;

    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
    //Note:   The BJet ScaleFactor is just for Medium WP
    for (int k = 0; k < jet.size(); k++) {
        int jetGenpdgid_ = jet[k].partonFlavour;
        isBtagged = isBJ.isbtagged13(jet[k].pt, jet[k].eta, jet[k].bDiscriminatiors_CSV, jetGenpdgid_, isdata, 0, 0, is2012);

        if (jet[k].pt > 20 && TMath::Abs(jet[k].eta) < 2.4 && jet[k].puJetIdLoose > 0.5 && isBtagged) { //adding PUJetId for bjets as well
            if (NonOverLapWithAB(a, b, jet[k])) goodbJet.push_back(jet[k]);
        }
    }
    sort(goodbJet.begin(), goodbJet.end(), myobject_grt());
    return goodbJet;
}

vector<myobject> GoodLoosebJet20(myevent *m, myobject const& a, myobject const& b) {

    vector<myobject> goodbJet;
    vector<myobject> jet = GoodJet20(m);

    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
    for (int k = 0; k < jet.size(); k++) {
        if (jet[k].pt > 20 && TMath::Abs(jet[k].eta) < 2.4 && jet[k].puJetIdLoose > 0.5 && jet[k].bDiscriminatiors_CSV > 0.244) {
            if (NonOverLapWithAB(a, b, jet[k])) goodbJet.push_back(jet[k]);
        }
    }
    sort(goodbJet.begin(), goodbJet.end(), myobject_grt());
    return goodbJet;
}

vector<myobject> GoodbJet20NoCor(myevent *m, myobject const& a, myobject const& b) {

    vector<myobject> goodbJet;
    vector<myobject> jet = GoodLoosebJet20(m, a, b);

    for (int k = 0; k < jet.size(); k++) {
        if (jet[k].pt > 20 && TMath::Abs(jet[k].eta) < 2.4 && jet[k].puJetIdLoose > 0.5 && jet[k].bDiscriminatiors_CSV > 0.679) {
            if (NonOverLapWithAB(a, b, jet[k])) goodbJet.push_back(jet[k]);
        }
    }
    sort(goodbJet.begin(), goodbJet.end(), myobject_grt());
    return goodbJet;
}

//******************************************************************************************
//*********************   DiBoson Removal   ************************************************************
//******************************************************************************************

bool secondElectronVeto(myevent * m) {
    bool ThereIsNoExtraLepton = true;
    vector<myobject> electron_ = GoodElectron10GeV(m);
    for (int i = 0; i < electron_.size(); i++) {
        for (int j = i + 1; j < electron_.size(); j++) {
            bool DiEl_Pt = electron_[i].pt > 15 && electron_[j].pt > 15;
            bool DiEl_Eta = fabs(electron_[i].eta) < 2.5 && fabs(electron_[j].eta) < 2.5;
            bool DiEl_Id = EleLooseForEtauVeto(electron_[i]) && EleLooseForEtauVeto(electron_[j]);
            bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.3 && Iso_Ele_dBeta(electron_[j]) < 0.3;
            bool DiEl_charge = electron_[i].charge * electron_[j].charge < 0;
            bool DiEl_dR = deltaR(electron_[i], electron_[j]) > 0.30; // changed from 0.15 on Oct9th

            if (DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && DiEl_charge && DiEl_dR)
                ThereIsNoExtraLepton = false;
        }
    }
    return ThereIsNoExtraLepton;
}

bool secondMuonVeto(myevent * m) {

    vector<myobject> mu_ = GoodMuon10GeV(m);


    bool ThereIsNoExtraLepton = true;
    for (int i = 0; i < mu_.size(); i++) {
        for (int j = i + 1; j < mu_.size(); j++) {
            bool DiMu_Pt = mu_[i].pt > 15 && mu_[j].pt > 15;
            bool DiMu_Eta = fabs(mu_[i].eta) < 2.4 && fabs(mu_[j].eta) < 2.4;
            bool DiMu_Id = mu_[i].isGlobalMuon && mu_[j].isGlobalMuon && mu_[i].isPFMuon && mu_[j].isPFMuon && mu_[i].isTrackerMuon && mu_[j].isTrackerMuon;
            bool DiMu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.3 && Iso_Mu_dBeta(mu_[j]) < 0.3;
            bool DiMu_dZ = mu_[i].dZ_in < 0.2 && mu_[j].dZ_in < 0.2 && mu_[i].d0 < 0.045 && mu_[j].d0 < 0.045;
            bool DiMu_charge = mu_[i].charge * mu_[j].charge < 0;
            bool DiMu_dR = deltaR(mu_[i], mu_[j]) > 0.30;

            if (DiMu_Pt && DiMu_Eta && DiMu_Id && DiMu_Iso && DiMu_dZ && DiMu_charge && DiMu_dR)
                ThereIsNoExtraLepton = false;
        }
    }
    return ThereIsNoExtraLepton;
}

bool thirdElectronVetoETau(myevent *m, myobject const& a, myobject const& b) {
    bool ThereIsNoExtraLepton = true;
    vector<myobject> electron_ = GoodElectron10GeV(m);

    for (int k = 0; k < electron_.size(); k++) {
        bool ThirdEl_Pt = electron_[k].pt > 10;
        bool ThirdEl_Eta = fabs(electron_[k].eta) < 2.5;
        bool ThirdEl_Id = EleMVANonTrigId_Loose(electron_[k]);
        bool ThirdEle_Iso = Iso_Ele_dBeta(electron_[k]) < 0.3;
        bool ThirdEle_dZ = electron_[k].dZ_in < 0.2;
        //        bool NoOverLapwithOthers(deltaR(electron_[k], a) > 0.15 && deltaR(electron_[k], b) > 0.15);  changed at October8 Sync with LLR
        bool NoOverLapwithOthers = deltaR(electron_[k], a) > 0.30;

        if (ThirdEl_Pt && ThirdEl_Eta && ThirdEl_Id && ThirdEle_Iso && NoOverLapwithOthers && ThirdEle_dZ)
            ThereIsNoExtraLepton = false;
    }

    return ThereIsNoExtraLepton;

}

bool thirdElectronVetoMuTau(myevent *m, myobject const& a, myobject const& b) {
    bool ThereIsNoExtraLepton = true;
    vector<myobject> electron_ = GoodElectron10GeV(m);

    for (int k = 0; k < electron_.size(); k++) {
        bool ThirdEl_Pt = electron_[k].pt > 10;
        bool ThirdEl_Eta = fabs(electron_[k].eta) < 2.5;
        bool ThirdEl_Id = EleMVANonTrigId_Loose(electron_[k]);
        bool ThirdEle_Iso = Iso_Ele_dBeta(electron_[k]) < 0.3;
        bool ThirdEle_dZ = electron_[k].dZ_in < 0.2;
        //        bool NoOverLapwithOthers(deltaR(electron_[k], a) > 0.15 && deltaR(electron_[k], b) > 0.15);  changed at October8 Sync with LLR
        //        bool NoOverLapwithOthers(deltaR(electron_[k], a) > 0.15);

        if (ThirdEl_Pt && ThirdEl_Eta && ThirdEl_Id && ThirdEle_Iso && ThirdEle_dZ)
            ThereIsNoExtraLepton = false;
    }

    return ThereIsNoExtraLepton;

}

bool thirdMuonVetoETau(myevent *m, myobject const& a, myobject const& b) {

    vector<myobject> mu_ = GoodMuon10GeV(m);
    bool ThereIsNoExtraLepton = true;

    for (int k = 0; k < mu_.size(); k++) {

        bool ThirdMu_Pt = mu_[k].pt > 10;
        bool ThirdMu_Eta = fabs(mu_[k].eta) < 2.4;
        bool ThirdMu_Id = Id_Mu_Tight(mu_[k]);
        bool ThirdMu_Iso = Iso_Mu_dBeta(mu_[k]) < 0.3;
        bool ThirdMu_dZ = mu_[k].dZ_in < 0.2;
        bool ThirdMu_d0 = mu_[k].d0 < 0.045;
        //        bool NoOverLapwithOthers(deltaR(mu_[k], a) > 0.15 && deltaR(mu_[k], b) > 0.15);

        if (ThirdMu_Pt && ThirdMu_Eta && ThirdMu_Id && ThirdMu_Iso && ThirdMu_dZ && ThirdMu_d0)
            ThereIsNoExtraLepton = false;
    }
    return ThereIsNoExtraLepton;
}

bool thirdMuonVetoMuTau(myevent *m, myobject const& a, myobject const& b) {

    vector<myobject> mu_ = GoodMuon10GeV(m);
    bool ThereIsNoExtraLepton = true;

    for (int k = 0; k < mu_.size(); k++) {

        bool ThirdMu_Pt = mu_[k].pt > 10;
        bool ThirdMu_Eta = fabs(mu_[k].eta) < 2.4;
        bool ThirdMu_Id = Id_Mu_Tight(mu_[k]);
        bool ThirdMu_Iso = Iso_Mu_dBeta(mu_[k]) < 0.3;
        bool ThirdMu_dZ = mu_[k].dZ_in < 0.2;
        bool ThirdMu_d0 = mu_[k].d0 < 0.045;
        bool NoOverLapwithOthers= deltaR(mu_[k], a) > 0.30 ;

        if (ThirdMu_Pt && ThirdMu_Eta && ThirdMu_Id && ThirdMu_Iso && NoOverLapwithOthers && ThirdMu_dZ && ThirdMu_d0)
            ThereIsNoExtraLepton = false;
    }
    return ThereIsNoExtraLepton;
}

//bool Multi_Lepton_Veto(std::string channel, myevent * m) {
//
//    vector<myobject> mu_ = GoodMuon10GeV(m);
//    vector<myobject> electron_ = GoodElectron10GeV(m);
//
//
//    bool ThereIsNoExtraLepton = true;
//    // second Muon Veto
//    if (channel == "MM") {
//        for (int i = 0; i < mu_.size(); i++) {
//            for (int j = i + 1; j < mu_.size(); j++) {
//                bool DiMu_Pt = mu_[i].pt > 15 && mu_[j].pt > 15;
//                bool DiMu_Eta = fabs(mu_[i].eta) < 2.4 && fabs(mu_[j].eta) < 2.4;
//                bool DiMu_Id = mu_[i].isGlobalMuon && mu_[j].isGlobalMuon && mu_[i].isPFMuon && mu_[j].isPFMuon && mu_[i].isTrackerMuon && mu_[j].isTrackerMuon;
//                bool DiMu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.3 && Iso_Mu_dBeta(mu_[j]) < 0.3;
//                bool DiMu_dZ = mu_[i].dZ_in < 0.2 && mu_[j].dZ_in < 0.2;
//                bool DiMu_charge = mu_[i].charge * mu_[j].charge < 0;
//                bool DiMu_dR = deltaR(mu_[i], mu_[j]) > 0.15;
//
//                if (DiMu_Pt && DiMu_Eta && DiMu_Id && DiMu_Iso && DiMu_dZ && DiMu_charge && DiMu_dR)
//                    ThereIsNoExtraLepton = false;
//
//            }
//        }
//    }
//
//    // second electron Veto
//    if (channel == "EE") {
//        for (int i = 0; i < electron_.size(); i++) {
//            for (int j = i + 1; j < electron_.size(); j++) {
//                bool DiEl_Pt = electron_[i].pt > 15 && electron_[j].pt > 15;
//                bool DiEl_Eta = fabs(electron_[i].eta) < 2.5 && fabs(electron_[j].eta) < 2.5;
//                bool DiEl_Id = EleLooseForEtauVeto(electron_[i]) && EleLooseForEtauVeto(electron_[j]);
//                bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.3 && Iso_Ele_dBeta(electron_[j]) < 0.3;
//                bool DiEl_dZ = electron_[i].dZ_in < 0.2 && electron_[j].dZ_in < 0.2;
//                bool DiEl_charge = electron_[i].charge * electron_[j].charge < 0;
//                bool DiEl_dR = deltaR(electron_[i], electron_[j]) > 0.15;
//
//                if (DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && DiEl_dZ && DiEl_charge && DiEl_dR)
//                    ThereIsNoExtraLepton = false;
//            }
//        }
//    }
//
//
//    if (channel == "ME") {
//        for (int i = 0; i < mu_.size(); i++) {
//            for (int j = 0; j < electron_.size(); j++) {
//                bool muEle_Pt = mu_[i].pt > 10 && electron_[j].pt > 10;
//                bool muEle_Eta = fabs(electron_[j].eta) < 2.5;
//                bool muEle_Id = EleMVANonTrigId_Loose(electron_[j]);
//                ;
//                //                bool muEle_Id = EleLooseForEtauVeto(electron_[j]);
//                bool muEle_Iso = Iso_Ele_dBeta(electron_[j]) < 0.3;
//                //                bool muEle_charge = electron_[i].charge * electron_[j].charge < 0;
//                bool muEle_charge = 1;
//                bool muEle_dR = deltaR(mu_[i], electron_[j]) > 0.15;
//
//                if (muEle_Pt && muEle_Eta && muEle_Id && muEle_Iso && muEle_charge && muEle_dR)
//                    ThereIsNoExtraLepton = false;
//            }
//        }
//    }
//
//    //    mutau: Tri-lepton veto: veto event if it contains another electron of Pt > 10 GeV && abs(eta) 10 GeV && abs(eta)
//    //etau: Tri-lepton veto: veto event if it contains another electron of Pt > 10 GeV && abs(eta) 10 GeV && abs(eta)
//
//    if (channel == "MMM") {
//        for (int i = 0; i < mu_.size(); i++) {
//            for (int j = i + 1; j < mu_.size(); j++) {
//                for (int k = j + 1; k < mu_.size(); k++) {
//                    bool DiMu_Pt = mu_[i].pt > 10 && mu_[j].pt > 10;
//                    bool DiMu_Eta = fabs(mu_[i].eta) < 2.4 && fabs(mu_[j].eta) < 2.4;
//                    bool DiMu_Id = mu_[i].isGlobalMuon && mu_[j].isGlobalMuon && mu_[i].isPFMuon && mu_[j].isPFMuon && mu_[i].isTrackerMuon && mu_[j].isTrackerMuon;
//                    bool DiMu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.3 && Iso_Mu_dBeta(mu_[j]) < 0.3;
//                    bool DiMu_dZ = mu_[i].dZ_in < 0.2 && mu_[j].dZ_in < 0.2;
//                    bool DiMu_charge = mu_[i].charge * mu_[j].charge < 0;
//
//                    bool ThirdMu_Pt = mu_[k].pt > 10;
//                    bool ThirdMu_Eta = fabs(mu_[k].eta) < 2.4;
//                    bool ThirdMu_Id = Id_Mu_Tight(mu_[k]);
//                    bool ThirdMu_Iso = Iso_Mu_dBeta(mu_[k]) < 0.3;
//
//                    if (DiMu_Pt && DiMu_Eta && DiMu_Id && DiMu_Iso && DiMu_dZ && DiMu_charge && ThirdMu_Pt && ThirdMu_Eta && ThirdMu_Id && ThirdMu_Iso)
//                        ThereIsNoExtraLepton = false;
//                }
//            }
//        }
//    }
//
//    if (channel == "MME") {
//        for (int i = 0; i < mu_.size(); i++) {
//            for (int j = i + 1; j < mu_.size(); j++) {
//                for (int k = 0; k < electron_.size(); k++) {
//                    bool DiMu_Pt = mu_[i].pt > 10 && mu_[j].pt > 10;
//                    bool DiMu_Eta = fabs(mu_[i].eta) < 2.4 && fabs(mu_[j].eta) < 2.4;
//                    bool DiMu_Id = mu_[i].isGlobalMuon && mu_[j].isGlobalMuon && mu_[i].isPFMuon && mu_[j].isPFMuon && mu_[i].isTrackerMuon && mu_[j].isTrackerMuon;
//                    bool DiMu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.3 && Iso_Mu_dBeta(mu_[j]) < 0.3;
//                    bool DiMu_dZ = mu_[i].dZ_in < 0.2 && mu_[j].dZ_in < 0.2;
//                    bool DiMu_charge = mu_[i].charge * mu_[j].charge < 0;
//
//                    bool ThirdEl_Pt = electron_[k].pt > 10;
//                    bool ThirdEl_Eta = fabs(electron_[k].eta) < 2.5;
//                    bool ThirdEl_Id = EleMVANonTrigId_Loose(electron_[k]);
//                    bool ThirdEle_Iso = Iso_Ele_dBeta(electron_[k]) < 0.3;
//
//                    if (DiMu_Pt && DiMu_Eta && DiMu_Id && DiMu_Iso && DiMu_dZ && DiMu_charge && ThirdEl_Pt && ThirdEl_Eta && ThirdEl_Id && ThirdEle_Iso)
//                        ThereIsNoExtraLepton = false;
//                }
//            }
//        }
//    }
//
//
//    if (channel == "EEE") {
//        for (int i = 0; i < electron_.size(); i++) {
//            for (int j = i + 1; j < electron_.size(); j++) {
//                for (int k = j + 1; k < electron_.size(); k++) {
//                    bool DiEl_Pt = electron_[i].pt > 10 && electron_[j].pt > 10;
//                    bool DiEl_Eta = fabs(electron_[i].eta) < 2.5 && fabs(electron_[j].eta) < 2.5;
//                    bool DiEl_Id = EleLooseForEtauVeto(electron_[i]) && EleLooseForEtauVeto(electron_[j]);
//                    bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.3 && Iso_Ele_dBeta(electron_[j]) < 0.3;
//                    bool DiEl_dZ = electron_[i].dZ_in < 0.2 && electron_[j].dZ_in < 0.2;
//                    bool DiEl_charge = electron_[i].charge * electron_[j].charge < 0;
//
//
//                    bool ThirdEl_Pt = electron_[k].pt > 10;
//                    bool ThirdEl_Eta = fabs(electron_[k].eta) < 2.5;
//                    bool ThirdEl_Id = EleMVANonTrigId_Loose(electron_[k]);
//                    bool ThirdEle_Iso = Iso_Ele_dBeta(electron_[k]) < 0.3;
//
//
//                    if (DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && DiEl_dZ && DiEl_charge && ThirdEl_Pt && ThirdEl_Eta && ThirdEl_Id && ThirdEle_Iso)
//                        ThereIsNoExtraLepton = false;
//                }
//            }
//        }
//    }
//
//    if (channel == "EEM") {
//        for (int i = 0; i < electron_.size(); i++) {
//            for (int j = i + 1; j < electron_.size(); j++) {
//                for (int k = 0; k < mu_.size(); k++) {
//                    bool DiEl_Pt = electron_[i].pt > 10 && electron_[j].pt > 10;
//                    bool DiEl_Eta = fabs(electron_[i].eta) < 2.5 && fabs(electron_[j].eta) < 2.5;
//                    bool DiEl_Id = EleLooseForEtauVeto(electron_[i]) && EleLooseForEtauVeto(electron_[j]);
//                    bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.3 && Iso_Ele_dBeta(electron_[j]) < 0.3;
//                    bool DiEl_dZ = electron_[i].dZ_in < 0.2 && electron_[j].dZ_in < 0.2;
//                    bool DiEl_charge = electron_[i].charge * electron_[j].charge < 0;
//
//
//                    bool ThirdMu_Pt = mu_[k].pt > 10;
//                    bool ThirdMu_Eta = fabs(mu_[k].eta) < 2.4;
//                    bool ThirdMu_Id = Id_Mu_Tight(mu_[k]);
//                    bool ThirdMu_Iso = Iso_Mu_dBeta(mu_[k]) < 0.3;
//
//
//                    if (DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && DiEl_dZ && DiEl_charge && ThirdMu_Pt && ThirdMu_Eta && ThirdMu_Id && ThirdMu_Iso)
//                        ThereIsNoExtraLepton = false;
//                }
//            }
//        }
//    }
//    if (channel == "EM") {
//        for (int i = 0; i < electron_.size(); i++) {
//            for (int k = 0; k < mu_.size(); k++) {
//                bool DiEl_Pt = electron_[i].pt > 10;
//                bool DiEl_Eta = fabs(electron_[i].eta) < 2.5;
//                bool DiEl_Id = EleMVANonTrigId_Tight(electron_[i]);
//                bool DiEl_Iso = Iso_Ele_dBeta(electron_[i]) < 0.1;
//
//                bool ThirdMu_Pt = mu_[k].pt > 10;
//                bool ThirdMu_Eta = fabs(mu_[k].eta) < 2.4;
//                bool ThirdMu_Id = Id_Mu_Loose(mu_[k]);
//                bool ThirdMu_Iso = Iso_Mu_dBeta(mu_[k]) < 0.3;
//
//                bool muEle_dR = deltaR(mu_[k], electron_[i]) > 0.15;
//
//                if (muEle_dR && DiEl_Pt && DiEl_Eta && DiEl_Id && DiEl_Iso && ThirdMu_Pt && ThirdMu_Eta && ThirdMu_Id && ThirdMu_Iso)
//                    ThereIsNoExtraLepton = false;
//            }
//        }
//    }
//
//
//
//    return ThereIsNoExtraLepton;
//
//
//
//}



#endif	/* _GOODMUON_H */
