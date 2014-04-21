#ifndef __MYGENOBJECT_HH__
#define __MYGENOBJECT_HH__
#include "TROOT.h"
#include "TObject.h"
using namespace std;
#include <vector>

class myGenobject : public TObject {
public:

    myGenobject() {
        ;
    }

    ~myGenobject() {
        ;
    }

    //General



    float pt, eta,  phi, charge, z, mass, et;
    float mod_pt, mod_eta, mod_phi, mod_charge, mod_z, mod_mass;
    float Gmod_pt, Gmod_eta, Gmod_phi, Gmod_charge, Gmod_z, Gmod_mass;
    //    float pfIsoAll, pfIsoCharged, pfIsoNeutral, pfIsoGamma, pfIsoPU, pfIsoPULow,Id_mvaTrg,Id_mvaNonTrg;
    // float pfIsoAll_NoPFId, pfIsoCharged_NoPFId, pfIsoNeutral_NoPFId, pfIsoGamma_NoPFId, pfIsoPU_NoPFId;
    // float z_expo;
    // float position_Rho, position_rho;

     int pdgId, status, mod_pdgId, mod_status, Gmod_pdgId, Gmod_status;
     int gen_index, decay_mode;
     
/*     //Muon */
/*     float dB, d0, emfraction; */
/*     float DepositR03Ecal; */
/*     float DepositR03Hcal; */
/*     float DepositR03TrackerOfficial; */
/*     float alpha; */
/*     bool GlobalMuonPromptTight; */
/*     bool TMOneStationLoose; */
/*     bool TM2DCompatibilityLoose; */
/*     bool isGlobalMuon, isTrackerMuon, isStandAloneMuon, isPFMuon; */
/*     int numberOfValidMuonHits, numberOfHits,numMatchStation; */
/*     int numLostHitEle, numValidHitEle, numHitEleInner, numLostHitEleInner; */
/*    int normalizedChi2_innTrk, numberOfValidMuonHits_innTrk,  numberOfHits_innTrk; */
/*    float normalizedChi2; */
/*    int trkLayerMeasure , intrkLayerMeasure,  intrkLayerpixel; */
/*     float    dxy_in , dZ_in ; */


/*     //Vertex Mettopology Trigger */
/*     bool isFake, isValid; */
/*     float ndof; */
/*     unsigned int Num_Vertex; */
/*     //    float METTOPOLOGY, tauRecoilEnergy; */
/*     bool jetId_loose, jetId_tight; */
/*     //    float TrgObjectEta, TrgObjectPt, TrgObjectPhi; */


/*     //Electron */
/*     float HoverE, deltaPhiSuperClusterTrackAtVtx, deltaEtaSuperClusterTrackAtVtx, sigmaIetaIeta, sigmaEtaEta; */
/*     float ecalIso, hcalIso, caloIso, trackIso, hcalOverEcal, SIP; */

/*     //For jet and taus */
/*     float bDiscriminatiors_CSV,bDiscriminatiors_JP,bDiscriminatiors_TCHPT; */
/*     float jetPt, jetEta, jetPhi; */
/*     float leadChargedParticlePt, leadTrackD0; */
/*     bool puJetIdLoose, puJetIdMedium, puJetIdTight; */
/*     //    float leadChargedParticlePt, leadNeutralParticlePt, leadParticlePt, leadTrackD0; */
/*     float mva_e_pi, mva_pi_mu, mva_e_mu, hcalEnergy, ecalEnergy, trackRefPt; */
/*     int numChargedParticlesSignalCone, numNeutralHadronsSignalCone, numPhotonsSignalCone, numParticlesSignalCone; */
/*     int numChargedParticlesIsoCone, numNeutralHadronsIsoCone, numPhotonsIsoCone, numParticlesIsoCone; */
/*     float ptSumChargedParticlesIsoCone, ptSumPhotonsIsoCone; */
/*     //    float discriminationByLeadingTrackFinding; */
/*     //    float discriminationByLeadingTrackPtCut; */
/*     //    float discriminationByDecayModeSelection; */
/*     //    float discriminationByFlightPath; */
/*     //    float discriminationByTaNC; */
/*     //    float discriminationByTaNCRaw; */
/*     //    float discriminationByTaNCVeryLoose; */
/*     //    float discriminationByTaNCLoose; */
/*     //    float discriminationByTaNCMedium; */
/*     //    float discriminationByTaNCTight; */
/*     bool discriminationByDecayModeFinding; */
/*     bool discriminationByVeryLooseIsolation; */
/*     bool discriminationByLooseIsolation; */
/*     bool discriminationByMediumIsolation; */
/*     bool discriminationByTightIsolation; */
/*     bool discriminationByElectronLoose; */
/*     bool discriminationByElectronMedium; */
/*     bool discriminationByElectronTight; */
/*     bool discriminationByElectronMVA; */
/*     bool discriminationByElectronMVA2Loose; */
/*     bool discriminationByElectronMVA2Medium; */
/*     bool discriminationByElectronMVA2Tight; */
/*     bool discriminationByElectronMVA3Loose; */
/*     bool discriminationByElectronMVA3Medium; */
/*     bool discriminationByElectronMVA3Tight; */
/*     bool discriminationByElectronMVA3VTight; */

/*     bool discriminationByMuonLoose; */
/*     bool discriminationByMuonMedium; */
/*     bool discriminationByMuonTight; */
/*     bool discriminationByMuonLoose2; */
/*     bool discriminationByMuonMedium2; */
/*     bool discriminationByMuonTight2; */
/*     bool byVLooseCombinedIsolationDeltaBetaCorr; */
/*     bool byLooseCombinedIsolationDeltaBetaCorr; */
/*     bool byMediumCombinedIsolationDeltaBetaCorr; */
/*     bool byTightCombinedIsolationDeltaBetaCorr; */
/*     bool byLooseCombinedIsolationDeltaBetaCorr3Hits; */
/*     bool byMediumCombinedIsolationDeltaBetaCorr3Hits; */
/*     bool byTightCombinedIsolationDeltaBetaCorr3Hits; */



/*     float byIsolationMVAraw; */
/*     float byIsolationMVA2raw; */
/*     bool byLooseIsolationMVA; */
/*     bool byMediumIsolationMVA; */
/*     bool byTightIsolationMVA; */
/*     bool byLooseIsolationMVA2; */
/*     bool byMediumIsolationMVA2; */
/*     bool byTightIsolationMVA2; */

/*   /\*   float CicVeryLoose; *\/ */
/* /\*     float CicLoose; *\/ */
/* /\*     float CicMedium; *\/ */
/* /\*     float CicTight; *\/ */
/* /\*     float CicSuperTight; *\/ */

/*     //    float CicHZZVeryLoose; */
/*     //    float CicHZZLoose; */
/*     //    float CicHZZMedium; */
/*     //    float CicHZZTight; */
/*     //    float CicHZZSuperTight; */

/* //    trigger matching */
/*         bool hasTrgObject_loose ; */
/*         float TrgObjectEta_loose ; */
/*         float TrgObjectPt_loose ; */
/*         float TrgObjectPhi_loose ; */

/*         bool hasTrgObject_medium ; */
/*         float TrgObjectEta_medium ; */
/*         float TrgObjectPt_medium ; */
/*         float TrgObjectPhi_medium ; */


    ClassDef(myGenobject, 1)
};
#endif
