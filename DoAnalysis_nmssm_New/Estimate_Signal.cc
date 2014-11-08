#include "interface/mssm_Tree.h"
#include "interface/TTEmbedCor.h"
//#include "../interface/zh_Tree.h"

int main(int argc, char** argv) {

    std::string out = *(argv + 1);
    std::string input = *(argv + 2);

    //PRINTING THE OUTPUT name
    cout << "\n\n\n OUTPUT NAME IS:    " << out << endl;
    TFile *fout = TFile::Open(out.c_str(), "RECREATE");

    using namespace std;

    myMap1 = new std::map<std::string, TH1F*>();
    myMap2 = new map<string, TH2F*>();
    //
    TFile *f_Double = new TFile(input.c_str());
    TTree *Run_Tree = (TTree*) f_Double->Get("InfoTree");
    Run_Tree->AddFriend("Mass_tree");

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(5);

    Run_Tree->SetBranchAddress("Channel", &Channel);
    Run_Tree->SetBranchAddress("run", &Run);
    Run_Tree->SetBranchAddress("lumi", &Lumi);
    Run_Tree->SetBranchAddress("evt", &Event);

    Run_Tree->SetBranchAddress("mvamet", &mvamet);
    Run_Tree->SetBranchAddress("mvametphi", &mvametphi);
    Run_Tree->SetBranchAddress("mvametNoRecoil", &mvametNoRecoil);
    Run_Tree->SetBranchAddress("mvametphiNoRecoil", &mvametphiNoRecoil);
    Run_Tree->SetBranchAddress("metcov00", &metcov00);
    Run_Tree->SetBranchAddress("metcov01", &metcov01);
    Run_Tree->SetBranchAddress("metcov10", &metcov10);
    Run_Tree->SetBranchAddress("metcov11", &metcov11);
    Run_Tree->SetBranchAddress("met", &met);
    Run_Tree->SetBranchAddress("metphi", &metphi);
    Run_Tree->SetBranchAddress("mvacov00", &mvacov00);
    Run_Tree->SetBranchAddress("mvacov01", &mvacov01);
    Run_Tree->SetBranchAddress("mvacov10", &mvacov10);
    Run_Tree->SetBranchAddress("mvacov11", &mvacov11);

    Run_Tree->SetBranchAddress("m_1", &l1M);
    Run_Tree->SetBranchAddress("E_1", &l1E);
    Run_Tree->SetBranchAddress("px_1", &l1Px);
    Run_Tree->SetBranchAddress("py_1", &l1Py);
    Run_Tree->SetBranchAddress("pz_1", &l1Pz);
    Run_Tree->SetBranchAddress("pt_1", &l1Pt);
    Run_Tree->SetBranchAddress("eta_1", &l1Eta);
    Run_Tree->SetBranchAddress("phi_1", &l1Phi);
    Run_Tree->SetBranchAddress("q_1", &l1Charge);
    Run_Tree->SetBranchAddress("l1_CloseJetPt", &l1_CloseJetPt);
    Run_Tree->SetBranchAddress("l1_CloseJetEta", &l1_CloseJetEta);
    Run_Tree->SetBranchAddress("l1_CloseJetPhi", &l1_CloseJetPhi);
    Run_Tree->SetBranchAddress("l1_muId_Loose", &l1_muId_Loose);
    Run_Tree->SetBranchAddress("l1_muId_Tight", &l1_muId_Tight);
    Run_Tree->SetBranchAddress("l1_eleId_Loose", &l1_eleId_Loose);
    Run_Tree->SetBranchAddress("l1_eleId_Tight", &l1_eleId_Tight);
    Run_Tree->SetBranchAddress("l1_muIso", &l1_muIso);
    Run_Tree->SetBranchAddress("l1_eleIso", &l1_eleIso);
    Run_Tree->SetBranchAddress("l1_eleMVANonTrg", &l1_eleMVANonTrg);
    Run_Tree->SetBranchAddress("l1_eleNumHit", &l1_eleNumHit);

    Run_Tree->SetBranchAddress("m_2", &l2M);
    Run_Tree->SetBranchAddress("e_2", &l2E);
    Run_Tree->SetBranchAddress("px_2", &l2Px);
    Run_Tree->SetBranchAddress("py_2", &l2Py);
    Run_Tree->SetBranchAddress("pz_2", &l2Pz);
    Run_Tree->SetBranchAddress("pt_2", &l2Pt);
    Run_Tree->SetBranchAddress("eta_2", &l2Eta);
    Run_Tree->SetBranchAddress("phi_2", &l2Phi);
    Run_Tree->SetBranchAddress("q_2", &l2Charge);
    Run_Tree->SetBranchAddress("l2_CloseJetPt", &l2_CloseJetPt);
    Run_Tree->SetBranchAddress("l2_CloseJetEta", &l2_CloseJetEta);
    Run_Tree->SetBranchAddress("l2_CloseJetPhi", &l2_CloseJetPhi);
    Run_Tree->SetBranchAddress("l2_tauIsoVL", &l2_tauIsoVL);
    Run_Tree->SetBranchAddress("byCombinedIsolationDeltaBetaCorrRaw3Hits_2", &byCombinedIsolationDeltaBetaCorrRaw3Hits_2); //NNNEW
    Run_Tree->SetBranchAddress("l2_tauIso3HitL", &l2_tauIso3HitL);
    Run_Tree->SetBranchAddress("l2_tauIso3HitM", &l2_tauIso3HitM);
    Run_Tree->SetBranchAddress("l2_tauIso3HitT", &l2_tauIso3HitT);
    Run_Tree->SetBranchAddress("l2_tauIsoL", &l2_tauIsoL);
    Run_Tree->SetBranchAddress("l2_tauIsoM", &l2_tauIsoM);
    Run_Tree->SetBranchAddress("l2_tauIsoT", &l2_tauIsoT);
    Run_Tree->SetBranchAddress("mva_2", &l2_tauIsoMVA2L);
    Run_Tree->SetBranchAddress("l2_tauIsoMVA2M", &l2_tauIsoMVA2M);
    Run_Tree->SetBranchAddress("l2_tauIsoMVA2T", &l2_tauIsoMVA2T);
    Run_Tree->SetBranchAddress("iso_2", &l2_tauIsoMVA2raw);
    Run_Tree->SetBranchAddress("l2_tauRejEleL", &l2_tauRejEleL);
    Run_Tree->SetBranchAddress("l2_tauRejEleM", &l2_tauRejEleM);
    Run_Tree->SetBranchAddress("againstElectronMVA3raw_2", &l2_tauRejEleMVA);
    Run_Tree->SetBranchAddress("l2_tauRejEleMVA3L", &l2_tauRejEleMVA3L);
    Run_Tree->SetBranchAddress("l2_tauRejEleMVA3M", &l2_tauRejEleMVA3M);
    Run_Tree->SetBranchAddress("l2_tauRejEleMVA3T", &l2_tauRejEleMVA3T);
    Run_Tree->SetBranchAddress("l2_RefJetPt", &l2_RefJetPt);
    Run_Tree->SetBranchAddress("l2_RefJetEta", &l2_RefJetEta);
    Run_Tree->SetBranchAddress("l2_RefJetPhi", &l2_RefJetPhi);

    Run_Tree->SetBranchAddress("mt_1", &mt_1);
    Run_Tree->SetBranchAddress("mt_2", &mt_2);
    Run_Tree->SetBranchAddress("mvis", &mvis);

    Run_Tree->SetBranchAddress("idweight_1", &idweight_1);
    Run_Tree->SetBranchAddress("trigweight_1", &trigweight_1);
    Run_Tree->SetBranchAddress("trigweight_2", &trigweight_2);
    Run_Tree->SetBranchAddress("rho", &rho);
    Run_Tree->SetBranchAddress("npv", &num_PV);
    Run_Tree->SetBranchAddress("npu", &npu); // NNNEW
    Run_Tree->SetBranchAddress("effweight", &eff_Correction);
    Run_Tree->SetBranchAddress("puweight", &pu_Weight);

    Run_Tree->SetBranchAddress("jpt_1", &jpt_1);
    Run_Tree->SetBranchAddress("jeta_1", &jeta_1);
    Run_Tree->SetBranchAddress("jphi_1", &jphi_1);
    Run_Tree->SetBranchAddress("jE_1", &jE_1);
    Run_Tree->SetBranchAddress("jpt_2", &jpt_2);
    Run_Tree->SetBranchAddress("jeta_2", &jeta_2);
    Run_Tree->SetBranchAddress("jphi_2", &jphi_2);
    Run_Tree->SetBranchAddress("jE_2", &jE_2);
    Run_Tree->SetBranchAddress("jpass_1", &jpass_1);
    Run_Tree->SetBranchAddress("jpass_2", &jpass_2);
    Run_Tree->SetBranchAddress("bpt_1", &bpt_1);
    Run_Tree->SetBranchAddress("beta_1", &beta_1);
    Run_Tree->SetBranchAddress("bphi_1", &bphi_1);
    Run_Tree->SetBranchAddress("bdiscriminant_1", &bdiscriminant_1);
    Run_Tree->SetBranchAddress("bpt_2", &bpt_2);
    Run_Tree->SetBranchAddress("beta_2", &beta_2);
    Run_Tree->SetBranchAddress("bphi_2", &bphi_2);
    Run_Tree->SetBranchAddress("bdiscriminant_2", &bdiscriminant_2);
    Run_Tree->SetBranchAddress("loosebpt_1", &loosebpt_1);
    Run_Tree->SetBranchAddress("loosebeta_1", &loosebeta_1);
    Run_Tree->SetBranchAddress("loosebphi_1", &loosebphi_1);
    Run_Tree->SetBranchAddress("loosebdiscriminant_1", &loosebdiscriminant_1);
    Run_Tree->SetBranchAddress("loosebpt_2", &loosebpt_2);
    Run_Tree->SetBranchAddress("loosebeta_2", &loosebeta_2);
    Run_Tree->SetBranchAddress("loosebphi_2", &loosebphi_2);
    Run_Tree->SetBranchAddress("loosebdiscriminant_2", &loosebdiscriminant_2);
    Run_Tree->SetBranchAddress("mjj", &mjj);
    Run_Tree->SetBranchAddress("jdeta", &jdeta);
    Run_Tree->SetBranchAddress("jdphi", &jdphi);
    Run_Tree->SetBranchAddress("jetpt", &jetpt);
    Run_Tree->SetBranchAddress("njets", &njets);
    Run_Tree->SetBranchAddress("njetpt20", &njetpt20);
    Run_Tree->SetBranchAddress("nbtag", &nbtag);
    Run_Tree->SetBranchAddress("nbtagLoose", &nbtagLoose);
    Run_Tree->SetBranchAddress("mcdata", &mcdata);

    Run_Tree->SetBranchAddress("l1_d0", &l1_d0);
    Run_Tree->SetBranchAddress("l1_dZ_in", &l1_dZ_in);
    Run_Tree->SetBranchAddress("l2_DecayModeFinding", &l2_DecayModeFinding);
    Run_Tree->SetBranchAddress("l2_DecayModeFindingOldDMs", &l2_DecayModeFindingOldDMs);
    Run_Tree->SetBranchAddress("againstMuonLoose3", &l2_tauRejMu3L);
    Run_Tree->SetBranchAddress("againstMuonTight3", &l2_tauRejMu3T);

    Run_Tree->SetBranchAddress("zCategory", &zCategory);
    Run_Tree->SetBranchAddress("l2_DecayMode", &l2_DecayMode);
    Run_Tree->SetBranchAddress("embedWeight", &embedWeight);
    Run_Tree->SetBranchAddress("nbtagNoCor", &nbtagNoCor);

    //SVMass from another Tree
    Run_Tree->SetBranchAddress("SVMass", &SVMass);
    Run_Tree->SetBranchAddress("SVMassUnc", &SVMassUnc);
    Run_Tree->SetBranchAddress("SVMassUp", &SVMassUp);
    Run_Tree->SetBranchAddress("SVMassUncUp", &SVMassUncUp);
    Run_Tree->SetBranchAddress("SVMassDown", &SVMassDown);
    Run_Tree->SetBranchAddress("SVMassUncDown", &SVMassUncDown);
    Run_Tree->SetBranchAddress("l2_LoosetauIsoMVA3oldDMwLT", &l2_LoosetauIsoMVA3oldDMwLT);
    Run_Tree->SetBranchAddress("l2_MediumtauIsoMVA3oldDMwLT", &l2_MediumtauIsoMVA3oldDMwLT);
    Run_Tree->SetBranchAddress("l2_TighttauIsoMVA3oldDMwLT", &l2_TighttauIsoMVA3oldDMwLT);

    Run_Tree->SetBranchAddress("Trigger_MuTau12", &Trigger_MuTau12);
    Run_Tree->SetBranchAddress("Trigger_EleTau12", &Trigger_EleTau12);
    Run_Tree->SetBranchAddress("Trigger_SingleMu12", &Trigger_SingleMu12);
    Run_Tree->SetBranchAddress("Trigger_SingleEle12", &Trigger_SingleEle12);
    Run_Tree->SetBranchAddress("Trigger_SingleJet12", &Trigger_SingleJet12);
    Run_Tree->SetBranchAddress("l1_trgMatche_Ele20Tau20", &l1_trgMatche_Ele20Tau20);
    Run_Tree->SetBranchAddress("l1_trgMatche_Mu17Tau20", &l1_trgMatche_Mu17Tau20);
    Run_Tree->SetBranchAddress("l1_trgMatche_Mu18Tau25", &l1_trgMatche_Mu18Tau25);
    Run_Tree->SetBranchAddress("l1_trgMatche_Mu24", &l1_trgMatche_Mu24);
    Run_Tree->SetBranchAddress("l2_trgMatche_Ele20Tau20", &l2_trgMatche_Ele20Tau20);
    Run_Tree->SetBranchAddress("l2_trgMatche_Mu17Tau20", &l2_trgMatche_Mu17Tau20);
    Run_Tree->SetBranchAddress("l2_trgMatche_Mu18Tau25", &l2_trgMatche_Mu18Tau25);
    Run_Tree->SetBranchAddress("num_gen_jets;", &num_gen_jets);
    Run_Tree->SetBranchAddress("gen_Higgs_pt", &gen_Higgs_pt);

    Run_Tree->SetBranchAddress("l1_ConversionVeto", &l1_ConversionVeto);
    Run_Tree->SetBranchAddress("l1_dxy_PV", &l1_dxy_PV);
    Run_Tree->SetBranchAddress("l1_dz_PV", &l1_dz_PV);
    Run_Tree->SetBranchAddress("l2_dxy_PV", &l2_dxy_PV);
    Run_Tree->SetBranchAddress("l2_dz_PV", &l2_dz_PV);

    Run_Tree->SetBranchAddress("l1_Index;", &l1_Index);
    Run_Tree->SetBranchAddress("l2_Index;", &l2_Index);
    Run_Tree->SetBranchAddress("pu_Weight_old;", &pu_Weight_old);
    Run_Tree->SetBranchAddress("l1Eta_SC;", &l1Eta_SC);
    Run_Tree->SetBranchAddress("GenTopPt;", &GenTopPt);
    Run_Tree->SetBranchAddress("GenAntiTopPt;", &GenAntiTopPt);
    Run_Tree->SetBranchAddress("Tau_Vertex_dz;", &Tau_Vertex_dz);
    Run_Tree->SetBranchAddress("gen_Higgs_Mass;", &gen_Higgs_Mass);
    Run_Tree->SetBranchAddress("l2_tauIsoMVAraw3newDMwLTraw", &l2_tauIsoMVAraw3newDMwLTraw);
    Run_Tree->SetBranchAddress("l2_tauIsoMVAraw3newDMwoLTraw", &l2_tauIsoMVAraw3newDMwoLTraw);
    Run_Tree->SetBranchAddress("l2_tauIsoMVAraw3oldDMwLTraw", &l2_tauIsoMVAraw3oldDMwLTraw);
    Run_Tree->SetBranchAddress("l2_tauIsoMVAraw3oldDMwoLTraw", &l2_tauIsoMVAraw3oldDMwoLTraw);

    //###############################################################################################
    //Initial Requirements
    //###############################################################################################
    // Specific Cuts to be changed fro MSSM and nMSSM
    float cutonSVmass = 0;
    float cutonTaupt = 20;
    int massBin = 300;
    //    float cutonSVmass= 0;
    //    float cutonTaupt= 20;
    //    int massBin = 300;
    //###############################################################################################
    int ptBin = 300;
    bool IsInCorrcetMassRange = true;
    bool verbose_ = false;
    int Event_Double[8][9];
    memset(Event_Double, 0, sizeof (Event_Double[0][0]) * 8 * 9);
    const int hsize = 21;
    std::string arrayMassOfHiggs_String[hsize] = {"80", "90", " 100", "110", " 120", "130", "140", " 160", "180", "200", "250", "300", "350", "400", "450", "500", "600", "700", "800", "900", "1000"};
    vector<std::string> MassOfHiggs_String;
    for (int i = 0; i < hsize; i++)
        MassOfHiggs_String.push_back(arrayMassOfHiggs_String[i]);
    int MassOfHiggs_Int[hsize] = {80, 90, 100, 110, 120, 130, 140, 160, 180, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900, 1000};
    //    TFile * HiggsUncertaintyFile = new TFile("interface/mssmHiggsPtReweightGluGlu_mhmod+_POWHEG.root", "r"); // OLD
    TFile * HiggsUncertaintyFile = new TFile("interface/mssmHiggsPtReweightGluGlu_mhmod_POWHEG.root", "r"); // New by Christian
    TF1 *TriggerWeightBarrel = new TF1("AddTriggerWeightMuTauBarrel", "1 - 9.01280e-04*(x - 140) + 4.81592e-07*(x - 140)*(x-140)", 0., 800.);
    TF1 *TriggerWeightEndcaps = new TF1("AddTriggerWeightMuTauEndcaps", "1 - 1.81148e-03*(x - 60) + 5.44335e-07*(x - 60)*(x-60)", 0., 800.);

    //###############################################################################################
    //Loop over all events/tau pairs
    //###############################################################################################
    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    int y = 0;
    for (Int_t i = 0; i < nentries_wtn; i++) {
        Run_Tree->GetEntry(i);
        if (i % 10000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
        fflush(stdout);

        //###############################################################################################
        //  CATEGORIZATION
        //###############################################################################################

        ////###############   New NMSSM Categorization
        const int size_mssmC = 5;
        bool selection_inclusive = 1;
        bool selection_nobtag = nbtag < 1;
        bool selection_btag = nbtag > 0 && njets < 2 && nbtag < 2;
        bool selection_btagLoose = nbtagLoose > 0 && njets < 2;
        bool selection_DoublebtagLoose = nbtag > 1 && njets < 3;
        bool MSSM_Category[size_mssmC] = {selection_inclusive, selection_nobtag, selection_btag, selection_btagLoose, selection_DoublebtagLoose};
        std::string index[size_mssmC] = {"_inclusive", "_nobtag", "_btag", "_btagLoose", "_doublebtag"};
        //        ////###############   MSSM Categorization
        //        const int size_mssmC = 4;
        //        bool selection_inclusive = 1;
        //        bool selection_nobtag = nbtag < 1;
        //        bool selection_btag = nbtag > 0 && njets < 2;
        //        bool selection_btagLoose = nbtagLoose > 0 && njets < 2;
        //        bool MSSM_Category[size_mssmC] = {selection_inclusive, selection_nobtag, selection_btag, selection_btagLoose};
        //        std::string index[size_mssmC] = {"_inclusive", "_nobtag", "_btag", "_btagLoose"};

        ////###############   Z Categorization
        int size_ZCat = 4;
        bool sel_No_Z = 1;
        bool sel_ZTT = (zCategory == 3);
        bool sel_ZL = (zCategory == 1 || zCategory == 2 || zCategory == 4);
        bool sel_ZJ = (zCategory == 5 || zCategory == 6);
        bool Z_Category[4] = {sel_No_Z, sel_ZTT, sel_ZL, sel_ZJ};
        std::string ZCat[4] = {"", "_ZTT", "_ZL", "_ZJ"};
        size_t DYsampleS = out.find("out_DY");
        if (!(DYsampleS != string::npos)) size_ZCat = 1;


        ////###############   Tau eta Categorization
        const int size_eta = 4;
        bool eta_Tot = fabs(l2Eta) < 2.3;
        bool eta_Bar = fabs(l2Eta) < 1.2;
        bool eta_Cen = fabs(l2Eta) > 1.2 && fabs(l2Eta) < 1.7;
        bool eta_End = fabs(l2Eta) > 1.7 && fabs(l2Eta) < 2.3;
        bool eta_category[size_eta] = {eta_Tot, eta_Bar, eta_Cen, eta_End};
        std::string eta_Cat[size_eta] = {"", "_Bar", "_Cen", "_End"};

        ////###############   Tau Lep Charge  Categorization
        const int size_Q = 2;
        bool charge_OS = l1Charge * l2Charge < 0;
        bool charge_SS = l1Charge * l2Charge > 0;
        bool charge_category[size_Q] = {charge_OS, charge_SS};
        std::string q_Cat[size_Q] = {"_OS", "_SS"};

        ////###############   mT  Categorization
        float mT = TMass_F(l1Pt, l1Px, l1Py, mvamet, mvametphi);
        const int size_MT = 2;
        bool mTLess30 = mT < 30;
        bool mTHigh70 = mT > 70;
        bool mT_category[size_MT] = {mTLess30, mTHigh70};
        std::string mT_Cat[size_MT] = {"_mTLess30", "_mTHigher70"};

        ////###############   Tau Isolation  Categorization
        const int size_isoCat = 2;
        bool TightIso = l2_TighttauIsoMVA3oldDMwLT > 0.5;
        bool RelaxIso = l2_TighttauIsoMVA3oldDMwLT < 0.5 && l2_LoosetauIsoMVA3oldDMwLT > 0.5;
        bool Iso_category[size_isoCat] = {TightIso, RelaxIso};
        std::string iso_Cat[size_isoCat] = {"", "_RelaxIso"};
        ////###############   Lepton Isolation  Categorization

        const int size_LepisoCat = 2;
        bool LepTightIso, LepRelaxIso;
        if (Channel == 1) {
            LepTightIso = l1_muIso < 0.10;
            LepRelaxIso = l1_muIso > 0.2 && l1_muIso < 0.5;
        }
        if (Channel == 3) {
            LepTightIso = l1_eleIso < 0.1;
            LepRelaxIso = l1_eleIso > 0.2 && l1_eleIso < 0.5;
        }
        bool LepIso_category[size_isoCat] = {LepTightIso, LepRelaxIso};
        std::string Lepiso_Cat[size_LepisoCat] = {"", "_LepAntiIso"};

        ////###############   Number of GenJet  Categorization
        int size_jet = 6;
        bool jetAll = 1;
        bool jet0 = num_gen_jets == 0;
        bool jet1 = num_gen_jets == 1;
        bool jet2 = num_gen_jets == 2;
        bool jet3 = num_gen_jets == 3;
        bool jet4 = num_gen_jets == 4;
        bool NgenJet_category[6] = {jetAll, jet0, jet1, jet2, jet3, jet4};
        std::string Gjet_Cat[6] = {"", "0j", "1j", "2j", "3j", "4j"};
        size_t DYsample = out.find("out_DY");
        size_t Wsample = out.find("out_W");
        if (!(DYsample != string::npos || Wsample != string::npos)) size_jet = 1;


        ////###############   tsu energy scale up and down  Categorization
        const int size_tscale = 3;
        float scaleTau[size_tscale] = {-0.03, 0.0, 0.03};
        bool tauPtCutMinus = l2Pt * (1 + scaleTau[0]) > cutonTaupt;
        bool tauPtCutNorm = l2Pt * (1 + scaleTau[1]) > cutonTaupt;
        bool tauPtCutPlus = l2Pt * (1 + scaleTau[2]) > cutonTaupt;
        bool TauScale[size_tscale] = {tauPtCutMinus, tauPtCutNorm, tauPtCutPlus};
        double SVMASS[size_tscale] = {SVMassDown, SVMass, SVMassUp};
        std::string TauScale_cat[size_tscale] = {"Down", "", "Up"};

        //####################################################
        // Check Higgs Mass intervals  0.7-1.3
        //####################################################
        size_t ggHiggsFind = out.find("ggH"); // Check in the name it IS ggH
        size_t bbHiggsFind = out.find("bbH"); // Check in the name it IS ggH
        size_t SMHFind = out.find("SM"); // Check that it IS NOT SM ggH_SM125 GeV
        if (ggHiggsFind != string::npos && SMHFind == string::npos) {
            std::string FirstPart = "OutFiles/out_ggH";
            std::string LastPart = "_8TeV.root";
            std::string newOut = out.substr(FirstPart.size());
            newOut = newOut.substr(0, newOut.size() - LastPart.size());
            vector<string>::iterator it;
            it = find(MassOfHiggs_String.begin(), MassOfHiggs_String.end(), newOut);
            if (it != MassOfHiggs_String.end()) {
                int massHiggs = MassOfHiggs_Int[it - MassOfHiggs_String.begin()];
                IsInCorrcetMassRange = (gen_Higgs_Mass > massHiggs * 0.7 && gen_Higgs_Mass < massHiggs * 1.3);
            }
        }
        if (bbHiggsFind != string::npos && SMHFind == string::npos) {
            std::string FirstPart = "OutFiles/out_bbH";
            std::string LastPart = "_8TeV.root";
            std::string newOut = out.substr(FirstPart.size());
            newOut = newOut.substr(0, newOut.size() - LastPart.size());
            vector<string>::iterator it;
            it = find(MassOfHiggs_String.begin(), MassOfHiggs_String.end(), newOut);
            if (it != MassOfHiggs_String.end()) {
                int massHiggs = MassOfHiggs_Int[it - MassOfHiggs_String.begin()];
                IsInCorrcetMassRange = (gen_Higgs_Mass > massHiggs * 0.7 && gen_Higgs_Mass < massHiggs * 1.3);
            }
        }

        //####################################################
        // Event Weight
        //####################################################
        //############ Higg Pt Reweighting just for GluGluH samples  [POWHEG/PYTHIA]
        vector<float> HiggsPtReweight(5, 1);
        size_t HiggsFind = out.find("ggH"); // Check in the name it IS ggH
        size_t SMFind = out.find("SM"); // Check that it IS NOT SM ggH_SM125 GeV
        bool isGluGluH = (HiggsFind != string::npos && SMFind == string::npos);
        if (isGluGluH) {
            std::string FirstPart = "OutFiles/out_ggH";
            std::string LastPart = "_8TeV.root";
            std::string newOut = out.substr(FirstPart.size());
            newOut = newOut.substr(0, newOut.size() - LastPart.size());
            HiggsPtReweight = HPtReWeight(gen_Higgs_pt, newOut, HiggsUncertaintyFile);
        }

        //############ Top Reweighting
        float TopPtReweighting = 1;
        size_t TTJets = input.find("TTJets");
        bool isTTJets = (TTJets != string::npos);
        if (isTTJets) TopPtReweighting = compTopPtWeight(GenTopPt, GenAntiTopPt);

        //############ Tau Pt Reweighting
        float tauPtReweightingUp = 1 + 0.2 * (l2Pt / 1000);
        float tauPtReweightingDown = 1 - 0.2 * (l2Pt / 1000);

        //############ Tau Energy Scale Reweighting
        float tauESWeight = TauESWeight(mcdata, l2_DecayMode, l2Eta);

        //############ Embedding Reweighting
        float WeightEmbed = (embedWeight == 0 ? 1 : embedWeight);

        //############ Ele to Tau fake rate  Reweighting  EleTau fake rate scale factors for etau channel and ZL background
        float EleTauFRWeight = 1;
        if (Channel == 3 && sel_ZL && fabs(l2Eta) < 1.479 && l2_DecayMode == 0) EleTauFRWeight = 1.37;
        if (Channel == 3 && sel_ZL && fabs(l2Eta) > 1.479 && l2_DecayMode == 0) EleTauFRWeight = 0.72;
        if (Channel == 3 && sel_ZL && fabs(l2Eta) < 1.479 && (l2_DecayMode == 1 || l2_DecayMode == 2)) EleTauFRWeight = 1.84;
        if (Channel == 3 && sel_ZL && fabs(l2Eta) > 1.479 && (l2_DecayMode == 1 || l2_DecayMode == 2)) EleTauFRWeight = 0.83;


        //####################################################
        // Trigger and Trigger Matching
        //####################################################
        float CorrectionForEmbed = 1.0;
        bool MuTrgMatched = 1;
        bool EleTrgMatched = 1;
        size_t EmbedFind = input.find("Embed");
        if (EmbedFind != string::npos)
            CorrectionForEmbed = getCorrFactorEMbed(mcdata, Channel, l1Pt, l1Eta, l2Pt, l2Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
        else {
            MuTrgMatched = (Channel == 1) && Trigger_MuTau12 && l1_trgMatche_Mu17Tau20 && l2_trgMatche_Mu17Tau20;
            EleTrgMatched = (Channel == 3) && Trigger_EleTau12 && l1_trgMatche_Ele20Tau20 && l2_trgMatche_Ele20Tau20;
        }

        //############ Full Reweighting
        float AllWeight = pu_Weight * eff_Correction * tauESWeight * WeightEmbed * HiggsPtReweight[1] * EleTauFRWeight * TopPtReweighting * CorrectionForEmbed;
        if (verbose_) cout << "AllWeight= " << AllWeight << "   pu_Weight= " << pu_Weight << "   eff_Correction=" << eff_Correction << "   tauESWeight=" << tauESWeight << "   WeightEmbed=" << WeightEmbed << "   HiggsPtReweight[1]=" << HiggsPtReweight[1] << "   EleTauFRWeight=" << EleTauFRWeight << "\n";
        //####################################################
        // Muon Selection
        //####################################################
        bool Mu_PtEta = l1Pt > 20 && fabs(l1Eta) < 2.1;
        bool Mu_IdTight = l1_muId_Tight;
        bool Mu_d0 = fabs(l1_d0) < 0.045; //the impact parameter in the transverse plane
        bool Mu_dZ = fabs(l1_dZ_in) < 0.2; //the impact parameter in the transverse plane
        //        bool Mu_Iso = l1_muIso < 0.10;
        //        bool Mu_Iso_Loose = l1_muIso > 0.2 && l1_muIso < 0.5;
        bool MU_CUTS = Mu_PtEta && Mu_IdTight && Mu_d0 && Mu_dZ;


        //####################################################
        // Electron Selection
        //####################################################
        bool El_PtEta = l1Pt > 24 && fabs(l1Eta) < 2.1;
        bool El_IdTight = l1_eleId_Tight;
        //        bool El_Iso = l1_eleIso < 0.1;
        //        bool El_Iso_Loose = l1_eleIso > 0.2 && l1_eleIso < 0.5;
        bool EL_CUTS = El_PtEta && El_IdTight;

        //####################################################
        // Tau Selection
        //####################################################
        //        bool Tau_Eta = fabs(l2Eta) < 2.3;
        //        bool Tau_PtEta = l2Pt > 30 && fabs(l2Eta) < 2.3;
        bool Tau_DMF = l2_DecayModeFindingOldDMs;
        //bool Tau_DMF = l2_DecayModeFinding && (l2_DecayMode < 4 || l2_DecayMode > 8);
        //bool Tau_Isolation = byCombinedIsolationDeltaBetaCorrRaw3Hits_2 < 1.5;
        //        bool Tau_Isolation = l2_TighttauIsoMVA3oldDMwLT > 0.5;
        //bool Tau_Isolation = l2_tauIsoMVA2T > 0.5;
        bool Tau_antiEl = 1; //applied at the level of tree Making
        bool Tau_antiMu = 1; //applied at the level of tree Making
        //bool Tau_antiEl = l2_tauRejEleL;
        //bool Tau_antiMu = l2_tauRejMu3L;
        //bool Tau_antiMu = l2_tauRejMu2T;
        bool TauVtxdZ = fabs(Tau_Vertex_dz) < 0.2;
        bool TAU_CUTS = SVMass > cutonSVmass && IsInCorrcetMassRange && TauVtxdZ && Tau_DMF && Tau_antiEl && Tau_antiMu;


        //########################################################################################################
        //test Data Categories

        //        int evenrList[40] = {103973225, 104420828, 106034910, 105660180, 106974476, 107092579, 108055243, 108396438, 108412892, 150562091, 152046510, 152847565, 152909050, 153063537, 153791799, 153918798, 39336845, 41209032, 41846823, 41850867, 43308698, 43246086, 56053990, 56782039, 58949867, 58715990, 58991357, 61799085, 61729268, 76680099, 79031407, 79231016, 78988784, 78601443, 79378433, 81225657, 84970796, 98546954, 100282678, 101178061};
        //        for (int ii = 0; ii < 40; ii++) {
        //            if (Event == evenrList[ii]) {
        //                cout << Event << "  " << El_PtEta << El_IdTight << (SVMass > 50) << IsInCorrcetMassRange << TauVtxdZ << Tau_DMF << Tau_antiEl << Tau_antiMu << (l2Pt > 30) << (fabs(l2Eta) < 2.3) << (l2_TighttauIsoMVA3oldDMwLT > 0.5) << "  pt of tau " << l2Pt << "  tau Isol MVA is " << l2_tauIsoMVAraw3oldDMwLTraw << "  tau Isol Comb is " << byCombinedIsolationDeltaBetaCorrRaw3Hits_2 << " eta is" << l2Eta << " decayMode is " << l2_DecayMode << "\n";
        //            }
        //        }
        //        if (selection_btag && EL_CUTS && TAU_CUTS && Channel == 3 && mTLess30 && charge_OS && TightIso && l2Pt > 30 && fabs(l2Eta) < 2.3) {
        //            cout << Run << ":" << Lumi << ":" << Event << "\n";
        //        }
        //if (selection && MuTrgMatched && MU_CUTS && TAU_CUTS && Channel == 1 && mTLess30 && charge_OS && TightIso && sel_ZJ) {
        //    if (Event == 524696738 ) {
        //    cout<< Run <<":"<< Lumi <<":"<<Event << "  Pt is   "<< l2Pt <<"  "<<nbtag<<" "<<njets<<" " << jeta_1 <<" "<< jeta_2 <<" " << selection_btag << MuTrgMatched << MU_CUTS << TAU_CUTS <<( Channel == 1) << mTLess30 << charge_SS << TightIso <<"\n";
        //        if (Event == 92686979 && l2Pt > 30 && selection_btag && MuTrgMatched && MU_CUTS && TAU_CUTS && Channel == 1 && mTLess30 && charge_SS && TightIso) {
        //cout << Run << ":" << Lumi << ":" << Event << "mu PT is " << l1Pt << " iso is " << l1_muIso << "\n";
        //        }

        //########################################################################################################
        //####################################################
        //####################################################
        //####################################################
        //####################################################
        //####################################################
        // Starting Analysis  //Loop Over  Categories
        //####################################################
        //####################################################
        //####################################################
        //####################################################
        //####################################################
        // QCD Norm does noe exist anymore,  insead use SVMass
        //########################################################################################################

        for (int tScalecat = 0; tScalecat < size_tscale; tScalecat++) {
            if (TauScale[tScalecat]) { //  HERE TOBEDONE TOMOORW
                for (int Jetcat = 0; Jetcat < size_jet; Jetcat++) {
                    if (NgenJet_category[Jetcat]) { //  HERE TOBEDONE TOMOORW
                        for (int icat = 0; icat < size_mssmC; icat++) {
                            if (MSSM_Category[icat]) {
                                for (int zcat = 0; zcat < size_ZCat; zcat++) {
                                    if (Z_Category[zcat]) {
                                        for (int isocat = 0; isocat < size_isoCat; isocat++) {
                                            if (Iso_category[isocat]) {
                                                for (int qcat = 0; qcat < size_Q; qcat++) {
                                                    if (charge_category[qcat]) {
                                                        for (int etacat = 0; etacat < size_eta; etacat++) {
                                                            if (eta_category[etacat]) {
                                                                memset(Event_Double, 0, sizeof (Event_Double[0][0]) * 8 * 9);
                                                                for (int mTcat = 0; mTcat < size_MT; mTcat++) {
                                                                    if (mT_category[mTcat]) {
                                                                        for (int lepiso = 0; lepiso < size_LepisoCat; lepiso++) {
                                                                            if (LepIso_category[lepiso]) {
                                                                                //###################################################
                                                                                bool SignalSelection = (lepiso == 0 && tScalecat == 1 && qcat == 0 && mTcat == 0 && isocat == 0 && zcat == 0 && Jetcat == 0 && etacat == 0);
                                                                                bool ZLSelection = (lepiso == 0 && tScalecat == 1 && qcat == 0 && mTcat == 0 && isocat == 0 && etacat == 0);
                                                                                bool QCDShape = (tScalecat == 1);
                                                                                bool WShape = (lepiso == 0 && qcat == 0 && mTcat == 0 && etacat == 0);
                                                                                //###################################################
                                                                                // Muon  Signal Selection   mT < 30 GeV
                                                                                //###################################################
                                                                                if (MuTrgMatched && MU_CUTS && TAU_CUTS && (Event != Event_Double[1][1])) {
                                                                                    if (lepiso == 0) plotFill("mutau_SVMass" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight);
                                                                                    if (WShape) plotFill("mutau_Wshape2DSVMassPt" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], l2Pt, massBin, 0, massBin, ptBin, 0, ptBin, AllWeight);
                                                                                    if (QCDShape) plotFill("mutau_QCDshape2DSVMassPt" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], l2Pt, massBin, 0, massBin, ptBin, 0, ptBin, AllWeight);
                                                                                    if (QCDShape) plotFill("mutau_TauPt" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], l2Pt, ptBin, 0, ptBin, AllWeight);
                                                                                    if (SignalSelection && isTTJets) plotFill("mutau_SVMassTopPtRWUp" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight * TopPtReweighting);
                                                                                    if (SignalSelection && isTTJets) plotFill("mutau_SVMassTopPtRWDown" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight / TopPtReweighting);
                                                                                    if (SignalSelection) plotFill("mutau_SVMassTauHighPtRWUp" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight * tauPtReweightingUp);
                                                                                    if (SignalSelection) plotFill("mutau_SVMassTauHighPtRWDown" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight * tauPtReweightingDown);
                                                                                    if (ZLSelection) plotFill("mutau_SVMassZLScaleUp" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat]*1.02, massBin, 0, massBin, AllWeight);
                                                                                    if (ZLSelection) plotFill("mutau_SVMassZLScaleDown" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat]*0.98, massBin, 0, massBin, AllWeight);

                                                                                }
                                                                                //####################################################

                                                                                //###################################################
                                                                                // Electron Signal Selection   mT < 30 GeV
                                                                                //###################################################
                                                                                if (EleTrgMatched && EL_CUTS && TAU_CUTS && (Event != Event_Double[2][2])) {
                                                                                    if (lepiso == 0) plotFill("etau_SVMass" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight);
                                                                                    if (WShape) plotFill("etau_Wshape2DSVMassPt" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], l2Pt, massBin, 0, massBin, ptBin, 0, ptBin, AllWeight);
                                                                                    if (QCDShape) plotFill("etau_QCDshape2DSVMassPt" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], l2Pt, massBin, 0, massBin, ptBin, 0, ptBin, AllWeight);
                                                                                    if (QCDShape) plotFill("etau_TauPt" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + eta_Cat[etacat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], l2Pt, ptBin, 0, ptBin, AllWeight);
                                                                                    if (SignalSelection && isTTJets) plotFill("etau_SVMassTopPtRWUp" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight * TopPtReweighting);
                                                                                    if (SignalSelection && isTTJets) plotFill("etau_SVMassTopPtRWDown" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight / TopPtReweighting);
                                                                                    if (SignalSelection) plotFill("etau_SVMassTauHighPtRWUp" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight * tauPtReweightingUp);
                                                                                    if (SignalSelection) plotFill("etau_SVMassTauHighPtRWDown" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat], massBin, 0, massBin, AllWeight * tauPtReweightingDown);
                                                                                    if (ZLSelection) plotFill("etau_SVMassZLScaleUp" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat]*1.02, massBin, 0, massBin, AllWeight);
                                                                                    if (ZLSelection) plotFill("etau_SVMassZLScaleDown" + Lepiso_Cat[lepiso] + mT_Cat[mTcat] + q_Cat[qcat] + iso_Cat[isocat] + ZCat[zcat] + index[icat] + Gjet_Cat[Jetcat] + TauScale_cat[tScalecat], SVMASS[tScalecat]*0.98, massBin, 0, massBin, AllWeight);
                                                                                }
                                                                                //####################################################
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            } //charge category
                                                        }
                                                    } // Z category
                                                } //MSSM category
                                            } //check if category is passed
                                        } // loop over categories
                                    }
                                }//categ GenJet
                            }
                        }
                    }
                }
            }
        }

    }
    fout->cd();
    //    BG_Tree->Write();

    map<string, TH1F*>::const_iterator iMap1 = myMap1->begin();
    map<string, TH1F*>::const_iterator jMap1 = myMap1->end();

    for (; iMap1 != jMap1; ++iMap1)
        nplot1(iMap1->first)->Write();

    map<string, TH2F*>::const_iterator iMap2 = myMap2->begin();
    map<string, TH2F*>::const_iterator jMap2 = myMap2->end();

    for (; iMap2 != jMap2; ++iMap2)
        nplot2(iMap2->first)->Write();

    fout->Close();
}

//####################################################
//#################  Selection for QCD Normalization from data
//################# Selection for QCD Normalization from data
//     Yield from (sideband normalisation)*(fixed extrapolation factor) in each category.
//     Sideband in data is ss && mT<30. Subtract contribution from all other background processes:
//     ZTT, ZL, ZJ, W, TOP, VV. DYJets MC is used to estimate directly the ZTT contribution in
//     this sideband. The W contribution similar to the default method above: normalisation is
//     data sideband ss && mT>70, and extrapolation factor from mT>70 to mT<30 from WJets
//     inclusive+njet samples using the category selection and ss events. The os/ss factor is 1.06.


