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
    algo.integrateMarkovChain();

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
    // access tree in file
    TTree* tree = (TTree*) file->Get("RLE_tree");
    // input variables
    float met, metPhi;
    float covMet11, covMet12;
    float covMet21, covMet22;
    float l3M, l3Px, l3Py, l3Pz;
    float l4M, l4Px, l4Py, l4Pz;
    float SVmass = 0;
    float SVmassUnc = 0;
    int Channel, Number;
    float scale;

    TFile* fileOut = new TFile(argv[3], "RECREATE");
    Number = int(atof(argv[1]));
    scale = float(atof(argv[2]));

    //New Trre Branch for Mass & Mass Uncertainty
    TTree *Mass_Tree = new TTree("Mass_tree", "Mass_tree");
    //    std::string m_name= "SVmass" + str(argv[2],1)
    Mass_Tree->Branch("SVmass", &SVmass, "SVmass/F");
    //    Mass_Tree->Branch("SVmass", &SVmass, "SVmass/F");
    Mass_Tree->Branch("SVmassUnc", &SVmassUnc, "SVmassUnc/F");


    // branch adresses
    tree->SetBranchAddress("met", &met);
    tree->SetBranchAddress("metPhi", &metPhi);
    tree->SetBranchAddress("covMet11", &covMet11);
    tree->SetBranchAddress("covMet12", &covMet12);
    tree->SetBranchAddress("covMet21", &covMet21);
    tree->SetBranchAddress("covMet22", &covMet22);
    tree->SetBranchAddress("l3M", &l3M);
    tree->SetBranchAddress("l3Px", &l3Px);
    tree->SetBranchAddress("l3Py", &l3Py);
    tree->SetBranchAddress("l3Pz", &l3Pz);
    tree->SetBranchAddress("l4M", &l4M);
    tree->SetBranchAddress("l4Px", &l4Px);
    tree->SetBranchAddress("l4Py", &l4Py);
    tree->SetBranchAddress("l4Pz", &l4Pz);
    //    tree->SetBranchAddress("m_true", &mTrue);
    tree->SetBranchAddress("Channel", &Channel);

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
            std::cout << "\n";
            //Fully Hadronic
            if (Channel == 1 || Channel == 5 || Channel == 91 || Channel == 95) {

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
                algo.integrateMarkovChain();

                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                }
            }

            //Semi-Leptonic
            if (Channel == 2 || Channel == 3 || Channel == 6 || Channel == 7 || Channel == 92 || Channel == 93 || Channel == 96 || Channel == 97) {

                NSVfitStandalone::Vector measuredMET(met * TMath::Cos(metPhi) + l4Px * (-1 * scale), met * TMath::Sin(metPhi) + l4Py * (-1 * scale), 0);

                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l3));
                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay,  l4 * (1 + scale)));

                NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
                algo.maxObjFunctionCalls(5000);
                algo.addLogM(false);
                algo.integrateMarkovChain();
                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                }
            }

            //fully Leptonic
            if (Channel == 4 || Channel == 8 || Channel == 94 || Channel == 98) {

                NSVfitStandalone::Vector measuredMET(met * TMath::Cos(metPhi), met * TMath::Sin(metPhi), 0);

                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l3));
                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l4));

                NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
                algo.maxObjFunctionCalls(5000);
                algo.addLogM(false);
                algo.integrateMarkovChain();
                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                }

            }
        }//end of check chanel with number

        // retrieve the results upon success
        //        std::cout << "... m truth : " << mTrue << std::endl;
        //##############################################
        //        std::cout << "\nChannle = " << Channel << "-------------------- SVmass==== " << SVmass << "\n";
        Mass_Tree->Fill();
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
