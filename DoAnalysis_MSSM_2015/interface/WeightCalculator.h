#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TSystem.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include <TLorentzVector.h>
using namespace std;



vector <float> W_EvenetMultiplicity(){
    vector<float> W_events;
    W_events.clear();
    TFile * myFile_W0 = new TFile("../FileROOT/MSSMROOTFiles/WJetsToLNu_8TeV.root");
    TH1F * Histo_W0 = (TH1F*) myFile_W0->Get("TotalEventsNumber");
    W_events.push_back(Histo_W0->Integral());
    
    TFile * myFile_W1 = new TFile("../FileROOT/MSSMROOTFiles/W1JetsToLNu_8TeV.root");
    TH1F * Histo_W1 = (TH1F*) myFile_W1->Get("TotalEventsNumber");
    W_events.push_back(Histo_W1->Integral());
    
    TFile * myFile_W2 = new TFile("../FileROOT/MSSMROOTFiles/W2JetsToLNu_8TeV.root");
    TH1F * Histo_W2 = (TH1F*) myFile_W2->Get("TotalEventsNumber");
    W_events.push_back(Histo_W2->Integral());
    
    TFile * myFile_W3 = new TFile("../FileROOT/MSSMROOTFiles/W3JetsToLNu_8TeV.root");
    TH1F * Histo_W3 = (TH1F*) myFile_W3->Get("TotalEventsNumber");
    W_events.push_back(Histo_W3->Integral());
    
    TFile * myFile_W4 = new TFile("../FileROOT/MSSMROOTFiles/W4JetsToLNu_8TeV.root");
    TH1F * Histo_W4 = (TH1F*) myFile_W4->Get("TotalEventsNumber");
    W_events.push_back(Histo_W4->Integral());
    
    return W_events ;
    
}

vector <float> DY_EvenetMultiplicity(){
    vector<float> DY_events;
    DY_events.clear();
    TFile * myFile_DY0 = new TFile("../FileROOT/MSSMROOTFiles/DYJetsToLL_8TeV.root");
//    TFile * myFile_DY0 = new TFile("../FileROOT/MSSMROOTFiles/DYJetsToLL_PolarOff_8TeV.root");
    TH1F * Histo_DY0 = (TH1F*) myFile_DY0->Get("TotalEventsNumber");
    DY_events.push_back(Histo_DY0->Integral());
    
    TFile * myFile_DY1 = new TFile("../FileROOT/MSSMROOTFiles/DY1JetsToLL_8TeV.root");
    TH1F * Histo_DY1 = (TH1F*) myFile_DY1->Get("TotalEventsNumber");
    DY_events.push_back(Histo_DY1->Integral());
    
    TFile * myFile_DY2 = new TFile("../FileROOT/MSSMROOTFiles/DY2JetsToLL_8TeV.root");
    TH1F * Histo_DY2 = (TH1F*) myFile_DY2->Get("TotalEventsNumber");
    DY_events.push_back(Histo_DY2->Integral());
    
    TFile * myFile_DY3 = new TFile("../FileROOT/MSSMROOTFiles/DY3JetsToLL_8TeV.root");
    TH1F * Histo_DY3 = (TH1F*) myFile_DY3->Get("TotalEventsNumber");
    DY_events.push_back(Histo_DY3->Integral());
    
    TFile * myFile_DY4 = new TFile("../FileROOT/MSSMROOTFiles/DY4JetsToLL_8TeV.root");
    TH1F * Histo_DY4 = (TH1F*) myFile_DY4->Get("TotalEventsNumber");
    DY_events.push_back(Histo_DY4->Integral());
    
    return DY_events ;
    
}


float XSection(std::string OutName) {

    
    
    float TOPXS= 241.5;
    float TOPCorr = 1.033;
 
    
    
    
    //WJet       float XSection_W[numBG] = {30400, 5400, 1750, 519, 214};
    if (OutName.compare("WJetsToLNu") == 0) return 30400;
    else if (OutName.compare("W1JetsToLNu") == 0) return 5400;
    else if (OutName.compare("W2JetsToLNu") == 0) return 1750;
    else if (OutName.compare("W3JetsToLNu") == 0) return 519;
    else if (OutName.compare("W4JetsToLNu") == 0) return 214;
    
    //DYJets   float XSection_DY[numBG] = {2950, 561, 181, 51.1, 23};
    
    else if (OutName.compare("DYJetsToLL") == 0) return 2950;
    else if (OutName.compare("DY1JetsToLL") == 0) return 561;
    else if (OutName.compare("DY2JetsToLL") == 0) return 181;
    else if (OutName.compare("DY3JetsToLL") == 0) return 51.1;
    else if (OutName.compare("DY4JetsToLL") == 0) return 23;


    else if (OutName.compare("signal") == 0) return 1.;

        //Di-boson
    else if (OutName.compare("WWJetsTo2L2Nu") == 0) return 5.824;
    else if (OutName.compare("WZJetsTo2L2Q") == 0) return 2.207;
    else if (OutName.compare("WZJetsTo3LNu") == 0) return 1.058;
    else if (OutName.compare("ZZJetsTo2L2Nu") == 0) return 0.716;
    else if (OutName.compare("ZZJetsTo2L2Q") == 0) return 2.502;
    else if (OutName.compare("ZZJetsTo4L") == 0) return 0.181;
    else if (OutName.compare("Tbar_tW") == 0) return 11.1;
    else if (OutName.compare("T_tW") == 0) return 11.1;

        //TTbar
    else if (OutName.compare("TTJets_FullLeptMGDecays") == 0) return 0.1050 * TOPXS * TOPCorr;
    else if (OutName.compare("TTJets_SemiLeptMGDecays") == 0) return 0.4380 * TOPXS * TOPCorr;
    else if (OutName.compare("TTJets_HadronicMGDecays") == 0) return 0.4570 * TOPXS * TOPCorr;




    else if (OutName.compare("DYJetsToLL_PolarOff") == 0) return 2950 * 1.187694915;

    else if (OutName.compare("ggH_SM125") == 0) return 1.23;
    else if (OutName.compare("qqH_SM125") == 0) return 0.100;
    else if (OutName.compare("VH_SM125") == 0) return 0.077;
    else if (OutName.compare("TTEmbeddedmutau") == 0) return (26.229 * 792835 / 12011428);
    else if (OutName.compare("TTEmbeddedetau") == 0) return (26.229 * 758691 / 12011428);


    else return 1;

}

float weightCalc(TH1F *Histo, std::string outputName, int njet, int mcdata, vector<float> W_events, vector<float> DY_events) {

    
    if (mcdata == 2 || mcdata==4 || mcdata==5) return 1;   //  This is for data or embedded data
    
    float LOtoNLO_DY = 1.187694915;
    float LOtoNLO_W = 1.233848684;
    float luminosity=19700;

    std::string FirstPart = "../FileROOT/MSSMROOTFiles/";
    std::string LastPart = "_8TeV.root";
    std::string newOut = outputName.substr(FirstPart.size());
    newOut = newOut.substr(0, newOut.size() - LastPart.size());

    size_t isW = outputName.find("JetsToLNu");
    size_t isDY = outputName.find("JetsToLL");
    size_t Signal_ggH = outputName.find("ggH");
    size_t Signal_bbH = outputName.find("bbH");
    size_t bg_SM = outputName.find("SM125");
    size_t bg_PolarOff = outputName.find("PolarOff");
//    cout<<" XSection(newOut) = "<<XSection(newOut) << "  Histo->Integral()"<<Histo->Integral()<<"\n";
    
    if (bg_PolarOff != string::npos){
        return luminosity * XSection(newOut)*1.0 / Histo->Integral();
    }
    else if (isW != string::npos) {
        if (njet == 0) return luminosity*LOtoNLO_W / (W_events[0] / XSection("WJetsToLNu"));
        else if (njet == 1) return luminosity*LOtoNLO_W / (W_events[1] / XSection("W1JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
        else if (njet == 2) return luminosity*LOtoNLO_W / (W_events[2] / XSection("W2JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
        else if (njet == 3) return luminosity*LOtoNLO_W / (W_events[3] / XSection("W3JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
        else if (njet == 4) return luminosity*LOtoNLO_W / (W_events[4] / XSection("W4JetsToLNu") + W_events[0] / XSection("WJetsToLNu"));
        else   {cout<<"**********   wooow  ********* There is a problem here\n";return 0;}
        

    } else if (isDY != string::npos) {
       
        if (njet == 0) return luminosity*LOtoNLO_DY / (DY_events[0] / XSection("DYJetsToLL"));
        else if (njet == 1) return luminosity*LOtoNLO_DY / (DY_events[1] / XSection("DY1JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
        else if (njet == 2) return luminosity*LOtoNLO_DY / (DY_events[2] / XSection("DY2JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
        else if (njet == 3) return luminosity*LOtoNLO_DY / (DY_events[3] / XSection("DY3JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
        else if (njet == 4) return luminosity*LOtoNLO_DY / (DY_events[4] / XSection("DY4JetsToLL") + DY_events[0] / XSection("DYJetsToLL"));
        else   {cout<<"**********   wooow  ********* There is a problem here\n";return 0;}
        
        
    } else if ((Signal_ggH != string::npos ||  Signal_bbH != string::npos) && bg_SM == string::npos){
        
      return luminosity * XSection("signal")*1.0 / Histo->Integral();
    } else
        
        return luminosity * XSection(newOut)*1.0 / Histo->Integral();


}



















