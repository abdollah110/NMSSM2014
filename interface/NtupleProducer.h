// system include_ files
#include <memory>
#include <string>
#include <iostream>

//module definition
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Utilities/interface/typelookup.h"
#include "Math/GenVector/VectorUtil.h"

//for HepMC
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

// user include files
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
//for geometry essource load
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ref.h"


#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"//for muon namespace
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronIsoCollection.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaEcalIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "RecoEcal/EgammaClusterProducers/interface/Multi5x5ClusterProducer.h"
#include "PhysicsTools/SelectorUtils/interface/SimpleCutBasedElectronIDSelectionFunctor.h"

//for ecal isolation
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

//for trigger results
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"


//for muon isolation
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
//For Hep3Vector
#include <CLHEP/Vector/LorentzVector.h>
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//Tau
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/BaseTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "RecoTauTag/TauTagTools/interface/PFTauElementsOperators.h"
#include "RecoTauTag/RecoTau/interface/TauDiscriminationProducerBase.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/TriggerObject.h"

//metTopology


//HLT
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

// PileUp reweighting
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
// Transient tracks
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

// Other specific
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

//Dz parameter
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "TrackingTools/TransientTrack/interface/GsfTransientTrack.h"

//Jet ES uncertainty
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Type1MET/interface/JetCorrExtractorT.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/PatCandidates/interface/MET.h"


#include "Analysis/NtupleProducer/interface/myevent.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

using namespace std;
using namespace reco;
using namespace edm;



//
// class decleration
//

class NtupleProducer : public edm::EDAnalyzer {
public:
    explicit NtupleProducer(const edm::ParameterSet&);
    ~NtupleProducer();


private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    void DoHLTAnalysis(const edm::Event&);
    void DoElectronAnalysis(const edm::Event&, const edm::EventSetup&);
    void DoMuonAnalysis(const edm::Event&, const edm::EventSetup&);
    void DoHPSTauAnalysis(const edm::Event&, const edm::EventSetup&);
    void DoJetAnalysis(const edm::Event&);
    void DoJetEC_Uncer_Analysis(const edm::Event&);
    void DoGenParticlesAnalysis(const edm::Event&);
    void DoMetAnalysis(const edm::Event&);
    void DoVertexAnalysis(const edm::Event&);
    void DoMetUncertaityAnalysis(const edm::Event&);


    // ----------member data ---------------------------
    //=========== config parameters ==================
    string fOutputFileName;

    edm::InputTag TrackCollection_;
    edm::InputTag VertexCollection_;
    edm::InputTag JetCollection_;
    edm::InputTag BJetCollection_;
    edm::InputTag rhoCorrection_;

    edm::InputTag rhoCenChargedPU_;
    edm::InputTag rhoCenNeutral_;
    edm::InputTag rhoCenNeutralTight_;

    edm::InputTag MetCollection_;
    edm::InputTag PFMetCollection_;
    edm::InputTag tcMetCollection_;
    edm::InputTag Type1CorMETCollection_;
    edm::InputTag MVAMetCollection_;

    edm::InputTag PreSelectedElectronCollection_;
    edm::InputTag PreSelectedMuonCollection_;
    edm::InputTag PreSelectedhpsCollection_;
    edm::InputTag srcTriggerResults_;

/*     std::vector<edm::InputTag> eleIDTag_; */
/*     edm::InputTag EleID_VeryLooseTag_; */
/*     edm::InputTag EleID_LooseTag_; */
/*     edm::InputTag EleID_MediumTag_; */
/*     edm::InputTag EleID_TightTag_; */

    edm::InputTag PileUpInfo_;
    edm::InputTag GenParticlesInfo_;
    edm::InputTag triggerEvent_;
    std::string tauMatch_Loose_;
    std::string electronMatch_Loose_;
    std::string tauMatch_Medium_;
    std::string muonMatch_Loose_;

    bool Include_HPSTau;
    bool Include_Muon;
    bool Include_Electron;
    bool Include_Jet;
    bool Include_JetCorrection;
    bool Include_MET_Uncertaity;
    bool Include_MET;
    bool Include_HLT;
    bool Include_Vertex;
    bool Include_GenPartiles;
    bool IsMC;
    
    bool filterTriggerResults;
    edm::InputTag muonPFIsoValueGammaTag_;
    edm::InputTag puJetIdFlag_;
    edm::InputTag rhoProducer_;

    std::string el_trigger_name;
    std::string mu_trigger_name;
    
    double tauPtcut_;

    myevent *m; // for root tree definition
    TTree *t;
    TFile* hOutputFile;


};
