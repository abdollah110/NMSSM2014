/* 
 * File:   trg_data_.h
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

bool Trg_Data_12(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerEle = false;
    bool TriggerMu = false;
    bool TriggerMuTr = false;

    string doubEle = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL";
    string doubMu = "HLT_Mu17_Mu8";
    string doubMuTr = "HLT_Mu17_TkMu8";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundEl = name.find(doubEle);
        size_t foundMu = name.find(doubMu);
        size_t foundMuTr = name.find(doubMuTr);

        if (foundEl != string::npos)
            TriggerEle = ihlt->second;
        if (foundMu != string::npos)
            TriggerMu = ihlt->second;
        if (foundMuTr != string::npos)
            TriggerMuTr = ihlt->second;

        Trigger = TriggerEle || TriggerMu || TriggerMuTr;
    }
    return Trigger;
}

bool Trg_MC_12(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerEle = false;
    bool TriggerMu = false;
    bool TriggerMuTr = false;

    string doubEle = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL";
    string doubMu = "HLT_Mu17_Mu8";
    string doubMuTr = "HLT_Mu17_TkMu8";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundEl = name.find(doubEle);
        size_t foundMu = name.find(doubMu);
        size_t foundMuTr = name.find(doubMuTr);

        if (foundEl != string::npos)
            TriggerEle = ihlt->second;
        if (foundMu != string::npos)
            TriggerMu = ihlt->second;
        if (foundMuTr != string::npos)
            TriggerMuTr = ihlt->second;

        Trigger = TriggerEle || TriggerMu || TriggerMuTr;
    }
    return Trigger;
}

bool Trg_Data_11(myevent *m) {
    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerEle1 = false;
    bool TriggerEle2 = false;
    bool TriggerMu1 = false;
    bool TriggerMu2 = false;
    bool TriggerMu3 = false;

    string doubEle1 = "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL";
    string doubEle2 = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL";
    string doubMu1 = "HLT_DoubleMu7";
    string doubMu2 = "HLT_Mu13_Mu8";
    string doubMu3 = "HLT_Mu17_Mu8";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {

        string name = ihlt->first;
        size_t foundEl1 = name.find(doubEle1);
        size_t foundEl2 = name.find(doubEle2);
        size_t foundMu1 = name.find(doubMu1);
        size_t foundMu2 = name.find(doubMu2);
        size_t foundMu3 = name.find(doubMu3);

        if (foundEl1 != string::npos)
            TriggerEle1 = ihlt->second;
        if (foundEl2 != string::npos)
            TriggerEle2 = ihlt->second;
        if (foundMu1 != string::npos)
            TriggerMu1 = ihlt->second;
        if (foundMu2 != string::npos)
            TriggerMu2 = ihlt->second;
        if (foundMu3 != string::npos)
            TriggerMu3 = ihlt->second;

        Trigger = TriggerEle1 || TriggerEle2 || TriggerMu1 || TriggerMu2 || TriggerMu3;
    }
    return Trigger;
}

bool Trg_MC_11(myevent *m) {

    map<string, int> myHLT = m->HLT;
    bool Trigger = false;
    bool TriggerEle = false;
    bool TriggerMu = false;

    string doubEle = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v8";
    string doubMu = "HLT_Mu13_Mu8_v7";

    for (map<string, int> ::iterator ihlt = myHLT.begin(); ihlt != myHLT.end(); ihlt++) {
        if (ihlt->first == doubEle)
            TriggerEle = ihlt->second;
        if (ihlt->first == doubMu)
            TriggerMu = ihlt->second;
        Trigger = TriggerEle || TriggerMu;
    }
    return Trigger;
}





#endif	/* TRG_DATA__H */

