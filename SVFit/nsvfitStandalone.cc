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
    NSVfitStandalone::LorentzVector l1(28.9132, -17.3888, 36.6411, 49.8088); //lepton
    NSVfitStandalone::LorentzVector l2(-24.19, 8.77449, 16.9413, 30.8086); //tau
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l2));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l1));
    // define algorithm (set the debug level to 3 for testing)
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, MET, covMET, /*debug=*/0);
    algo.addLogM(false);
    /*
       the following lines show how to use the different mvamethods on a single event
     */
    // minuit fit mvamethod
    //algo.fit();
    // integration by VEGAS (same as function algo.integrate() that has been in use when markov chain integration had not yet been implemented)
    //algo.integrateVEGAS();
    // integration by markov chain MC
    //    algo.integrateMarkovChain();
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
    // access tree in file
    TTree* tree = (TTree*) file->Get("InfoTree");
    // input variables
    float mvamet, mvametphi;
    float mvacov00, mvacov01;
    float mvacov10, mvacov11;
    float m_1, px_1, py_1, pz_1;
    float m_2, px_2, py_2, pz_2;
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
    tree->SetBranchAddress("mvamet", &mvamet);
    tree->SetBranchAddress("mvametphi", &mvametphi);
    tree->SetBranchAddress("mvacov00", &mvacov00);
    tree->SetBranchAddress("mvacov01", &mvacov01);
    tree->SetBranchAddress("mvacov10", &mvacov10);
    tree->SetBranchAddress("mvacov11", &mvacov11);
    tree->SetBranchAddress("m_1", &m_1);
    tree->SetBranchAddress("px_1", &px_1);
    tree->SetBranchAddress("py_1", &py_1);
    tree->SetBranchAddress("pz_1", &pz_1);
    tree->SetBranchAddress("m_2", &m_2);
    tree->SetBranchAddress("px_2", &px_2);
    tree->SetBranchAddress("py_2", &py_2);
    tree->SetBranchAddress("pz_2", &pz_2);
    //    tree->SetBranchAddress("m_true", &mTrue);
    tree->SetBranchAddress("Channel", &Channel);

    int nevent = tree->GetEntries();
    for (int i = 0; i < nevent; ++i) {
        tree->GetEvent(i);

        SVmass = 0;
        SVmassUnc = 0;

        // setup the MET significance
        TMatrixD covMET(2, 2);
        covMET[0][0] = mvacov00;
        covMET[0][1] = mvacov01;
        covMET[1][0] = mvacov10;
        covMET[1][1] = mvacov11;
        // setup measure tau lepton vectors
        NSVfitStandalone::LorentzVector l1(px_1, py_1, pz_1, TMath::Sqrt(m_1 * m_1 + px_1 * px_1 + py_1 * py_1 + pz_1 * pz_1));
        NSVfitStandalone::LorentzVector l2(px_2, py_2, pz_2, TMath::Sqrt(m_2 * m_2 + px_2 * px_2 + py_2 * py_2 + pz_2 * pz_2));
        std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;

        //    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kLepDecay, l1));
        //    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, l2));

        if (Channel == Number) {
            std::cout << "\n";
            //Semi-Leptonic  MuTau
            if (Channel == 1) {

                NSVfitStandalone::Vector measuredMET(mvamet * TMath::Cos(mvametphi) + px_2 * (-1 * scale), mvamet * TMath::Sin(mvametphi) + py_2 * (-1 * scale), 0);

                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l1));
                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l2 * (1 + scale)));

                NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
                algo.maxObjFunctionCalls(5000);
                algo.addLogM(false);
                //                algo.integrateMarkovChain();
                algo.integrateVEGAS();
                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                    std::cout << "... m  : " << algo.mass() << "  +-  " << algo.massUncert() << std::endl;
                }
            }
            //Semi-Leptonic  EleTau
            if (Channel == 3  ) {

                NSVfitStandalone::Vector measuredMET(mvamet * TMath::Cos(mvametphi) + px_2 * (-1 * scale), mvamet * TMath::Sin(mvametphi) + py_2 * (-1 * scale), 0);

                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l1));
                measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l2 * (1 + scale)));

                NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
                algo.maxObjFunctionCalls(5000);
                algo.addLogM(false);
                //                algo.integrateMarkovChain();
                algo.integrateVEGAS();
                if (algo.isValidSolution()) {
                    SVmass = algo.mass();
                    SVmassUnc = algo.massUncert();
                    std::cout << "... m  : " << algo.mass() << "  +-  " << algo.massUncert() << std::endl;
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
