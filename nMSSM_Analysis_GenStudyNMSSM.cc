// REmaining issue
// Add MVA Medium   discriminationByMuonMVAMedium
// add Muele and EleMu Veto
// lower the tau Pt from 30 to 20 GeV
// also run on nMSSM
// no requirement on Tau decay mode finding

// Missing for next time is
// 1) tau decay  correction  DONE
// 2) btag correction at analysis level from Veelken  DONE
// 3) check the trigger eff efficiency correction
// 4) pu reweighting correction


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

int main(int argc, char** argv) {

    using namespace std;
    //define myevent class
    myevent *m = new myevent;
    //define 1D and 2D histogram
    myMap1 = new map<string, TH1F*>();
    myMap2 = new map<string, TH2F*>();

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
    bool embed12 = (is_data_mc.compare("embed12") == 0 ? true : false);
    if (!(mc12 || mc11 || data12 || data11 || embed12))
        cout << "xxxxxxxxxxxxxxx Error, please slecet between: mc12 || mc11 || data12 || data11 " << endl;

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

    Run_Tree->Branch("l2_tauRejMuL", &l2_tauRejMuL, "l2_tauRejMuL/O");
    Run_Tree->Branch("l2_tauRejMuM", &l2_tauRejMuM, "l2_tauRejMuM/O");
    Run_Tree->Branch("l2_tauRejMuT", &l2_tauRejMuT, "l2_tauRejMuT/O");
    Run_Tree->Branch("againstMuonLoose3", &l2_tauRejMu3L, "againstMuonLoose3/O");
    Run_Tree->Branch("againstMuonMedium3", &l2_tauRejMu2M, "againstMuonMedium3/O");
    Run_Tree->Branch("againstMuonTight3", &l2_tauRejMu3T, "againstMuonTight3/O");
    Run_Tree->Branch("l2_discriminationByMuonMVAMedium", &l2_discriminationByMuonMVAMedium, "l2_discriminationByMuonMVAMedium/O");

    Run_Tree->Branch("againstElectronMVA3raw_2", &l2_tauRejEleMVA, "againstElectronMVA3raw_2/F");
    Run_Tree->Branch("l2_tauRejEleL", &l2_tauRejEleL, "l2_tauRejEleL/O");
    Run_Tree->Branch("l2_tauRejEleM", &l2_tauRejEleM, "l2_tauRejEleM/O");
    Run_Tree->Branch("l2_tauRejEleMVA3L", &l2_tauRejEleMVA3L, "l2_tauRejEleMVA3L/O");
    Run_Tree->Branch("l2_tauRejEleMVA3M", &l2_tauRejEleMVA3M, "l2_tauRejEleMVA3M/O");
    Run_Tree->Branch("l2_tauRejEleMVA3T", &l2_tauRejEleMVA3T, "l2_tauRejEleMVA3T/O");

    Run_Tree->Branch("l2_RefJetPt", &l2_RefJetPt, "l2_RefJetPt/F");
    Run_Tree->Branch("l2_RefJetEta", &l2_RefJetEta, "l2_RefJetEta/F");
    Run_Tree->Branch("l2_RefJetPhi", &l2_RefJetPhi, "l2_RefJetPhi/F");

    Run_Tree->Branch("mt_1", &mt_1, "mt_1/F");
    Run_Tree->Branch("mt_2", &mt_2, "mt_2/F");
    Run_Tree->Branch("mvis", &mvis, "mvis/F");

    Run_Tree->Branch("idweight_1", &idweight_1, "idweight_1/F");
    Run_Tree->Branch("trigweight_1", &trigweight_1, "trigweight_1/F");
    Run_Tree->Branch("trigweight_2", &trigweight_2, "trigweight_2/F");
    Run_Tree->Branch("rho", &rho, "rho/F");
    Run_Tree->Branch("npv", &num_PV, "npv/I");
    Run_Tree->Branch("npu", &npu, "npu/I"); // NNNEW
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
        //                        for (int i = 0; i < 1; i++) {
        for (int i = 0; i < nev; i++) {
            rootTree->GetEvent(i);
            if (i % 100 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nev);
            fflush(stdout);

            //*********************************************************************************************
            //****************************    Object definitions    ***************************************
            //*********************************************************************************************
            vector<myobject> mu_ = GoodMuon10GeV(m);
            vector<myobject> electron_ = GoodElectron10GeV(m);
            vector<myobject> tau_ = GoodTau20GeV(m);

            //#################################################################################################
            bool doMuTauAnalysis = true;
            bool doElTauAnalysis = false;
            //#################################################################################################
            //########################## MuTau Selection         ##############################################
            //#################################################################################################
            vector<myGenobject> genPar_ = m->RecGenParticle;
            vector<myGenobject> genMet_ = m->RecGenMet;
//            vector<myobject> recMet_ = m->RecMVAMet_mutau;
//            vector<myobject> recMetRecoil_ = m->RecMVAMet_mutau;
            bool Trigger_MuTau12 = Trigger_MuTau_12(m);
            bool Trigger_EleTau12 = Trigger_EleTau_12(m);



            //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
            bool ItisMu = false;
            bool ItisTau = false;
            bool MuIsSelected = false;
            bool TauMuIsSelected = false;
            bool TauMuBJetIsSelected = false;
            int numBjet = 0;


            
            for (int ii = 0; ii < mu_.size(); ii++) {
                for (int k = 0; k < tau_.size(); k++) {

                    
                    if (mu_[ii].pt > 18 && tau_[k].pt > 20 && tau_[k].byTightIsolationMVA3oldDMwLT > 0.5 && tau_[k].discriminationByMuonMVAMedium > 0.5){
                        for (int j = 0; j < genPar_.size(); j++) {
//                            if (fabs(genPar_[j].pdgId) == 15) cout<< "fabs(genPar_[j].mod_pdgId) is  "<<fabs(genPar_[j].mod_pdgId) <<"\n";
                            for (int i = 0; i < genPar_.size(); i++) {
                                if (genPar_[j].pdgId == 15 && genPar_[j].mod_pdgId == 23 &&genPar_[i].pdgId == -15 &&genPar_[i].mod_pdgId == 23 ){
//                                    if (InvarMass_2(genPar_[j],genPar_[i])  < 50 )cout << InvarMass_2(genPar_[j],genPar_[i])<<"\n";
                                    plotFill("Mass_diTau_EmbeddedData", InvarMass_2(genPar_[j],genPar_[i]), 150, 0, 300);
                                
                                }
                                
                            }
                    
                            
                        }
                }
            }
            }
            for (int j = 0; j < genPar_.size(); j++) {
                bool muFromTau = fabs(genPar_[j].pdgId) == 13 && fabs(genPar_[j].status) == 1 && fabs(genPar_[j].mod_pdgId) == 15 && fabs(genPar_[j].mod_status) == 2 && fabs(genPar_[j].Gmod_pdgId) == 15 && fabs(genPar_[j].Gmod_status) == 3;
                if (muFromTau) {
                    plotFill("TotalEventsNumber_MU", 0, 1, 0, 1);
                    ItisMu = true;

                }
            }
            for (int i = 0; i < genPar_.size(); i++) {
                bool tauFromTau = (fabs(genPar_[i].pdgId) != 11 && fabs(genPar_[i].pdgId) != 12 && fabs(genPar_[i].pdgId) != 13 && fabs(genPar_[i].pdgId) != 14 && fabs(genPar_[i].pdgId) != 16 && fabs(genPar_[i].pdgId) != 24 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3);
                //                bool tauFromW = (fabs(genPar_[i].pdgId) != 11 && fabs(genPar_[i].pdgId) != 12 && fabs(genPar_[i].pdgId) != 13 && fabs(genPar_[i].pdgId) != 14 && fabs(genPar_[i].pdgId) != 16 && fabs(genPar_[i].pdgId) != 24 && fabs(genPar_[i].mod_pdgId) == 24 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 2);
                if (tauFromTau) {
                    plotFill("TotalEventsNumber_Tau", 0, 1, 0, 1);
                    ItisTau = true;
                }
            }

            if (ItisMu && ItisTau) {

                for (int i = 0; i < genPar_.size(); i++) {
                    if (fabs(genPar_[i].pdgId) == 13 && fabs(genPar_[i].status) == 1 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3) {
                        if (genPar_[i].pt > 20 && fabs(genPar_[i].eta) < 2.1) {
                            plotFill("StandAloneMu", genPar_[i].pt, 100, 0, 100);
                            MuIsSelected = true;

                        }
                    }
                }
            }


            if (MuIsSelected) {
                for (int i = 0; i < genPar_.size(); i++) {

                    if (fabs(genPar_[i].pdgId) != 11 && fabs(genPar_[i].pdgId) != 12 && fabs(genPar_[i].pdgId) != 13 && fabs(genPar_[i].pdgId) != 14 && fabs(genPar_[i].pdgId) != 16 && fabs(genPar_[i].pdgId) != 24 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3) {
                        //                        cout<<m->eventNumber <<"    Moder= "<<fabs(genPar_[i].mod_pdgId) << "  pt= "<<genPar_[i].mod_pt << "             object= "<<fabs(genPar_[i].pdgId) << "  pt= "<<genPar_[i].pt << " \n";
                        if (genPar_[i].pt > 20 && fabs(genPar_[i].eta) < 2.3) {
                            plotFill("StandAloneTau", genPar_[i].pt, 100, 0, 100);
                            TauMuIsSelected = true;
                        }
                    }
                }
            }
            if (TauMuIsSelected) {
                plotFill("ItisMuTauChannel", 0, 1, 0, 1);
                for (int i = 0; i < genPar_.size(); i++) {
                    if (fabs(genPar_[i].pdgId) == 5 && genPar_[i].status == 3 && genPar_[i].pt > 25 && fabs(genPar_[i].eta) < 2.4) {
                        plotFill("StandAloneBJet", numBjet++, 10, 0, 10);
                        TauMuBJetIsSelected = true;

                    }
                }
            }

            if (TauMuBJetIsSelected) plotFill("FinalAllCuts", 1, 10, 0, 10);




            //
            //            for (int i = 0; i < genPar_.size(); i++) {
            //                //                if (fabs(genPar_[i].pdgId) == 15 ) cout << fabs(genPar_[i].pdgId) << "  " << fabs(genPar_[i].mod_pdgId)<<"\n";
            //                if (fabs(genPar_[i].pdgId) == 5 && genPar_[i].status == 3 && fabs(genPar_[i].mod_pdgId) == 21) plotFill("bJetPt", genPar_[i].pt, 50, 0, 50);
            //                if (fabs(genPar_[i].pdgId) == 5 && genPar_[i].status == 3 && fabs(genPar_[i].mod_pdgId) == 21) plotFill("bJetEta", genPar_[i].eta, 50, -5, 5);
            //
            //                //    For Filtering
            //                //                if (fabs(genPar_[i].pdgId) == 5 && genPar_[i].status == 3 && fabs(genPar_[i].mod_pdgId) == 21 && genPar_[i].pt > 25 && fabs(genPar_[i].et) < 2.4) ThereIs1BJet = true;
            //
            //                if ((fabs(genPar_[i].pdgId) == 11 || fabs(genPar_[i].pdgId) == 13) && genPar_[i].pt > 15 && Just1Lep15) {
            //                    plotFill("AtLeast1Lep15", genPar_[i].pt, 100, 0, 100);
            //                    Just1Lep15 = false;
            //                }
            //                if ((fabs(genPar_[i].pdgId) == 11 || fabs(genPar_[i].pdgId) == 13) && genPar_[i].pt > 10 && Just1Lep10) {
            //                    plotFill("AtLeast1Lep10", genPar_[i].pt, 100, 0, 100);
            //                    Just1Lep10 = false;
            //                }
            //                //    For Filtering
            //                for (int j = i + 1; j < genPar_.size(); j++) {
            //                    //                if (fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].pdgId) != 11 && fabs(genPar_[i].pdgId) != 12 && fabs(genPar_[i].pdgId) != 13 && fabs(genPar_[i].pdgId) != 14 && fabs(genPar_[i].pdgId) != 15 && fabs(genPar_[i].pdgId) != 16 && fabs(genPar_[i].pdgId) != 22 && fabs(genPar_[i].pdgId) != 24 && fabs(genPar_[i].pdgId) != 211 && fabs(genPar_[i].pdgId) != 213 && fabs(genPar_[i].pdgId) != 20213&& fabs(genPar_[i].pdgId) != 321&& fabs(genPar_[i].pdgId) != 323) {
            //
            //                    /////////// EleTau
            //                    if (fabs(genPar_[i].pdgId) != 11 && fabs(genPar_[i].pdgId) != 12 && fabs(genPar_[i].pdgId) != 13 && fabs(genPar_[i].pdgId) != 14 && fabs(genPar_[i].pdgId) != 16 && fabs(genPar_[i].pdgId) != 24 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3) {
            //                        if (fabs(genPar_[i].pdgId) == 11 && fabs(genPar_[i].status) == 1 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3) {
            //                            plotFill("TauPt_etau", genPar_[i].pt, 50, 0, 50);
            //                            plotFill("ElePt_etau", genPar_[j].pt, 50, 0, 50);
            //                            plotFill("TauEta_etau", genPar_[i].eta, 50, -5, 5);
            //                            plotFill("EleEta_etau", genPar_[j].eta, 50, -5, 5);
            //                            plotFill("TauPtElePt2D_etau", genPar_[i].pt, genPar_[j].pt, 50, 0, 50, 50, 0, 50);
            //                            if (fabs(genPar_[i].eta) < 2.3 && fabs(genPar_[j].eta) < 2.1) plotFill("TauPtElePt2D_ER_etau", genPar_[i].pt, genPar_[j].pt, 50, 0, 50, 50, 0, 50);
            //                            if (genPar_[i].pt > 20 && genPar_[j].pt > 24) plotFill("dR_etau", dR(genPar_[i].eta, genPar_[i].phi, genPar_[j].eta, genPar_[j].eta), 100, 0, 10);
            //                            if (genPar_[i].pt > 20 && genPar_[j].pt > 24) plotFill("dPhi_etau", deltaPhi(genPar_[i].phi, genPar_[j].eta), 100, 0, 10);
            //                        }
            //                    }
            //                    /////////// MuTau
            //                    if (fabs(genPar_[i].pdgId) != 11 && fabs(genPar_[i].pdgId) != 12 && fabs(genPar_[i].pdgId) != 13 && fabs(genPar_[i].pdgId) != 14 && fabs(genPar_[i].pdgId) != 16 && fabs(genPar_[i].pdgId) != 24 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3) {
            //                        //                        if (fabs(genPar_[j].mod_pdgId) == 15 && fabs(genPar_[j].pdgId) == 13) {
            //                        if (fabs(genPar_[i].pdgId) == 13 && fabs(genPar_[i].status) == 1 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3) {
            //                            plotFill("TauPt_mutau", genPar_[i].pt, 50, 0, 50);
            //                            plotFill("MuPt_mutau", genPar_[j].pt, 50, 0, 50);
            //                            plotFill("TauEta_mutau", genPar_[i].eta, 50, -5, 5);
            //                            plotFill("MuEta_mutau", genPar_[j].eta, 50, -5, 5);
            //                            plotFill("TauPtMuPt2D_mutau", genPar_[i].pt, genPar_[j].pt, 50, 0, 50, 50, 0, 50);
            //                            if (fabs(genPar_[i].eta) < 2.3 && fabs(genPar_[j].eta) < 2.1) plotFill("TauPtMuPt2D_ER_mutau", genPar_[i].pt, genPar_[j].pt, 50, 0, 50, 50, 0, 50);
            //                            if (genPar_[i].pt > 20 && genPar_[j].pt > 20) plotFill("dR_mutau", dR(genPar_[i].eta, genPar_[i].phi, genPar_[j].eta, genPar_[j].eta), 100, 0, 10);
            //                            if (genPar_[i].pt > 20 && genPar_[j].pt > 20) plotFill("dPhi_mutau", deltaPhi(genPar_[i].phi, genPar_[j].eta), 100, 0, 10);
            //                            if (genPar_[i].pt > 20 && genPar_[j].pt > 20 && fabs(genPar_[i].eta) < 2.3 && fabs(genPar_[j].eta) < 2.1 && ThereIs1BJet) plotFill("FinalSelection", 1, 2, 0, 2);
            //
            //
            //                        }
            //                    }
            //                    /////////// EleMu
            //                    if (fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].pdgId) == 11) {
            //                        if (fabs(genPar_[i].pdgId) == 13 && fabs(genPar_[i].status) == 1 && fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].mod_status) == 2 && fabs(genPar_[i].Gmod_pdgId) == 15 && fabs(genPar_[i].Gmod_status) == 3) {
            //                            plotFill("ElePt_emu", genPar_[i].pt, 50, 0, 50);
            //                            plotFill("MuPt_emu", genPar_[j].pt, 50, 0, 50);
            //                            plotFill("EleEta_emu", genPar_[i].eta, 50, -5, 5);
            //                            plotFill("MuEta_emu", genPar_[j].eta, 50, -5, 5);
            //                            plotFill("TauPtMuPt2D_emu", genPar_[i].pt, genPar_[j].pt, 50, 0, 50, 50, 0, 50);
            //                            if (fabs(genPar_[i].eta) < 2.5 && fabs(genPar_[j].eta) < 2.4) plotFill("TauPtMuPt2D_ER_emu", genPar_[i].pt, genPar_[j].pt, 50, 0, 50, 50, 0, 50);
            //                            if ((genPar_[i].pt > 20 && genPar_[j].pt > 10) || (genPar_[i].pt > 10 && genPar_[j].pt > 20)) plotFill("dR_emu", dR(genPar_[i].eta, genPar_[i].phi, genPar_[j].eta, genPar_[j].eta), 100, 0, 10);
            //                            if ((genPar_[i].pt > 20 && genPar_[j].pt > 10) || (genPar_[i].pt > 10 && genPar_[j].pt > 20)) plotFill("dPhi_emu", deltaPhi(genPar_[i].phi, genPar_[j].eta), 100, 0, 10);
            //                        }
            //                    }
            //
            //
            //                    if (fabs(genPar_[i].mod_pdgId) == 36 && fabs(genPar_[i].pdgId) == 15) {
            //                        if (fabs(genPar_[j].mod_pdgId) == 36 && fabs(genPar_[j].pdgId) == 15) {
            //                            TLorentzVector TV_i, TV_j, TV_tot;
            //                            TV_i.SetPtEtaPhiM(genPar_[i].pt, genPar_[i].eta, genPar_[i].phi, genPar_[i].mass);
            //                            TV_j.SetPtEtaPhiM(genPar_[j].pt, genPar_[j].eta, genPar_[j].phi, genPar_[j].mass);
            //                            TV_tot = TV_i + TV_j;
            //                            plotFill("Higgs_Mass", TV_tot.M(), 10000, 0, 100);
            //
            //                        }
            //                    }
            //                }
            //
            //
            //            }

            //                        cout << "genPar_[i].pdgId= " << genPar_[i].pdgId << "   pt= " << genPar_[i].pt << "\n";

            //                        inline void SetPtEtaPhiM(Double_t pt, Double_t eta, Double_t phi, Double_t m);



            //                        float theta_i = 2 * TMath::ATan(TMath::Exp(-1 * genPar_[i].eta));
            //                        float theta_j = 2 * TMath::ATan(TMath::Exp(-1 * genPar_[j].eta));
            //                        float p_i = genPar_[i].pt / TMath::Cos(theta_i);
            //                        float p_j = genPar_[j].pt / TMath::Cos(theta_j);
            //                        cout << InvarMass_F(genPar_[i].mass, genPar_[j].mass,     TMath::Cos(genPar_[i]. phi)*genPar_[i]. pt    ,    TMath::Cos(genPar_[j]. phi)*genPar_[j]. pt    ,  TMath::Sin(genPar_[i]. phi)*genPar_[i]. pt,TMath::Sin(genPar_[j]. phi)*genPar_[j]. pt, p_i*TMath::Sin(theta_i),p_j*TMath::Sin(theta_j) ) << endl;


            //                    if (fabs(genPar_[i].pdgId==11 )) cout <<  genPar_[i].pdgId << "  " << genPar_[i].mass << "   "<<genPar_[i].status << "  " <<  "\n";
            //                    }
            //                if (fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].pdgId) == 11 ) {
            //                    cout << "eeeeeeeeeeeeeeeeeeeeeeeee genPar_[i].pdgId= " << genPar_[i].pdgId << "   pt= "<< genPar_[i].pt <<  "\n";
            //                }
            //                if (fabs(genPar_[i].mod_pdgId) == 15 && fabs(genPar_[i].pdgId) == 13 ) {
            //                    cout << "mmmmmmmmmmmm genPar_[i].pdgId= " << genPar_[i].pdgId << "   pt= "<< genPar_[i].pt <<  "\n";
            //                }





            plotFill("TotalEventsNumber", 0, 1, 0, 1);
            //#################################################################################################
            //            if (doMuTauAnalysis && Trigger_MuTau12) {
            if (doMuTauAnalysis) {
                //##############################################################################
                // mutau
                //##############################################################################
                std::string FinalState = "mutau";

                int mutau = -1;
                //                plotFill("TotalEventsNumber", 0, 1, 0, 1);
                plotFill("mutau", ++mutau, 20, 0., 20.);

                for (int i = 0; i < mu_.size(); i++) {
                    for (int k = 0; k < tau_.size(); k++) {

                        //                        bool Mu_PtEta = mu_[i].pt > 20 && fabs(mu_[i].eta) < 2.1;
                        bool Mu_PtEta = mu_[i].pt > 15 && fabs(mu_[i].eta) < 2.1;
                        bool Mu_IdTight = Id_Mu_Tight(mu_[i]);
                        bool Mu_d0 = mu_[i].d0 < 0.045; //the impact parameter in the transverse plane
                        bool Mu_dZ = mu_[i].dZ_in < 0.2; //the impact parameter in the transverse plane
                        bool Mu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.1;
                        bool MU_CUTS = Mu_PtEta && Mu_IdTight && Mu_d0 && Mu_dZ && Mu_Iso;

                        //                        bool Tau_PtEta = tau_[k].pt > 20 && fabs(tau_[k].eta) < 2.3;
                        bool Tau_PtEta = tau_[k].pt > 15 && fabs(tau_[k].eta) < 2.3;
                        bool Tau_DMF = tau_[k].discriminationByDecayModeFindingOldDMs;
                        bool Tau_Isolation = tau_[k].byTightIsolationMVA3oldDMwLT > 0.5;
                        bool Tau_antiEl = tau_[k].discriminationByElectronLoose;
                        bool Tau_antiMu = tau_[k].discriminationByMuonMVAMedium;
                        bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;

                        bool MuTau_Charge = mu_[i].charge * tau_[k].charge < 0;
                        bool MuTau_dR = deltaR(mu_[i], tau_[k]) > 0.5;

//
//                        bool Veto_ME = Multi_Lepton_Veto("ME", m);
//                        bool Veto_MM = Multi_Lepton_Veto("MM", m);
//                        bool Veto_MMM = Multi_Lepton_Veto("MMM", m);
//                        bool Veto_MME = Multi_Lepton_Veto("MME", m);
//                        
                        bool Veto_ME = 1;
                        bool Veto_MM = 1;
                        bool Veto_MMM = 1;
                        bool Veto_MME = 1;


                        vector<myobject> JETS = GoodJet30(m, mu_[i], tau_[k]);
                        vector<myobject> BJETS = GoodbJet20(m, mu_[i], tau_[k], 0, 1);
//                        vector<myobject> MVAMetRecoil_mutau = m->RecoilMetmutau;
                        mt_1 = 1;

                        bool LooseSelection = Mu_PtEta && Tau_PtEta && MuTau_dR && Veto_MM && Veto_MMM && Veto_MME && Veto_ME;
                        bool VLooseTauIso = tau_[k].byIsolationMVA3oldDMwLTraw > 0;
                        bool bjetCut_TMass = BJETS.size() > 0 && JETS.size() < 2 && mt_1 < 30;

                        //                        bool MatchedTrigger = mu_[i].hasTrgObject_Mu17Tau20 && tau_[k].hasTrgObject_Mu17Tau20;
                        bool MatchedTrigger = 1;

                        //                            //Final selection
//                        if (MatchedTrigger && MU_CUTS && TAU_CUTS && MuTau_Charge && MuTau_dR && Veto_MM && Veto_MMM && Veto_MME && bjetCut_TMass) {
//                            plotFill("mutau", ++mutau, 20, 0., 20.);
//                            float MaxPtLep = 0;
//                            float MaxEtaLep = -100;
//                            float MaxPhiLep = -100;
//                            for (int ig = 0; ig < genPar_.size(); ig++) {
//                                if (fabs(genPar_[ig].pdgId) == 13) {
//                                    if (genPar_[ig].pt > MaxPtLep) {
//                                        MaxPtLep = genPar_[ig].pt;
//                                        MaxEtaLep = genPar_[ig].eta;
//                                        MaxPhiLep = genPar_[ig].phi;
//                                    }
//                                }
//                            }
//
//                            plotFill("GenMuonPT_InPassRecoEvents", MaxPtLep, 50, 0., 50.);
//                            plotFill("GenMuon_InPassRecoEvents_DR", dR(MaxEtaLep, MaxPhiLep, mu_[i].eta, mu_[i].phi), 500, 0., 10);
//
//                            //                            fillTree(2, Run_Tree, m, is_data_mc.c_str(), FinalState, mu_[i], tau_[k]);
////                            break;
//                        }


                        //##################################################################
                        //  Filling Tree
                        //##################################################################
                        if (TauMuBJetIsSelected && Veto_MM && Veto_ME && Veto_MMM && Veto_MME) {
                            fillTree(1, Run_Tree, m, is_data_mc.c_str(), FinalState, mu_[i], tau_[k]);
                        }
                    }
                }
            }//end of only Muon
            //#################################################################################################
            //#################################################################################################
            //#######################  EleTau Selection #######################
            //#################################################################################################
            //#################################################################################################
            //            if (doElTauAnalysis && Trigger_EleTau12) {
            if (doElTauAnalysis) {
                //##############################################################################
                // eltau
                //##############################################################################
                int ThereExist1GenEle = 0;
                std::string FinalState = "eltau";
                int eltau = -1;
                plotFill("eltau", ++eltau, 20, 0., 20.);
                for (int i = 0; i < electron_.size(); i++) {
                    for (int k = 0; k < tau_.size(); k++) {

                        //                        bool El_PtEta = electron_[i].pt > 24 && fabs(electron_[i].eta) < 2.1;
                        bool El_PtEta = electron_[i].pt > 15 && fabs(electron_[i].eta) < 2.1;
                        bool El_IdTight = EleMVANonTrigId_Tight(electron_[i]);
                        bool El_Iso = Iso_Ele_dBeta(electron_[i]) < 0.1;
                        bool EL_CUTS = El_PtEta && El_IdTight && El_Iso;

                        //                        bool Tau_PtEta = tau_[k].pt > 20 && fabs(tau_[k].eta) < 2.3;
                        bool Tau_PtEta = tau_[k].pt > 15 && fabs(tau_[k].eta) < 2.3;
                        bool Tau_DMF = tau_[k].discriminationByDecayModeFindingOldDMs;
                        bool Tau_Isolation = tau_[k].byTightIsolationMVA3oldDMwLT > 0.5;
                        bool Tau_antiEl = tau_[k].discriminationByElectronMVA5Medium;
                        bool Tau_antiMu = tau_[k].discriminationByMuonLoose3;
                        bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;

                        bool ElTau_Charge = electron_[i].charge * tau_[k].charge < 0;
                        bool ElTau_dR = deltaR(electron_[i], tau_[k]) > 0.5;
//
//                        bool Veto_EM = Multi_Lepton_Veto("EM", m);
//                        bool Veto_EE = Multi_Lepton_Veto("EE", m);
//                        bool Veto_EEM = Multi_Lepton_Veto("EEM", m);
//                        bool Veto_EEE = Multi_Lepton_Veto("EEE", m);
//                        
                        bool Veto_EM = 1;
                        bool Veto_EE = 1;
                        bool Veto_EEM = 1;
                        bool Veto_EEE = 1;

                        bool LooseSelection = El_PtEta && Tau_PtEta && ElTau_dR && Veto_EE && Veto_EEM && Veto_EEE && Veto_EM;
                        //                        bool VLooseTauIso = tau_[k].byTightIsolationMVA3newDMwLT;
                        //                        bool VLooseTauIso = tau_[k].byTightIsolationMVA3oldDMwLT;
                        bool VLooseTauIso = tau_[k].byIsolationMVA3oldDMwLTraw > 0;
                        //                        bool VLooseTauIso = tau_[k].byIsolationMVA3newDMwLTraw > 0;
                        //                            bool VLooseEl_Iso = Iso_Ele_dBeta(electron_[i]) < 1;
                        vector<myobject> JETS = GoodJet30(m, electron_[i], tau_[k]);
                        vector<myobject> BJETS = GoodbJet20(m, electron_[i], tau_[k], 0, 1);
//                        vector<myobject> MVAMetRecoil_etau = m->RecoilMetetau;
                        mt_1 =1;

                        bool bjetCut_TMass = BJETS.size() > 0 && JETS.size() < 2 && mt_1 < 30;
                        //                        bool MatchedTrigger =electron_[i].hasTrgObject_Ele20Tau20  &&  tau_[k].hasTrgObject_Ele20Tau20;
                        bool MatchedTrigger = 1;
                        //                        Final selection
                        if (MatchedTrigger && EL_CUTS && TAU_CUTS && ElTau_Charge && ElTau_dR && Veto_EE && Veto_EEM && Veto_EEE && bjetCut_TMass) {
                            plotFill("eltau", ++eltau, 20, 0., 20.);
                            float MaxPtLep = 0;
                            float MaxEtaLep = -100;
                            float MaxPhiLep = -100;
                            for (int ig = 0; ig < genPar_.size(); ig++) {
                                if (fabs(genPar_[ig].pdgId) == 11) {
                                    if (genPar_[ig].pt > MaxPtLep) {
                                        MaxPtLep = genPar_[ig].pt;
                                        MaxEtaLep = genPar_[ig].eta;
                                        MaxPhiLep = genPar_[ig].phi;
                                    }
                                }
                            }
                            plotFill("GenElePT_InPassRecoEvents", MaxPtLep, 50, 0., 50.);
                            plotFill("GenEle_InPassRecoEvents_DR", dR(MaxEtaLep, MaxPhiLep, electron_[i].eta, electron_[i].phi), 500, 0., 10);
//                            break;

                        }
                        //##################################################################
                        //  Filling Tree
                        //##################################################################
                        if (Tau_antiEl && Tau_antiMu && LooseSelection && VLooseTauIso) {
                            fillTree(3, Run_Tree, m, is_data_mc.c_str(), FinalState, electron_[i], tau_[k]);
                        }

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

    map<string, TH2F*>::const_iterator iMap2 = myMap2->begin();
    map<string, TH2F*>::const_iterator jMap2 = myMap2->end();
    //
    for (; iMap2 != jMap2; ++iMap2)
        nplot2(iMap2->first)->Write();



    fout->Close();
    return 0;
}

