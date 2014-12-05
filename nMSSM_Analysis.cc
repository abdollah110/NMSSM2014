//    =======> Done  Add the spinor weight   16 October
//aaply correction for embeeded   [check embedded data ad MC]
//   Run new sdamples    HWW  ,   New lowMassDY1Jet     New tauPolar Off , new signal
//   Add new VLooose WP




// The code to do teh ZH totautau Analysis
// to make it excutable run: ./Make.sh nMSSM_Analysis.cc 
// to make it excutable run: ./nMSSM_Analysis.exe   mc12  out.root InputFile.root

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
//needed to make the executable
#include "interface/myevent.h"
#include "interface/LinkDef.h"
#include "interface/myobject.h"
// needed header files
#include "interface/makeHisto.h"
#include "interface/Leptons_PreSelection.h"
#include "interface/zh_Auxiliary.h"
#include "interface/Corrector.h"
#include "interface/htt_Trigger.h"
#include "interface/htt_Tree.h"
#include "interface/Leptons_IdIso.h"
#include "interface/zh_Functions.h"
//#include "DoAnalysis_NMSSM/myHelper.h"

int main(int argc, char** argv) {

    using namespace std;
    //define myevent class
    myevent *m = new myevent;
    //define 1D and 2D histogram
    myMap1 = new map<string, TH1F*>();
    //    myMap2 = new map<string, TH2F*>();

    cout << "\n######################### Analysis is initializing ####################################### " << endl;

    //#################################################################################################
    //################# First Argument, Data or MC, which type of data or MC    #######################
    //#################################################################################################

    string is_data_mc = *(argv + 1);
    cout << "\n *** It is found that this sample is  " << is_data_mc.c_str() << endl;

    bool mc12 = (is_data_mc.compare("mc12") == 0 ? true : false);
    bool mc11 = (is_data_mc.compare("mc11") == 0 ? true : false);
    bool data12 = (is_data_mc.compare("data12") == 0 ? true : false);
    bool data11 = (is_data_mc.compare("data11") == 0 ? true : false);
    bool embeddata12 = (is_data_mc.compare("embeddata12") == 0 ? true : false);
    bool embedmc12 = (is_data_mc.compare("embedmc12") == 0 ? true : false);
    if (!(mc12 || mc11 || data12 || data11 || embeddata12 || embedmc12))
        cout << "xxxxxxxxxxxxxxx Error, please slecet between: mc12 || mc11 || data12 || data11 ||  embeddata12 || embedmc12" << endl;

    //#################################################################################################
    //############## Second anad Third Argument,   OutPut Name/ Input Files                         ########################
    //#################################################################################################

    string out = *(argv + 2);

    std::vector<string> fileNames;
    for (int f = 3; f < argc; f++) {
        fileNames.push_back(*(argv + f));
        // printing the input NAME
        cout << "\n INPUT NAME IS:   " << fileNames[f - 3] << "\t";
    }
    //#################################################################################################

    //#################################################################################################
    //############## defining an out_file name need on the given argument  information  ###############
    //#################################################################################################

    //    string outname = is_data_mc + "_" + out;
    string outname = out;
    //PRINTING THE OUTPUT name
    cout << "\n\n\n OUTPUT NAME IS:    " << outname << endl;
    TFile *fout = TFile::Open(outname.c_str(), "RECREATE");

    //#################################################################################################
    //############## defining Tree Branches Filled via fillTree function                ###############
    //#################################################################################################
    TTree *Run_Tree = new TTree("InfoTree", "InfoTree");
    //    To force a memory-resident Tree
    Run_Tree->SetDirectory(0);



    Run_Tree->Branch("Channel", &Channel, "Channel/I");
    Run_Tree->Branch("run", &Run, "run/I");
    Run_Tree->Branch("lumi", &Lumi, "lumi/I");
    Run_Tree->Branch("evt", &Event, "evt/I");

    Run_Tree->Branch("mvamet", &mvamet, "mvamet/F");
    Run_Tree->Branch("mvametphi", &mvametphi, "mvametphi/F");
    Run_Tree->Branch("mvametNoRecoil", &mvametNoRecoil, "mvametNoRecoil/F");
    Run_Tree->Branch("mvametphiNoRecoil", &mvametphiNoRecoil, "mvametphiNoRecoil/F");
    Run_Tree->Branch("metcov00", &metcov00, "metcov00/F");
    Run_Tree->Branch("metcov01", &metcov01, "metcov01/F");
    Run_Tree->Branch("metcov10", &metcov10, "metcov10/F");
    Run_Tree->Branch("metcov11", &metcov11, "metcov11/F");
    Run_Tree->Branch("met", &met, "met/F");
    Run_Tree->Branch("metphi", &metphi, "metphi/F");
    Run_Tree->Branch("mvacov00", &mvacov00, "mvacov00/F");
    Run_Tree->Branch("mvacov01", &mvacov01, "mvacov01/F");
    Run_Tree->Branch("mvacov10", &mvacov10, "mvacov10/F");
    Run_Tree->Branch("mvacov11", &mvacov11, "mvacov11/F");

    Run_Tree->Branch("m_1", &l1M, "m_1/F");
    Run_Tree->Branch("E_1", &l1E, "E_1/F");
    Run_Tree->Branch("px_1", &l1Px, "px_1/F");
    Run_Tree->Branch("py_1", &l1Py, "py_1/F");
    Run_Tree->Branch("pz_1", &l1Pz, "pz_1/F");
    Run_Tree->Branch("pt_1", &l1Pt, "pt_1/F");
    Run_Tree->Branch("eta_1", &l1Eta, "eta_1/F");
    Run_Tree->Branch("phi_1", &l1Phi, "phi_1/F");
    Run_Tree->Branch("q_1", &l1Charge, "q_1/F");
    Run_Tree->Branch("l1_CloseJetPt", &l1_CloseJetPt, "l1_CloseJetPt/F");
    Run_Tree->Branch("l1_CloseJetEta", &l1_CloseJetEta, "l1_CloseJetEta/F");
    Run_Tree->Branch("l1_CloseJetPhi", &l1_CloseJetPhi, "l1_CloseJetPhi/F");
    Run_Tree->Branch("l1_muId_Loose", &l1_muId_Loose, "l1_muId_Loose/O");
    Run_Tree->Branch("l1_muId_Tight", &l1_muId_Tight, "l1_muId_Tight/O");
    Run_Tree->Branch("l1_eleId_Loose", &l1_eleId_Loose, "l1_eleId_Loose/O");
    Run_Tree->Branch("l1_eleId_Tight", &l1_eleId_Tight, "l1_eleId_Tight/O");
    Run_Tree->Branch("l1_muIso", &l1_muIso, "l1_muIso/F");
    Run_Tree->Branch("l1_eleIso", &l1_eleIso, "l1_eleIso/F");
    Run_Tree->Branch("l1_eleMVANonTrg", &l1_eleMVANonTrg, "l1_eleMVANonTrg/F");
    Run_Tree->Branch("l1_eleNumHit", &l1_eleNumHit, "l1_eleNumHit/F");



    Run_Tree->Branch("m_2", &l2M, "m_2/F");
    Run_Tree->Branch("e_2", &l2E, "e_2/F");
    Run_Tree->Branch("px_2", &l2Px, "px_2/F");
    Run_Tree->Branch("py_2", &l2Py, "py_2/F");
    Run_Tree->Branch("pz_2", &l2Pz, "pz_2/F");
    Run_Tree->Branch("pt_2", &l2Pt, "pt_2/F");
    Run_Tree->Branch("eta_2", &l2Eta, "eta_2/F");
    Run_Tree->Branch("phi_2", &l2Phi, "phi_2/F");
    Run_Tree->Branch("q_2", &l2Charge, "q_2/F");
    Run_Tree->Branch("l2_CloseJetPt", &l2_CloseJetPt, "l2_CloseJetPt/F");
    Run_Tree->Branch("l2_CloseJetEta", &l2_CloseJetEta, "l2_CloseJetEta/F");
    Run_Tree->Branch("l2_CloseJetPhi", &l2_CloseJetPhi, "l2_CloseJetPhi/F");


    Run_Tree->Branch("l2_tauIsoVL", &l2_tauIsoVL, "l2_tauIsoVL/O");
    Run_Tree->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_2", &byCombinedIsolationDeltaBetaCorrRaw3Hits_2, "byCombinedIsolationDeltaBetaCorrRaw3Hits_2/F"); //NNNEW
    Run_Tree->Branch("l2_tauIso3HitL", &l2_tauIso3HitL, "l2_tauIso3HitL/O");
    Run_Tree->Branch("l2_tauIso3HitM", &l2_tauIso3HitM, "l2_tauIso3HitM/O");
    Run_Tree->Branch("l2_tauIso3HitT", &l2_tauIso3HitT, "l2_tauIso3HitT/O");

    Run_Tree->Branch("l2_tauIsoL", &l2_tauIsoL, "l2_tauIsoL/O");
    Run_Tree->Branch("l2_tauIsoM", &l2_tauIsoM, "l2_tauIsoM/O");
    Run_Tree->Branch("l2_tauIsoT", &l2_tauIsoT, "l2_tauIsoT/O");

    Run_Tree->Branch("mva_2", &l2_tauIsoMVA2L, "mva_2/O");
    Run_Tree->Branch("iso_2", &l2_tauIsoMVA2raw, "iso_2/F");
    Run_Tree->Branch("l2_tauIsoMVA2M", &l2_tauIsoMVA2M, "l2_tauIsoMVA2M/O");
    Run_Tree->Branch("l2_tauIsoMVA2T", &l2_tauIsoMVA2T, "l2_tauIsoMVA2T/O");

    Run_Tree->Branch("againstMuonLoose3", &l2_tauRejMu3L, "againstMuonLoose3/O");
    Run_Tree->Branch("againstMuonMedium3", &l2_tauRejMu2M, "againstMuonMedium3/O");
    Run_Tree->Branch("againstMuonTight3", &l2_tauRejMu3T, "againstMuonTight3/O");
    Run_Tree->Branch("l2_discriminationByMuonMVALoose", &l2_discriminationByMuonMVALoose, "l2_discriminationByMuonMVALoose/O");
    Run_Tree->Branch("l2_discriminationByMuonMVAMedium", &l2_discriminationByMuonMVAMedium, "l2_discriminationByMuonMVAMedium/O");
    Run_Tree->Branch("l2_discriminationByMuonMVATight", &l2_discriminationByMuonMVATight, "l2_discriminationByMuonMVATight/O");
    Run_Tree->Branch("l2_discriminationByMuonMVAraw", &l2_discriminationByMuonMVAraw, "l2_discriminationByMuonMVAraw/F");

    Run_Tree->Branch("l2_tauRejEleL", &l2_tauRejEleL, "l2_tauRejEleL/O");
    Run_Tree->Branch("l2_tauRejEleM", &l2_tauRejEleM, "l2_tauRejEleM/O");
    Run_Tree->Branch("l2_tauRejEleMVA3L", &l2_tauRejEleMVA3L, "l2_tauRejEleMVA3L/O");
    Run_Tree->Branch("l2_tauRejEleMVA3M", &l2_tauRejEleMVA3M, "l2_tauRejEleMVA3M/O");
    Run_Tree->Branch("l2_tauRejEleMVA3T", &l2_tauRejEleMVA3T, "l2_tauRejEleMVA3T/O");
    Run_Tree->Branch("againstElectronMVA3raw_2", &l2_tauRejEleMVA, "againstElectronMVA3raw_2/F");

    Run_Tree->Branch("l2_RefJetPt", &l2_RefJetPt, "l2_RefJetPt/F");
    Run_Tree->Branch("l2_RefJetEta", &l2_RefJetEta, "l2_RefJetEta/F");
    Run_Tree->Branch("l2_RefJetPhi", &l2_RefJetPhi, "l2_RefJetPhi/F");

    Run_Tree->Branch("mt_1", &mt_1, "mt_1/F");
    Run_Tree->Branch("mt_2", &mt_2, "mt_2/F");
    Run_Tree->Branch("mvis", &mvis, "mvis/F");

    Run_Tree->Branch("idweight_1", &idweight_1, "idweight_1/F");
    Run_Tree->Branch("isoweight_1", &isoweight_1, "isoweight_1/F");
    Run_Tree->Branch("trigweight_1", &trigweight_1, "trigweight_1/F");
    Run_Tree->Branch("trigweight_2", &trigweight_2, "trigweight_2/F");
    Run_Tree->Branch("rho", &rho, "rho/F");
    Run_Tree->Branch("npv", &num_PV, "npv/I");
    Run_Tree->Branch("npu", &npu, "npu/F"); // NNNEW
    Run_Tree->Branch("effweight", &eff_Correction, "effweight/F");
    Run_Tree->Branch("puweight", &pu_Weight, "puweight/F");

    Run_Tree->Branch("jpt_1", &jpt_1, "jpt_1/F");
    Run_Tree->Branch("jeta_1", &jeta_1, "jeta_1/F");
    Run_Tree->Branch("jphi_1", &jphi_1, "jphi_1/F");
    Run_Tree->Branch("jE_1", &jE_1, "jE_1/F");
    Run_Tree->Branch("jpt_2", &jpt_2, "jpt_2/F");
    Run_Tree->Branch("jeta_2", &jeta_2, "jeta_2/F");
    Run_Tree->Branch("jphi_2", &jphi_2, "jphi_2/F");
    Run_Tree->Branch("jE_2", &jE_2, "jE_2/F");
    Run_Tree->Branch("jpass_1", &jpass_1, "jpass_1/O");
    Run_Tree->Branch("jpass_2", &jpass_2, "jpass_2/O");
    Run_Tree->Branch("bpt_1", &bpt_1, "bpt_1/F");
    Run_Tree->Branch("beta_1", &beta_1, "beta_1/F");
    Run_Tree->Branch("bphi_1", &bphi_1, "bphi_1/F");
    Run_Tree->Branch("bdiscriminant_1", &bdiscriminant_1, "bdiscriminant_1/F");
    Run_Tree->Branch("bpt_2", &bpt_2, "bpt_2/F");
    Run_Tree->Branch("beta_2", &beta_2, "beta_2/F");
    Run_Tree->Branch("bphi_2", &bphi_2, "bphi_2/F");
    Run_Tree->Branch("bdiscriminant_2", &bdiscriminant_2, "bdiscriminant_2/F");
    Run_Tree->Branch("loosebpt_1", &loosebpt_1, "loosebpt_1/F");
    Run_Tree->Branch("loosebeta_1", &loosebeta_1, "loosebeta_1/F");
    Run_Tree->Branch("loosebphi_1", &loosebphi_1, "loosebphi_1/F");
    Run_Tree->Branch("loosebdiscriminant_1", &loosebdiscriminant_1, "loosebdiscriminant_1/F");
    Run_Tree->Branch("loosebpt_2", &loosebpt_2, "loosebpt_2/F");
    Run_Tree->Branch("loosebeta_2", &loosebeta_2, "loosebeta_2/F");
    Run_Tree->Branch("loosebphi_2", &loosebphi_2, "loosebphi_2/F");
    Run_Tree->Branch("loosebdiscriminant_2", &loosebdiscriminant_2, "loosebdiscriminant_2/F");
    Run_Tree->Branch("mjj", &mjj, "mjj/F");
    Run_Tree->Branch("jdeta", &jdeta, "jdeta/F");
    Run_Tree->Branch("jdphi", &jdphi, "jdphi/F");
    Run_Tree->Branch("jetpt", &jetpt, "jetpt/F");
    Run_Tree->Branch("njets", &njets, "njets/I");
    Run_Tree->Branch("njetpt20", &njetpt20, "njetpt20/I");
    Run_Tree->Branch("nbtag", &nbtag, "nbtag/I");
    Run_Tree->Branch("nbtagLoose", &nbtagLoose, "nbtagLoose/I");
    Run_Tree->Branch("mcdata", &mcdata, "mcdata/I");

    //Newly added
    Run_Tree->Branch("l1_d0", &l1_d0, "l1_d0/F");
    Run_Tree->Branch("l1_dZ_in", &l1_dZ_in, "l1_dZ_in/F");
    Run_Tree->Branch("l2_DecayModeFinding", &l2_DecayModeFinding, "l2_DecayModeFinding/O");
    Run_Tree->Branch("zCategory", &zCategory, "zCategory/I");
    Run_Tree->Branch("l2_DecayMode", &l2_DecayMode, "l2_DecayMode/I");
    Run_Tree->Branch("embedWeight", &embedWeight, "embedWeight/F");
    Run_Tree->Branch("nbtagNoCor", &nbtagNoCor, "nbtagNoCor/I");


    Run_Tree->Branch("l2_LoosetauIsoMVA3newDMwLT", &l2_LoosetauIsoMVA3newDMwLT, "l2_LoosetauIsoMVA3newDMwLT/O");
    Run_Tree->Branch("l2_MediumtauIsoMVA3newDMwLT", &l2_MediumtauIsoMVA3newDMwLT, "l2_MediumtauIsoMVA3newDMwLT/O");
    Run_Tree->Branch("l2_TighttauIsoMVA3newDMwLT", &l2_TighttauIsoMVA3newDMwLT, "l2_TighttauIsoMVA3newDMwLT/O");
    Run_Tree->Branch("l2_LoosetauIsoMVA3oldDMwLT", &l2_LoosetauIsoMVA3oldDMwLT, "l2_LoosetauIsoMVA3oldDMwLT/O");
    Run_Tree->Branch("l2_MediumtauIsoMVA3oldDMwLT", &l2_MediumtauIsoMVA3oldDMwLT, "l2_MediumtauIsoMVA3oldDMwLT/O");
    Run_Tree->Branch("l2_TighttauIsoMVA3oldDMwLT", &l2_TighttauIsoMVA3oldDMwLT, "l2_TighttauIsoMVA3oldDMwLT/O");
    Run_Tree->Branch("l2_LoosetauIsoMVA3newDMwoLT", &l2_LoosetauIsoMVA3newDMwoLT, "l2_LoosetauIsoMVA3newDMwoLT/O");
    Run_Tree->Branch("l2_MediumtauIsoMVA3newDMwoLT", &l2_MediumtauIsoMVA3newDMwoLT, "l2_MediumtauIsoMVA3newDMwoLT/O");
    Run_Tree->Branch("l2_TighttauIsoMVA3newDMwoLT", &l2_TighttauIsoMVA3newDMwoLT, "l2_TighttauIsoMVA3newDMwoLT/O");
    Run_Tree->Branch("l2_LoosetauIsoMVA3oldDMwoLT", &l2_LoosetauIsoMVA3oldDMwoLT, "l2_LoosetauIsoMVA3oldDMwoLT/O");
    Run_Tree->Branch("l2_MediumtauIsoMVA3oldDMwoLT", &l2_MediumtauIsoMVA3oldDMwoLT, "l2_MediumtauIsoMVA3oldDMwoLT/O");
    Run_Tree->Branch("l2_TighttauIsoMVA3oldDMwoLT", &l2_TighttauIsoMVA3oldDMwoLT, "l2_TighttauIsoMVA3oldDMwoLT/O");


    Run_Tree->Branch("l2_DecayModeFindingNewDMs", &l2_DecayModeFindingNewDMs, "l2_DecayModeFindingNewDMs/O");
    Run_Tree->Branch("l2_DecayModeFindingOldDMs", &l2_DecayModeFindingOldDMs, "l2_DecayModeFindingOldDMs/O");

    Run_Tree->Branch("l2_tauIsoMVAraw3newDMwLTraw", &l2_tauIsoMVAraw3newDMwLTraw, "l2_tauIsoMVAraw3newDMwLTraw/F");
    Run_Tree->Branch("l2_tauIsoMVAraw3newDMwoLTraw", &l2_tauIsoMVAraw3newDMwoLTraw, "l2_tauIsoMVAraw3newDMwoLTraw/F");
    Run_Tree->Branch("l2_tauIsoMVAraw3oldDMwLTraw", &l2_tauIsoMVAraw3oldDMwLTraw, "l2_tauIsoMVAraw3oldDMwLTraw/F");
    Run_Tree->Branch("l2_tauIsoMVAraw3oldDMwoLTraw", &l2_tauIsoMVAraw3oldDMwoLTraw, "l2_tauIsoMVAraw3oldDMwoLTraw/F");



    //    Run_Tree->Branch("Trigger_LepTau12", &Trigger_LepTau12, "Trigger_LepTau12/O");
    Run_Tree->Branch("Trigger_MuTau12", &Trigger_MuTau12, "Trigger_MuTau12/O");
    Run_Tree->Branch("Trigger_EleTau12", &Trigger_EleTau12, "Trigger_EleTau12/O");
    Run_Tree->Branch("Trigger_SingleMu12", &Trigger_SingleMu12, "Trigger_SingleMu12/O");
    Run_Tree->Branch("Trigger_SingleEle12", &Trigger_SingleEle12, "Trigger_SingleEle12/O");
    Run_Tree->Branch("Trigger_SingleJet12", &Trigger_SingleJet12, "Trigger_SingleJet12/O");


    Run_Tree->Branch("l1_trgMatche_Ele20Tau20", &l1_trgMatche_Ele20Tau20, "l1_trgMatche_Ele20Tau20/O");
    Run_Tree->Branch("l1_trgMatche_Mu17Tau20", &l1_trgMatche_Mu17Tau20, "l1_trgMatche_Mu17Tau20/O");
    Run_Tree->Branch("l1_trgMatche_Mu18Tau25", &l1_trgMatche_Mu18Tau25, "l1_trgMatche_Mu18Tau25/O");
    Run_Tree->Branch("l1_trgMatche_Mu24", &l1_trgMatche_Mu24, "l1_trgMatche_Mu24/O");
    Run_Tree->Branch("l2_trgMatche_Ele20Tau20", &l2_trgMatche_Ele20Tau20, "l2_trgMatche_Ele20Tau20/O");
    Run_Tree->Branch("l2_trgMatche_Mu17Tau20", &l2_trgMatche_Mu17Tau20, "l2_trgMatche_Mu17Tau20/O");
    Run_Tree->Branch("l2_trgMatche_Mu18Tau25", &l2_trgMatche_Mu18Tau25, "l2_trgMatche_Mu18Tau25/O");


    Run_Tree->Branch("gen_Higgs_pt", &gen_Higgs_pt, "gen_Higgs_pt/F");
    Run_Tree->Branch("num_gen_jets;", &num_gen_jets, "num_gen_jets/I");

    Run_Tree->Branch("l1_ConversionVeto", &l1_ConversionVeto, "l1_ConversionVeto/O");
    Run_Tree->Branch("l1_dxy_PV", &l1_dxy_PV, "l1_dxy_PV/F");
    Run_Tree->Branch("l1_dz_PV", &l1_dz_PV, "l1_dz_PV/F");
    Run_Tree->Branch("l2_dxy_PV", &l2_dxy_PV, "l2_dxy_PV/F");
    Run_Tree->Branch("l2_dz_PV", &l2_dz_PV, "l2_dz_PV/F");

    Run_Tree->Branch("l1_Index;", &l1_Index, "l1_Index/I");
    Run_Tree->Branch("l2_Index;", &l2_Index, "l2_Index/I");
    Run_Tree->Branch("pu_Weight_old;", &pu_Weight_old, "pu_Weight_old/F");
    Run_Tree->Branch("l1Eta_SC;", &l1Eta_SC, "l1Eta_SC/F");
    Run_Tree->Branch("GenTopPt;", &GenTopPt, "GenTopPt/F");
    Run_Tree->Branch("GenAntiTopPt;", &GenAntiTopPt, "GenAntiTopPt/F");
    Run_Tree->Branch("Tau_Vertex_dz;", &Tau_Vertex_dz, "Tau_Vertex_dz/F");
    Run_Tree->Branch("gen_Higgs_Mass;", &gen_Higgs_Mass, "gen_Higgs_Mass/F");
    Run_Tree->Branch("spinnerWeight_;", &spinnerWeight_, "spinnerWeight_/F");

//Run_Tree->Branch("njetsCen", &njetsCen, "njetsCen/I");
//Run_Tree->Branch("njetsFwd", &njetsFwd, "njetsFwd/I");
//Run_Tree->Branch("nbtag30", &nbtag30, "nbtag30/I");



Run_Tree->Branch("l2_VLoosetauIsoMVA3oldDMwLT", &l2_VLoosetauIsoMVA3oldDMwLT, "l2_VLoosetauIsoMVA3oldDMwLT/O");
Run_Tree->Branch("ZimpactTau", &ZimpactTau, "ZimpactTau/F");
Run_Tree->Branch("VtxZ", &VtxZ, "VtxZ/F");


    //#################################################################################################
    //###################      Starting the analysis, making loop over files    #######################
    //#################################################################################################
    //running over the
    for (int k = 0; k < fileNames.size(); k++) {

        TChain *rootTree = new TChain("t");
        rootTree->Add(fileNames[k].c_str());
        int nev = int(rootTree->GetEntries());
        TBranch* branch = rootTree->GetBranch("myevent");
        branch->SetAddress(&m);
        cout << "number of entries is : " << nev << endl;


        // running over the root files
        for (int i = 0; i < nev; i++) {
            rootTree->GetEvent(i);
            if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nev);
            fflush(stdout);

            //*********************************************************************************************
            //****************************    Object definitions    ***************************************
            //*********************************************************************************************
            vector<myobject> mu_ = GoodMuon10GeV(m);
            vector<myobject> electron_ = GoodElectron10GeV(m);
            vector<myobject> tau_ = GoodTau20GeV(m);

            //#################################################################################################
//            bool doMuTauAnalysis = 1;
//            bool doElTauAnalysis = 0;
            bool doMuTauAnalysis = true;
            bool doElTauAnalysis = true;
            //#################################################################################################
            size_t eleTauEmbed = fileNames[k].find("PFembedded_ETau");
            if (eleTauEmbed != string::npos) doMuTauAnalysis = false;
            size_t muTauEmbed = fileNames[k].find("PFembedded_MuTau");
            if (muTauEmbed != string::npos) doElTauAnalysis = false;
            //#################################################################################################
            plotFill("TotalEventsNumber", 0, 1, 0, 1);
            //#################################################################################################
            //########################## MuTau Selection         ##############################################
            //#################################################################################################
            //#################################################################################################
            if (doMuTauAnalysis) {
                //##############################################################################
                // mutau
                //##############################################################################
                std::string FinalState = "mutau";

                for (int i = 0; i < mu_.size(); i++) {
                    for (int k = 0; k < tau_.size(); k++) {

                        bool Mu_PtEta = mu_[i].pt > 18 && fabs(mu_[i].eta) < 2.1;
                        bool Mu_IdTight = Id_Mu_Tight(mu_[i]);
                        bool Mu_d0 = fabs(mu_[i].d0) < 0.045; //the impact parameter in the transverse plane
                        bool Mu_dZ = fabs(mu_[i].dZ_in) < 0.2; //the impact parameter in the transverse plane
                        bool Mu_Iso = Iso_Mu_dBeta(mu_[i]) < 1.0;
                        //                        bool MU_CUTS = Mu_PtEta && Mu_IdTight && Mu_d0 && Mu_dZ && Mu_Iso;
                        bool MU_CUTS_Loose = Mu_PtEta && Mu_IdTight && Mu_d0 && Mu_dZ && Mu_Iso;

                        bool Tau_PtEta = tau_[k].pt > 19 && fabs(tau_[k].eta) < 2.3;
                        bool Tau_DMF = tau_[k].discriminationByDecayModeFinding;
                        //                        bool Tau_Isolation = tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits < 1.5;
                        bool Tau_antiEl = tau_[k].discriminationByElectronLoose;
                        bool Tau_antiMu = tau_[k].discriminationByMuonMVAMedium;
                        bool VLooseTauIso = tau_[k].byVLooseIsolationMVA3oldDMwLT;
                        bool taudz_Ver_match = fabs(tau_[k].dz_Ver_match) < 0.2;
                        //                        bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;
                        bool TAU_CUTS_Loose = Tau_PtEta && VLooseTauIso && Tau_antiEl && Tau_antiMu && taudz_Ver_match;

                        //                        bool MuTau_Charge = mu_[i].charge * tau_[k].charge < 0;
                        bool MuTau_dR = deltaR(mu_[i], tau_[k]) > 0.5;
                        bool secondMuVeto = secondMuonVeto(m);
                        bool thirdEleVeto = thirdElectronVetoMuTau(m, mu_[i], tau_[k]);
                        bool thirdMuVeto = thirdMuonVetoMuTau(m, mu_[i], tau_[k]);
                        bool extraSelection = MuTau_dR && secondMuVeto && thirdEleVeto && thirdMuVeto;

                        //Loose Selection
                        if (MU_CUTS_Loose && TAU_CUTS_Loose && extraSelection) {
                            fillTree(1, Run_Tree, m, is_data_mc.c_str(), FinalState, mu_[i], tau_[k]);
                        }
                        //                            //Final selection
                        //                            if (MU_CUTS && TAU_CUTS && MuTau_Charge && MuTau_dR && Veto_MM && Veto_MMM && Veto_MME) {
                        //                                plotFill("mutau", ++mutau, 20, 0., 20.);
                        //                                fillTree(2, Run_Tree, m, is_data_mc.c_str(), FinalState, mu_[i], tau_[k]);
                        //                                break;
                        //                            }
                    }
                }
            }//end of only Muon
            //#################################################################################################
            //#################################################################################################
            //#######################  EleTau Selection #######################
            //#################################################################################################
            //#################################################################################################
            if (doElTauAnalysis) {
                //##############################################################################
                // eltau
                //##############################################################################
                std::string FinalState = "eltau";
                for (int i = 0; i < electron_.size(); i++) {
                    for (int k = 0; k < tau_.size(); k++) {

                        bool El_PtEta = electron_[i].pt > 24 && fabs(electron_[i].eta) < 2.1;
                        bool El_IdTight = EleMVANonTrigId_Tight(electron_[i]);
                        bool El_Iso = Iso_Ele_dBeta(electron_[i]) < 1.0;
                        //                        bool EL_CUTS = El_PtEta && El_IdTight && El_Iso;
                        bool EL_CUTS_Loose = El_PtEta && El_IdTight && El_Iso;

                        bool Tau_PtEta = tau_[k].pt > 19 && fabs(tau_[k].eta) < 2.3;
                        bool Tau_DMF = tau_[k].discriminationByDecayModeFinding;
                        //                        bool Tau_Isolation = tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits < 1.5;
                        bool Tau_antiEl = tau_[k].discriminationByElectronMVA5Medium;
                        bool Tau_antiMu = tau_[k].discriminationByMuonLoose3;
                        bool VLooseTauIso = tau_[k].byVLooseIsolationMVA3oldDMwLT;
                        bool taudz_Ver_match = fabs(tau_[k].dz_Ver_match) < 0.2;
                        //                        bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;
                        bool TAU_CUTS_Loose = Tau_PtEta && VLooseTauIso && Tau_antiEl && Tau_antiMu && taudz_Ver_match;

                        //                        bool ElTau_Charge = electron_[i].charge * tau_[k].charge < 0;
                        bool ElTau_dR = deltaR(electron_[i], tau_[k]) > 0.5;
                        bool secondEleVeto = secondElectronVeto(m);
                        bool thirdEleVeto = thirdElectronVetoETau(m, electron_[i], tau_[k]);
                        bool thirdMuVeto = thirdMuonVetoETau(m, electron_[i], tau_[k]);
                        bool extraSelection = ElTau_dR && secondEleVeto && thirdEleVeto && thirdMuVeto;



                        //Loose Selection
                        if (EL_CUTS_Loose && TAU_CUTS_Loose && extraSelection) {
                            fillTree(3, Run_Tree, m, is_data_mc.c_str(), FinalState, electron_[i], tau_[k]);
                        }
                        //Final selection
                        //                            if (EL_CUTS && TAU_CUTS && ElTau_Charge && ElTau_dR && Veto_EE && Veto_EEM && Veto_EEE) {
                        //                                plotFill("eltau", ++eltau, 20, 0., 20.);
                        //                                fillTree(4, Run_Tree, m, is_data_mc.c_str(), FinalState, electron_[i], tau_[k]);
                        //                                break;
                        //
                        //                            }


                    }
                }
            }//end of Only Electron
        }//loop over events


        delete rootTree;
    }
    delete m;


    fout->cd();

    Run_Tree->Write();

    map<string, TH1F*>::const_iterator iMap1 = myMap1->begin();
    map<string, TH1F*>::const_iterator jMap1 = myMap1->end();

    for (; iMap1 != jMap1; ++iMap1)
        nplot1(iMap1->first)->Write();

    //    map<string, TH2F*>::const_iterator iMap2 = myMap2->begin();
    //    map<string, TH2F*>::const_iterator jMap2 = myMap2->end();
    //
    //    for (; iMap2 != jMap2; ++iMap2)
    //        nplot2(iMap2->first)->Write();



    fout->Close();
    return 0;
}
