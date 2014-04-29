#ifndef __MYOBJECT_HH__
#define __MYOBJECT_HH__
#include "TROOT.h"
#include "TObject.h"
using namespace std;
#include <vector>

class myobject : public TObject {
public:

    myobject() {
        ;
    }

    ~myobject() {
        ;
    }

    //General



    float pt, eta, px, py, phi, charge, E, et, pz, z, mass, dz_Ver_match, Energy, mt, jetMass, eta_SC;
    float mod_pt, mod_eta, mod_phi, mod_charge, mod_z, mod_mass;
    float Gmod_pt, Gmod_eta, Gmod_phi, Gmod_charge, Gmod_z, Gmod_mass;
    float pfIsoAll, pfIsoCharged, pfIsoNeutral, pfIsoGamma, pfIsoPU, pfIsoPULow,Id_mvaTrg,Id_mvaNonTrg;
    float pfIsoAll_NoPFId, pfIsoCharged_NoPFId, pfIsoNeutral_NoPFId, pfIsoGamma_NoPFId, pfIsoPU_NoPFId;
    float z_expo;
    float position_Rho, position_rho;

     int pdgId, status, mod_pdgId, mod_status, Gmod_pdgId, Gmod_status, tracksSize;
     int gen_index, decay_mode;
    //Muon
     float dB, d0, emfraction;
    float DepositR03Ecal;
    float DepositR03Hcal;
    float DepositR03TrackerOfficial;
    float alpha;
    bool GlobalMuonPromptTight;
    bool TMOneStationLoose;
    bool TM2DCompatibilityLoose;
    bool isGlobalMuon, isTrackerMuon, isStandAloneMuon, isPFMuon;
    int numberOfValidMuonHits, numberOfHits,numMatchStation;
    int numLostHitEle, numValidHitEle, numHitEleInner, numLostHitEleInner;
   int normalizedChi2_innTrk, numberOfValidMuonHits_innTrk,  numberOfHits_innTrk;
   float normalizedChi2;
   int trkLayerMeasure , intrkLayerMeasure,  intrkLayerpixel;
    float    dxy_in , dZ_in ;
    
    // 2D IP wrt primary vertex
    float dxy_PV,dz_PV;
    // 3D impact parameter
    float IP3D;

    //Vertex Mettopology Trigger
    bool isFake, isValid;
    float ndof;
    unsigned int Num_Vertex;
    //    float METTOPOLOGY, tauRecoilEnergy;
    bool jetId_loose, jetId_tight;
    //    float TrgObjectEta, TrgObjectPt, TrgObjectPhi;


    //Electron
    float HoverE, deltaPhiSuperClusterTrackAtVtx, deltaEtaSuperClusterTrackAtVtx, sigmaIetaIeta, sigmaEtaEta;
    float ecalIso, hcalIso, caloIso, trackIso, hcalOverEcal, SIP;
    bool passConversionVeto;
    float rawE_SC, preshowerE_SC;
    float EleId95rel, EleId90rel, EleId85rel, EleId80rel, EleId70rel, EleId60rel;
    float CicVeryLoose, CicLoose, CicMedium, CicTight, CicSuperTight;

    //For jet and taus
    int decayMode;
    float bDiscriminatiors_CSV,bDiscriminatiors_JP,bDiscriminatiors_TCHPT;
    float jetPt, jetEta, jetPhi;
    float leadChargedParticlePt, leadTrackD0;
    bool puJetIdLoose, puJetIdMedium, puJetIdTight;
    //    float leadChargedParticlePt, leadNeutralParticlePt, leadParticlePt, leadTrackD0;
    float mva_e_pi, mva_pi_mu, mva_e_mu, hcalEnergy, ecalEnergy, trackRefPt;
    int numChargedParticlesSignalCone, numNeutralHadronsSignalCone, numPhotonsSignalCone, numParticlesSignalCone, signalPiZeroCandidates;
    int numChargedParticlesIsoCone, numNeutralHadronsIsoCone, numPhotonsIsoCone, numParticlesIsoCone;
    float ptSumChargedParticlesIsoCone, ptSumPhotonsIsoCone;
    
    float sig_track1_pt, sig_track1_phi, sig_track1_eta, sig_track1_m;
    float sig_track2_pt, sig_track2_phi, sig_track2_eta, sig_track2_m;
    float sig_track3_pt, sig_track3_phi, sig_track3_eta, sig_track3_m;

    float sig_pi0_1_pt, sig_pi0_1_phi, sig_pi0_1_eta, sig_pi0_1_m;
    float sig_pi0_2_pt, sig_pi0_2_phi, sig_pi0_2_eta, sig_pi0_2_m;

    bool discriminationByDecayModeFinding;
    bool discriminationByVeryLooseIsolation;
    bool discriminationByLooseIsolation;
    bool discriminationByMediumIsolation;
    bool discriminationByTightIsolation;
    bool discriminationByElectronLoose;
    bool discriminationByElectronMedium;
    bool discriminationByElectronTight;
    bool discriminationByElectronMVA5VLoose;
    bool discriminationByElectronMVA5Loose;
    bool discriminationByElectronMVA5Medium;
    bool discriminationByElectronMVA5Tight;
    bool discriminationByElectronMVA5VTight;

    bool discriminationByMuonLoose;
    bool discriminationByMuonMedium;
    bool discriminationByMuonTight;
    bool discriminationByMuonLoose2;
    bool discriminationByMuonMedium2;
    bool discriminationByMuonTight2;
    bool discriminationByMuonLoose3;
    bool discriminationByMuonTight3;
    bool discriminationByMuonMVALoose;
    bool discriminationByMuonMVAMedium;
    bool discriminationByMuonMVATight;

    bool byVLooseCombinedIsolationDeltaBetaCorr;
    bool byLooseCombinedIsolationDeltaBetaCorr;
    bool byMediumCombinedIsolationDeltaBetaCorr;
    bool byTightCombinedIsolationDeltaBetaCorr;
    bool byLooseCombinedIsolationDeltaBetaCorr3Hits;
    bool byMediumCombinedIsolationDeltaBetaCorr3Hits;
    bool byTightCombinedIsolationDeltaBetaCorr3Hits;
    float byRawCombinedIsolationDeltaBetaCorr3Hits;

    float byIsolationMVAraw;
    float byIsolationMVA2raw;
    float discriminationByMuonMVAraw;
    bool byLooseIsolationMVA;
    bool byMediumIsolationMVA;
    bool byTightIsolationMVA;
    bool byLooseIsolationMVA2;
    bool byMediumIsolationMVA2;
    bool byTightIsolationMVA2;

    float byIsolationMVA3oldDMwLTraw;
    float byIsolationMVA3newDMwLTraw;
    float byIsolationMVA3oldDMwoLTraw;
    float byIsolationMVA3newDMwoLTraw;

    bool byVLooseIsolationMVA3oldDMwLT;
    bool byVLooseIsolationMVA3newDMwLT;
    bool byVLooseIsolationMVA3oldDMwoLT;
    bool byVLooseIsolationMVA3newDMwoLT;
    bool byLooseIsolationMVA3oldDMwLT;
    bool byLooseIsolationMVA3newDMwLT;
    bool byLooseIsolationMVA3oldDMwoLT;
    bool byLooseIsolationMVA3newDMwoLT;
    bool byMediumIsolationMVA3oldDMwLT;
    bool byMediumIsolationMVA3newDMwLT;
    bool byMediumIsolationMVA3oldDMwoLT;
    bool byMediumIsolationMVA3newDMwoLT;
    bool byTightIsolationMVA3oldDMwLT;
    bool byTightIsolationMVA3newDMwLT;
    bool byTightIsolationMVA3oldDMwoLT;
    bool byTightIsolationMVA3newDMwoLT;
    bool byVTightIsolationMVA3oldDMwLT;
    bool byVTightIsolationMVA3newDMwLT;
    bool byVTightIsolationMVA3oldDMwoLT;
    bool byVTightIsolationMVA3newDMwoLT;
    bool byVVTightIsolationMVA3oldDMwLT;
    bool byVVTightIsolationMVA3newDMwLT;
    bool byVVTightIsolationMVA3oldDMwoLT;
    bool byVVTightIsolationMVA3newDMwoLT;

    bool discriminationByDecayModeFindingNewDMs;
    bool discriminationByDecayModeFindingOldDMs;
    float discriminationByRawCombinedIsolationDBSumPtCorr;
    float MVA3IsolationChargedIsoPtSum;
    float MVA3IsolationNeutralIsoPtSum;
    float MVA3IsolationPUcorrPtSum;
    float discriminationByMVA5rawElectronRejection;
    float discriminationByMVA5rawElectronRejectionCategory;
    bool discriminationByDeadECALElectronRejection;

//    trigger matching
        bool hasTrgObject_Mu17Tau20 ;
        float TrgObjectEta_Mu17Tau20 ;
        float TrgObjectPt_Mu17Tau20 ;
        float TrgObjectPhi_Mu17Tau20 ;
        bool hasTrgObject_SoftMu ;
        float TrgObjectEta_SoftMu ;
        float TrgObjectPt_SoftMu ;
        float TrgObjectPhi_SoftMu ;
        bool hasTrgObject_SoftEle ;
        float TrgObjectEta_SoftEle ;
        float TrgObjectPt_SoftEle ;
        float TrgObjectPhi_SoftEle ;
        bool hasTrgObject_Mu18Tau25 ;
        float TrgObjectEta_Mu18Tau25 ;
        float TrgObjectPt_Mu18Tau25 ;
        float TrgObjectPhi_Mu18Tau25 ;
        bool hasTrgObject_Ele20Tau20 ;
        float TrgObjectEta_Ele20Tau20 ;
        float TrgObjectPt_Ele20Tau20 ;
        float TrgObjectPhi_Ele20Tau20 ;
        bool hasTrgObject_Ele8Mu17 ;
        float TrgObjectEta_Ele8Mu17 ;
        float TrgObjectPt_Ele8Mu17 ;
        float TrgObjectPhi_Ele8Mu17 ;
        bool hasTrgObject_Ele17Mu8 ;
        float TrgObjectEta_Ele17Mu8 ;
        float TrgObjectPt_Ele17Mu8 ;
        float TrgObjectPhi_Ele17Mu8 ;
        bool hasTrgObject_Ditau30Jet30 ;
        float TrgObjectEta_Ditau30Jet30 ;
        float TrgObjectPt_Ditau30Jet30 ;
        float TrgObjectPhi_Ditau30Jet30 ;
        bool hasTrgObject_Ditau35 ;
        float TrgObjectEta_Ditau35 ;
        float TrgObjectPt_Ditau35 ;
        float TrgObjectPhi_Ditau35 ;
        bool hasTrgObject_Mu24 ;
        float TrgObjectEta_Mu24 ;
        float TrgObjectPt_Mu24 ;
        float TrgObjectPhi_Mu24 ;

    ClassDef(myobject, 1)
};
#endif
