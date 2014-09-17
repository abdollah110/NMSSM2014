#ifndef __MYEVENT_HH__
#define __MYEVENT_HH__
#include "TObject.h"
using namespace std;
#include <vector>
#include <map>
#include <utility>
#include "myobject.h"
#include "myGenobject.h"

class myevent : public TObject {
public:

    myevent() {
        ;
    }

    ~myevent() {
        ;
    }

    vector<myobject> RecPFJetsAK5;
    vector<myGenobject> RecGenParticle;
    vector<myGenobject> RecGenMet;
    vector<myGenobject> RecGenJet;
    vector<myGenobject> RecGenTauVisible;
    vector<myobject> PreSelectedElectrons;
    vector<myobject> L2Particles;
    vector<myobject> L1Tau;
    vector<myobject> L1Jet;
    vector<myobject> L1Etm;
    vector<myobject> PreSelectedMuons;
    vector<myobject> PreSelectedHPSTaus;
    vector<myobject> PreSelectedHPSTausLT;
    vector<myobject> PreSelectedHPSTausTT;
    //    vector<myobject> RecMet;
    vector<myobject> RecPFMet;
    vector<myobject> RecPFMetCor;
    vector<myobject> RecMVAMet;
    vector<myobject> PairMet_etau;
    vector<myobject> PairMet_mutau;
    vector<myobject> PairMet_tautau;
    vector<myobject> PairMet_emu;
    vector<myobject> PairRecoilMet_etau;
    vector<myobject> PairRecoilMet_mutau;
    vector<myobject> PairRecoilMet_tautau;
    vector<myobject> PairRecoilMet_emu;
    vector<float> PairMet_etau_sigMatrix_00;
    vector<float> PairMet_etau_sigMatrix_10;
    vector<float> PairMet_etau_sigMatrix_01;
    vector<float> PairMet_etau_sigMatrix_11;
    vector<float> PairMet_mutau_sigMatrix_00;
    vector<float> PairMet_mutau_sigMatrix_10;
    vector<float> PairMet_mutau_sigMatrix_01;
    vector<float> PairMet_mutau_sigMatrix_11;
    vector<float> PairMet_tautau_sigMatrix_00;
    vector<float> PairMet_tautau_sigMatrix_10;
    vector<float> PairMet_tautau_sigMatrix_01;
    vector<float> PairMet_tautau_sigMatrix_11;
    vector<float> PairMet_emu_sigMatrix_00;
    vector<float> PairMet_emu_sigMatrix_10;
    vector<float> PairMet_emu_sigMatrix_01;
    vector<float> PairMet_emu_sigMatrix_11;
    vector<myobject> RecoilMet; 
    vector<myobject> CorrectedL1ETM;
    vector<myobject> UncorrectedL1ETM;
    vector<myobject> CorrectedCaloMetNoHF;
    vector<myobject> MetNoHFresidualCorrected;
    vector<myobject> MetNoHFresidualCorrectedUp;
    vector<myobject> MetNoHFresidualCorrectedDown;
    vector<myobject> MetNoHF;
    vector<myobject> RectcMet;
    vector<myobject> RecPFMetCorElectronEnUp;
    vector<myobject> RecPFMetCorElectronEnDown;
    vector<myobject> RecPFMetCorMuonEnUp;
    vector<myobject> RecPFMetCorMuonEnDown;
    vector<myobject> RecPFMetCorTauEnUp;
    vector<myobject> RecPFMetCorTauEnDown;
    vector<myobject> RecPFMetCorJetEnUp;
    vector<myobject> RecPFMetCorJetEnDown;
    vector<myobject> RecPFMetCorJetResUp;
    vector<myobject> RecPFMetCorJetResDown;
    vector<myobject> RecPFMetCorUnclusteredEnUp;
    vector<myobject> RecPFMetCorUnclusteredEnDown;
    vector<myobject> smearedPatJets;
    vector<myobject> smearedPatJetsResUp;
    vector<myobject> smearedPatJetsResDown;
    vector<myobject> shiftedPatJetsEnUpForCorrMEt;
    vector<myobject> shiftedPatJetsEnDownForCorrMEt;
    vector<myobject> cleanPatJets;
    vector<myobject> Vertex;

    map<string, int> HLT;

    unsigned int runNumber;
    unsigned int eventNumber;
    unsigned int lumiNumber;
    float embeddingWeight;
    float spinnerWeight;
    float PU_Weight;
    float PUInfo;
    float PUInfo_true;
    int PUInfo_Bunch0;
    float RhoCorr;
    float RhoCenNeutral;
    float RhoCenCharged;
    float RhoCenNeutralTight;
    float Rho;

    // MET significance matrix
    float MET_sigMatrix_00;
    float MET_sigMatrix_01;
    float MET_sigMatrix_10;
    float MET_sigMatrix_11;

    float MVAMet_sigMatrix_00;
    float MVAMet_sigMatrix_01;
    float MVAMet_sigMatrix_10;
    float MVAMet_sigMatrix_11;

private:

    ClassDef(myevent, 1)
};
#endif
