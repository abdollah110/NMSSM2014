#include "TTree.h"
#include "TFile.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

/**
   \class nsvfitStandalone nsvfitStandalone.cc "TauAnalysis/CandidateTools/bin/nsvfitStandalone.cc"
   \brief Basic example of the use of the standalone version of NSVfit

   This is an example executable to show the use of the standalone version of NSVfit form a flat
   n-tuple or single event.
 */

void singleEvent() {
    /*
       This is a single event for testing in the integration mode.
     */
    // define MET
    Vector MET(11.7491, -51.9172, 0.);
    // define MET covariance
    TMatrixD covMET(2, 2);
    /*
    covMET[0][0] = 0.;
    covMET[1][0] = 0.;
    covMET[0][1] = 0.;
    covMET[1][1] = 0.;
     */
    covMET[0][0] = 787.352;
    covMET[1][0] = -178.63;
    covMET[0][1] = -178.63;
    covMET[1][1] = 179.545;
    // define lepton four vectors
    NSVfitStandalone::LorentzVector l3(28.9132, -17.3888, 36.6411, 49.8088); //lepton
    NSVfitStandalone::LorentzVector l4(-24.19, 8.77449, 16.9413, 30.8086); //tau
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l4));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l3));
    // define algorithm (set the debug level to 3 for testing)
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, MET, covMET, /*debug=*/0);
    algo.addLogM(false);
    /*
       the following lines show how to use the different methods on a single event
     */
    // minuit fit method
    //algo.fit();
    // integration by VEGAS (same as function algo.integrate() that has been in use when markov chain integration had not yet been implemented)
    //algo.integrateVEGAS();
    // integration by markov chain MC
    algo.integrateVEGAS();

    double mass = algo.getMass(); // mass uncertainty not implemented yet
    if (algo.isValidSolution()) {
        std::cout << "found mass    = " << mass << std::endl;
    } else {
        std::cout << "sorry -- status of NLL is not valid [" << algo.isValidSolution() << "]" << std::endl;
    }
    return;
}

void eventsFromTree(int argc, char* argv[]) {
    // parse arguments
    //    if (argc < 3) {
    //        std::cout << "Usage : " << argv[0] << " [inputfile.root] [tree_name]" << std::endl;
    //        return;
    //    }
    // get intput directory up to one before mass points
    TFile* file = new TFile(argv[4]);
    //######################//######################//######################//######################
    //Writing a TREE
    //######################
    // access tree in file
    TTree* tree = (TTree*) file->Get("BG_Tree");
    // input variables
    float met, metPhi;
    float covMet11, covMet12;
    float covMet21, covMet22;
    float l3M, l3Px, l3Py, l3Pz, l3E;
    float l4M, l4Px, l4Py, l4Pz, L4E;
    float SVmass = 0;
    float SVmassUnc = 0;
    int Channel, subChannel, Number;
    float scale;
    int Run, Lumi, Event;
    float mysvmass;
    float eff_Correction, pu_Weight;

    // branch adresses
    tree->SetBranchAddress("SVMass_", &mysvmass);
    tree->SetBranchAddress("met_", &met);
    tree->SetBranchAddress("metPhi_", &metPhi);
    tree->SetBranchAddress("covMet11_", &covMet11);
    tree->SetBranchAddress("covMet12_", &covMet12);
    tree->SetBranchAddress("covMet21_", &covMet21);
    tree->SetBranchAddress("covMet22_", &covMet22);
    tree->SetBranchAddress("l3M_", &l3M);
    tree->SetBranchAddress("l3Px_", &l3Px);
    tree->SetBranchAddress("l3Py_", &l3Py);
    tree->SetBranchAddress("l3Pz_", &l3Pz);
    tree->SetBranchAddress("l4M_", &l4M);
    tree->SetBranchAddress("l4Px_", &l4Px);
    tree->SetBranchAddress("l4Py_", &l4Py);
    tree->SetBranchAddress("l4Pz_", &l4Pz);
    tree->SetBranchAddress("Channel_", &Channel);
    tree->SetBranchAddress("subChannel_", &subChannel);
    tree->SetBranchAddress("Run_", &Run);
    tree->SetBranchAddress("Lumi_", &Lumi);
    tree->SetBranchAddress("Event_", &Event);
    tree->SetBranchAddress("eff_Correction_", &eff_Correction);
    tree->SetBranchAddress("pu_Weight_", &pu_Weight);

    //######################//######################//######################//######################
    //Writing a TREE
    //######################
    int Run_;
    int Lumi_;
    int Event_, Channel_;
    float met_, metPhi_;
    float covMet11_, covMet12_;
    float covMet21_, covMet22_;
    float l3M_, l3Px_, l3Py_, l3Pz_;
    float l4M_, l4Px_, l4Py_, l4Pz_;
    float eff_Correction_, pu_Weight_;
    float HMass_Markov;

    //New Trre Branch for Mass & Mass Uncertainty
    TTree *Mass_Tree = new TTree("Mass_tree", "Mass_tree");
    //    std::string m_name= "SVmass" + str(argv[2],1)
    Mass_Tree->Branch("SVmass", &SVmass, "SVmass/F");
    //    Mass_Tree->Branch("SVmass", &SVmass, "SVmass/F");
    Mass_Tree->Branch("SVmassUnc", &SVmassUnc, "SVmassUnc/F");
    Mass_Tree->Branch("HMass_Markov", &HMass_Markov, "HMass_Markov/F");
    Mass_Tree->Branch("met_", &met_, "met_/F");
    Mass_Tree->Branch("metPhi_", &metPhi_, "metPhi_/F");
    Mass_Tree->Branch("covMet11_", &covMet11_, "covMet11_/F");
    Mass_Tree->Branch("covMet12_", &covMet12_, "covMet12_/F");
    Mass_Tree->Branch("covMet21_", &covMet21_, "covMet21_/F");
    Mass_Tree->Branch("covMet22_", &covMet22_, "covMet22_/F");
    Mass_Tree->Branch("covMet22_", &covMet22_, "covMet22_/F");
    Mass_Tree->Branch("l3M_", &l3M_, "l3M_/F");
    Mass_Tree->Branch("l3Px_", &l3Px_, "l3Px_/F");
    Mass_Tree->Branch("l3Py_", &l3Py_, "l3Py_/F");
    Mass_Tree->Branch("l3Pz_", &l3Pz_, "l3Pz_/F");
    Mass_Tree->Branch("l4M_", &l4M_, "l4M_/F");
    Mass_Tree->Branch("l4Px_", &l4Px_, "l4Px_/F");
    Mass_Tree->Branch("l4Py_", &l4Py_, "l4Py_/F");
    Mass_Tree->Branch("l4Pz_", &l4Pz_, "l4Pz_/F");
    Mass_Tree->Branch("Run_", &Run_, "Run_/I");
    Mass_Tree->Branch("Lumi_", &Lumi_, "Lumi_/I");
    Mass_Tree->Branch("Event_", &Event_, "Event_/I");
    Mass_Tree->Branch("Chanel_", &Channel_, "Channel_/I");
    Mass_Tree->Branch("eff_Correction_", &eff_Correction_, "eff_Correction_/F");
    Mass_Tree->Branch("pu_Weight_", &pu_Weight_, "pu_Weight_/F");


    //######################//######################//######################//######################

    TFile* fileOut = new TFile(argv[3], "RECREATE");
    Number = int(atof(argv[1]));
    scale = float(atof(argv[2]));

    int nevent = tree->GetEntries();
    //    for (int i = 0; i < 20; ++i) {
    for (int i = 0; i < nevent; ++i) {
        tree->GetEvent(i);
        //        std::cout << "event " << i + 1 <<" ";
        // setup MET input vector

        SVmass = 0;
        SVmassUnc = 0;

        // setup the MET significance
        TMatrixD covMET(2, 2);
        covMET[0][0] = covMet11;
        covMET[0][1] = covMet12;
        covMET[1][0] = covMet21;
        covMET[1][1] = covMet22;
        // setup measure tau lepton vectors
        NSVfitStandalone::LorentzVector l3(l3Px, l3Py, l3Pz, TMath::Sqrt(l3M * l3M + l3Px * l3Px + l3Py * l3Py + l3Pz * l3Pz));
        NSVfitStandalone::LorentzVector l4(l4Px, l4Py, l4Pz, TMath::Sqrt(l4M * l4M + l4Px * l4Px + l4Py * l4Py + l4Pz * l4Pz));
        std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;

        //    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kLepDecay, l3));
        //    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, l4));

        if (Channel == Number) {
            //std::cout << "\n";
            //Fully Hadronic
            if (subChannel == 3 && (Channel == 1 || Channel == 5 || Channel == 91 || Channel == 95)) {

                NSVfitStandalone::Vector measuredMET(met * TMath::Cos(metPhi) + l3Px * (-1 * scale) + l4Px * (-1 * scale), met * TMath::Sin(metPhi) + l3Py * (-1 * scale) + l4Py * (-1 * scale), 0);

                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l3 * (1 + scale)));
                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l4 * (1 + scale)));

                // construct the class object from the minimal necesarry information
                NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
                // apply customized configurations if wanted (examples are given below)
                algo.maxObjFunctionCalls(5000);
                //algo.addLogM(false);
                //algo.metPower(0.5)
                // run the fit
                algo.addLogM(false);
                algo.integrateVEGAS();

                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                }


                met_ = met;
                metPhi_ = metPhi;
                covMet11_ = covMet11;
                covMet12_ = covMet12;
                covMet21_ = covMet21;
                covMet22_ = covMet22;
                l3M_ = l3M;
                l3Px_ = l3Px;
                l3Py_ = l3Py;
                l3Pz_ = l3Pz;
                l4M_ = l4M;
                l4Px_ = l4Px;
                l4Py_ = l4Py;
                l4Pz_ = l4Pz;
                HMass_Markov = mysvmass;
                Run_ = Run;
                Lumi_ = Lumi;
                Event_ = Event;
                Channel_ = Channel;
                eff_Correction_ = eff_Correction;
                pu_Weight_ = pu_Weight;

                Mass_Tree->Fill();
                //std::cout << in_run2 << " " << in_event2 << " " << in_svMass << " " 
                //		 << in_px_H1 << " " << in_py_H1 << " " << in_pz_H1 << " " << in_E_H1
                //		 << " " << in_px_H2 << " " << in_py_H2 << " " << in_pz_H2 << " " << in_E_H2
                //		 << " " << in_MET_x << " " << in_MET_y << " "
                //		 << in_covMET_00 << " " << in_covMET_01 << " " << in_covMET_10 << " " << in_covMET_11
                //		 << std::endl;
                //                    std::cout<<"   l3Px="<<l3Px<<"   l3Py="<<l3Py<<"   l3Px="<<l3Pz<<"   l3M="<<l3M<<"\n";
                //                std::cout<<"   l4Px="<<l4Px<<"   l4Py="<<l4Py<<"   l3Px="<<l4Pz<<"   l3M="<<l4M<<"\n";
                //             	std::cout<<"   covMet11="<<covMet11<<"   covMet12="<<covMet12<<"   covMet21="<<covMet21<<"   covMet22="<<covMet22<<"\n";
                //             	std::cout<<"   met="<<met<<"   metPhi="<<metPhi<<"\n";
            }

            //Semi-Leptonic
            if (subChannel == 3 && (Channel == 2 || Channel == 3 || Channel == 6 || Channel == 7 || Channel == 92 || Channel == 93 || Channel == 96 || Channel == 97)) {

                NSVfitStandalone::Vector measuredMET(met * TMath::Cos(metPhi) + l4Px * (-1 * scale), met * TMath::Sin(metPhi) + l4Py * (-1 * scale), 0);

                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l3));
                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l4 * (1 + scale)));

                NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
                algo.maxObjFunctionCalls(5000);
                algo.addLogM(false);
                algo.integrateVEGAS();
                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                }

                met_ = met;
                metPhi_ = metPhi;
                covMet11_ = covMet11;
                covMet12_ = covMet12;
                covMet21_ = covMet21;
                covMet22_ = covMet22;
                l3M_ = l3M;
                l3Px_ = l3Px;
                l3Py_ = l3Py;
                l3Pz_ = l3Pz;
                l4M_ = l4M;
                l4Px_ = l4Px;
                l4Py_ = l4Py;
                l4Pz_ = l4Pz;
                HMass_Markov = mysvmass;
                Run_ = Run;
                Lumi_ = Lumi;
                Event_ = Event;
                Channel_ = Channel;
                eff_Correction_ = eff_Correction;
                pu_Weight_ = pu_Weight;
                Mass_Tree->Fill();
                //                std::cout<<"   l3Px="<<l3Px<<"   l3Py="<<l3Py<<"   l3Px="<<l3Pz<<"   l3M="<<l3M<<"\n";
                //                std::cout<<"   l4Px="<<l4Px<<"   l4Py="<<l4Py<<"   l3Px="<<l4Pz<<"   l3M="<<l4M<<"\n";
                //             	std::cout<<"   covMet11="<<covMet11<<"   covMet12="<<covMet12<<"   covMet21="<<covMet21<<"   covMet22="<<covMet22<<"\n";
                //             	std::cout<<"   met="<<met<<"   metPhi="<<metPhi<<"\n";
            }

            //fully Leptonic
            if (subChannel == 3 && (Channel == 4 || Channel == 8 || Channel == 94 || Channel == 98)) {

                NSVfitStandalone::Vector measuredMET(met * TMath::Cos(metPhi), met * TMath::Sin(metPhi), 0);

                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l3));
                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l4));

                NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
                algo.maxObjFunctionCalls(5000);
                algo.addLogM(false);
                algo.integrateVEGAS();
                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                }

                met_ = met;
                metPhi_ = metPhi;
                covMet11_ = covMet11;
                covMet12_ = covMet12;
                covMet21_ = covMet21;
                covMet22_ = covMet22;
                l3M_ = l3M;
                l3Px_ = l3Px;
                l3Py_ = l3Py;
                l3Pz_ = l3Pz;
                l4M_ = l4M;
                l4Px_ = l4Px;
                l4Py_ = l4Py;
                l4Pz_ = l4Pz;
                HMass_Markov = mysvmass;
                Run_ = Run;
                Lumi_ = Lumi;
                Event_ = Event;
                Channel_ = Channel;
                eff_Correction_ = eff_Correction;
                pu_Weight_ = pu_Weight;
                Mass_Tree->Fill();
                //		    std::cout<<"   l3Px="<<l3Px<<"   l3Py="<<l3Py<<"   l3Px="<<l3Pz<<"   l3M="<<l3M<<"\n";
                //                std::cout<<"   l4Px="<<l4Px<<"   l4Py="<<l4Py<<"   l3Px="<<l4Pz<<"   l3M="<<l4M<<"\n";
                //             	std::cout<<"   covMet11="<<covMet11<<"   covMet12="<<covMet12<<"   covMet21="<<covMet21<<"   covMet22="<<covMet22<<"\n";
                //             	std::cout<<"   met="<<met<<"   metPhi="<<metPhi<<"\n";
            }
        }//end of check chanel with number

        // retrieve the results upon success
        //        std::cout << "... m truth : " << mTrue << std::endl;
        //##############################################
        //        std::cout << "\nChannle = " << Channel << "-------------------- SVmass==== " << SVmass << "\n";
        //Mass_Tree->Fill();
        //##############################################
        //            std::cout << "... m svfit : " << algo.mass() << "+/-" << algo.massUncert() << std::endl;
        //            double diTauMass = algo.getMass();
        //            double diTauMassErr = algo.getMassUncert(); // mass uncertainty and Pt of Z/Higgs are new features of the Markov Chain integration
        //            double diTauPt = algo.getPt();
        //            double diTauPtErr = algo.getPtUncert();

    }
    fileOut->cd();
    Mass_Tree->Write();

    return;
}

int main(int argc, char* argv[]) {
    eventsFromTree(argc, argv);
    //  singleEvent();
    return 0;
}
