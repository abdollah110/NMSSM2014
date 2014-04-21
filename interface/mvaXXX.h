/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "/Users/abdollahmohammadi/Downloads/root/tmva/test/TMVAGui.C"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

//#if not defined(__CINT__) || defined(__MAKECINT__)
//#endif

using namespace TMVA;

Float_t N_LT, N_IsoTau1, N_IsoTau2, N_IsoTot, N_MET;

float mvaXXX(TMVA::Reader *reader, float val_N_LT = 20, float val_N_MET = 20, float val_N_IsoTau1 = 20, float val_N_IsoTau2 = 20) {
    //#ifdef __CINT__
    gROOT->ProcessLine(".O0"); // turn off optimization in CINT
    //#endif

    //---------------------------------------------------------------

    // This loads the library
    //    TMVA::Tools::Instance();


    //    std::cout << "==> Start TMVAClassificationApplication" << std::endl;

    // --- Create the Reader object




    N_LT = val_N_LT;
    N_MET = val_N_MET;
    N_IsoTau1 = val_N_IsoTau1;
    N_IsoTau2 = val_N_IsoTau2;


    //    return 88;
    return reader->EvaluateMVA("BDT method");

    //    delete reader;

}
