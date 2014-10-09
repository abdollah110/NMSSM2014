

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

float Iso_Mu_dBeta(myobject const& a) { //REVISITED


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

bool Id_Mu_Tight(myobject const& a) { // REVISITED

    bool muGlobal = a.isGlobalMuon;
    bool muPF = a.isPFMuon;
    float MuChi2 = a.normalizedChi2;
    int MuValHit = a.numberOfValidMuonHits;
    int numMatchStat = a.numMatchStation;
    float dZ_in = a.dZ_in;
    int intrkLayerpixel_ = a.intrkLayerpixel;
    int trkLayerMeasure_ = a.trkLayerMeasure;

    //    bool muTracker = a.isTrackerMuon;

    if (muPF && muGlobal && MuChi2 < 10 && MuValHit > 0 && numMatchStat > 1 && a.dB < 0.2 && dZ_in < 0.5 && intrkLayerpixel_ > 0 && trkLayerMeasure_ > 5)
        return true;
    else
        return false;

}

//https://twiki.cern.ch/twiki/bin/view/Main/HVVElectronId2012/
//bool EleMVANonTrigId(float pt, float eta, float value){

bool EleMVANonTrigId_Loose(myobject const& a) { //REVISIED based on HTT groiup definition
    bool passingId = false;
    float pt = a.pt;
    float eta = fabs(a.eta_SC);
    float value = a.Id_mvaNonTrg;


    if (pt < 20. && eta < 0.8 && value > 0.925)
        passingId = true;
    if (pt < 20. && eta >= 0.8 && eta < 1.479 && value > 0.915)
        passingId = true;
    if (pt < 20. && eta >= 1.479 && value > 0.965)
        passingId = true;
    if (pt > 20. && eta < 0.8 && value > 0.905)
        passingId = true;
    if (pt > 20. && eta >= 0.8 && eta < 1.479 && value > 0.955)
        passingId = true;
    if (pt > 20. && eta >= 1.479 && value > 0.975)
        passingId = true;

    bool numHit = a.numHitEleInner < 1;
    bool ConversionVeto = a.passConversionVeto;
    bool Ele_d0 = a.dxy_PV < 0.045; //the impact parameter in the transverse plane
    bool Ele_dZ = a.dz_PV < 0.2; //the impact parameter in the transverse plane

    return passingId && numHit && ConversionVeto && Ele_d0 && Ele_dZ;
}

bool EleMVANonTrigId_Tight(myobject const& a) { //REVISIED based on HTT groiup definition
    bool passingId = false;
    float pt = a.pt;
    float eta = fabs(a.eta_SC);
    float value = a.Id_mvaNonTrg;



    if (pt > 20. && eta < 0.8 && value > 0.925)
        passingId = true;
    if (pt > 20. && eta >= 0.8 && eta < 1.479 && value > 0.975)
        passingId = true;
    if (pt > 20. && eta >= 1.479 && value > 0.985)
        passingId = true;

    bool numHit = a.numHitEleInner < 1;
    bool ConversionVeto = a.passConversionVeto;
    bool Ele_d0 = a.dxy_PV < 0.045; //the impact parameter in the transverse plane
    bool Ele_dZ = a.dz_PV < 0.2; //the impact parameter in the transverse plane

    return passingId && numHit && ConversionVeto && Ele_d0 && Ele_dZ;
}

bool EleLooseForEtauVeto(myobject const& a) {

    if (fabs(a.dz_PV) >= 0.2) return false;

    bool EB = fabs(a.eta_SC) < 1.479;
    bool EE = fabs(a.eta_SC) >= 1.479;

    float hoe = a.HoverE;
    float deta = fabs(a.deltaEtaSuperClusterTrackAtVtx);
    float dphi = fabs(a.deltaPhiSuperClusterTrackAtVtx);
    float sihih = a.sigmaIetaIeta;
    bool Ele_d0 = a.dxy_PV < 0.045; //the impact parameter in the transverse plane
    bool Ele_dZ = a.dz_PV < 0.2; //the impact parameter in the transverse plane
    
    if (Ele_d0 && Ele_dZ && EB && sihih < 0.010 && dphi < 0.80 && deta < 0.007 && hoe < 0.15) return true;
    else if (Ele_d0 && Ele_dZ &&  EE && sihih < 0.030 && dphi < 0.70 && deta < 0.010 ) return true; //  hoe < 0.07 is droped at October 6th
    else return false;


}

//bool getTauIsolation(std::string channel, myobject const& a) {
//    if (channel == "mmtt" || channel == "eett") {
//        //        if (a.byMediumCombinedIsolationDeltaBetaCorr3Hits) //changed in 19April
//        if (a.byLooseCombinedIsolationDeltaBetaCorr3Hits)
//            return true;
//        else
//            return false;
//    }
//    if (channel == "mmet" || channel == "eeet" || channel == "mmmt" || channel == "eemt") {
//        if (a.byLooseCombinedIsolationDeltaBetaCorr3Hits)
//            return true;
//        else
//            return false;
//    }
//}

//bool getEleRejection(std::string channel, myobject const& a) {
//    if (channel == "mmtt" || channel == "eett" || channel == "mmmt" || channel == "eemt") {
//        if (a.discriminationByElectronLoose)
//            return true;
//        else
//            return false;
//    }
//    if (channel == "mmet" || channel == "eeet") {
//        //        if (a.discriminationByElectronMVA2Tight) //changed in 19April
//        //        if (a.discriminationByElectronMVA3Tight)
//        if (a.discriminationByElectronMVA5Tight) // There is no 3 ???? but 5
//            return true;
//        else
//            return false;
//    }
//
//}

//bool getMuRejection(std::string channel, myobject const& a) {
//    if (channel == "mmtt" || channel == "eett" || channel == "mmet" || channel == "eeet") {
//        if (a.discriminationByMuonLoose2)
//            return true;
//        else
//            return false;
//    }
//    if (channel == "mmmt" || channel == "eemt") {
//        if (a.discriminationByMuonTight2)
//            return true;
//        else
//            return false;
//    }
//
//}




#endif
