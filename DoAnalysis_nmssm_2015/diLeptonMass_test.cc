#include <string>
#include <ostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "math.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TInterpreter.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFile.h"
#include <map>
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TFrame.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <utility>
//#include "array.h"
#include <iostream>
#include "TLorentzVector.h"



int Channel = 0;
int Run = 0;
int Lumi = 0;
int Event = 0;

float l1M, l1Px, l1Py, l1Pz, l1E, l1Pt, l1Phi, l1Eta, l1Charge, l1_muIso, l1_eleIso ;
float l2M, l2Px, l2Py, l2Pz, l2E, l2Pt, l2Phi, l2Eta, l2Charge, l2_muIso, l2_eleIso;



int main() {
    
    using namespace std;
    
    TFile *fout = TFile::Open("OutFile.root", "RECREATE");
    
    TH1F * myhist=new TH1F("inVarMass","inVarMass",200,0,200);
    
    TFile *file_input = new TFile("../FileROOT/MSSMROOTFiles/DYJetsToLL_8TeV.root");
    
    TTree *Run_Tree = (TTree*) file_input->Get("InfoTree");
    
    
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(20);
    
    Run_Tree->SetBranchAddress("Channel", &Channel);
    Run_Tree->SetBranchAddress("run", &Run);
    Run_Tree->SetBranchAddress("lumi", &Lumi);
    Run_Tree->SetBranchAddress("evt", &Event);
    
    
    Run_Tree->SetBranchAddress("m_1", &l1M);
    Run_Tree->SetBranchAddress("E_1", &l1E);
    Run_Tree->SetBranchAddress("px_1", &l1Px);
    Run_Tree->SetBranchAddress("py_1", &l1Py);
    Run_Tree->SetBranchAddress("pz_1", &l1Pz);
    Run_Tree->SetBranchAddress("pt_1", &l1Pt);
    Run_Tree->SetBranchAddress("eta_1", &l1Eta);
    Run_Tree->SetBranchAddress("phi_1", &l1Phi);
    Run_Tree->SetBranchAddress("q_1", &l1Charge);
    
    
    Run_Tree->SetBranchAddress("m_2", &l2M);
    Run_Tree->SetBranchAddress("e_2", &l2E);
    Run_Tree->SetBranchAddress("px_2", &l2Px);
    Run_Tree->SetBranchAddress("py_2", &l2Py);
    Run_Tree->SetBranchAddress("pz_2", &l2Pz);
    Run_Tree->SetBranchAddress("pt_2", &l2Pt);
    Run_Tree->SetBranchAddress("eta_2", &l2Eta);
    Run_Tree->SetBranchAddress("phi_2", &l2Phi);
    Run_Tree->SetBranchAddress("q_2", &l2Charge);
    
    
    //###############################################################################################
    //Loop over all events
    //###############################################################################################
    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    
    
    for (Int_t i = 0; i < nentries_wtn; i++) {
        Run_Tree->GetEntry(i);
        
        
        if (i % 10000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
        fflush(stdout);
        
        
        
        
        double inVarMass=0;
        inVarMass=sqrt(   pow((l1E+l2E),2 )- pow((l1Px + l2Px),2) - pow((l1Py + l2Py),2) -pow((l1Pz + l2Pz),2) );
        
        
        if ( l1Charge * l2Charge < 0   && l1Pt > 20 && l2Pt > 30){
            cout<< "Muon charge= "<<  l1Charge  <<  "  Tau Charge" << l2Charge  <<"    InvarMass is = "<<inVarMass<<"\n";
            
            myhist->Fill(inVarMass);
            
        }
        
        
        
    }
    
    
    fout->cd();
    myhist->Write();
    fout->Close();
}




