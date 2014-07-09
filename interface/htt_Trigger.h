/* 
 * File:   Trigger_data_.h
 * Author: abdollah
 *
 * Created on April 18, 2012, 12:12 PM
 */

#ifndef TRIGGER_H
#define	TRIGGER_H

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

//bool Trigger_12(myevent *m) {
//    map<string, int> myHLT = m->HLT;
//    bool Trigger = false;
//    bool TriggerEle1 = false;
//    bool TriggerEle2 = false;
//    bool TriggerMu1 = false;
//    bool TriggerMu2 = false;
//
//    string EleTau1 = "HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20";
//    string EleTau2 = "HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20";
//    string MuTau1 = "HLT_IsoMu17_eta2p1_LooseIsoPFTau20";
//    string MuTau2 = "HLT_IsoMu18_eta2p1_LooseIsoPFTau20";
//
//    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {
//
//        string name = ihlt->first;
//        size_t foundEl1 = name.find(EleTau1);
//        size_t foundEl2 = name.find(EleTau2);
//        size_t foundMu1 = name.find(MuTau1);
//        size_t foundMu2 = name.find(MuTau2);
//
//        if (foundEl1 != string::npos)
//            TriggerEle1 = ihlt->second;
//        if (foundEl2 != string::npos)
//            TriggerEle2 = ihlt->second;
//        if (foundMu1 != string::npos)
//            TriggerMu1 = ihlt->second;
//        if (foundMu2 != string::npos)
//            TriggerMu2 = ihlt->second;
//
//        Trigger = TriggerEle1 || TriggerEle2 || TriggerMu1 || TriggerMu2;
//    }
//    return Trigger;
//}

bool Trigger_MuTau_12(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerMu1 = false;
    bool TriggerMu2 = false;

    string MuTau1 = "HLT_IsoMu17_eta2p1_LooseIsoPFTau20";
    string MuTau2 = "HLT_IsoMu18_eta2p1_LooseIsoPFTau20";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundMu1 = name.find(MuTau1);
        size_t foundMu2 = name.find(MuTau2);

        if (foundMu1 != string::npos)
            TriggerMu1 = ihlt->second;
        if (foundMu2 != string::npos)
            TriggerMu2 = ihlt->second;

        Trigger = TriggerMu1 || TriggerMu2;
    }
    return Trigger;
}

bool Trigger_EleTau_12(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerEle1 = false;
    bool TriggerEle2 = false;

    string EleTau1 = "HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20";
    string EleTau2 = "HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundEl1 = name.find(EleTau1);
        size_t foundEl2 = name.find(EleTau2);

        if (foundEl1 != string::npos)
            TriggerEle1 = ihlt->second;
        if (foundEl2 != string::npos)
            TriggerEle2 = ihlt->second;

        Trigger = TriggerEle1 || TriggerEle2;
    }
    return Trigger;
}
bool Trigger_SingleMu_12(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerMu1 = false;

    string MuTau1 = "HLT_IsoMu24";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundMu1 = name.find(MuTau1);

        if (foundMu1 != string::npos)
            TriggerMu1 = ihlt->second;

        Trigger = TriggerMu1 ;
    }
    return Trigger;
}

bool Trigger_SingleEle_12(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerEle1 = false;

    string EleTau1 = "HLT_Ele27_WP80";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundEl1 = name.find(EleTau1);

        if (foundEl1 != string::npos)
            TriggerEle1 = ihlt->second;

        Trigger = TriggerEle1;
    }
    return Trigger;
}

bool Trigger_SingleJet_12(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerJet = false;

    string EleTau1 = "HLT_PFJet320";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundEl1 = name.find(EleTau1);

        if (foundEl1 != string::npos)
            TriggerJet = ihlt->second;

        Trigger = TriggerJet;
    }
    return Trigger;
}

//bool Trigger_MC_12(myevent *m) {
//    map<string, int> myHLT = m->HLT;
//    bool Trigger = false;
//    bool TriggerEle = false;
//    bool TriggerMu = false;
//
//
//    string MuTauTrg = "HLT_IsoMu17_eta2p1_LooseIsoPFTau20";
//    string ElTauTrg = "HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20";
//
//    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {
//
//        string name = ihlt->first;
//        size_t foundMu = name.find(MuTauTrg);
//        size_t foundEl = name.find(ElTauTrg);
//
//        if (foundMu != string::npos)
//            TriggerMu = ihlt->second;
//        if (foundEl != string::npos)
//            TriggerEle = ihlt->second;
//
//        Trigger = TriggerMu || TriggerEle;
//    }
//    return Trigger;
//}


//bool Trigger_Data_11(myevent *m) {
//    map<string, int> myHLT = m->HLT;
//    bool Trigger = false;
//    bool TriggerEle1 = false;
//    bool TriggerEle2 = false;
//    bool TriggerMu1 = false;
//    bool TriggerMu2 = false;
//    bool TriggerMu3 = false;
//
//    string doubEle1 = "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL";
//    string doubEle2 = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL";
//    string doubMu1 = "HLT_DoubleMu7";
//    string doubMu2 = "HLT_Mu13_Mu8";
//    string doubMu3 = "HLT_Mu17_Mu8";
//
//    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {
//
//        string name = ihlt->first;
//        size_t foundEl1 = name.find(doubEle1);
//        size_t foundEl2 = name.find(doubEle2);
//        size_t foundMu1 = name.find(doubMu1);
//        size_t foundMu2 = name.find(doubMu2);
//        size_t foundMu3 = name.find(doubMu3);
//
//        if (foundEl1 != string::npos)
//            TriggerEle1 = ihlt->second;
//        if (foundEl2 != string::npos)
//            TriggerEle2 = ihlt->second;
//        if (foundMu1 != string::npos)
//            TriggerMu1 = ihlt->second;
//        if (foundMu2 != string::npos)
//            TriggerMu2 = ihlt->second;
//        if (foundMu3 != string::npos)
//            TriggerMu3 = ihlt->second;
//
//        Trigger = TriggerEle1 || TriggerEle2 || TriggerMu1 || TriggerMu2 || TriggerMu3;
//    }
//    return Trigger;
//}
//
//bool Trigger_MC_11(myevent *m) {
//
//    map<string, int> myHLT = m->HLT;
//    bool Trigger = false;
//    bool TriggerEle = false;
//    bool TriggerMu = false;
//
//    string doubEle = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v8";
//    string doubMu = "HLT_Mu13_Mu8_v7";
//
//    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {
//        if (ihlt->first == doubEle)
//            TriggerEle = ihlt->second;
//        if (ihlt->first == doubMu)
//            TriggerMu = ihlt->second;
//        Trigger = TriggerEle || TriggerMu;
//    }
//    return Trigger;
//}





#endif	/* Trigger_DATA__H */

