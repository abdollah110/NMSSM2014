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
#include "interface/zh_Trigger.h"
#include "interface/zh_Tree.h"
#include "interface/Leptons_IdIso.h"
#include "interface/zh_Functions.h"

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
    if (!(mc12 || mc11 || data12 || data11))
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

    string outname = is_data_mc + "_" + out;
    //PRINTING THE OUTPUT name
    cout << "\n\n\n OUTPUT NAME IS:    " << outname << endl;
    TFile *fout = TFile::Open(outname.c_str(), "RECREATE");

    //#################################################################################################
    //############## defining Tree Branches Filled via fillTree function                ###############
    //#################################################################################################
    TTree *Run_Tree = new TTree("TauCheck", "TauCheck");
    //    To force a memory-resident Tree
    Run_Tree->SetDirectory(0);


    //
    //
    //    //    run
    //    //    lumi
    //    //    evt
    //    //    npv
    //    //    npu
    //    //    rho
    //    //    mcweight
    //    //    puweight
    //    //    trigweight_1
    //    //    trigweight_2
    //    //    idweight_1
    //    //    idweight_2  // Empty
    //    //    isoweight_1  not filled
    //    //    isoweight_2  Empty
    //    //    effweight
    //    weight
    //    embeddedWeight
    //    //    mvis
    //    m_sv
    //    pt_sv
    //    eta_sv
    //    phi_sv
    //    m_sv_Up
    //    m_sv_Down
    //
    //    //    pt_1
    //    //    phi_1
    //    //    eta_1
    //    //    m_1
    //    //    q_1
    //    //    iso_1   NOT Now  For Mu Ele
    //    //    mva_1   NOT now   For Mu Ele
    //    //    passid_1  not needed
    //    //    passiso_1  not needed
    //    //    mt_1
    //
    //    //    pt_2
    //    //    phi_2
    //    //    eta_2
    //    //    m_2
    //    //    q_2
    //    //    iso_2
    //    //    mva_2
    //    //    passid_2   not needed
    //    //    passiso_2   not needed
    //    //    mt_2
    //
    //    //    byCombinedIsolationDeltaBetaCorrRaw3Hits_2
    //    //    againstElectronMVA3raw_2
    //    //    byIsolationMVA2raw_2
    //    //    againstMuonLoose2_2
    //    //    againstMuonMedium2_2
    //    //    againstMuonTight2_2
    //    //    met
    //    //    metphi
    //    //    mvamet
    //    //    mvametphi
    //    pzetavis
    //    pzetamiss
    //
    //    //    metcov00
    //    //    metcov01
    //    //    metcov10
    //    //    metcov11
    //    //    mvacov00
    //    //    mvacov01
    //    //    mvacov10
    //    //    mvacov11
    //    //    jpt_1
    //    //    jeta_1
    //    //    jphi_1
    //    jptraw_1
    //    jptunc_1
    //    jmva_1
    //    jlrm_1
    //    jctm_1
    //    //    jpass_1
    //    //    jpt_2
    //    //    jeta_2
    //    //    jphi_2
    //    jptraw_2
    //    jptunc_2
    //    jmva_2
    //    jlrm_2
    //    jctm_2
    //    //    jpass_2
    //    //    bpt
    //    //    beta
    //    //    bphi
    //    //    mjj
    //    //    jdeta
    //    njetingap
    //    mva
    //    //    jdphi
    //    //    dijetpt
    //    //    dijetphi
    //    hdijetphi
    //    visjeteta
    //    ptvis
    //    //    nbtag
    //    //    njets
    //    //    njetpt20
    //    //    mva_gf   noinfo
    //    //    mva_vbf   noinfo
    //


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
    Run_Tree->Branch("l2_tauIsoMVA2M", &l2_tauIsoMVA2M, "l2_tauIsoMVA2M/O");
    Run_Tree->Branch("l2_tauIsoMVA2T", &l2_tauIsoMVA2T, "l2_tauIsoMVA2T/O");
    Run_Tree->Branch("iso_2", &l2_tauIsoMVA2raw, "iso_2/F");
    Run_Tree->Branch("l2_tauRejMuL", &l2_tauRejMuL, "l2_tauRejMuL/O");
    Run_Tree->Branch("againstMuonLoose2_2", &l2_tauRejMu2L, "againstMuonLoose2_2/O");
    Run_Tree->Branch("l2_tauRejMuM", &l2_tauRejMuM, "l2_tauRejMuM/O");
    Run_Tree->Branch("againstMuonMedium2_2", &l2_tauRejMu2M, "againstMuonMedium2_2/O");
    Run_Tree->Branch("l2_tauRejMuT", &l2_tauRejMuT, "l2_tauRejMuT/O");
    Run_Tree->Branch("againstMuonTight2_2", &l2_tauRejMu2T, "againstMuonTight2_2/O");
    Run_Tree->Branch("l2_tauRejEleL", &l2_tauRejEleL, "l2_tauRejEleL/O");
    Run_Tree->Branch("l2_tauRejEleM", &l2_tauRejEleM, "l2_tauRejEleM/O");
    Run_Tree->Branch("againstElectronMVA3raw_2", &l2_tauRejEleMVA, "againstElectronMVA3raw_2/F");
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
    Run_Tree->Branch("bpt", &bpt, "bpt/F");
    Run_Tree->Branch("beta", &beta, "beta/F");
    Run_Tree->Branch("bphi", &bphi, "bphi/F");
    Run_Tree->Branch("bdiscriminant", &bdiscriminant, "bdiscriminant/F");
    Run_Tree->Branch("bpt_2", &bpt_2, "bpt_2/F");
    Run_Tree->Branch("beta_2", &beta_2, "beta_2/F");
    Run_Tree->Branch("bphi_2", &bphi_2, "bphi_2/F");
    Run_Tree->Branch("bdiscriminant_2", &bdiscriminant_2, "bdiscriminant_2/F");
    Run_Tree->Branch("mjj", &mjj, "mjj/F");
    Run_Tree->Branch("jdeta", &jdeta, "jdeta/F");
    Run_Tree->Branch("jdphi", &jdphi, "jdphi/F");
    Run_Tree->Branch("jetpt", &jetpt, "jetpt/F");
    Run_Tree->Branch("njets", &njets, "njets/I");
    Run_Tree->Branch("njetpt20", &njetpt20, "njetpt20/I");
    Run_Tree->Branch("nbtag", &nbtag, "nbtag/I");
    Run_Tree->Branch("mcdata", &mcdata, "mcdata/I");


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

            //*********************************************************************************************
            //****************************    Trigger      ************************************************
            //*********************************************************************************************
            bool Trigger;
            if (mc12) Trigger = Trg_MC_12(m);
            if (mc11) Trigger = Trg_MC_11(m);
            if (data12) Trigger = Trg_Data_12(m);
            if (data11) Trigger = Trg_Data_11(m);

            //#################################################################################################
            bool doMuTauAnalysis = true;
            bool doElTauAnalysis = true;
            //#################################################################################################
            //########################## MuTau Selection         ##############################################
            //#################################################################################################
            //#################################################################################################
            if (doMuTauAnalysis) {
                //##############################################################################
                // mutau
                //##############################################################################
                std::string FinalState = "mutau";
                int mutau = -1;
                plotFill("TotalEventsNumber", 0, 1, 0, 1);
                plotFill("mutau", ++mutau, 20, 0., 20.);
                if (Trigger) {
                    plotFill("mutau", ++mutau, 20, 0., 20.);

                    for (int i = 0; i < mu_.size(); i++) {
                        for (int k = 0; k < tau_.size(); k++) {

                            bool Mu_PtEta = mu_[i].pt > 20 && fabs(mu_[i].eta) < 2.1;
                            bool Mu_IdTight = Id_Mu_Tight(mu_[i]);
                            bool Mu_d0 = mu_[i].d0 < 0.045; //the impact parameter in the transverse plane
                            bool Mu_dZ = mu_[i].dZ_in < 0.2; //the impact parameter in the transverse plane
                            bool Mu_Iso = Iso_Mu_dBeta(mu_[i]) < 0.1;
                            bool MU_CUTS = Mu_PtEta && Mu_IdTight && Mu_d0 && Mu_dZ && Mu_Iso;

                            bool Tau_PtEta = tau_[k].pt > 20 && fabs(tau_[k].eta) < 2.3;
                            bool Tau_DMF = tau_[k].discriminationByDecayModeFinding;
                            bool Tau_Isolation = tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits < 1.5;
                            bool Tau_antiEl = tau_[k].discriminationByElectronLoose;
                            bool Tau_antiMu = tau_[k].discriminationByMuonTight2;
                            bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;

                            bool MuTau_Charge = mu_[i].charge * tau_[k].charge < 0;
                            bool MuTau_dR = deltaR(mu_[i], tau_[k]) > 0.5;


                            bool Veto_MM = Multi_Lepton_Veto("MM", m);
                            bool Veto_MMM = Multi_Lepton_Veto("MMM", m);
                            bool Veto_MME = Multi_Lepton_Veto("MME", m);


                            if (Mu_PtEta && Tau_PtEta && MuTau_dR && Veto_MM && Veto_MMM && Veto_MME) {
                                fillTree(1, Run_Tree, m, is_data_mc.c_str(), FinalState, mu_[i], tau_[k]);
                            }
                            if (MU_CUTS && TAU_CUTS && MuTau_Charge && MuTau_dR && Veto_MM && Veto_MMM && Veto_MME) {
                                plotFill("mutau", ++mutau, 20, 0., 20.);
                                fillTree(2, Run_Tree, m, is_data_mc.c_str(), FinalState, mu_[i], tau_[k]);
                                break;
                                //                                cout << "tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits= " << tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits << "\t";

                            }
                        }
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
                int eltau = -1;
                plotFill("eltau", ++eltau, 20, 0., 20.);
                if (Trigger) {
                    plotFill("eltau", ++eltau, 20, 0., 20.);

                    for (int i = 0; i < electron_.size(); i++) {
                        for (int k = 0; k < tau_.size(); k++) {

                            bool El_PtEta = electron_[i].pt > 24 && fabs(electron_[i].eta) < 2.1;
                            bool El_IdTight = EleMVANonTrigId_Tight(electron_[i]);
                            bool El_Iso = Iso_Ele_dBeta(electron_[i]) < 0.1;
                            bool EL_CUTS = El_PtEta && El_IdTight && El_Iso;

                            bool Tau_PtEta = tau_[k].pt > 20 && fabs(tau_[k].eta) < 2.3;
                            bool Tau_DMF = tau_[k].discriminationByDecayModeFinding;
                            bool Tau_Isolation = tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits < 1.5;
                            bool Tau_antiEl = tau_[k].discriminationByElectronMVA5Medium;
                            bool Tau_antiMu = tau_[k].discriminationByMuonLoose2;
                            bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;

                            bool ElTau_Charge = electron_[i].charge * tau_[k].charge < 0;
                            bool ElTau_dR = deltaR(electron_[i], tau_[k]) > 0.5;

                            bool Veto_EE = Multi_Lepton_Veto("EE", m);
                            bool Veto_EEM = Multi_Lepton_Veto("EEM", m);
                            bool Veto_EEE = Multi_Lepton_Veto("EEE", m);

                            if (El_PtEta && Tau_PtEta && ElTau_dR && Veto_EE && Veto_EEM && Veto_EEE) {
                                fillTree(3, Run_Tree, m, is_data_mc.c_str(), FinalState, electron_[i], tau_[k]);
                            }
                            if (EL_CUTS && TAU_CUTS && ElTau_Charge && ElTau_dR && Veto_EE && Veto_EEM && Veto_EEE) {
                                plotFill("eltau", ++eltau, 20, 0., 20.);
                                fillTree(4, Run_Tree, m, is_data_mc.c_str(), FinalState, electron_[i], tau_[k]);
                                break;
                                //                                cout << "----------------------------- tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits=   " << tau_[k].byRawCombinedIsolationDeltaBetaCorr3Hits << "   ___   "<< Event <<endl;

                            }


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

    //    map<string, TH2F*>::const_iterator iMap2 = myMap2->begin();
    //    map<string, TH2F*>::const_iterator jMap2 = myMap2->end();
    //
    //    for (; iMap2 != jMap2; ++iMap2)
    //        nplot2(iMap2->first)->Write();



    fout->Close();
    return 0;
}