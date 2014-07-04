#include "tr_Tree.h"

int main(int argc, char** argv) {
    //    std::string chanelType = *(argv + 1);
    //    std::string givenTauIso = *(argv + 2);
    //    float float_LepIso;
    //    float float_LTCut;
    //    if (argc > 1) {
    //        float_LepIso = atof(argv[3]); // alternative strtod
    //        float_LTCut = atof(argv[4]); // alternative strtod
    //    }
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
    cout.precision(1);

    Run_Tree->SetBranchAddress("Channel", &Channel);




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
    Run_Tree->SetBranchAddress("l2_tauRejMuL", &l2_tauRejMuL);
    Run_Tree->SetBranchAddress("againstMuonLoose2_2", &l2_tauRejMu2L);
    Run_Tree->SetBranchAddress("l2_tauRejMuM", &l2_tauRejMuM);
    Run_Tree->SetBranchAddress("againstMuonMedium2_2", &l2_tauRejMu2M);
    Run_Tree->SetBranchAddress("l2_tauRejMuT", &l2_tauRejMuT);
    Run_Tree->SetBranchAddress("againstMuonTight2_2", &l2_tauRejMu2T);
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
    Run_Tree->SetBranchAddress("bpt", &bpt);
    Run_Tree->SetBranchAddress("beta", &beta);
    Run_Tree->SetBranchAddress("bphi", &bphi);
    Run_Tree->SetBranchAddress("bdiscriminant", &bdiscriminant);
    Run_Tree->SetBranchAddress("bpt_2", &bpt_2);
    Run_Tree->SetBranchAddress("beta_2", &beta_2);
    Run_Tree->SetBranchAddress("bphi_2", &bphi_2);
    Run_Tree->SetBranchAddress("bdiscriminant_2", &bdiscriminant_2);
    Run_Tree->SetBranchAddress("loosebpt", &loosebpt);
    Run_Tree->SetBranchAddress("loosebeta", &loosebeta);
    Run_Tree->SetBranchAddress("loosebphi", &loosebphi);
    Run_Tree->SetBranchAddress("loosebdiscriminant", &loosebdiscriminant);
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
    Run_Tree->SetBranchAddress("mcdata", &mcdata);

    Run_Tree->SetBranchAddress("l1_d0", &l1_d0);
    Run_Tree->SetBranchAddress("l1_dZ_in", &l1_dZ_in);
    Run_Tree->SetBranchAddress("l2_DecayModeFinding", &l2_DecayModeFinding);

    Run_Tree->SetBranchAddress("zCategory", &zCategory);
    //SVMass from another Tree
    Run_Tree->SetBranchAddress("SVMass", &SVMass);
    Run_Tree->SetBranchAddress("SVMassUnc", &SVMassUnc);
    Run_Tree->SetBranchAddress("SVMassUp", &SVMassUp);
    Run_Tree->SetBranchAddress("SVMassUncUp", &SVMassUncUp);
    Run_Tree->SetBranchAddress("SVMassDown", &SVMassDown);
    Run_Tree->SetBranchAddress("SVMassUncDown", &SVMassUncDown);

    //
    //    //New BG_Tree
    //    TTree * BG_Tree = new TTree("BG_Tree", "BG_Tree");
    //    //    To force a memory-resident Tree
    //    BG_Tree->SetDirectory(0);
    //
    //    BG_Tree->Branch("Channel_", &Channel_, "Channel_/I");
    //    BG_Tree->Branch("subChannel_", &subChannel_, "subChannel_/I");
    //    BG_Tree->Branch("Run_", &Run_, "Run_/I");
    //    BG_Tree->Branch("Lumi_", &Lumi_, "Lumi_/I");
    //    BG_Tree->Branch("Event_", &Event_, "Event_/I");
    //
    //    BG_Tree->Branch("HMass_", &HMass_, "HMass_/F");
    //    BG_Tree->Branch("SVMass_", &SVMass_, "SVMass_/F");
    //
    //    BG_Tree->Branch("l3Pt_", &l3Pt_, "l3Pt_/F");
    //    BG_Tree->Branch("l3Eta_", &l3Eta_, "l3Eta_/F");
    //    BG_Tree->Branch("l3_CloseJetPt_", &l3_CloseJetPt_, "l3_CloseJetPt_/F");
    //    BG_Tree->Branch("l3_CloseJetEta_", &l3_CloseJetEta_, "l3_CloseJetEta_/F");
    //    BG_Tree->Branch("l4Pt_", &l4Pt_, "l4Pt_/F");
    //    BG_Tree->Branch("l4Eta_", &l4Eta_, "l4Eta_/F");
    //    BG_Tree->Branch("l4_CloseJetPt_", &l4_CloseJetPt_, "l4_CloseJetPt_/F");
    //    BG_Tree->Branch("l4_CloseJetEta_", &l4_CloseJetEta_, "l4_CloseJetEta_/F");
    //
    //    BG_Tree->Branch("met_", &met_, "met_/F");
    //    BG_Tree->Branch("metPhi_", &metPhi_, "metPhi_/F");
    //    BG_Tree->Branch("covMet11_", &covMet11_, "covMet11_/F");
    //    BG_Tree->Branch("covMet12_", &covMet12_, "covMet12_/F");
    //    BG_Tree->Branch("covMet21_", &covMet21_, "covMet21_/F");
    //    BG_Tree->Branch("covMet22_", &covMet22_, "covMet22_/F");
    //
    //    BG_Tree->Branch("l3M_", &l3M_, "l3M_/F");
    //    BG_Tree->Branch("l3Px_", &l3Px_, "l3Px_/F");
    //    BG_Tree->Branch("l3Py_", &l3Py_, "l3Py_/F");
    //    BG_Tree->Branch("l3Pz_", &l3Pz_, "l3Pz_/F");
    //
    //    BG_Tree->Branch("l4M_", &l4M_, "l4M_/F");
    //    BG_Tree->Branch("l4Px_", &l4Px_, "l4Px_/F");
    //    BG_Tree->Branch("l4Py_", &l4Py_, "l4Py_/F");
    //    BG_Tree->Branch("l4Pz_", &l4Pz_, "l4Pz_/F");
    //
    //    BG_Tree->Branch("eff_Correction_", &eff_Correction_, "eff_Correction_/F");
    //    BG_Tree->Branch("pu_Weight_", &pu_Weight_, "pu_Weight_/F");


    //###############################################################################################
    //Just each categori should be filled once
    int Event_Double[8][9];
    memset(Event_Double, 0, sizeof (Event_Double[0][0]) * 8 * 9);
    float QCD_OSSS_SFactor = 1.06;
    int low_bin = 0;
    int high_bin = 1500;
    //###############################################################################################

    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    int y = 0;
    for (Int_t i = 0; i < nentries_wtn; i++) {
        Run_Tree->GetEntry(i);
        if (i % 10000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
        fflush(stdout);


        //####################################################
        // Common Cuts
        //####################################################
        bool OS = l1Charge * l2Charge < 0;
        bool SS = l1Charge * l2Charge > 0;
        float mT = TMass_F(l1Pt, l1Px, l1Py, mvamet, mvametphi);

        //MSSM Categorization
        bool selection_inclusive = 1;
        bool selection_nobtag = nbtag < 1;
        bool selection_btag = nbtag > 0 && njets < 2;
        bool selection_btagLoose = loosebpt > 0 && njets < 2;
        bool MSSM_Category[4] = {selection_inclusive, selection_nobtag, selection_btag, selection_btagLoose};
        std::string index[4] = {"_inclusive", "_nobtag", "_btag", "_btagLoose"};

        bool sel_No_Z = 1;
        bool sel_ZTT = zCategory == 1;
        bool sel_ZL = zCategory == 2;
        bool sel_ZJ = zCategory == 3;
        bool Z_Category[4] = {sel_No_Z, sel_ZTT, sel_ZL, sel_ZJ};
        std::string ZCat[4] = {"", "_ZTT", "_ZL", "_ZJ"};

        //####################################################
        if (Run > 160431 &&  Run < 163261) cout << "Buggy Runs= " << Run << endl;
        //####################################################
        // MuTau Channel
        //####################################################
        if (Channel == 1) {

            bool Mu_PtEta = l1Pt > 20 && fabs(l1Eta) < 2.1;
            bool Mu_IdTight = l1_muId_Tight;
            bool Mu_d0 = l1_d0 < 0.045; //the impact parameter in the transverse plane
            bool Mu_dZ = l1_dZ_in < 0.2; //the impact parameter in the transverse plane
            bool Mu_Iso = l1_muIso < 0.10;
            bool Mu_Iso_Loose = l1_muIso > 0.2 && l1_muIso < 0.5;
            bool MU_CUTS = Mu_PtEta && Mu_IdTight && Mu_d0 && Mu_dZ && Mu_Iso;
            bool MU_CUTS_Loose = Mu_PtEta && Mu_IdTight && Mu_d0 && Mu_dZ && Mu_Iso_Loose;

            bool Tau_PtEta = l2Pt > 30 && fabs(l2Eta) < 2.3;
            bool Tau_DMF = l2_DecayModeFinding;
            //            bool Tau_Isolation = byCombinedIsolationDeltaBetaCorrRaw3Hits_2 < 1.5;
            bool Tau_Isolation = l2_tauIsoMVA2T > 0.5;
            bool Tau_antiEl = l2_tauRejEleL;
            bool Tau_antiMu = l2_tauRejMu2T;
            bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;


            //Loop Over 3 Categories
            for (int icat = 0; icat < 4; icat++) {
                if (MSSM_Category[icat]) {
                    memset(Event_Double, 0, sizeof (Event_Double[0][0]) * 8 * 9);
                    for (int zcat = 0; zcat < 4; zcat++) {
                        if (Z_Category[zcat]) {


                            //####################################################
                            //#################  Selection for QCD Normalization from data
                            //################# Selection for QCD Normalization from data
                            //                 Yield from (sideband normalisation)*(fixed extrapolation factor) in each category.
                            //                 Sideband in data is ss && mT<30. Subtract contribution from all other background processes:
                            //                 ZTT, ZL, ZJ, W, TOP, VV. DYJets MC is used to estimate directly the ZTT contribution in
                            //                 this sideband. The W contribution similar to the default method above: normalisation is
                            //                 data sideband ss && mT>70, and extrapolation factor from mT>70 to mT<30 from WJets
                            //                 inclusive+njet samples using the category selection and ss events. The os/ss factor is 1.06.



                            //################# Signal Selectiopn
                            if (MU_CUTS && TAU_CUTS && OS && mT < 30 && (Event != Event_Double[1][1])) {
                                plotFill("muTau_visibleMass_mTLess30_OS_NOCorrection" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("muTau_visibleMass_mTLess30_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTLess30_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_Multiplicity" + ZCat[zcat] + index[icat], 0, 1, 0, 1);
                                //                                Event_Double[1][1] = Event;
                            }
                            if (MU_CUTS && TAU_CUTS && SS && mT < 30 && (Event != Event_Double[1][1])) {
                                //                                Event_Double[1][1] = Event;
                                plotFill("muTau_visibleMass_NOCorrection_mTLess30_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("muTau_visibleMass_mTLess30_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTLess30_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# Needed to Estimate WJets [need other BG to be subtracted]
                            if (MU_CUTS && TAU_CUTS && OS && mT > 70 && (Event != Event_Double[1][1])) {
                                //                                Event_Double[1][1] = Event;
                                plotFill("muTau_visibleMass_NOCorrection_mTHigher70_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("muTau_visibleMass_mTHigher70_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTHigher70_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# W Subtraction for QCD Normalization from data
                            if (MU_CUTS && TAU_CUTS && SS && mT > 70 && (Event != Event_Double[1][1])) {
                                //                                Event_Double[1][1] = Event;
                                plotFill("muTau_visibleMass_NOCorrection_mTHigher70_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("muTau_visibleMass_mTHigher70_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTHigher70_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }

                            //####################################################
                            //################# QCD Shape
                            //####################################################

                            if (MU_CUTS_Loose && TAU_CUTS && OS) {
                                plotFill("muTau_visibleMass_QCDshape_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_QCDshape_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            if (MU_CUTS_Loose && TAU_CUTS && SS) {
                                plotFill("muTau_visibleMass_QCDshape_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_QCDshape_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            if (MU_CUTS_Loose && TAU_CUTS && OS && mT < 30) {
                                plotFill("muTau_visibleMass_mTLess30_QCDshape_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTLess30_QCDshape_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            if (MU_CUTS_Loose && TAU_CUTS && SS && mT < 30) {
                                plotFill("muTau_visibleMass_mTLess30_QCDshape_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTLess30_QCDshape_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# Needed to Estimate WJets [need other BG to be subtracted]
                            if (MU_CUTS_Loose && TAU_CUTS && OS && mT > 70) {
                                plotFill("muTau_visibleMass_mTHigher70_QCDshape_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTHigher70_QCDshape_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# W Subtraction for QCD Normalization from data
                            if (MU_CUTS_Loose && TAU_CUTS && SS && mT > 70) {
                                plotFill("muTau_visibleMass_mTHigher70_QCDshape_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("muTau_SVMass_mTHigher70_QCDshape_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //####################################################


                        }
                    }
                } //check if category is passed
            } // loop over categories
        }

        //####################################################
        // ET FakeRateation
        //####################################################

        if (Channel == 3) {


            bool El_PtEta = l1Pt > 24 && fabs(l1Eta) < 2.1;
            bool El_IdTight = l1_eleId_Tight;
            bool El_Iso = l1_eleIso < 0.1;
            bool El_Iso_Loose = l1_eleIso > 0.2 && l1_eleIso < 0.5;
            bool EL_CUTS = El_PtEta && El_IdTight && El_Iso;
            bool EL_CUTS_Loose = El_PtEta && El_IdTight && El_Iso_Loose;

            bool Tau_PtEta = l2Pt > 30 && fabs(l2Eta) < 2.3;
            bool Tau_DMF = l2_DecayModeFinding;
            //            bool Tau_Isolation = byCombinedIsolationDeltaBetaCorrRaw3Hits_2 < 1.5;
            bool Tau_Isolation = l2_tauIsoMVA2T > 0.5;
            bool Tau_antiEl = l2_tauRejEleMVA3M;
            bool Tau_antiMu = l2_tauRejMu2L;
            bool TAU_CUTS = Tau_PtEta && Tau_DMF && Tau_Isolation && Tau_antiEl && Tau_antiMu;




            //Loop Over 3 Categories
            for (int icat = 0; icat < 4; icat++) {
                if (MSSM_Category[icat]) {
                    memset(Event_Double, 0, sizeof (Event_Double[0][0]) * 8 * 9);
                    for (int zcat = 0; zcat < 4; zcat++) {
                        if (Z_Category[zcat]) {


                            //####################################################
                            //#################  Selection for QCD Normalization from data
                            //################# Selection for QCD Normalization from data
                            //                 Yield from (sideband normalisation)*(fixed extrapolation factor) in each category.
                            //                 Sideband in data is ss && mT<30. Subtract contribution from all other background processes:
                            //                 ZTT, ZL, ZJ, W, TOP, VV. DYJets MC is used to estimate directly the ZTT contribution in
                            //                 this sideband. The W contribution similar to the default method above: normalisation is
                            //                 data sideband ss && mT>70, and extrapolation factor from mT>70 to mT<30 from WJets
                            //                 inclusive+njet samples using the category selection and ss events. The os/ss factor is 1.06.



                            //################# Signal Selectiopn
                            if (EL_CUTS && TAU_CUTS && OS && mT < 30 && (Event != Event_Double[2][1])) {
                                plotFill("eleTau_visibleMass_mTLess30_OS_NOCorrection" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("eleTau_visibleMass_mTLess30_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTLess30_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_Eleltiplicity" + ZCat[zcat] + index[icat], 0, 1, 0, 1);
                                //                                Event_Double[2][1] = Event;
                            }
                            if (EL_CUTS && TAU_CUTS && SS && mT < 30 && (Event != Event_Double[2][1])) {
                                //                                Event_Double[2][1] = Event;
                                plotFill("eleTau_visibleMass_NOCorrection_mTLess30_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("eleTau_visibleMass_mTLess30_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTLess30_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# Needed to Estimate WJets [need other BG to be subtracted]
                            if (EL_CUTS && TAU_CUTS && OS && mT > 70 && (Event != Event_Double[2][1])) {
                                //                                Event_Double[2][1] = Event;
                                plotFill("eleTau_visibleMass_NOCorrection_mTHigher70_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("eleTau_visibleMass_mTHigher70_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTHigher70_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# W Subtraction for QCD Normalization from data
                            if (EL_CUTS && TAU_CUTS && SS && mT > 70 && (Event != Event_Double[2][1])) {
                                //                                Event_Double[2][1] = Event;
                                plotFill("eleTau_visibleMass_NOCorrection_mTHigher70_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin);
                                plotFill("eleTau_visibleMass_mTHigher70_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTHigher70_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }

                            //####################################################
                            //################# QCD Shape
                            //####################################################

                            if (EL_CUTS_Loose && TAU_CUTS && OS) {
                                plotFill("eleTau_visibleMass_QCDshape_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_QCDshape_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            if (EL_CUTS_Loose && TAU_CUTS && SS) {
                                plotFill("eleTau_visibleMass_QCDshape_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_QCDshape_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            if (EL_CUTS_Loose && TAU_CUTS && OS && mT < 30) {
                                plotFill("eleTau_visibleMass_mTLess30_QCDshape_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTLess30_QCDshape_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            if (EL_CUTS_Loose && TAU_CUTS && SS && mT < 30) {
                                plotFill("eleTau_visibleMass_mTLess30_QCDshape_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTLess30_QCDshape_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# Needed to Estimate WJets [need other BG to be subtracted]
                            if (EL_CUTS_Loose && TAU_CUTS && OS && mT > 70) {
                                plotFill("eleTau_visibleMass_mTHigher70_QCDshape_OS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTHigher70_QCDshape_OS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //################# W Subtraction for QCD Normalization from data
                            if (EL_CUTS_Loose && TAU_CUTS && SS && mT > 70) {
                                plotFill("eleTau_visibleMass_mTHigher70_QCDshape_SS" + ZCat[zcat] + index[icat], mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                                plotFill("eleTau_SVMass_mTHigher70_QCDshape_SS" + ZCat[zcat] + index[icat], SVMass, high_bin, 0, high_bin, pu_Weight * eff_Correction);
                            }
                            //####################################################


                        }
                    }
                } //check if category is passed
            } // loop over categories


            //            if (EL_CUTS && TAU_CUTS && ElTau_Charge && (Event != Event_Double[2][1])) {
            //                plotFill("ETau_visibleMass_NOCorrection", mvis, high_bin, 0, high_bin);
            //                plotFill("ETau_visibleMass", mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
            //                plotFill("ETau_Multiplicity", 0, 1, 0, 1);
            //                Event_Double[2][1] = Event;
            //
            //            }
            //
            //            float mT = TMass_F(l1Pt, l1Px, l1Py, mvamet, mvametphi);
            //            if (EL_CUTS && TAU_CUTS && ElTau_Charge && mT < 30 && (Event != Event_Double[2][2])) {
            //                plotFill("ETau_visibleMass_NOCorrection", mvis, high_bin, 0, high_bin);
            //                plotFill("ETau_visibleMass", mvis, high_bin, 0, high_bin, pu_Weight * eff_Correction);
            //                Event_Double[2][2] = Event;
            //            }



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

