// system include files
// http://cmslxr.fnal.gov/lxr/source/RecoJets/JetAlgorithms/src/JetIDHelper.cc?v=CMSSW_3_3_6
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Utilities/interface/InputTag.h"
// gen particles
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// parton jets
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
// MC info
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
//#include "CLHEP/HepPDT/DefaultConfig.hh"
//
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "DataFormats/Math/interface/deltaR.h"
//double dR = deltaR( c1, c2 );
//
// root
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
//
using namespace std;
using namespace reco;

//
// class declaration
//

class nmssm_bba2tau : public edm::EDAnalyzer {
public:
  explicit nmssm_bba2tau(const edm::ParameterSet&);
  ~nmssm_bba2tau();


private:
  //      virtual void beginJob(const edm::EventSetup&) ;
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  // output root file
  string fOutputFileName ;
  // names of modules, producing object collections
  edm::InputTag partonjetsSrc; 
  // variables to store in ntpl
  double ptb1, etab1, phib1;
  double ptb2, etab2, phib2;
  double ptmu, etamu, phimu;
  double pttauh, etatauh, phitauh;
  //
  TFile*      hOutputFile ;
  TTree*      t1;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

// ------------ method called once each job just before starting event loop  ------------
void 
//nmssm_bba2tau::beginJob(const edm::EventSetup&)
nmssm_bba2tau::beginJob()
{
  using namespace edm;

  // creating a simple tree

  hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;

  t1 = new TTree("t1","analysis tree");

  t1->Branch("ptb1",&ptb1,"ptb1/D");
  t1->Branch("etab1",&etab1,"etab1/D");
  t1->Branch("phib1",&phib1,"phib1/D");
  //
  t1->Branch("ptb2",&ptb2,"ptb2/D");
  t1->Branch("etab2",&etab2,"etab2/D");
  t1->Branch("phib2",&phib2,"phib2/D");
  //
  t1->Branch("ptmu",&ptmu,"ptmu/D");
  t1->Branch("etamu",&etamu,"etamu/D");
  t1->Branch("phimu",&phimu,"phimu/D");
  //
  t1->Branch("pttauh",&pttauh,"pttauh/D");
  t1->Branch("etatauh",&etatauh,"etatauh/D");
  t1->Branch("phitauh",&phitauh,"phitauh/D");
  //  t1->Branch("EtaRaw","vector<double>",&EtaRaw);

  return ;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
nmssm_bba2tau::endJob() {

  hOutputFile->Write() ;
  hOutputFile->Close() ;

  return ;
}

//
// constructors and destructor
//
nmssm_bba2tau::nmssm_bba2tau(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  using namespace edm;
  // 
  // get name of output file with histogramms
  fOutputFileName = iConfig.getUntrackedParameter<string>("HistOutFile");
  //
  // get parton jets
  partonjetsSrc      = iConfig.getParameter<edm::InputTag>("parton_jets");
}


nmssm_bba2tau::~nmssm_bba2tau()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
nmssm_bba2tau::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  ptb1    = 0.; 
  etab1   = 0.; 
  phib1   = 0.;
  ptb2    = 0.; 
  etab2   = 0.; 
  phib2   = 0.;
  ptmu    = 0.; 
  etamu   = 0.; 
  phimu   = 0.;
  pttauh  = 0.; 
  etatauh = 0.; 
  phitauh = 0.;

  edm::Handle<GenEventInfoProduct> genEvt;;
  iEvent.getByLabel("generator",genEvt);

  //  int procid = (int) genEvt->signalProcessID();

  //  edm::Handle<std::vector<PileupSummaryInfo> > infoPU;
  //  iEvent.getByLabel("addPileupInfo",infoPU);
  //  for(std::vector<PileupSummaryInfo>::const_iterator it = infoPU->begin(); it != infoPU->end(); it++)
  //    {
  //      genPU = it->getPU_NumInteractions();
  //      if (verbosity > 0) {   std::cout<<" Pileup "<<genPU<<std::endl;}
  //    }

  //  edm::Handle<reco::CandidateView> mctruth;
  //  for (unsigned int i = 0; i < mctruth->size(); ++ i) {
  //    if(ii > 29) break;
  //    const reco::Candidate& p = (*mctruth)[i];
  //  }
  


  //  cout <<"  Event particles " << endl;

  edm::Handle<reco::GenParticleCollection> genparticles;
  iEvent.getByLabel("genParticles", genparticles);

  math::XYZTLorentzVector  tauh(0.,0.,0.,0.);

  for( size_t i = 0; i < genparticles->size(); i++)
    {
      const reco::GenParticle & p = (*genparticles)[i];

      int motherID = 0;
      int motherSt = 0;

      int grmotherID = 0;
      int grmotherSt = 0;

      int grgrmotherID = 0;
      int grgrmotherSt = 0;
      double grgrmotherpx = 0.;

      const Candidate * moth = p.mother();
      if(moth) {
	motherID = moth->pdgId();
	motherSt = moth->status();	
	const Candidate * grmoth = moth->mother();
	if(grmoth) {
	  grmotherID = grmoth->pdgId();
	  grmotherSt = grmoth->status();
	  const Candidate * grgrmoth = grmoth->mother();
	  if(grgrmoth) {
	    grgrmotherID = grgrmoth->pdgId();
	    grgrmotherSt = grgrmoth->status();
	    grgrmotherpx = grgrmoth->px();
	  }	  
	}
      }
      /*
      cout <<" i " << i 
	   <<" ID " << p.pdgId() 
	   <<" status " << p.status() 
	   <<" motherID " << motherID 
	   <<" motherST " << motherSt
	   <<" grmotherID " << grmotherID 
	   <<" grmotherST " << grmotherSt 
	   <<" grgrmotherID " << grgrmotherID 
	   <<" grgrmotherST " << grgrmotherSt
	   <<" grgrmotherpx " << grgrmotherpx << endl;
      */

      // select muon from tau->mu nu from h->2tau
      if(abs(p.pdgId()) == 13 && p.status() == 1 &&
	 abs(motherID) == 15 && motherSt == 2 &&
	 abs(grmotherID) == 15 && grmotherSt == 3 &&
	 grgrmotherID == 36) {
	ptmu  = p.pt();
	etamu = p.eta();
	phimu = p.phi();
	//	cout <<"   --> muon from tau decay " << i << endl;
      }

      // select tau_h decay products from h->2tau
      if(abs(p.pdgId()) != 13 && abs(p.pdgId()) != 16 & 
	 abs(p.pdgId()) != 14 && abs(p.pdgId()) != 24 &
	 abs(motherID) == 15 && motherSt == 2 &&
	 abs(grmotherID) == 15 && grmotherSt == 3 &&
	 grgrmotherID == 36) {
	math::XYZTLorentzVector tauh_c(p.px(),p.py(),p.pz(),p.p());
	tauh += tauh_c;
	//	cout <<"   --> product of tau->hadr decay " << i << endl;
      }
      // select tau_h decay products from h->2tau
      if(abs(p.pdgId()) != 13 && abs(p.pdgId()) != 16 & 
	 abs(p.pdgId()) != 14 && abs(p.pdgId()) != 24 &
	 abs(motherID)     == 24  &&
	 abs(grmotherID)   == 15 && grmotherSt   == 2 &&
	 abs(grgrmotherID) == 15 && grgrmotherSt == 3) 
	{
	  math::XYZTLorentzVector tauh_c(p.px(),p.py(),p.pz(),p.p());
	  tauh += tauh_c;
	  //	  cout <<"   --> product of tau->hadr decay " << i << endl;
	}

      // select b quarks
      if(p.pdgId() == 5 && p.status() == 3) {
	ptb1  = p.pt();
	etab1 = p.eta();
	phib1 = p.phi();
	//	cout <<"   --> b-quark " << i <<" ptx = " << p.px() << endl;
      }

      // select b_bar quarks
      if(p.pdgId() == -5 && p.status() == 3) {
	ptb2  = p.pt();
	etab2 = p.eta();
	phib2 = p.phi();
	//	cout <<"   --> unti-b-quark " << i <<" ptx = " << p.px() << endl;
      }
    }

  pttauh  = tauh.pt();
  etatauh = tauh.eta();
  phitauh = tauh.phi();

  t1->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(nmssm_bba2tau);
