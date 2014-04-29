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
    vector<myobject> RecPFJetsAK5_Up_SubTotal;
    vector<myobject> RecPFJetsAK5_Down_SubTotal;
    vector<myobject> RecPFJetsAK5_Up_Total;
    vector<myobject> RecPFJetsAK5_Down_Total;
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
    vector<myobject> SelectedHPSTaus;
    //    vector<myobject> RecMet;
    vector<myobject> RecPFMet;
    vector<myobject> RecPFMetCor;
    vector<myobject> RecMVAMet;
    vector<myobject> RecMVAMet_ditau;
    vector<myobject> RecMVAMet_mutau;
    vector<myobject> RecMVAMet_etau;
    vector<myobject> RecMVAMet_emu;
    vector<myobject> CorrectedL1ETM;
    vector<myobject> UncorrectedL1ETM;
    vector<myobject> CorrectedCaloMetNoHF;
    vector<myobject> GenTauMatchedCaloJet;
    vector<myobject> MetNoHFresidualCorrected;
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
    unsigned int HLT_DiElectron;
    unsigned int HLT_DiMuon;
    int PUInfo;
    int PUInfo_true;
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

    float MVAMet_sigMatrix_00_emu;
    float MVAMet_sigMatrix_01_emu;
    float MVAMet_sigMatrix_10_emu;
    float MVAMet_sigMatrix_11_emu;

    float MVAMet_sigMatrix_00_mutau;
    float MVAMet_sigMatrix_01_mutau;
    float MVAMet_sigMatrix_10_mutau;
    float MVAMet_sigMatrix_11_mutau;

    float MVAMet_sigMatrix_00_etau;
    float MVAMet_sigMatrix_01_etau;
    float MVAMet_sigMatrix_10_etau;
    float MVAMet_sigMatrix_11_etau;

    float MVAMet_sigMatrix_00_ditau;
    float MVAMet_sigMatrix_01_ditau;
    float MVAMet_sigMatrix_10_ditau;
    float MVAMet_sigMatrix_11_ditau;


private:

    ClassDef(myevent, 1)
};
#endif
