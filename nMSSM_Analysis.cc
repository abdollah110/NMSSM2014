// The code to do teh ZH totautau Analysis
// to make it excutable run: ./Make.sh ZH_Analysis.cc

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
#include "interface/zh_Corrector.h"
#include "interface/zh_Trigger.h"
#include "interface/zh_Tree.h"
#include "interface/Leptons_IdIso.h"
#include "interface/zh_Functions.h"
#include "interface/LumiReweightingStandAlone.h"

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

    string status_sample = *(argv + 1);
    cout << "*** First Argument, Data or MC, which type of data or MC ***" << endl;
    cout << status_sample.c_str() << endl;

    bool mc12 = (status_sample.compare("mc12") == 0 ? true : false);
    bool mc11 = (status_sample.compare("mc11") == 0 ? true : false);
    bool data12 = (status_sample.compare("data12") == 0 ? true : false);
    bool data11 = (status_sample.compare("data11") == 0 ? true : false);
    if (!(mc12 || mc11 || data12 || data11))
        cout << "xxxxx Error, please slecet between: mc12 || mc11 || data12 || data11 " << endl;

    //#################################################################################################
    //############## Second Argument, Run over just di-ele, just di-mu (for data) or total (for MC) ###
    //#################################################################################################

    string status_type = *(argv + 2);
    cout << "**** Second Argument, Run over just di-ele, just di-mu (for data) or total (for MC) ***" << endl;
    cout << status_type.c_str() << endl;
    bool is_tot = (status_type.compare("Tot") == 0 ? true : false);
    bool is_ele = (status_type.compare("Ele") == 0 ? true : false);
    bool is_mu = (status_type.compare("Mu") == 0 ? true : false);
    if (!(is_tot || is_ele || is_mu))
        cout << "xxxxx Error, please slecet between: Tot || Ele || Mu " << endl;

    //#################################################################################################
    //############## Third anad Forth Argument,   OutPut Name/ Input Files                         ########################
    //#################################################################################################

    string out = *(argv + 3);

    std::vector<string> fileNames;
    for (int f = 4; f < argc; f++) {
        fileNames.push_back(*(argv + f));
        // printing the input NAME
        cout << "\n INPUT NAME IS:   " << fileNames[f - 4] << "\t";
    }
    //#################################################################################################
    //############## defining an out_file name need on the given argument  information  ###############
    //#################################################################################################

    string outname = status_sample + "_" + status_type + "_" + out;
    //PRINTING THE OUTPUT name
    cout << "\n\n\n OUTPUT NAME IS:    " << outname << endl;
    TFile *fout = TFile::Open(outname.c_str(), "RECREATE");

    //#################################################################################################
    //############## initializing the PU correction                                    ###############
    //#################################################################################################

    reweight::LumiReWeighting* LumiWeights_12;
    LumiWeights_12 = new reweight::LumiReWeighting("interface/Summer12_PU.root", "interface/dataPileUpHistogram_True_2012.root", "mcPU", "pileup");
    reweight::LumiReWeighting* LumiWeights_11;
    //    LumiWeights_11 = new reweight::LumiReWeighting("interface/Fall11_PU_observed.root", "interface/dataPileUpHistogram_Observed_2011.root", "mcPU", "pileup"); // Last Bug found in 25 Nov
    LumiWeights_11 = new reweight::LumiReWeighting("interface/Fall11_PU.root", "interface/dataPileUpHistogram_True_2011.root", "mcPU", "pileup");

    //#################################################################################################
    //############## defining Tree Branches Filled via fillTree function                ###############
    //#################################################################################################
    TTree *Run_Tree = new TTree("RLE_tree", "RLE_tree");
    //    To force a memory-resident Tree
    Run_Tree->SetDirectory(0);

    Run_Tree->Branch("Channel", &Channel, "Channel/I");
    Run_Tree->Branch("Run", &Run, "Run/I");
    Run_Tree->Branch("Lumi", &Lumi, "Lumi/I");
    Run_Tree->Branch("Event", &Event, "Event/I");
    Run_Tree->Branch("IMass", &IMass, "IMass/F");
    Run_Tree->Branch("ZMass", &ZMass, "ZMass/F");
    Run_Tree->Branch("HMass", &HMass, "HMass/F");
    Run_Tree->Branch("met", &met, "met/F");
    Run_Tree->Branch("pfmet", &pfmet, "pfmet/F");
    Run_Tree->Branch("metPhi", &metPhi, "metPhi/F");
    Run_Tree->Branch("pfmetPhi", &pfmetPhi, "pfmetPhi/F");
    Run_Tree->Branch("covMet11", &covMet11, "covMet11/F");
    Run_Tree->Branch("covMet12", &covMet12, "covMet12/F");
    Run_Tree->Branch("covMet21", &covMet21, "covMet21/F");
    Run_Tree->Branch("covMet22", &covMet22, "covMet22/F");
    Run_Tree->Branch("pfcovMet11", &pfcovMet11, "pfcovMet11/F");
    Run_Tree->Branch("pfcovMet12", &pfcovMet12, "pfcovMet12/F");
    Run_Tree->Branch("pfcovMet21", &pfcovMet21, "pfcovMet21/F");
    Run_Tree->Branch("pfcovMet22", &pfcovMet22, "pfcovMet22/F");
    Run_Tree->Branch("num_PV", &num_PV, "num_PV/I");
    Run_Tree->Branch("num_bjet", &num_bjet, "num_bjet/I");
    Run_Tree->Branch("num_goodjet", &num_goodjet, "num_goodjet/I");
    Run_Tree->Branch("eff_Correction", &eff_Correction, "eff_Correction/F");
    Run_Tree->Branch("pu_Weight", &pu_Weight, "pu_Weight/F");

    Run_Tree->Branch("mu_Size", &mu_Size, "mu_Size/I");
    Run_Tree->Branch("BareMuon_Size", &BareMuon_Size, "BareMuon_Size/I");
    Run_Tree->Branch("electron_Size", &electron_Size, "electron_Size/I");
    Run_Tree->Branch("BareElectron_Size", &BareElectron_Size, "BareElectron_Size/I");
    Run_Tree->Branch("tau_Size", &tau_Size, "tau_Size/I");
    Run_Tree->Branch("BareTau_Size", &BareTau_Size, "BareTau_Size/I");
    Run_Tree->Branch("mu_partTight_Size", &mu_partTight_Size, "mu_partTight_Size/I");
    Run_Tree->Branch("ele_partTight_Size", &ele_partTight_Size, "ele_partTight_Size/I");

    Run_Tree->Branch("l1M", &l1M, "l1M/F");
    Run_Tree->Branch("l1E", &l1E, "l1E/F");
    Run_Tree->Branch("l1Px", &l1Px, "l1Px/F");
    Run_Tree->Branch("l1Py", &l1Py, "l1Py/F");
    Run_Tree->Branch("l1Pz", &l1Pz, "l1Pz/F");
    Run_Tree->Branch("l1Pt", &l1Pt, "l1Pt/F");
    Run_Tree->Branch("l1Eta", &l1Eta, "l1Eta/F");
    Run_Tree->Branch("l1Phi", &l1Phi, "l1Phi/F");
    Run_Tree->Branch("l1Charge", &l1Charge, "l1Charge/F");
    Run_Tree->Branch("l1_muId", &l1_muId, "l1_muId/O");
    Run_Tree->Branch("l1_muIso", &l1_muIso, "l1_muIso/F");
    Run_Tree->Branch("l1_eleId", &l1_eleId, "l1_eleId/O");
    Run_Tree->Branch("l1_eleIso", &l1_eleIso, "l1_eleIso/F");
    Run_Tree->Branch("l1_eleMVANonTrg", &l1_eleMVANonTrg, "l1_eleMVANonTrg/F");
    Run_Tree->Branch("l1_eleNumHit", &l1_eleNumHit, "l1_eleNumHit/F");

    Run_Tree->Branch("l2M", &l2M, "l2M/F");
    Run_Tree->Branch("l2E", &l2E, "l2E/F");
    Run_Tree->Branch("l2Px", &l2Px, "l2Px/F");
    Run_Tree->Branch("l2Py", &l2Py, "l2Py/F");
    Run_Tree->Branch("l2Pz", &l2Pz, "l2Pz/F");
    Run_Tree->Branch("l2Pt", &l2Pt, "l2Pt/F");
    Run_Tree->Branch("l2Eta", &l2Eta, "l2Eta/F");
    Run_Tree->Branch("l2Phi", &l2Phi, "l2Phi/F");
    Run_Tree->Branch("l2Charge", &l2Charge, "l2Charge/F");
    Run_Tree->Branch("l2_muId", &l2_muId, "l2_muId/O");
    Run_Tree->Branch("l2_muIso", &l2_muIso, "l2_muIso/F");
    Run_Tree->Branch("l2_eleId", &l2_eleId, "l2_eleId/O");
    Run_Tree->Branch("l2_eleIso", &l2_eleIso, "l2_eleIso/F");
    Run_Tree->Branch("l2_eleMVANonTrg", &l2_eleMVANonTrg, "l2_eleMVANonTrg/F");
    Run_Tree->Branch("l2_eleNumHit", &l2_eleNumHit, "l2_eleNumHit/F");

    Run_Tree->Branch("l3M", &l3M, "l3M/F");
    Run_Tree->Branch("l3E", &l3E, "l3E/F");
    Run_Tree->Branch("l3Px", &l3Px, "l3Px/F");
    Run_Tree->Branch("l3Py", &l3Py, "l3Py/F");
    Run_Tree->Branch("l3Pz", &l3Pz, "l3Pz/F");
    Run_Tree->Branch("l3Pt", &l3Pt, "l3Pt/F");
    Run_Tree->Branch("l3Eta", &l3Eta, "l3Eta/F");
    Run_Tree->Branch("l3Phi", &l3Phi, "l3Phi/F");
    Run_Tree->Branch("l3Charge", &l3Charge, "l3Charge/F");
    Run_Tree->Branch("l3_CloseJetPt", &l3_CloseJetPt, "l3_CloseJetPt/F");
    Run_Tree->Branch("l3_CloseJetEta", &l3_CloseJetEta, "l3_CloseJetEta/F");
    Run_Tree->Branch("l3_CloseJetPhi", &l3_CloseJetPhi, "l3_CloseJetPhi/F");
    Run_Tree->Branch("l3_muId_Loose", &l3_muId_Loose, "l3_muId_Loose/O");
    Run_Tree->Branch("l3_muId_Tight", &l3_muId_Tight, "l3_muId_Tight/O");
    Run_Tree->Branch("l3_eleId_Loose", &l3_eleId_Loose, "l3_eleId_Loose/O");
    Run_Tree->Branch("l3_eleId_Tight", &l3_eleId_Tight, "l3_eleId_Tight/O");
    Run_Tree->Branch("l3_muIso", &l3_muIso, "l3_muIso/F");
    Run_Tree->Branch("l3_eleIso", &l3_eleIso, "l3_eleIso/F");
    Run_Tree->Branch("l3_eleMVANonTrg", &l3_eleMVANonTrg, "l3_eleMVANonTrg/F");
    Run_Tree->Branch("l3_eleNumHit", &l3_eleNumHit, "l3_eleNumHit/F");
    Run_Tree->Branch("l3_tauIsoVL", &l3_tauIsoVL, "l3_tauIsoVL/O");
    Run_Tree->Branch("l3_tauIso3HitL", &l3_tauIso3HitL, "l3_tauIso3HitL/O");
    Run_Tree->Branch("l3_tauIso3HitM", &l3_tauIso3HitM, "l3_tauIso3HitM/O");
    Run_Tree->Branch("l3_tauIso3HitT", &l3_tauIso3HitT, "l3_tauIso3HitT/O");
    Run_Tree->Branch("l3_tauIsoL", &l3_tauIsoL, "l3_tauIsoL/O");
    Run_Tree->Branch("l3_tauIsoM", &l3_tauIsoM, "l3_tauIsoM/O");
    Run_Tree->Branch("l3_tauIsoT", &l3_tauIsoT, "l3_tauIsoT/O");
    Run_Tree->Branch("l3_tauIsoMVA2L", &l3_tauIsoMVA2L, "l3_tauIsoMVA2L/O");
    Run_Tree->Branch("l3_tauIsoMVA2M", &l3_tauIsoMVA2M, "l3_tauIsoMVA2M/O");
    Run_Tree->Branch("l3_tauIsoMVA2T", &l3_tauIsoMVA2T, "l3_tauIsoMVA2T/O");
    Run_Tree->Branch("l3_tauIsoMVA2raw", &l3_tauIsoMVA2raw, "l3_tauIsoMVA2raw/F");
    Run_Tree->Branch("l3_tauRejMuL", &l3_tauRejMuL, "l3_tauRejMuL/O");
    Run_Tree->Branch("l3_tauRejMu2L", &l3_tauRejMu2L, "l3_tauRejMu2L/O");
    Run_Tree->Branch("l3_tauRejMuM", &l3_tauRejMuM, "l3_tauRejMuM/O");
    Run_Tree->Branch("l3_tauRejMu2M", &l3_tauRejMu2M, "l3_tauRejMu2M/O");
    Run_Tree->Branch("l3_tauRejMuT", &l3_tauRejMuT, "l3_tauRejMuT/O");
    Run_Tree->Branch("l3_tauRejMu2T", &l3_tauRejMu2T, "l3_tauRejMu2T/O");
    Run_Tree->Branch("l3_tauRejEleL", &l3_tauRejEleL, "l3_tauRejEleL/O");
    Run_Tree->Branch("l3_tauRejEleM", &l3_tauRejEleM, "l3_tauRejEleM/O");
    Run_Tree->Branch("l3_tauRejEleMVA", &l3_tauRejEleMVA, "l3_tauRejEleMVA/O");
    Run_Tree->Branch("l3_tauRejEleMVA3L", &l3_tauRejEleMVA3L, "l3_tauRejEleMVA3L/O");
    Run_Tree->Branch("l3_tauRejEleMVA3M", &l3_tauRejEleMVA3M, "l3_tauRejEleMVA3M/O");
    Run_Tree->Branch("l3_tauRejEleMVA3T", &l3_tauRejEleMVA3T, "l3_tauRejEleMVA3T/O");
    Run_Tree->Branch("l3_RefJetPt", &l3_RefJetPt, "l3_RefJetPt/F");
    Run_Tree->Branch("l3_RefJetEta", &l3_RefJetEta, "l3_RefJetEta/F");
    Run_Tree->Branch("l3_RefJetPhi", &l3_RefJetPhi, "l3_RefJetPhi/F");



    Run_Tree->Branch("l4M", &l4M, "l4M/F");
    Run_Tree->Branch("l4E", &l4E, "l4E/F");
    Run_Tree->Branch("l4Px", &l4Px, "l4Px/F");
    Run_Tree->Branch("l4Py", &l4Py, "l4Py/F");
    Run_Tree->Branch("l4Pz", &l4Pz, "l4Pz/F");
    Run_Tree->Branch("l4Pt", &l4Pt, "l4Pt/F");
    Run_Tree->Branch("l4Eta", &l4Eta, "l4Eta/F");
    Run_Tree->Branch("l4Phi", &l4Phi, "l4Phi/F");
    Run_Tree->Branch("l4Charge", &l4Charge, "l4Charge/F");
    Run_Tree->Branch("l4_muId_Loose", &l4_muId_Loose, "l4_muId_Loose/O");
    Run_Tree->Branch("l4_muId_Tight", &l4_muId_Tight, "l4_muId_Tight/O");
    Run_Tree->Branch("l4_eleId_Loose", &l4_eleId_Loose, "l4_eleId_Loose/O");
    Run_Tree->Branch("l4_eleId_Tight", &l4_eleId_Tight, "l4_eleId_Tight/O");
    Run_Tree->Branch("l4_muIso", &l4_muIso, "l4_muIso/F");
    Run_Tree->Branch("l4_eleIso", &l4_eleIso, "l4_eleIso/F");
    Run_Tree->Branch("l4_eleMVANonTrg", &l4_eleMVANonTrg, "l4_eleMVANonTrg/F");
    Run_Tree->Branch("l4_eleNumHit", &l4_eleNumHit, "l4_eleNumHit/F");
    Run_Tree->Branch("l4_CloseJetPt", &l4_CloseJetPt, "l4_CloseJetPt/F");
    Run_Tree->Branch("l4_CloseJetEta", &l4_CloseJetEta, "l4_CloseJetEta/F");
    Run_Tree->Branch("l4_CloseJetPhi", &l4_CloseJetPhi, "l4_CloseJetPhi/F");
    Run_Tree->Branch("l4_tauIsoVL", &l4_tauIsoVL, "l4_tauIsoVL/O");
    Run_Tree->Branch("l4_tauIso3HitL", &l4_tauIso3HitL, "l4_tauIso3HitL/O");
    Run_Tree->Branch("l4_tauIso3HitM", &l4_tauIso3HitM, "l4_tauIso3HitM/O");
    Run_Tree->Branch("l4_tauIso3HitT", &l4_tauIso3HitT, "l4_tauIso3HitT/O");
    Run_Tree->Branch("l4_tauIsoL", &l4_tauIsoL, "l4_tauIsoL/O");
    Run_Tree->Branch("l4_tauIsoM", &l4_tauIsoM, "l4_tauIsoM/O");
    Run_Tree->Branch("l4_tauIsoT", &l4_tauIsoT, "l4_tauIsoT/O");
    Run_Tree->Branch("l4_tauIsoMVA2L", &l4_tauIsoMVA2L, "l4_tauIsoMVA2L/O");
    Run_Tree->Branch("l4_tauIsoMVA2M", &l4_tauIsoMVA2M, "l4_tauIsoMVA2M/O");
    Run_Tree->Branch("l4_tauIsoMVA2T", &l4_tauIsoMVA2T, "l4_tauIsoMVA2T/O");
    Run_Tree->Branch("l4_tauIsoMVA2raw", &l4_tauIsoMVA2raw, "l4_tauIsoMVA2raw/F");
    Run_Tree->Branch("l4_tauRejMuL", &l4_tauRejMuL, "l4_tauRejMuL/O");
    Run_Tree->Branch("l4_tauRejMu2L", &l4_tauRejMu2L, "l4_tauRejMu2L/O");
    Run_Tree->Branch("l4_tauRejMuM", &l4_tauRejMuM, "l4_tauRejMuM/O");
    Run_Tree->Branch("l4_tauRejMu2M", &l4_tauRejMu2M, "l4_tauRejMu2M/O");
    Run_Tree->Branch("l4_tauRejMuT", &l4_tauRejMuT, "l4_tauRejMuT/O");
    Run_Tree->Branch("l4_tauRejMu2T", &l4_tauRejMu2T, "l4_tauRejMu2T/O");
    Run_Tree->Branch("l4_tauRejEleL", &l4_tauRejEleL, "l4_tauRejEleL/O");
    Run_Tree->Branch("l4_tauRejEleM", &l4_tauRejEleM, "l4_tauRejEleM/O");
    Run_Tree->Branch("l4_tauRejEleMVA", &l4_tauRejEleMVA, "l4_tauRejEleMVA/O");
    Run_Tree->Branch("l4_tauRejEleMVA3L", &l4_tauRejEleMVA3L, "l4_tauRejEleMVA3L/O");
    Run_Tree->Branch("l4_tauRejEleMVA3M", &l4_tauRejEleMVA3M, "l4_tauRejEleMVA3M/O");
    Run_Tree->Branch("l4_tauRejEleMVA3T", &l4_tauRejEleMVA3T, "l4_tauRejEleMVA3T/O");
    Run_Tree->Branch("l4_RefJetPt", &l4_RefJetPt, "l4_RefJetPt/F");
    Run_Tree->Branch("l4_RefJetEta", &l4_RefJetEta, "l4_RefJetEta/F");
    Run_Tree->Branch("l4_RefJetPhi", &l4_RefJetPhi, "l4_RefJetPhi/F");
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
            vector<myobject> mu_ = myCleanLepton(m, "mu");
            vector<myobject> BareMuon = myCleanBareLepton(m, "mu");
            vector<myobject> electron_ = myCleanLepton(m, "ele");
            vector<myobject> BareElectron = myCleanBareLepton(m, "ele");
            vector<myobject> tau_ = myCleanLepton(m, "tau");
            vector<myobject> BareTau = myCleanBareLepton(m, "tau");

            vector<myobject> mu_partTight = LeptonSubSet(m, "mu_tight_partly");
            vector<myobject> ele_partTight = LeptonSubSet(m, "ele_tight_partly");

            //Number of B-jets
            int num_Bjet = bjet_Multiplicity(m);
            //*********************************************************************************************
            //****************************    PileUp re weighting    ***************************************
            //*********************************************************************************************
            int num_PU = 1;
            float PU_Weight = 1;

            if (mc12) {
                num_PU = m->PUInfo_true;
                PU_Weight = LumiWeights_12->weight(num_PU);
            }
            if (mc11) {
                //                num_PU = m->PUInfo; // Last Bug found in 25 Nov
                num_PU = m->PUInfo_true;
                PU_Weight = LumiWeights_11->weight(num_PU);
            }
            //*********************************************************************************************
            //****************************    Trigger      ************************************************
            //*********************************************************************************************
            bool Trigger;
            if (mc12) Trigger = Trg_MC_12(m);
            if (mc11) Trigger = Trg_MC_11(m);
            if (data12) Trigger = Trg_Data_12(m);
            if (data11) Trigger = Trg_Data_11(m);
            //*********************************************************************************************
            //*********************************************************************************************
            //*******************    Default Values     ***************************************************
            //*********************************************************************************************
            float nominalM_Z = 91.2;

            float HighPt_Lep = 20;
            float tight_Iso_Mu = 0.25; //it was 0.15
            float loose_Iso_Mu = 0.30;
            float tight_Iso_Ele = 0.15; // it eas 0.10
            float loose_Iso_Ele = 0.30;
            float loose_Iso_Lepton = 0.30; // it was 0.3
            float z_lowMass = 60;
            float z_highMass = 120;

            float bin_size = 300;
            float bin_max = 300;
            bool fill_tree[40];
            for (int u = 0; u < 10; u++) {
                fill_tree[u] = true;
            }
            float Cor_eff = 1;
            int MyChannel;

            //#################################################################################################
            //#################################################################################################
            //###############    2l2tau Analysis       #########################################################
            //#################################################################################################
            //#################################################################################################
            if (is_mu || is_tot) {
                //##############################################################################
                // MMTT
                //##############################################################################
                int mmtt = -1;
                plotFill("TotalEventsNumber", 0, 1, 0, 1);
                plotFill("mmtt", ++mmtt, 20, 0., 20.);
                if (mu_.size() == 2 && electron_.size() == 0 && Trigger) {
                    plotFill("mmtt", ++mmtt, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < mu_.size(); i++) {
                        for (int j = i + 1; j < mu_.size(); j++) {
                            for (int k = 0; k < tau_.size(); k++) {
                                for (int l = k + 1; l < tau_.size(); l++) {
                                    float Zboson_Mass = InvarMass_2(mu_[i], mu_[j]);
                                    float Higgs_Mass = InvarMass_2(tau_[k], tau_[l]);
                                    Cor_eff = getCorrFactor("mmtt", status_sample.c_str(), mu_[i], mu_[j], tau_[k], tau_[l]);

                                    bool first_l_HighPt = mu_[i].pt > HighPt_Lep;
                                    bool Z_Charge = mu_[i].charge * mu_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(mu_[i], mu_[j], tau_[k], tau_[l]);
                                    bool Di_mu_Id = Id_Mu_Loose(mu_[i]) && Id_Mu_Loose(mu_[j]);
                                    bool Di_mu_Iso = Iso_Mu_dBeta(mu_[i]) < loose_Iso_Mu && Iso_Mu_dBeta(mu_[j]) < loose_Iso_Mu;
                                    bool mu_Rejection = getMuRejection("mmtt", tau_[k]) && getMuRejection("mmtt", tau_[l]);
                                    bool ele_Rejection = getEleRejection("mmtt", tau_[k]) && getEleRejection("mmtt", tau_[l]);
                                    bool tau_Iso_1 = getTauIsolation("mmtt", tau_[k]);
                                    bool tau_Iso_2 = getTauIsolation("mmtt", tau_[l]);
                                    bool H_Charge = tau_[k].charge * tau_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("mmtt", mu_[i], mu_[j], tau_[k], tau_[l]);
                                    bool bjet_num = num_Bjet < 1;

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_mu_Id && Di_mu_Iso && mu_Rejection && ele_Rejection && tau_Iso_1 && tau_Iso_2;
                                    if (generalCuts && specialCuts) {

                                        //Filling the tree
                                        MyChannel = 1;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("Zboson_Mass_mmtt", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_mmtt", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("mmtt", ++mmtt, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("mmtt", ++mmtt, 20, 0., 20., PU_Weight);
                                            plotFill("mmtt", ++mmtt, 20, 0., 20., Cor_eff);
                                            plotFill("mmtt", ++mmtt, 20, 0., 20.);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 1, mu_[i], mu_[j], tau_[k], tau_[l]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                //##############################################################################
                // MMET
                //##############################################################################
                int mmet = -1;
                plotFill("mmet", ++mmet, 20, 0., 20.);
                if (mu_.size() == 2 && electron_.size() == 1 && Trigger) {
                    plotFill("mmet", ++mmet, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < mu_.size(); i++) {
                        for (int j = i + 1; j < mu_.size(); j++) {
                            for (int k = 0; k < electron_.size(); k++) {
                                for (int l = 0; l < tau_.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(mu_[i], mu_[j]);
                                    float Higgs_Mass = InvarMass_2(electron_[k], tau_[l]);

                                    bool first_l_HighPt = mu_[i].pt > HighPt_Lep;
                                    bool Z_Charge = mu_[i].charge * mu_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(mu_[i], mu_[j], electron_[k], tau_[l]);
                                    bool Di_mu_Id = Id_Mu_Loose(mu_[i]) && Id_Mu_Loose(mu_[j]);
                                    bool Di_mu_Iso = Iso_Mu_dBeta(mu_[i]) < loose_Iso_Mu && Iso_Mu_dBeta(mu_[j]) < loose_Iso_Mu;
                                    bool ele_Id = EleMVANonTrigId_Tight(electron_[k]) && electron_[k].numHitEleInner < 1;
                                    bool ele_Iso = Iso_Ele_dBeta(electron_[k]) < tight_Iso_Ele;
                                    bool tau_Iso = getTauIsolation("mmet", tau_[l]);
                                    bool ele_Rejection = getEleRejection("mmet", tau_[l]);
                                    bool H_Charge = electron_[k].charge * tau_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("mmet", mu_[i], mu_[j], electron_[k], tau_[l]);
                                    bool bjet_num = num_Bjet < 1;
                                    Cor_eff = getCorrFactor("mmet", status_sample.c_str(), mu_[i], mu_[j], electron_[k], tau_[l]);

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_mu_Id && Di_mu_Iso && ele_Id && ele_Iso && tau_Iso && ele_Rejection;
                                    if (generalCuts && specialCuts) {

                                        //Filling the tree
                                        MyChannel = 2;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("mmet", ++mmet, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("mmet", ++mmet, 20, 0., 20., PU_Weight);
                                            plotFill("mmet", ++mmet, 20, 0., 20., Cor_eff);
                                            plotFill("mmet", ++mmet, 20, 0., 20.);
                                            plotFill("Zboson_Mass_mmet", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_mmet", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 2, mu_[i], mu_[j], electron_[k], tau_[l]);

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //##############################################################################
                // MMMT
                //##############################################################################
                int mmmt = -1;
                plotFill("mmmt", ++mmmt, 20, 0., 20.);
                if (mu_.size() == 3 && electron_.size() == 0 && Trigger) {
                    plotFill("mmmt", ++mmmt, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < mu_.size(); i++) {
                        for (int j = i + 1; j < mu_.size(); j++) {
                            for (int k = j + 1; k < mu_.size(); k++) {

                                float mass_ij = (mu_[i].charge * mu_[j].charge < 0 ? TMath::Abs(InvarMass_2(mu_[i], mu_[j]) - nominalM_Z) : 1000);
                                float mass_ik = (mu_[i].charge * mu_[k].charge < 0 ? TMath::Abs(InvarMass_2(mu_[i], mu_[k]) - nominalM_Z) : 1000);
                                float mass_jk = (mu_[j].charge * mu_[k].charge < 0 ? TMath::Abs(InvarMass_2(mu_[j], mu_[k]) - nominalM_Z) : 1000);

                                if (mass_ik < mass_ij && mass_ik < mass_jk) swap(mu_[j], mu_[k]);
                                if (mass_jk < mass_ij && mass_jk < mass_ik) swap(mu_[i], mu_[k]);

                                for (int l = 0; l < tau_.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(mu_[i], mu_[j]);
                                    float Higgs_Mass = InvarMass_2(mu_[k], tau_[l]);

                                    if (mu_[i].pt < mu_[j].pt) swap(mu_[i], mu_[j]); // order muon based on Pt
                                    bool first_l_HighPt = mu_[i].pt > HighPt_Lep;
                                    bool Z_Charge = mu_[i].charge * mu_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(mu_[i], mu_[j], mu_[k], tau_[l]);
                                    bool Di_mu_Iso = Iso_Mu_dBeta(mu_[i]) < loose_Iso_Mu && Iso_Mu_dBeta(mu_[j]) < loose_Iso_Mu;
                                    bool Di_mu_Id = Id_Mu_Loose(mu_[i]) && Id_Mu_Loose(mu_[j]);
                                    bool mu_Id = Id_Mu_Tight(mu_[k]);
                                    bool mu_Iso = Iso_Mu_dBeta(mu_[k]) < tight_Iso_Mu;
                                    bool tau_Iso = getTauIsolation("mmmt", tau_[l]);
                                    bool mu_Rejection = getMuRejection("mmmt", tau_[l]);
                                    bool H_Charge = mu_[k].charge * tau_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("mmmt", mu_[i], mu_[j], mu_[k], tau_[l]);
                                    bool bjet_num = num_Bjet < 1;
                                    Cor_eff = getCorrFactor("mmmt", status_sample.c_str(), mu_[i], mu_[j], mu_[k], tau_[l]);

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_mu_Iso && Di_mu_Id && mu_Iso && mu_Id && tau_Iso && mu_Rejection;
                                    if (generalCuts && specialCuts) {

                                        //Filling the tree
                                        MyChannel = 3;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("Zboson_Mass_mmmt", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_mmmt", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("mmmt", ++mmmt, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("mmmt", ++mmmt, 20, 0., 20., PU_Weight);
                                            plotFill("mmmt", ++mmmt, 20, 0., 20., Cor_eff);
                                            plotFill("mmmt", ++mmmt, 20, 0., 20.);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 3, mu_[i], mu_[j], mu_[k], tau_[l]);

                                        }
                                    }
                                }
                                mu_ = GoodMuon(m);
                            }
                        }
                    }
                }
                //##############################################################################
                // MMME
                //##############################################################################
                int mmme = -1;
                plotFill("mmme", ++mmme, 20, 0., 20.);
                if (mu_.size() == 3 && electron_.size() == 1 && Trigger) {
                    plotFill("mmme", ++mmme, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < mu_.size(); i++) {
                        for (int j = i + 1; j < mu_.size(); j++) {
                            for (int k = j + 1; k < mu_.size(); k++) {

                                float mass_ij = (mu_[i].charge * mu_[j].charge < 0 ? TMath::Abs(InvarMass_2(mu_[i], mu_[j]) - nominalM_Z) : 1000);
                                float mass_ik = (mu_[i].charge * mu_[k].charge < 0 ? TMath::Abs(InvarMass_2(mu_[i], mu_[k]) - nominalM_Z) : 1000);
                                float mass_jk = (mu_[j].charge * mu_[k].charge < 0 ? TMath::Abs(InvarMass_2(mu_[j], mu_[k]) - nominalM_Z) : 1000);

                                if (mass_ik < mass_ij && mass_ik < mass_jk) swap(mu_[j], mu_[k]);
                                if (mass_jk < mass_ij && mass_jk < mass_ik) swap(mu_[i], mu_[k]);

                                for (int l = 0; l < electron_.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(mu_[i], mu_[j]);
                                    float Higgs_Mass = InvarMass_2(mu_[k], electron_[l]);

                                    if (mu_[i].pt < mu_[j].pt) swap(mu_[i], mu_[j]); // order muon based on Pt
                                    bool first_l_HighPt = mu_[i].pt > HighPt_Lep;
                                    bool Z_Charge = mu_[i].charge * mu_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass< z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(mu_[i], mu_[j], mu_[k], electron_[l]);
                                    bool Di_mu_Iso = Iso_Mu_dBeta(mu_[i]) < loose_Iso_Mu && Iso_Mu_dBeta(mu_[j]) < loose_Iso_Mu;
                                    bool Di_mu_Id = Id_Mu_Loose(mu_[i]) && Id_Mu_Loose(mu_[j]);
                                    bool mu_Iso = Iso_Mu_dBeta(mu_[k]) < loose_Iso_Lepton;
                                    bool mu_Id = Id_Mu_Loose(mu_[k]);
                                    bool electron_Id = EleMVANonTrigId_Loose(electron_[l]);
                                    bool electron_Iso = Iso_Ele_dBeta(electron_[l]) < loose_Iso_Lepton;
                                    bool H_Charge = mu_[k].charge * electron_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("mmme", mu_[i], mu_[j], mu_[k], electron_[l]);
                                    bool bjet_num = num_Bjet < 1;
                                    Cor_eff = getCorrFactor("mmme", status_sample.c_str(), mu_[i], mu_[j], mu_[k], electron_[l]);

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_mu_Iso && Di_mu_Id && mu_Iso && mu_Id && electron_Iso && electron_Id;
                                    if (generalCuts && specialCuts) {


                                        //Filling the tree
                                        MyChannel = 4;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("Zboson_Mass_mmme", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_mmme", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("mmme", ++mmme, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("mmme", ++mmme, 20, 0., 20., PU_Weight);
                                            plotFill("mmme", ++mmme, 20, 0., 20., Cor_eff);
                                            plotFill("mmme", ++mmme, 20, 0., 20.);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 4, mu_[i], mu_[j], mu_[k], electron_[l]);
                                        }
                                    }
                                }
                                mu_ = GoodMuon(m);
                            }
                        }
                    }
                }

            }//end of only Muon
            //#################################################################################################
            //#################################################################################################
            //#######################  Double Electron #######################
            //#################################################################################################
            //#################################################################################################
            if (is_ele || is_tot) {
                //##############################################################################
                // EETT
                //##############################################################################
                int eett = -1;
                plotFill("eett", ++eett, 20, 0., 20.);
                if (electron_.size() == 2 && mu_.size() == 0 && Trigger) {
                    plotFill("eett", ++eett, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < electron_.size(); i++) {
                        for (int j = i + 1; j < electron_.size(); j++) {
                            for (int k = 0; k < tau_.size(); k++) {
                                for (int l = 0; l < k; l++) {

                                    float Zboson_Mass = InvarMass_2(electron_[i], electron_[j]);
                                    float Higgs_Mass = InvarMass_2(tau_[k], tau_[l]);

                                    bool first_l_HighPt = electron_[i].pt > HighPt_Lep;
                                    bool Z_Charge = electron_[i].charge * electron_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(electron_[i], electron_[j], tau_[k], tau_[l]);
                                    bool Di_Ele_Id = EleMVANonTrigId_Loose(electron_[i]) && EleMVANonTrigId_Loose(electron_[j]);
                                    bool Di_Ele_Iso = Iso_Ele_dBeta(electron_[i]) < loose_Iso_Ele && Iso_Ele_dBeta(electron_[j]) < loose_Iso_Ele;
                                    bool mu_Rejection = getMuRejection("eett", tau_[k]) && getMuRejection("eett", tau_[l]);
                                    bool ele_Rejection = getEleRejection("eett", tau_[k]) && getEleRejection("eett", tau_[l]);
                                    bool tau_Iso_1 = getTauIsolation("eett", tau_[k]);
                                    bool tau_Iso_2 = getTauIsolation("eett", tau_[l]);
                                    bool H_Charge = tau_[k].charge * tau_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("eett", electron_[i], electron_[j], tau_[k], tau_[l]);
                                    bool bjet_num = num_Bjet < 1;
                                    Cor_eff = getCorrFactor("eett", status_sample.c_str(), electron_[i], electron_[j], tau_[k], tau_[l]);

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_Ele_Iso && Di_Ele_Id && ele_Rejection && mu_Rejection && tau_Iso_1 && tau_Iso_2;
                                    if (generalCuts && specialCuts) {

                                        //Filling the tree
                                        MyChannel = 5;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("eett", ++eett, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("eett", ++eett, 20, 0., 20., PU_Weight);
                                            plotFill("eett", ++eett, 20, 0., 20., Cor_eff);
                                            plotFill("eett", ++eett, 20, 0., 20.);
                                            plotFill("Zboson_Mass_eett", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_eett", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 5, electron_[i], electron_[j], tau_[k], tau_[l]);

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //##############################################################################
                // EEMT
                //##############################################################################
                int eemt = -1;
                plotFill("eemt", ++eemt, 20, 0., 20.);
                if (electron_.size() == 2 && mu_.size() == 1 && Trigger) {
                    plotFill("eemt", ++eemt, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < electron_.size(); i++) {
                        for (int j = i + 1; j < electron_.size(); j++) {
                            for (int k = 0; k < mu_.size(); k++) {
                                for (int l = 0; l < tau_.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(electron_[i], electron_[j]);
                                    float Higgs_Mass = InvarMass_2(mu_[k], tau_[l]);

                                    bool first_l_HighPt = electron_[i].pt > HighPt_Lep;
                                    bool Z_Charge = electron_[i].charge * electron_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(electron_[i], electron_[j], mu_[k], tau_[l]);
                                    bool Di_Ele_Id = EleMVANonTrigId_Loose(electron_[i]) && EleMVANonTrigId_Loose(electron_[j]);
                                    bool Di_Ele_Iso = Iso_Ele_dBeta(electron_[i]) < loose_Iso_Ele && Iso_Ele_dBeta(electron_[j]) < loose_Iso_Ele;
                                    bool mu_Id = Id_Mu_Tight(mu_[k]);
                                    bool mu_Iso = Iso_Mu_dBeta(mu_[k]) < tight_Iso_Mu;
                                    bool tau_Iso = getTauIsolation("eemt", tau_[l]);
                                    bool mu_Rejection = getMuRejection("eemt", tau_[l]);
                                    bool H_Charge = mu_[k].charge * tau_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("eemt", electron_[i], electron_[j], mu_[k], tau_[l]);
                                    bool bjet_num = num_Bjet < 1;
                                    Cor_eff = getCorrFactor("eemt", status_sample.c_str(), electron_[i], electron_[j], mu_[k], tau_[l]);

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_Ele_Iso && Di_Ele_Id && mu_Iso && mu_Id && tau_Iso && mu_Rejection;
                                    if (generalCuts && specialCuts) {

                                        //Filling the tree
                                        MyChannel = 6;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("Zboson_Mass_eemt", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_eemt", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("eemt", ++eemt, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("eemt", ++eemt, 20, 0., 20., PU_Weight);
                                            plotFill("eemt", ++eemt, 20, 0., 20., Cor_eff);
                                            plotFill("eemt", ++eemt, 20, 0., 20.);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 6, electron_[i], electron_[j], mu_[k], tau_[l]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //##############################################################################
                // EEET
                //##############################################################################
                int eeet = -1;
                plotFill("eeet", ++eeet, 20, 0., 20.);
                if (electron_.size() == 3 && mu_.size() == 0 && Trigger) {
                    plotFill("eeet", ++eeet, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < electron_.size(); i++) {
                        for (int j = i + 1; j < electron_.size(); j++) {
                            for (int k = j + 1; k < electron_.size(); k++) {

                                float mass_ij = (electron_[i].charge * electron_[j].charge < 0 ? TMath::Abs(InvarMass_2(electron_[i], electron_[j]) - nominalM_Z) : 1000);
                                float mass_ik = (electron_[i].charge * electron_[k].charge < 0 ? TMath::Abs(InvarMass_2(electron_[i], electron_[k]) - nominalM_Z) : 1000);
                                float mass_jk = (electron_[j].charge * electron_[k].charge < 0 ? TMath::Abs(InvarMass_2(electron_[j], electron_[k]) - nominalM_Z) : 1000);

                                if (mass_ik < mass_ij && mass_ik < mass_jk) swap(electron_[j], electron_[k]);
                                if (mass_jk < mass_ij && mass_jk < mass_ik) swap(electron_[i], electron_[k]);

                                for (int l = 0; l < tau_.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(electron_[i], electron_[j]);
                                    float Higgs_Mass = InvarMass_2(electron_[k], tau_[l]);

                                    if (electron_[i].pt < electron_[j].pt) swap(electron_[i], electron_[j]);
                                    bool first_l_HighPt = electron_[i].pt > HighPt_Lep;
                                    bool Z_Charge = electron_[i].charge * electron_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(electron_[i], electron_[j], electron_[k], tau_[l]);
                                    bool Di_Ele_Id = EleMVANonTrigId_Loose(electron_[i]) && EleMVANonTrigId_Loose(electron_[j]);
                                    bool Di_Ele_Iso = Iso_Ele_dBeta(electron_[i]) < loose_Iso_Ele && Iso_Ele_dBeta(electron_[j]) < loose_Iso_Ele;
                                    bool electron_Id = EleMVANonTrigId_Tight(electron_[k]) && electron_[k].numHitEleInner < 1;
                                    bool electron_Iso = Iso_Ele_dBeta(electron_[k]) < tight_Iso_Ele;
                                    bool tau_Iso = getTauIsolation("eeet", tau_[l]);
                                    bool ele_Rejection = getEleRejection("eeet", tau_[l]);
                                    bool H_Charge = electron_[k].charge * tau_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("eeet", electron_[i], electron_[j], electron_[k], tau_[l]);
                                    bool bjet_num = num_Bjet < 1;
                                    Cor_eff = getCorrFactor("eeet", status_sample.c_str(), electron_[i], electron_[j], electron_[k], tau_[l]);

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_Ele_Iso && Di_Ele_Id && electron_Iso && electron_Id && tau_Iso && ele_Rejection;
                                    if (generalCuts && specialCuts) {

                                        //Filling the tree
                                        MyChannel = 7;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("Zboson_Mass_eeet", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_eeet", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("eeet", ++eeet, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("eeet", ++eeet, 20, 0., 20., PU_Weight);
                                            plotFill("eeet", ++eeet, 20, 0., 20., Cor_eff);
                                            plotFill("eeet", ++eeet, 20, 0., 20.);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 7, electron_[i], electron_[j], electron_[k], tau_[l]);
                                        }
                                    }
                                }
                                electron_ = GoodElectron(m);
                            }
                        }
                    }
                }
                //##############################################################################
                // EEEM
                //##############################################################################
                int eeem = -1;
                plotFill("eeem", ++eeem, 20, 0., 20.);
                if (electron_.size() == 3 && mu_.size() == 1 && Trigger) {
                    plotFill("eeem", ++eeem, 20, 0., 20., PU_Weight);

                    for (int i = 0; i < electron_.size(); i++) {
                        for (int j = i + 1; j < electron_.size(); j++) {
                            for (int k = j + 1; k < electron_.size(); k++) {

                                float mass_ij = (electron_[i].charge * electron_[j].charge < 0 ? TMath::Abs(InvarMass_2(electron_[i], electron_[j]) - nominalM_Z) : 1000);
                                float mass_ik = (electron_[i].charge * electron_[k].charge < 0 ? TMath::Abs(InvarMass_2(electron_[i], electron_[k]) - nominalM_Z) : 1000);
                                float mass_jk = (electron_[j].charge * electron_[k].charge < 0 ? TMath::Abs(InvarMass_2(electron_[j], electron_[k]) - nominalM_Z) : 1000);
                                if (mass_ik < mass_ij && mass_ik < mass_jk) swap(electron_[j], electron_[k]);
                                if (mass_jk < mass_ij && mass_jk < mass_ik) swap(electron_[i], electron_[k]);

                                for (int l = 0; l < mu_.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(electron_[i], electron_[j]);
                                    float Higgs_Mass = InvarMass_2(electron_[k], mu_[l]);

                                    if (electron_[i].pt < electron_[j].pt) swap(electron_[i], electron_[j]);
                                    bool first_l_HighPt = electron_[i].pt > HighPt_Lep;
                                    bool Z_Charge = electron_[i].charge * electron_[j].charge < 0;
                                    bool Z_Mass_b = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(electron_[i], electron_[j], electron_[k], mu_[l]);
                                    bool Di_Ele_Id = EleMVANonTrigId_Loose(electron_[i]) && EleMVANonTrigId_Loose(electron_[j]);
                                    bool Di_Ele_Iso = Iso_Ele_dBeta(electron_[i]) < loose_Iso_Ele && Iso_Ele_dBeta(electron_[j]) < loose_Iso_Ele;
                                    bool electron_Id = EleMVANonTrigId_Loose(electron_[k]);
                                    bool electron_Iso = Iso_Ele_dBeta(electron_[k]) < loose_Iso_Lepton;
                                    bool mu_Id = Id_Mu_Loose(mu_[l]);
                                    bool mu_Iso = Iso_Mu_dBeta(mu_[l]) < loose_Iso_Lepton;
                                    bool H_Charge = electron_[k].charge * mu_[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("eeem", electron_[i], electron_[j], electron_[k], mu_[l]);
                                    bool bjet_num = num_Bjet < 1;
                                    Cor_eff = getCorrFactor("eeem", status_sample.c_str(), electron_[i], electron_[j], electron_[k], mu_[l]);

                                    bool generalCuts = bjet_num && first_l_HighPt && Z_Charge && Z_Mass_b && Overlap_Dz && SumPtCut && H_Charge;
                                    bool specialCuts = Di_Ele_Iso && Di_Ele_Id && electron_Iso && electron_Id && mu_Id && mu_Iso;
                                    if (generalCuts && specialCuts) {

                                        //Filling the tree
                                        MyChannel = 8;
                                        if (fill_tree[MyChannel]) {
                                            fill_tree[MyChannel] = false;
                                            plotFill("eeem", ++eeem, 20, 0., 20., PU_Weight * Cor_eff);
                                            plotFill("eeem", ++eeem, 20, 0., 20., PU_Weight);
                                            plotFill("eeem", ++eeem, 20, 0., 20., Cor_eff);
                                            plotFill("eeem", ++eeem, 20, 0., 20.);
                                            plotFill("Zboson_Mass_eeem", Zboson_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            plotFill("Higgs_Mass_eeem", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 8, electron_[i], electron_[j], electron_[k], mu_[l]);
                                        }
                                    }
                                }
                                electron_ = GoodElectron(m);
                            }
                        }
                    }
                }

            }//end of Only Electron
            //#################################################################################################
            //#################################################################################################
            //#################################################################################################
            //#########################        Fake Rate Estimation        ##########################################
            //#################################################################################################
            //#################################################################################################
            //#################################################################################################
            if (is_mu || is_tot) {
                //##############################################################################
                // MMTT
                //##############################################################################
                bool Fill_Once_ff = true;
                bool Fill_Once_fp = true;
                bool Fill_Once_pf = true;
                bool fill_once_Estimate = true;

                if (BareTau.size() > 1 && BareMuon.size() > 1 && Trigger) {
                    for (int i = 0; i < BareMuon.size(); i++) {
                        for (int j = i + 1; j < BareMuon.size(); j++) {
                            for (int k = 0; k < BareTau.size(); k++) {
                                for (int l = k + 1; l < BareTau.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(BareMuon[i], BareMuon[j]);
                                    float Higgs_Mass = InvarMass_2(BareTau[k], BareTau[l]);

                                    bool first_l_HighPt = BareMuon[i].pt > HighPt_Lep;
                                    bool Z_Charge = BareMuon[i].charge * BareMuon[j].charge < 0;
                                    bool mu_Iso_1 = Iso_Mu_dBeta(BareMuon[i]) < loose_Iso_Mu;
                                    bool mu_Iso_2 = Iso_Mu_dBeta(BareMuon[j]) < loose_Iso_Mu;
                                    bool mu_Id_1 = Id_Mu_Loose(BareMuon[i]);
                                    bool mu_Id_2 = Id_Mu_Loose(BareMuon[j]);
                                    bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);
                                    bool mu_Rejection = getMuRejection("mmtt", BareTau[k]) && getMuRejection("mmtt", BareTau[l]);
                                    bool ele_Rejection = getEleRejection("mmtt", BareTau[k]) && getEleRejection("mmtt", BareTau[l]);
                                    bool tau_Iso_1 = getTauIsolation("mmtt", BareTau[k]);
                                    bool tau_Iso_2 = getTauIsolation("mmtt", BareTau[l]);
                                    bool H_Charge = BareTau[k].charge * BareTau[l].charge < 0;
                                    bool No_extra_lepton = mu_.size() == 2 && electron_.size() == 0;
                                    bool bjet_num = num_Bjet < 1;
                                    bool SumPtCut = getSumPtCut("mmtt", BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);
                                    Cor_eff = getCorrFactor("mmtt", status_sample.c_str(), BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);

                                    bool preSelection = bjet_num && No_extra_lepton && Overlap_Dz && first_l_HighPt && Z_Charge && mu_Iso_1 && mu_Iso_2 && mu_Id_1 && mu_Id_2 && Leading_Z_Mass;
                                    bool FR_Application = SumPtCut && BareTau[k].pt > 20 && BareTau[l].pt > 20 && H_Charge && mu_Rejection && ele_Rejection;
                                    bool FR_Estimation = fill_once_Estimate && !H_Charge && BareTau[k].pt > 10 && BareTau[l].pt > 10 && mu_Rejection && ele_Rejection;

                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************

                                    if (preSelection && BareTau[k].pt > 10 && BareTau[l].pt > 10) {
                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 91, BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);
                                    }
                                    //                                    if (preSelection && FR_Application) {
                                    //
                                    //                                        if (Fill_Once_ff && !tau_Iso_1 && !tau_Iso_2) {
                                    //                                            Fill_Once_ff = false;
                                    //                                            plotFill("FakeRate_MMTT_apply_ff", BareTau[k].pt, BareTau[l].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 11, BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && !tau_Iso_1 && tau_Iso_2) {
                                    //                                            Fill_Once_fp = false;
                                    //                                            plotFill("FakeRate_MMTT_apply_fp", BareTau[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 21, BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_pf && tau_Iso_1 && !tau_Iso_2) {
                                    //                                            Fill_Once_pf = false;
                                    //                                            plotFill("FakeRate_MMTT_apply_pf", BareTau[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 31, BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_mmtt_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 41, BareMuon[i], BareMuon[j], BareTau[k], BareTau[l]);
                                    //                                    }//end fake rate application

                                    //*********************************************************************************
                                    //***************************Fake Rate Estimation SS ***************************
                                    //*********************************************************************************
                                    //                                    if (preSelection && FR_Estimation) {
                                    //
                                    //                                        //Denumerator
                                    //                                        plotFill("4objFR_Tau_Denum", BareTau[k].pt, 200, 0, 200);
                                    //                                        plotFill("4objFR_Tau_Denum", BareTau[l].pt, 200, 0, 200);
                                    //
                                    //                                        //numerator Medium
                                    //                                        if (tau_Iso_1) {
                                    //                                            plotFill("4objFR_Tau_NumMedium", BareTau[k].pt, 200, 0, 200);
                                    //                                        }
                                    //                                        //numerator Medium
                                    //                                        if (tau_Iso_2) {
                                    //                                            plotFill("4objFR_Tau_NumMedium", BareTau[l].pt, 200, 0, 200);
                                    //                                        }
                                    //                                        fill_once_Estimate = false;
                                    //                                    }//end fake rate estimation

                                }
                            }
                        }
                    }
                }
                //##############################################################################
                // MMET
                //##############################################################################
                Fill_Once_ff = true;
                Fill_Once_fp = true;
                Fill_Once_pf = true;

                if (BareTau.size() > 0 && BareElectron.size() > 0 && BareMuon.size() > 1 && Trigger) {
                    for (int i = 0; i < BareMuon.size(); i++) {
                        for (int j = i + 1; j < BareMuon.size(); j++) {
                            for (int k = 0; k < BareElectron.size(); k++) {
                                for (int l = 0; l < BareTau.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(BareMuon[i], BareMuon[j]);
                                    float Higgs_Mass = InvarMass_2(BareElectron[k], BareTau[l]);

                                    bool first_l_HighPt = BareMuon[i].pt > HighPt_Lep;
                                    bool Z_Charge = BareMuon[i].charge * BareMuon[j].charge < 0;
                                    bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    bool mu_Iso_1 = Iso_Mu_dBeta(BareMuon[i]) < loose_Iso_Mu;
                                    bool mu_Iso_2 = Iso_Mu_dBeta(BareMuon[j]) < loose_Iso_Mu;
                                    bool mu_Id_1 = Id_Mu_Loose(BareMuon[i]);
                                    bool mu_Id_2 = Id_Mu_Loose(BareMuon[j]);
                                    bool electron_Isol_Tight = Iso_Ele_dBeta(BareElectron[k]) < tight_Iso_Ele;
                                    bool electron_Isol_Loose = Iso_Ele_dBeta(BareElectron[k]) < loose_Iso_Ele;
                                    bool electron_Id = EleMVANonTrigId_Tight(BareElectron[k]);
                                    bool H_Charge = BareElectron[k].charge * BareTau[l].charge < 0;
                                    bool tau_Iso = getTauIsolation("mmet", BareTau[l]);
                                    bool ele_Rejection = getEleRejection("mmet", BareTau[l]);
                                    bool ele_numHit = BareElectron[k].numHitEleInner < 1;
                                    bool No_extra_lepton = (mu_.size() == 2);
                                    bool bjet_num = num_Bjet < 1;
                                    bool SumPtCut = getSumPtCut("mmet", BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    Cor_eff = getCorrFactor("mmet", status_sample.c_str(), BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);

                                    bool preSelection = Overlap_Dz && No_extra_lepton && ele_numHit && bjet_num && first_l_HighPt && mu_Iso_1 && mu_Iso_2 && mu_Id_1 && mu_Id_2 && Z_Charge && Leading_Z_Mass;
                                    bool FR_Application = SumPtCut && BareTau[l].pt > 20 && H_Charge && ele_Rejection;
                                    bool FR_Estimation_tau0 = BareTau[l].pt > 0 && !H_Charge && ele_Rejection;
                                    bool tauPtCheck = ((mc11 || mc12) ? BareTau[l].pt > 10 : BareTau[l].pt > 5); // This is to have less candidate in ZZ4L events for SVMass 

                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************
                                    if (preSelection && tauPtCheck) {
                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 92, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    }

                                    //                                    if (preSelection && FR_Application) {
                                    //                                        if (Fill_Once_ff && !tau_Iso && (!electron_Isol_Tight || !electron_Id) && (electron_.size() - ele_partTight.size() == 0)) {
                                    //                                            Fill_Once_ff = false;
                                    //                                            plotFill("FakeRate_MMET_apply_ff", BareTau[l].pt, BareElectron[k].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 12, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && !tau_Iso && electron_Isol_Tight && electron_Id && (electron_.size() == 1)) {
                                    //                                            Fill_Once_fp = false;
                                    //                                            plotFill("FakeRate_MMET_apply_fp", BareTau[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 22, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_pf && tau_Iso && (!electron_Isol_Tight || !electron_Id) && (electron_.size() - ele_partTight.size() == 0)) {
                                    //                                            Fill_Once_pf = false;
                                    //                                            plotFill("FakeRate_MMET_apply_pf", BareElectron[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 32, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_mmet_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 42, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    //                                    }//end of fake apply
                                    //*********************************************************************************
                                    //***************************Fake Rate Estimation ***************************
                                    //*********************************************************************************
                                    //                                    if (preSelection && FR_Estimation_tau0) {
                                    //
                                    //                                        //Denumerator
                                    //                                        if (WZ_Rej(m, BareElectron[k]) && BareElectron.size() < 2) {
                                    //                                            plotFill("4objFR_Ele_Denum", BareElectron[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 52, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        //numerator Tight
                                    //                                        if (WZ_Rej(m, BareElectron[k]) && BareElectron.size() < 2 && electron_Id && electron_Isol_Tight) {
                                    //                                            plotFill("4objFR_Ele_NumTight", BareElectron[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 62, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        //numerator Loose
                                    //                                        if (WZ_Rej(m, BareElectron[k]) && BareElectron.size() < 2 && electron_Id && electron_Isol_Loose) {
                                    //                                            plotFill("4objFR_Ele_NumLoose", BareElectron[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 72, BareMuon[i], BareMuon[j], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                    }//end of fake estimate //tau Pt > 0
                                }
                            }
                        }
                    }
                }
                //##############################################################################
                // BEST ZMM SELECTION
                //##############################################################################

                if (BareMuon.size() > 1 && Trigger) {
                    float Refer_Mass = 1000;
                    float Mass_LeadingZ;
                    int p, q;
                    bool Selected_Lead_Z = false;
                    for (int i = 0; i < BareMuon.size(); i++) {
                        for (int j = i + 1; j < BareMuon.size(); j++) {

                            float Zboson_Mass = InvarMass_2(BareMuon[i], BareMuon[j]);
                            float Zboson_Mass_diff = TMath::Abs(Zboson_Mass - nominalM_Z);


                            bool mu_Iso_1 = Iso_Mu_dBeta(BareMuon[i]) < loose_Iso_Mu;
                            bool mu_Iso_2 = Iso_Mu_dBeta(BareMuon[j]) < loose_Iso_Mu;
                            bool mu_Id_1 = Id_Mu_Loose(BareMuon[i]);
                            bool mu_Id_2 = Id_Mu_Loose(BareMuon[j]);
                            if (BareMuon[i].pt < BareMuon[j].pt) swap(BareMuon[i], BareMuon[j]);
                            bool first_l_HighPt = BareMuon[i].pt > 20;
                            bool Z_Charge = BareMuon[i].charge * BareMuon[j].charge < 0;
                            bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                            bool bjet_num = num_Bjet < 1;
                            if (bjet_num && first_l_HighPt && Z_Charge && Leading_Z_Mass && mu_Iso_1 && mu_Iso_2 && mu_Id_1 && mu_Id_2) {
                                if (Zboson_Mass_diff < Refer_Mass) {
                                    Refer_Mass = Zboson_Mass_diff;
                                    p = i;
                                    q = j;
                                    Mass_LeadingZ = Zboson_Mass;
                                    Selected_Lead_Z = true;

                                }
                            }
                        }
                    }
                    if (Selected_Lead_Z) {

                        //##############################################################################
                        // MMMT
                        //##############################################################################
                        Fill_Once_ff = true;
                        Fill_Once_fp = true;
                        Fill_Once_pf = true;

                        if (BareTau.size() > 0 && BareMuon.size() > 2) {
                            for (int k = 0; k < BareMuon.size(); k++) {
                                for (int l = 0; l < BareTau.size(); l++) {

                                    float Higgs_Mass = InvarMass_2(BareMuon[k], BareTau[l]);

                                    bool Overlap_Dz = OverLapWithTaus(BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    bool Muon_Isol_Tight = Iso_Mu_dBeta(BareMuon[k]) < tight_Iso_Mu;
                                    bool Muon_Isol_Loose = Iso_Mu_dBeta(BareMuon[k]) < loose_Iso_Lepton;
                                    bool Muon_Id = Id_Mu_Tight(BareMuon[k]);
                                    bool H_Charge = BareTau[l].charge * BareMuon[k].charge < 0;
                                    bool tau_Iso = getTauIsolation("mmmt", BareTau[l]);
                                    bool tau_Mu_Veto = getMuRejection("mmmt", BareTau[l]);
                                    bool No_extra_lepton = electron_.size() == 0;
                                    bool SumPtCut = getSumPtCut("mmmt", BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    Cor_eff = getCorrFactor("mmmt", status_sample.c_str(), BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);

                                    bool preSelection = Overlap_Dz && No_extra_lepton;
                                    bool FR_Application = SumPtCut && BareTau[l].pt > 20 && H_Charge && tau_Mu_Veto;
                                    bool FR_Estimation_tau0 = BareTau[l].pt > 0 && !H_Charge && tau_Mu_Veto;
                                    bool tauPtCheck = ((mc11 || mc12) ? BareTau[l].pt > 10 : BareTau[l].pt > 5); // This is to have less candidate in ZZ4L events for SVMass 
                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************

                                    if (preSelection && tauPtCheck) {
                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 93, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    }

                                    //                                    if (preSelection && FR_Application) {
                                    //
                                    //                                        if (Fill_Once_ff && !tau_Iso && (!Muon_Isol_Tight || !Muon_Id) && (mu_.size() - mu_partTight.size() == 2)) {
                                    //                                            Fill_Once_ff = false;
                                    //                                            plotFill("FakeRate_MMMT_apply_ff", BareTau[l].pt, BareMuon[k].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 13, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && !tau_Iso && Muon_Isol_Tight && Muon_Id && (mu_.size() == 3)) {
                                    //                                            Fill_Once_fp = false;
                                    //                                            plotFill("FakeRate_MMMT_apply_fp", BareTau[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 23, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_pf && tau_Iso && (!Muon_Isol_Tight || !Muon_Id) && (mu_.size() - mu_partTight.size() == 2)) {
                                    //                                            Fill_Once_pf = false;
                                    //                                            plotFill("FakeRate_MMMT_apply_pf", BareMuon[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 33, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_mmmt_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 43, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    //                                    }//end of fake apply

                                    //*********************************************************************************
                                    //***************************Fake Rate Estimation ***************************
                                    //*********************************************************************************
                                    //Fake Rate Estimation tau Pt > 0
                                    //                                    if (preSelection && FR_Estimation_tau0) {
                                    //
                                    //                                        //Denumerator
                                    //                                        if (WZ_Rej(m, BareMuon[k]) && BareMuon.size() < 4) {
                                    //                                            plotFill("4objFR_Mu_Denum", BareMuon[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 53, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        //Numerator
                                    //                                        if (WZ_Rej(m, BareMuon[k]) && BareMuon.size() < 4 && Muon_Id && Muon_Isol_Tight) {
                                    //                                            plotFill("4objFR_Mu_NumTight", BareMuon[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 63, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        //Numerator
                                    //                                        if (WZ_Rej(m, BareMuon[k]) && BareMuon.size() < 4 && Muon_Id && Muon_Isol_Loose) {
                                    //                                            plotFill("4objFR_Mu_NumLoose", BareMuon[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 73, BareMuon[p], BareMuon[q], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                    }//tau Pt > 0
                                }
                            }
                        }
                        //##############################################################################
                        // MMME
                        //##############################################################################
                        Fill_Once_ff = true;
                        Fill_Once_fp = true;
                        Fill_Once_pf = true;

                        if (BareElectron.size() > 0 && BareMuon.size() > 2) {

                            for (int k = 0; k < BareMuon.size(); k++) {
                                for (int l = 0; l < BareElectron.size(); l++) {

                                    float Higgs_Mass = InvarMass_2(BareMuon[k], BareElectron[l]);

                                    bool Overlap_Dz = OverLapWithTaus(BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);
                                    bool muon_Isol_Loose = Iso_Mu_dBeta(BareMuon[k]) < loose_Iso_Lepton;
                                    bool electron_Isol_Loose = Iso_Ele_dBeta(BareElectron[l]) < loose_Iso_Lepton;
                                    bool muon_Id = Id_Mu_Loose(BareMuon[k]);
                                    bool electron_Id = EleMVANonTrigId_Loose(BareElectron[l]);
                                    bool H_Charge = BareElectron[l].charge * BareMuon[k].charge < 0;
                                    bool SumPtCut = getSumPtCut("mmme", BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);
                                    Cor_eff = getCorrFactor("mmme", status_sample.c_str(), BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);

                                    bool preSelection = Overlap_Dz;
                                    bool FR_Application = SumPtCut && H_Charge;
                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************

                                    if (preSelection) {
                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 94, BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);
                                    }
                                    //
                                    //                                    if (preSelection && FR_Application) {
                                    //
                                    //
                                    //                                        //Fake Rate Application
                                    //                                        if (Fill_Once_ff && (!electron_Isol_Loose || !electron_Id) && (!muon_Isol_Loose || !muon_Id) && (mu_.size() == 2) && (electron_.size() == 0)) {
                                    //                                            Fill_Once_ff = false;
                                    //                                            plotFill("FakeRate_MMME_apply_ff", BareElectron[l].pt, BareMuon[k].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 14, BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && (!electron_Isol_Loose || !electron_Id) && (muon_Isol_Loose && muon_Id) && mu_.size() == 3 && (electron_.size() == 0)) {
                                    //                                            Fill_Once_fp = false;
                                    //                                            plotFill("FakeRate_MMME_apply_fp", BareElectron[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 24, BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_pf && (electron_Isol_Loose && electron_Id) && (!muon_Isol_Loose || !muon_Id) && (mu_.size() == 2) && electron_.size() == 1) {
                                    //                                            Fill_Once_pf = false;
                                    //                                            plotFill("FakeRate_MMME_apply_pf", BareMuon[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 34, BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_mmme_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 44, BareMuon[p], BareMuon[q], BareMuon[k], BareElectron[l]);
                                    //                                    }//end of fake apply
                                }
                            }
                        }
                    } //finosh good leading muon
                }// Finish muon trg and 2 good muon



            }// finish Only Muon
            //##############################################################################
            //####################### Double Electron
            //##############################################################################
            if (is_ele || is_tot) {
                //##############################################################################
                // EETT
                //##############################################################################
                bool Fill_Once_ff = true;
                bool Fill_Once_fp = true;
                bool Fill_Once_pf = true;
                bool fill_once_Estimate = true;

                // ABCD eett
                if (BareTau.size() > 1 && BareElectron.size() > 1 && Trigger) {
                    for (int i = 0; i < BareElectron.size(); i++) {
                        for (int j = i + 1; j < BareElectron.size(); j++) {
                            for (int k = 0; k < BareTau.size(); k++) {
                                for (int l = k + 1; l < BareTau.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(BareElectron[i], BareElectron[j]);
                                    float Higgs_Mass = InvarMass_2(BareTau[k], BareTau[l]);

                                    bool first_l_HighPt = BareElectron[i].pt > HighPt_Lep;
                                    bool Z_Charge = BareElectron[i].charge * BareElectron[j].charge < 0;
                                    bool ele_Iso_1 = Iso_Ele_dBeta(BareElectron[i]) < loose_Iso_Ele;
                                    bool ele_Iso_2 = Iso_Ele_dBeta(BareElectron[j]) < loose_Iso_Ele;
                                    bool ele_Id_1 = EleMVANonTrigId_Loose(BareElectron[i]);
                                    bool ele_Id_2 = EleMVANonTrigId_Loose(BareElectron[j]);
                                    bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);
                                    bool mu_Rejection = getMuRejection("eett", BareTau[k]) && getMuRejection("eett", BareTau[l]);
                                    bool ele_Rejection = getEleRejection("eett", BareTau[k]) && getMuRejection("eett", BareTau[l]);
                                    bool tau_Iso_1 = getTauIsolation("eett", BareTau[k]);
                                    bool tau_Iso_2 = getTauIsolation("eett", BareTau[l]);
                                    bool H_Charge = BareTau[k].charge * BareTau[l].charge < 0;
                                    bool bjet_num = num_Bjet < 1;
                                    bool No_extra_lepton = electron_.size() == 2 && mu_.size() == 0;
                                    bool SumPtCut = getSumPtCut("eett", BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);
                                    Cor_eff = getCorrFactor("eett", status_sample.c_str(), BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);

                                    bool preSelection = No_extra_lepton && Overlap_Dz && bjet_num && first_l_HighPt && Z_Charge && ele_Iso_1 && ele_Iso_2 && ele_Id_1 && ele_Id_2 && Leading_Z_Mass;
                                    bool FR_Application = SumPtCut && BareTau[k].pt > 20 && BareTau[l].pt > 20 && H_Charge && mu_Rejection && ele_Rejection;
                                    bool FR_Estimation = fill_once_Estimate && !H_Charge && BareTau[k].pt > 10 && BareTau[l].pt > 10 && mu_Rejection && ele_Rejection;

                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************
                                    if (preSelection && BareTau[k].pt > 10 && BareTau[l].pt > 10) {
                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 95, BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);
                                    }
                                    //                                    if (preSelection && FR_Application) {
                                    //
                                    //                                        if (Fill_Once_ff && !tau_Iso_1 && !tau_Iso_2) {
                                    //                                            plotFill("FakeRate_EETT_apply_ff", BareTau[k].pt, BareTau[l].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            Fill_Once_ff = false;
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 15, BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && !tau_Iso_1 && tau_Iso_2) {
                                    //                                            plotFill("FakeRate_EETT_apply_fp", BareTau[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            Fill_Once_fp = false;
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 25, BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_pf && tau_Iso_1 && !tau_Iso_2) {
                                    //                                            plotFill("FakeRate_EETT_apply_pf", BareTau[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            Fill_Once_pf = false;
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 35, BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_eett_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 45, BareElectron[i], BareElectron[j], BareTau[k], BareTau[l]);
                                    //                                    }
                                    //*********************************************************************************
                                    //***************************Fake Rate Estimation ***************************
                                    //*********************************************************************************
                                    //                                    if (preSelection && FR_Estimation) {
                                    //
                                    //                                        //Denumerator
                                    //                                        plotFill("4objFR_Tau_Denum", BareTau[k].pt, 200, 0, 200);
                                    //                                        plotFill("4objFR_Tau_Denum", BareTau[l].pt, 200, 0, 200);
                                    //
                                    //                                        //numerator Medium
                                    //                                        if (tau_Iso_1) {
                                    //                                            plotFill("4objFR_Tau_NumMedium", BareTau[k].pt, 200, 0, 200);
                                    //                                        }
                                    //                                        //numerator Medium
                                    //                                        if (tau_Iso_2) {
                                    //                                            plotFill("4objFR_Tau_NumMedium", BareTau[l].pt, 200, 0, 200);
                                    //                                        }
                                    //
                                    //                                        fill_once_Estimate = false;
                                    //                                    }//end of fake rate estimation
                                }
                            }
                        }
                    }
                }
                //##############################################################################
                // EEMT
                //##############################################################################
                Fill_Once_ff = true;
                Fill_Once_fp = true;
                Fill_Once_pf = true;

                if (BareTau.size() > 0 && BareMuon.size() > 0 && BareElectron.size() > 1 && Trigger) {
                    for (int i = 0; i < BareElectron.size(); i++) {
                        for (int j = i + 1; j < BareElectron.size(); j++) {
                            for (int k = 0; k < BareMuon.size(); k++) {
                                for (int l = 0; l < BareTau.size(); l++) {

                                    float Zboson_Mass = InvarMass_2(BareElectron[i], BareElectron[j]);
                                    float Higgs_Mass = InvarMass_2(BareMuon[k], BareTau[l]);

                                    bool first_l_HighPt = BareElectron[i].pt > HighPt_Lep;
                                    bool Z_Charge = BareElectron[i].charge * BareElectron[j].charge < 0;
                                    bool ele_Iso_1 = Iso_Ele_dBeta(BareElectron[i]) < loose_Iso_Ele;
                                    bool ele_Iso_2 = Iso_Ele_dBeta(BareElectron[j]) < loose_Iso_Ele;
                                    bool ele_Id_1 = EleMVANonTrigId_Loose(BareElectron[i]);
                                    bool ele_Id_2 = EleMVANonTrigId_Loose(BareElectron[j]);
                                    bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                                    bool Overlap_Dz = OverLapWithTaus(BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    bool Muon_Isol_Tight = Iso_Mu_dBeta(BareMuon[k]) < tight_Iso_Mu;
                                    bool Muon_Isol_Loose = Iso_Mu_dBeta(BareMuon[k]) < loose_Iso_Mu;
                                    bool Muon_Id = Id_Mu_Tight(BareMuon[k]);
                                    bool H_Charge = BareTau[l].charge * BareMuon[k].charge < 0;
                                    bool tau_Iso = getTauIsolation("eemt", BareTau[l]);
                                    bool tau_Mu_Veto = getMuRejection("eemt", BareTau[l]);
                                    bool No_extra_lepton = electron_.size() == 2;
                                    bool bjet_num = num_Bjet < 1;
                                    bool SumPtCut = getSumPtCut("eemt", BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    Cor_eff = getCorrFactor("eemt", status_sample.c_str(), BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);

                                    bool preSelection = bjet_num && No_extra_lepton && Overlap_Dz && ele_Iso_1 && ele_Iso_2 && ele_Id_1 && ele_Id_2 && first_l_HighPt && Z_Charge && Leading_Z_Mass;
                                    bool FR_Application = SumPtCut && BareTau[l].pt > 20 && H_Charge && tau_Mu_Veto;
                                    bool FR_Estimation_tau0 = BareTau[l].pt > 0 && !H_Charge && tau_Mu_Veto;
                                    bool tauPtCheck = ((mc11 || mc12) ? BareTau[l].pt > 10 : BareTau[l].pt > 5); // This is to have less candidate in ZZ4L events for SVMass 
                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************
                                    if (preSelection && tauPtCheck) {
                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 96, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    }
                                    //                                    if (preSelection && FR_Application) {
                                    //
                                    //                                        if (Fill_Once_ff && !tau_Iso && (!Muon_Isol_Tight || !Muon_Id) && ((mu_.size() - mu_partTight.size() == 0))) {
                                    //                                            Fill_Once_ff = false;
                                    //                                            plotFill("FakeRate_EEMT_apply_ff", BareTau[l].pt, BareMuon[k].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 16, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && !tau_Iso && (Muon_Isol_Tight && Muon_Id) && mu_.size() == 1) {
                                    //                                            Fill_Once_fp = false;
                                    //                                            plotFill("FakeRate_EEMT_apply_fp", BareTau[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 26, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_pf && tau_Iso && (!Muon_Isol_Tight || !Muon_Id) && ((mu_.size() - mu_partTight.size() == 0))) {
                                    //                                            Fill_Once_pf = false;
                                    //                                            plotFill("FakeRate_EEMT_apply_pf", BareMuon[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 36, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_eemt_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 46, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    //                                    }//end of fake apply
                                    //*********************************************************************************
                                    //***************************Fake Rate Estimation ***************************
                                    //*********************************************************************************
                                    //                                    if (preSelection && FR_Estimation_tau0) {
                                    //                                        //Fake rate tau Pt > 0 GeV
                                    //                                        //Denumerator
                                    //                                        if (WZ_Rej(m, BareMuon[k]) && BareMuon.size() < 2) {
                                    //                                            plotFill("4objFR_Mu_Denum", BareMuon[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 56, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        //Numerator Tight
                                    //                                        if (WZ_Rej(m, BareMuon[k]) && BareMuon.size() < 2 && Muon_Id && Muon_Isol_Tight) {
                                    //                                            plotFill("4objFR_Mu_NumTight", BareMuon[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 66, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                        //Numerator Loose
                                    //                                        if (WZ_Rej(m, BareMuon[k]) && BareMuon.size() < 2 && Muon_Id && Muon_Isol_Loose) {
                                    //                                            plotFill("4objFR_Mu_NumLoose", BareMuon[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 76, BareElectron[i], BareElectron[j], BareMuon[k], BareTau[l]);
                                    //                                        }
                                    //                                    }// tau pt > 0
                                    //*****
                                }
                            }
                        }
                    }
                }
                //##############################################################################
                // BEST ZEE SELECTION
                //##############################################################################

                if (BareElectron.size() > 1 && Trigger) {
                    float Refer_Mass = 1000;
                    float Mass_LeadingZ;
                    int p, q;
                    bool Selected_Lead_Z = false;
                    for (int i = 0; i < BareElectron.size(); i++) {
                        for (int j = i + 1; j < BareElectron.size(); j++) {

                            float Zboson_Mass = InvarMass_2(BareElectron[i], BareElectron[j]);
                            float Zboson_Mass_diff = TMath::Abs(Zboson_Mass - nominalM_Z);

                            bool ele_Iso_1 = Iso_Ele_dBeta(BareElectron[i]) < loose_Iso_Ele;
                            bool ele_Iso_2 = Iso_Ele_dBeta(BareElectron[j]) < loose_Iso_Ele;
                            bool ele_Id_1 = EleMVANonTrigId_Loose(BareElectron[i]);
                            bool ele_Id_2 = EleMVANonTrigId_Loose(BareElectron[j]);
                            if (BareElectron[i].pt < BareElectron[j].pt) swap(BareElectron[i], BareElectron[j]);
                            bool first_l_HighPt = BareElectron[i].pt > 20 || BareElectron[j].pt > 20;
                            bool Z_Charge = BareElectron[i].charge * BareElectron[j].charge < 0;
                            bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                            bool bjet_num = num_Bjet < 1;
                            if (bjet_num && first_l_HighPt && Z_Charge && Leading_Z_Mass && ele_Iso_1 && ele_Iso_2 && ele_Id_1 && ele_Id_2) {
                                if (Zboson_Mass_diff < Refer_Mass) {
                                    Refer_Mass = Zboson_Mass_diff;
                                    p = i;
                                    q = j;
                                    Mass_LeadingZ = Zboson_Mass;
                                    Selected_Lead_Z = true;
                                }
                            }
                        }
                    }

                    if (Selected_Lead_Z) {

                        //##############################################################################
                        // EEET
                        //##############################################################################
                        Fill_Once_ff = true;
                        Fill_Once_fp = true;
                        Fill_Once_pf = true;

                        if (BareTau.size() > 0 && BareElectron.size() > 2) {

                            for (int k = 0; k < BareElectron.size(); k++) {
                                for (int l = 0; l < BareTau.size(); l++) {

                                    float Higgs_Mass = InvarMass_2(BareElectron[k], BareTau[l]);

                                    bool Overlap_Dz = OverLapWithTaus(BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    bool electron_Isol_Tight = Iso_Ele_dBeta(BareElectron[k]) < tight_Iso_Ele;
                                    bool electron_Isol_Loose = Iso_Ele_dBeta(BareElectron[k]) < loose_Iso_Lepton;
                                    bool electron_Id = EleMVANonTrigId_Tight(BareElectron[k]);
                                    bool tau_Iso = getTauIsolation("eeet", BareTau[l]);
                                    bool H_Charge = BareTau[l].charge * BareElectron[k].charge < 0;
                                    bool ele_Rejection = getEleRejection("eeet", BareTau[l]);
                                    bool ele_numHit = BareElectron[k].numHitEleInner < 1;
                                    bool No_extra_lepton = mu_.size() == 0;
                                    bool SumPtCut = getSumPtCut("eeet", BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    Cor_eff = getCorrFactor("eeet", status_sample.c_str(), BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);

                                    bool preSelection = Overlap_Dz && No_extra_lepton && ele_numHit;
                                    bool FR_Application = SumPtCut && BareTau[l].pt > 20 && H_Charge && ele_Rejection;
                                    bool FR_Estimation_tau0 = BareTau[l].pt > 0 && !H_Charge && ele_Rejection;
                                    bool tauPtCheck = ((mc11 || mc12) ? BareTau[l].pt > 10 : BareTau[l].pt > 5); // This is to have less candidate in ZZ4L events for SVMass 
                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************
                                    if (preSelection && tauPtCheck) {
                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 97, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    }
                                    //                                    if (preSelection && FR_Application) {
                                    //
                                    //                                        if (Fill_Once_ff && !tau_Iso && (!electron_Isol_Tight || !electron_Id) && (electron_.size() - ele_partTight.size() == 2)) {
                                    //                                            Fill_Once_ff = false;
                                    //                                            plotFill("FakeRate_EEET_apply_ff", BareTau[l].pt, BareElectron[k].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 17, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && !tau_Iso && (electron_Isol_Tight && electron_Id) && electron_.size() == 3) {
                                    //                                            Fill_Once_fp = false;
                                    //                                            plotFill("FakeRate_EEET_apply_fp", BareTau[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 27, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_pf && tau_Iso && (!electron_Isol_Tight || !electron_Id) && (electron_.size() - ele_partTight.size() == 2)) {
                                    //                                            Fill_Once_pf = false;
                                    //                                            plotFill("FakeRate_EEET_apply_pf", BareElectron[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 37, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_eeet_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 47, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    //                                    }//end of fake apply

                                    //*********************************************************************************
                                    //***************************Fake Rate Estimation ***************************
                                    //*********************************************************************************
                                    //tau fake rate
                                    //                                    if (preSelection && FR_Estimation_tau0) {
                                    //
                                    //                                        //Denumerator
                                    //                                        if (WZ_Rej(m, BareElectron[k]) && BareElectron.size() < 4) {
                                    //                                            plotFill("4objFR_Ele_Denum", BareElectron[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 57, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        //Numerator Tight
                                    //                                        if (WZ_Rej(m, BareElectron[k]) && BareElectron.size() < 4 && electron_Id && electron_Isol_Tight) {
                                    //                                            plotFill("4objFR_Ele_NumTight", BareElectron[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 67, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                        //Numerator Loose
                                    //                                        if (WZ_Rej(m, BareElectron[k]) && BareElectron.size() < 4 && electron_Id && electron_Isol_Loose) {
                                    //                                            plotFill("4objFR_Ele_NumLoose", BareElectron[k].pt, 200, 0, 200);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 77, BareElectron[p], BareElectron[q], BareElectron[k], BareTau[l]);
                                    //                                        }
                                    //                                    }// tau Pt > 0

                                }
                            }
                        }
                        //##############################################################################
                        // EEEM
                        //##############################################################################
                        Fill_Once_ff = true;
                        Fill_Once_fp = true;
                        Fill_Once_pf = true;

                        if (BareMuon.size() > 0 && BareElectron.size() > 2) {


                            for (int k = 0; k < BareElectron.size(); k++) {
                                for (int l = 0; l < BareMuon.size(); l++) {

                                    float Higgs_Mass = InvarMass_2(BareMuon[l], BareElectron[k]);


                                    bool Overlap_Dz = OverLapWithTaus(BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);
                                    bool muon_Isol_Loose = Iso_Mu_dBeta(BareMuon[l]) < loose_Iso_Lepton;
                                    bool electron_Isol_Loose = Iso_Ele_dBeta(BareElectron[k]) < loose_Iso_Lepton;
                                    bool muon_Id = Id_Mu_Loose(BareMuon[l]);
                                    bool electron_Id = EleMVANonTrigId_Loose(BareElectron[k]);
                                    bool H_Charge = BareElectron[k].charge * BareMuon[l].charge < 0;
                                    bool SumPtCut = getSumPtCut("eeem", BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);
                                    Cor_eff = getCorrFactor("eeem", status_sample.c_str(), BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);

                                    bool preSelection = Overlap_Dz;
                                    bool FR_Application = SumPtCut && H_Charge;
                                    //*********************************************************************************
                                    //***************************Fake Rate Application ***************************
                                    //*********************************************************************************
                                    if (preSelection) {

                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 98, BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);
                                    }
                                    //                                    if (preSelection && FR_Application) {
                                    //
                                    //                                        //Fake Rate Application
                                    //                                        if (Fill_Once_ff && (!electron_Isol_Loose || !electron_Id) && (!muon_Isol_Loose || !muon_Id) && (electron_.size() == 2) && (mu_.size() == 0)) {
                                    //                                            Fill_Once_ff = false;
                                    //                                            plotFill("FakeRate_EEEM_apply_ff", BareElectron[k].pt, BareMuon[l].pt, 200, 0, 200, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 18, BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);
                                    //                                        }
                                    //                                        if (Fill_Once_fp && (!electron_Isol_Loose || !electron_Id) && (muon_Isol_Loose && muon_Id) && (electron_.size() == 2) && (mu_.size() == 1)) {
                                    //                                            Fill_Once_fp = false;
                                    //                                            plotFill("FakeRate_EEEM_apply_fp", BareElectron[k].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 28, BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);
                                    //                                        }
                                    //
                                    //                                        if (Fill_Once_pf && (electron_Isol_Loose && electron_Id) && (!muon_Isol_Loose || !muon_Id) && (electron_.size() == 3) && (mu_.size() == 0)) {
                                    //                                            Fill_Once_pf = false;
                                    //                                            plotFill("FakeRate_EEEM_apply_pf", BareMuon[l].pt, 200, 0, 200, PU_Weight * Cor_eff);
                                    //                                            fillTree(Run_Tree, m, PU_Weight, Cor_eff, 38, BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);
                                    //                                        }
                                    //                                        plotFill("Higgs_Mass_eeem_ABCD", Higgs_Mass, bin_size, 0, bin_max, PU_Weight * Cor_eff);
                                    //                                        fillTree(Run_Tree, m, PU_Weight, Cor_eff, 48, BareElectron[p], BareElectron[q], BareElectron[k], BareMuon[l]);
                                    //                                    }//end of fake apply
                                }
                            }
                        }//eeem
                    } //selected first Z
                }//more than 1 good electron
            }//only Electron



            //##############################################################################
            //##############################################################################
            //##############################################################################
            //##############################################################################
            //##############################################################################
            //##############################################################################
            //##############################################################################
            //##############################################################################
            //##############################################################################
            //##############################################################################
            // BEST ZMM SELECTION
            //##############################################################################

            if ((is_mu || is_tot) && BareMuon.size() > 1 && Trigger) {
                float Refer_Mass = 1000;
                float Mass_LeadingZ;
                int p, q;
                bool Selected_Lead_Z = false;
                for (int i = 0; i < BareMuon.size(); i++) {
                    for (int j = i + 1; j < BareMuon.size(); j++) {

                        float Zboson_Mass = InvarMass_2(BareMuon[i], BareMuon[j]);
                        float Zboson_Mass_diff = TMath::Abs(Zboson_Mass - nominalM_Z);

                        bool mu_Iso_1 = Iso_Mu_dBeta(BareMuon[i]) < loose_Iso_Mu;
                        bool mu_Iso_2 = Iso_Mu_dBeta(BareMuon[j]) < loose_Iso_Mu;
                        bool mu_Id_1 = Id_Mu_Loose(BareMuon[i]);
                        bool mu_Id_2 = Id_Mu_Loose(BareMuon[j]);
                        if (BareMuon[i].pt < BareMuon[j].pt) swap(BareMuon[i], BareMuon[j]);
                        bool first_l_HighPt = BareMuon[i].pt > 20;
                        bool Z_Charge = BareMuon[i].charge * BareMuon[j].charge < 0;
                        bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                        bool bjet_num = num_Bjet < 1;
                        if (bjet_num && first_l_HighPt && Z_Charge && Leading_Z_Mass && mu_Iso_1 && mu_Iso_2 && mu_Id_1 && mu_Id_2) {
                            if (Zboson_Mass_diff < Refer_Mass) {
                                Refer_Mass = Zboson_Mass_diff;
                                p = i;
                                q = j;
                                Mass_LeadingZ = Zboson_Mass;
                                Selected_Lead_Z = true;
                            }
                        }
                    }
                }
                if (Selected_Lead_Z) {

                    //  Di-Muon Mass Plots
                    Cor_eff = getCorrFactor("mm", status_sample.c_str(), BareMuon[p], BareMuon[q], BareMuon[p], BareMuon[q]);
                    plotFill("Zboson_Mass_MM_PUCor", Mass_LeadingZ, bin_size, 0., bin_max, PU_Weight * Cor_eff);
                    plotFill("Zboson_Mass_MM_PU", Mass_LeadingZ, bin_size, 0., bin_max, PU_Weight);
                    plotFill("Zboson_Mass_MM_Cor", Mass_LeadingZ, bin_size, 0., bin_max, Cor_eff);
                    plotFill("Zboson_Mass_MM_", Mass_LeadingZ, bin_size, 0., bin_max);

                    //##############################################################################
                    // MMM
                    //##############################################################################
                    if (BareMuon.size() == 3 && p < 3 && q < 3) {
                        for (int y = 0; y < BareMuon.size(); y++) {
                            if (OverLap(BareMuon[q], BareMuon[p], BareMuon[y])) {
                                fillTree(Run_Tree, m, PU_Weight, Cor_eff, 101, BareMuon[p], BareMuon[q], BareMuon[y], BareMuon[y]);
                                bool suppres_realLep = WZ_Rej(m, BareMuon[y]) && TMath::Abs(Mass_LeadingZ - nominalM_Z) < 10;
                                //                                //denum
                                //                                if (suppres_realLep) {
                                //                                    plotFill("3objFR_Mu_Denum", BareMuon[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 102, BareMuon[p], BareMuon[q], BareMuon[y], BareMuon[y]);
                                //                                }
                                //                                //num Loose
                                //                                if (suppres_realLep && Iso_Mu_dBeta(BareMuon[y]) < loose_Iso_Mu && Id_Mu_Loose(BareMuon[y])) {
                                //                                    plotFill("3objFR_Mu_NumLoose", BareMuon[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 103, BareMuon[p], BareMuon[q], BareMuon[y], BareMuon[y]);
                                //                                }
                                //                                //num Tight
                                //                                if (suppres_realLep && Iso_Mu_dBeta(BareMuon[y]) < tight_Iso_Mu && Id_Mu_Tight(BareMuon[y])) {
                                //                                    plotFill("3objFR_Mu_NumTight", BareMuon[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 104, BareMuon[p], BareMuon[q], BareMuon[y], BareMuon[y]);
                                //                                }
                            }
                        }
                    }
                    //##############################################################################
                    // MME
                    //##############################################################################
                    if (BareElectron.size() == 1) {
                        for (int y = 0; y < BareElectron.size(); y++) {
                            if (OverLap(BareMuon[q], BareMuon[p], BareElectron[y])) {
                                fillTree(Run_Tree, m, PU_Weight, Cor_eff, 105, BareMuon[p], BareMuon[q], BareElectron[y], BareElectron[y]);
                                bool suppres_realLep = WZ_Rej(m, BareElectron[y]) && TMath::Abs(Mass_LeadingZ - nominalM_Z) < 10;
                                //                                //denum
                                //                                if (suppres_realLep) {
                                //                                    plotFill("3objFR_Ele_Denum", BareElectron[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 106, BareMuon[p], BareMuon[q], BareElectron[y], BareElectron[y]);
                                //                                }
                                //                                //num loose
                                //                                if (suppres_realLep && Iso_Ele_dBeta(BareElectron[y]) < loose_Iso_Ele && EleMVANonTrigId_Loose(BareElectron[y])) {
                                //                                    plotFill("3objFR_Ele_NumLoose", BareElectron[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 107, BareMuon[p], BareMuon[q], BareElectron[y], BareElectron[y]);
                                //                                }
                                //                                //num tight
                                //                                if (suppres_realLep && Iso_Ele_dBeta(BareElectron[y]) < tight_Iso_Ele && EleMVANonTrigId_Tight(BareElectron[y])) {
                                //                                    plotFill("3objFR_Ele_NumTight", BareElectron[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 108, BareMuon[p], BareMuon[q], BareElectron[y], BareElectron[y]);
                                //                                }

                            }
                        }
                    }
                }//good Z
            }
            //##############################################################################
            // BEST ZEE SELECTION
            //##############################################################################
            if ((is_ele || is_tot) && BareElectron.size() > 1 && Trigger) {
                float Refer_Mass = 1000;
                float Mass_LeadingZ;
                int p, q;
                bool Selected_Lead_Z = false;
                for (int i = 0; i < BareElectron.size(); i++) {
                    for (int j = i + 1; j < BareElectron.size(); j++) {

                        float Zboson_Mass = InvarMass_2(BareElectron[i], BareElectron[j]);
                        float Zboson_Mass_diff = TMath::Abs(Zboson_Mass - nominalM_Z);

                        bool ele_Iso_1 = Iso_Ele_dBeta(BareElectron[i]) < loose_Iso_Ele;
                        bool ele_Iso_2 = Iso_Ele_dBeta(BareElectron[j]) < loose_Iso_Ele;
                        bool ele_Id_1 = EleMVANonTrigId_Loose(BareElectron[i]);
                        bool ele_Id_2 = EleMVANonTrigId_Loose(BareElectron[j]);
                        if (BareElectron[i].pt < BareElectron[j].pt) swap(BareElectron[i], BareElectron[j]);
                        bool first_l_HighPt = BareElectron[i].pt > 20;
                        bool Z_Charge = BareElectron[i].charge * BareElectron[j].charge < 0;
                        bool Leading_Z_Mass = Zboson_Mass > z_lowMass && Zboson_Mass < z_highMass;
                        bool bjet_num = num_Bjet < 1;
                        if (bjet_num && first_l_HighPt && Z_Charge && Leading_Z_Mass && ele_Iso_1 && ele_Iso_2 && ele_Id_1 && ele_Id_2) {
                            if (Zboson_Mass_diff < Refer_Mass) {
                                Refer_Mass = Zboson_Mass_diff;
                                p = i;
                                q = j;
                                Mass_LeadingZ = Zboson_Mass;
                                Selected_Lead_Z = true;
                            }
                        }
                    }
                }

                if (Selected_Lead_Z) {


                    //  Di-Muon Mass Plots
                    Cor_eff = getCorrFactor("ee", status_sample.c_str(), BareElectron[p], BareElectron[q], BareElectron[p], BareElectron[q]);
                    plotFill("Zboson_Mass_EE_PUCor", Mass_LeadingZ, bin_size, 0., bin_max, PU_Weight * Cor_eff);
                    plotFill("Zboson_Mass_EE_PU", Mass_LeadingZ, bin_size, 0., bin_max, PU_Weight);
                    plotFill("Zboson_Mass_EE_Cor", Mass_LeadingZ, bin_size, 0., bin_max, Cor_eff);
                    plotFill("Zboson_Mass_EE_", Mass_LeadingZ, bin_size, 0., bin_max);

                    //##############################################################################
                    // EEM
                    //##############################################################################
                    if (BareMuon.size() == 1) {
                        for (int y = 0; y < BareMuon.size(); y++) {
                            if (OverLap(BareElectron[q], BareElectron[p], BareMuon[y])) {
                                fillTree(Run_Tree, m, PU_Weight, Cor_eff, 101, BareElectron[p], BareElectron[q], BareMuon[y], BareMuon[y]);
                                bool suppres_realLep = WZ_Rej(m, BareMuon[y]) && TMath::Abs(Mass_LeadingZ - nominalM_Z) < 10;
                                //                                //denum
                                //                                if (suppres_realLep) {
                                //                                    plotFill("3objFR_Mu_Denum", BareMuon[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 102, BareElectron[p], BareElectron[q], BareMuon[y], BareMuon[y]);
                                //                                }
                                //                                //num loose
                                //                                if (suppres_realLep && Iso_Mu_dBeta(BareMuon[y]) < loose_Iso_Mu && Id_Mu_Loose(BareMuon[y])) {
                                //                                    plotFill("3objFR_Mu_NumLoose", BareMuon[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 103, BareElectron[p], BareElectron[q], BareMuon[y], BareMuon[y]);
                                //                                }
                                //                                //num Tight
                                //                                if (suppres_realLep && Iso_Mu_dBeta(BareMuon[y]) < tight_Iso_Mu && Id_Mu_Tight(BareMuon[y])) {
                                //                                    plotFill("3objFR_Mu_NumTight", BareMuon[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 104, BareElectron[p], BareElectron[q], BareMuon[y], BareMuon[y]);
                                //                                }
                            }
                        }
                    }
                    //##############################################################################
                    // EEE
                    //##############################################################################
                    if (BareElectron.size() == 3 && p < 3 && q < 3 && TMath::Abs(Mass_LeadingZ - nominalM_Z) < 10) {
                        for (int y = 0; y < BareElectron.size(); y++) {
                            if (WZ_Rej(m, BareElectron[y]) && OverLap(BareElectron[q], BareElectron[p], BareElectron[y])) {
                                fillTree(Run_Tree, m, PU_Weight, Cor_eff, 105, BareElectron[p], BareElectron[q], BareElectron[y], BareElectron[y]);
                                bool suppres_realLep = WZ_Rej(m, BareElectron[y]) && TMath::Abs(Mass_LeadingZ - nominalM_Z) < 10;
                                //                                num
                                //                                if (suppres_realLep) {
                                //                                    plotFill("3objFR_Ele_Denum", BareElectron[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 106, BareElectron[p], BareElectron[q], BareElectron[y], BareElectron[y]);
                                //                                }
                                //                                //denum loose
                                //                                if (suppres_realLep && Iso_Ele_dBeta(BareElectron[y]) < loose_Iso_Ele && EleMVANonTrigId_Loose(BareElectron[y])) {
                                //                                    plotFill("3objFR_Ele_NumLoose", BareElectron[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 107, BareElectron[p], BareElectron[q], BareElectron[y], BareElectron[y]);
                                //                                }
                                //                                //denum tight
                                //                                if (suppres_realLep && Iso_Ele_dBeta(BareElectron[y]) < tight_Iso_Ele && EleMVANonTrigId_Tight(BareElectron[y])) {
                                //                                    plotFill("3objFR_Ele_NumTight", BareElectron[y].pt, 200, 0, 200);
                                //                                    fillTree(Run_Tree, m, PU_Weight, Cor_eff, 108, BareElectron[p], BareElectron[q], BareElectron[y], BareElectron[y]);
                                //                                }
                            }
                        }
                    }
                }
            }

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

    for (; iMap2 != jMap2; ++iMap2)
        nplot2(iMap2->first)->Write();



    fout->Close();
    return 0;
}
