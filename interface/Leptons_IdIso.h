

#ifndef _ELECTRINID_H
#define	_ELECTRINID_H

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
#include "zh_Auxiliary.h"

float EAMuon(float eta) {
    float EffectiveArea = 0.0;
    if (TMath::Abs(eta) >= 0.0 && TMath::Abs(eta) < 1.0) EffectiveArea = 0.132;
    if (TMath::Abs(eta) >= 1.0 && TMath::Abs(eta) < 1.5) EffectiveArea = 0.120;
    if (TMath::Abs(eta) >= 1.5 && TMath::Abs(eta) < 2.0) EffectiveArea = 0.114;
    if (TMath::Abs(eta) >= 2.0 && TMath::Abs(eta) < 2.2) EffectiveArea = 0.139;
    if (TMath::Abs(eta) >= 2.2 && TMath::Abs(eta) < 2.3) EffectiveArea = 0.168;
    if (TMath::Abs(eta) >= 2.3) EffectiveArea = 0.189;
    return EffectiveArea;
}

float EAElectron(float eta) {
    float EffectiveArea = 0.;
    if (TMath::Abs(eta) >= 0.0 && TMath::Abs(eta) < 1.0) EffectiveArea = 0.18;
    if (TMath::Abs(eta) >= 1.0 && TMath::Abs(eta) < 1.479) EffectiveArea = 0.20;
    if (TMath::Abs(eta) >= 1.479 && TMath::Abs(eta) < 2.0) EffectiveArea = 0.15;
    if (TMath::Abs(eta) >= 2.0 && TMath::Abs(eta) < 2.2) EffectiveArea = 0.19;
    if (TMath::Abs(eta) >= 2.2 && TMath::Abs(eta) < 2.3) EffectiveArea = 0.21;
    if (TMath::Abs(eta) >= 2.3 && TMath::Abs(eta) < 2.4) EffectiveArea = 0.22;
    if (TMath::Abs(eta) >= 2.4) EffectiveArea = 0.29;
    return EffectiveArea;
}


//Muon PF Isolation

float Iso_Mu_dBeta(myobject const& a) {


    //    float MuIsoTrk = a.pfIsoCharged; //has been changed at 19 April to be sync with inclusive H->tautau
    float MuIsoTrk = a.pfIsoAll;
    float MuIsoEcal = a.pfIsoGamma;
    float MuIsoHcal = a.pfIsoNeutral;
    float MuIsoPU = a.pfIsoPU;

    float relIso = (MuIsoTrk) / a.pt;
    if (MuIsoEcal + MuIsoHcal - 0.5 * MuIsoPU > 0)
        relIso = (MuIsoTrk + MuIsoEcal + MuIsoHcal - 0.5 * MuIsoPU) / (a.pt);

    return relIso;

}

//Electron PF Isolation

float Iso_Ele_dBeta(myobject const& a) {



    //    float EleIsoTrk = a.pfIsoCharged; //has been changed at 19 April to be sync with inclusive H->tautau
    float EleIsoTrk = a.pfIsoAll;
    float EleIsoEcal = a.pfIsoGamma;
    float EleIsoHcal = a.pfIsoNeutral;
    float EleIsoPU = a.pfIsoPU;

    float relIso = (EleIsoTrk) / a.pt;
    if (EleIsoEcal + EleIsoHcal - 0.5 * EleIsoPU > 0)
        relIso = (EleIsoTrk + EleIsoEcal + EleIsoHcal - 0.5 * EleIsoPU) / (a.pt);

    return relIso;
}

//Electron PF Isolation

float Iso_Ele_Rho(myevent *m, myobject const& a) {



    //    float EleIsoTrk = a.pfIsoCharged;
    float EleIsoTrk = a.pfIsoAll;
    float EleIsoEcal = a.pfIsoGamma;
    float EleIsoHcal = a.pfIsoNeutral;
    float corr = m->RhoCorr * EAElectron(a.eta_SC);

    float relIso = (EleIsoTrk) / a.pt;
    if (EleIsoEcal + EleIsoHcal - corr > 0)
        relIso = (EleIsoTrk + EleIsoEcal + EleIsoHcal - corr) / (a.pt);

    return relIso;
}

float Iso_Mu_Rho(myevent *m, myobject const& a) {


    //    float MuIsoTrk = a.pfIsoCharged;
    float MuIsoTrk = a.pfIsoAll;
    float MuIsoEcal = a.pfIsoGamma;
    float MuIsoHcal = a.pfIsoNeutral;
    float corr = m->RhoCorr * EAMuon(a.eta);

    float relIso = (MuIsoTrk) / a.pt;
    if (MuIsoEcal + MuIsoHcal - corr > 0)
        relIso = (MuIsoTrk + MuIsoEcal + MuIsoHcal - corr) / (a.pt);

    return relIso;

}

bool Id_Mu_Loose(myobject const& a) {

    bool muPF = a.isPFMuon;
    bool muGlobal = a.isGlobalMuon;
    bool muTracker = a.isTrackerMuon;

    if (muPF && muGlobal && muTracker)
        return true;
    else
        return false;

}

bool Id_Mu_Tight(myobject const& a) {

    bool muPF = a.isPFMuon;
    bool muGlobal = a.isGlobalMuon;
    bool muTracker = a.isTrackerMuon;
    float MuChi2 = a.normalizedChi2;
    int MuValHit = a.numberOfValidMuonHits;
    int numMatchStat = a.numMatchStation;
    float dZ_in = a.dZ_in;
    int intrkLayerpixel_ = a.intrkLayerpixel;
    int trkLayerMeasure_ = a.trkLayerMeasure;

    if (muPF && (muGlobal && muTracker) && MuChi2 < 10 && MuValHit > 0 && numMatchStat > 1 && a.dB < 0.2 && dZ_in < 0.5 && intrkLayerpixel_ > 0 && trkLayerMeasure_ > 5)
        return true;
    else
        return false;

}

//https://twiki.cern.ch/twiki/bin/view/Main/HVVElectronId2012/
//bool EleMVANonTrigId(float pt, float eta, float value){

bool EleMVANonTrigId_Loose(myobject const& a) {
    bool passingId = false;
    float pt = a.pt;
    float eta = a.eta_SC;
    float value = a.Id_mvaNonTrg;

    //Similar to HZZ4L analysis
    if (pt > 5. && pt < 10. && TMath::Abs(eta) < 0.8 && value > 0.47)
        passingId = true;
    if (pt > 5. && pt < 10. && TMath::Abs(eta) >= 0.8 && TMath::Abs(eta) < 1.479 && value > 0.004)
        passingId = true;
    if (pt > 5. && pt < 10. && TMath::Abs(eta) >= 1.479 && value > 0.295)
        passingId = true;

    if (pt > 10. && TMath::Abs(eta) < 0.8 && value > 0.5)
        passingId = true;
    if (pt > 10. && TMath::Abs(eta) >= 0.8 && TMath::Abs(eta) < 1.479 && value > 0.12)
        passingId = true;
    if (pt > 10. && TMath::Abs(eta) >= 1.479 && value > 0.6)
        passingId = true;

    bool numHit = a.numHitEleInner < 2;
    return numHit && passingId;
}

bool EleMVANonTrigId_Tight(myobject const& a) {
    bool passingId = false;
    float pt = a.pt;
    float eta = a.eta_SC;
    float value = a.Id_mvaNonTrg;

    //Similar to HTT analysis
    if (pt < 20. && TMath::Abs(eta) < 0.8 && value > 0.925)
        passingId = true;
    if (pt < 20. && TMath::Abs(eta) >= 0.8 && TMath::Abs(eta) < 1.479 && value > 0.915)
        passingId = true;
    if (pt < 20. && TMath::Abs(eta) >= 1.479 && value > 0.965)
        passingId = true;

    if (pt > 20. && TMath::Abs(eta) < 0.8 && value > 0.905)
        passingId = true;
    if (pt > 20. && TMath::Abs(eta) >= 0.8 && TMath::Abs(eta) < 1.479 && value > 0.955)
        passingId = true;
    if (pt > 20. && TMath::Abs(eta) >= 1.479 && value > 0.975)
        passingId = true;
    bool numHit = a.numHitEleInner < 2;
    return numHit && passingId;
}

bool getTauIsolation(std::string channel, myobject const& a) {
    if (channel == "mmtt" || channel == "eett") {
        //        if (a.byMediumCombinedIsolationDeltaBetaCorr3Hits) //changed in 19April
        if (a.byLooseCombinedIsolationDeltaBetaCorr3Hits)
            return true;
        else
            return false;
    }
    if (channel == "mmet" || channel == "eeet" || channel == "mmmt" || channel == "eemt") {
        if (a.byLooseCombinedIsolationDeltaBetaCorr3Hits)
            return true;
        else
            return false;
    }
}

bool getEleRejection(std::string channel, myobject const& a) {
    if (channel == "mmtt" || channel == "eett" || channel == "mmmt" || channel == "eemt") {
        if (a.discriminationByElectronLoose)
            return true;
        else
            return false;
    }
    if (channel == "mmet" || channel == "eeet") {
        //        if (a.discriminationByElectronMVA2Tight) //changed in 19April
        if (a.discriminationByElectronMVA3Tight)
            return true;
        else
            return false;
    }

}

bool getMuRejection(std::string channel, myobject const& a) {
    if (channel == "mmtt" || channel == "eett" || channel == "mmet" || channel == "eeet") {
        if (a.discriminationByMuonLoose2)
            return true;
        else
            return false;
    }
    if (channel == "mmmt" || channel == "eemt") {
        if (a.discriminationByMuonTight2)
            return true;
        else
            return false;
    }

}


#endif
