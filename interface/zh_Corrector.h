/* 
 * File:   jetVeto.h
 * Author: abdollah
 *
 * Created on July 21, 2010, 3:39 PM
 */






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
#include "TSystem.h"
#include "myevent.h"
//#include "ZHtautauAnalysis_LinkDef.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include "myobject.h"
#include <TLorentzVector.h>
//#include "Analysis.h"
using namespace std;

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

bool isMC = true;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//from OLD 2l2tay

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// *** 2011 *** //
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float Cor_ID_Iso_Mu_Loose_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 1.5) return 0.9895;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) >= 1.5 && fabs(a.eta) < 2.1) return 1.0303;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 1.5) return 1.0168;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) >= 1.5 && fabs(a.eta) < 2.1) return 1.0247;
    if (a.pt > 20 && fabs(a.eta) < 1.5) return 1.0061;
    if (a.pt > 20 && fabs(a.eta) >= 1.5 && fabs(a.eta) < 2.1) return 1.0144;
    if (a.pt < 10 || a.pt >= 100 || fabs(a.eta) >= 2.10) return 1.000;
    return 1.0;
}

float Cor_ID_Iso_Mu_Tight_2011(myobject const& a) {
    if (a.pt > 17 && a.pt < 20 && fabs(a.eta) < 1.6) return 0.997 * 0.930;
    if (a.pt > 17 && a.pt < 20 && fabs(a.eta) > 1.6 && fabs(a.eta) < 2.1) return 0.986 * 0.929;
    if (a.pt > 20 && a.pt < 30 && fabs(a.eta) < 1.6) return 0.995 * 0.977;
    if (a.pt > 20 && a.pt < 30 && fabs(a.eta) > 1.6 && fabs(a.eta) < 2.1) return 0.986 * 0.984;
    if (a.pt > 30 && fabs(a.eta) < 1.6) return 1.030 * 1.010;
    if (a.pt > 30 && fabs(a.eta) > 1.6 && fabs(a.eta) < 2.1) return 0.990 * 0.997;
    if (a.pt < 17 || a.pt >= 100 || fabs(a.eta) >= 2.1) return 1.000;
    return 1.0;
}

float Cor_ID_Iso_Ele_Loose_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 1.479) return 1.0396;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.1) return 0.9758;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 1.479) return 0.9622;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.1) return 1.1483;
    if (a.pt >= 20 && fabs(a.eta) < 1.479) return 0.9849;
    if (a.pt >= 20 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.1) return 1.0117;
    if (a.pt < 10 || a.pt >= 100 || fabs(a.eta) >= 2.1) return 1.000;
    return 1.0;
}

float Cor_ID_Iso_Ele_Tight_2011(myobject const& a) {
    if (a.pt > 20 && a.pt < 30 && fabs(a.eta) < 1.479) return 0.955 * 0.980;
    if (a.pt > 20 && a.pt < 30 && fabs(a.eta) > 1.479 && fabs(a.eta) < 2.1) return 0.938 * 0.967;
    if (a.pt > 30 && fabs(a.eta) < 1.479) return 1.044 * 0.984;
    if (a.pt > 30 && fabs(a.eta) > 1.479 && fabs(a.eta) < 2.1) return 0.977 * 0.989;
    if (a.pt < 20 || a.pt >= 100 || fabs(a.eta) >= 2.1) return 1.000;
    return 1.0;
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// *** 2012 *** //
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// *** 2012 53X *** // New From rebecca
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float Cor_ID_Iso_Mu_Loose_2012_53X(myobject const& a) {
    if (a.pt >= 10 && a.pt < 20 && fabs(a.eta) < 0.8) return 0.987404 * 0.981105;
    if (a.pt >= 10 && a.pt < 20 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.976942 * 0.980886;
    if (a.pt >= 10 && a.pt < 20 && 1.2 <= fabs(a.eta)) return 0.9931 * 0.998097;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.985313 * 0.993823;
    if (a.pt >= 20 && a.pt < 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.981847 * 1.0;
    if (a.pt >= 20 && a.pt < 30 && 1.2 <= fabs(a.eta)) return 0.989903 * 0.999919;
    if (a.pt >= 30 && fabs(a.eta) < 0.8) return 0.985675 * 0.999024;
    if (a.pt >= 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.980492 * 0.999228;
    if (a.pt >= 30 && 1.2 <= fabs(a.eta)) return 0.990018 * 0.998869;
    return 1.0;
}

float Cor_ID_Iso_Mu_Tight_2012_53X(myobject const& a) {
    if (a.pt > 20 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9853 * 0.9685;
    if (a.pt > 20 && a.pt < 30 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.2) return 0.9818 * 0.9808;
    if (a.pt > 20 && a.pt < 30 && fabs(a.eta) >= 1.2 && fabs(a.eta) < 2.1) return 0.9899 * 0.9972;
    if (a.pt > 30 && fabs(a.eta) < 0.8) return 0.9857 * 0.9872;
    if (a.pt > 30 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.2) return 0.9805 * 0.9924;
    if (a.pt > 30 && fabs(a.eta) >= 1.2 && fabs(a.eta) < 2.1) return 0.9900 * 1.0012;
    return 1.0;
}

float Cor_ID_Iso_Ele_Loose_2012_53X(myobject const& a) {
    if (a.pt >= 10 && a.pt < 20 && fabs(a.eta) >= 0 && fabs(a.eta) < 1.479) return 0.9794 * 0.970332;
    if (a.pt >= 10 && a.pt < 20 && 1.479 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.942182 * 0.984779;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) >= 0 && fabs(a.eta) < 1.479) return 0.955022 * 0.987348;
    if (a.pt >= 20 && a.pt < 30 && 1.479 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.960961 * 0.99015;
    if (a.pt >= 30 && fabs(a.eta) >= 0 && fabs(a.eta) < 1.479) return 0.964001 * 0.998837;
    if (a.pt >= 30 && 1.479 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.970416 * 0.999246;
    return 1.0;
}

float Cor_ID_Iso_Ele_Tight_2012_53X(myobject const& a) {
    if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) < 1.479) return 0.9100 * 0.9468;
    if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.1) return 0.8244 * 0.9586;
    if (a.pt >= 30 && fabs(a.eta) < 1.479) return 0.9493 * 0.9820;
    if (a.pt >= 30 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.1) return 0.9260 * 0.9948;
    return 1.0;
}




// from Alexei and Lucia begin

float Corr_Trg_Mu_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 1.5) return 1.01;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) > 1.5) return 1.03;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 1.5) return 0.99;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) > 1.5) return 1.07;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 1.5) return 0.99;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) > 1.5) return 1.4;
    if (a.pt > 30 && fabs(a.eta) < 1.5) return 0.992;
    if (a.pt > 30 && fabs(a.eta) > 1.5) return 1.06;
    return 1;
}

float Corr_Trg_Ele_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 1.479) return 0.98;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) > 1.479) return 0.97;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 1.479) return 1.00;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) > 1.479) return 1.05;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 1.479) return 1.001;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) > 1.479) return 1.00;
    if (a.pt > 30 && fabs(a.eta) < 1.479) return 1.003;
    if (a.pt > 30 && fabs(a.eta) > 1.479) return 1.008;
    return 1;
}

float Corr_Trg_Mu_2012_53X(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 0.8) return 0.9818;
    if (a.pt >= 10 && a.pt < 15 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9713;
    if (a.pt >= 10 && a.pt < 15 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.9675;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 0.8) return 0.9781;
    if (a.pt >= 15 && a.pt < 20 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9782;
    if (a.pt >= 15 && a.pt < 20 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.9587;
    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) < 0.8) return 0.9873;
    if (a.pt >= 20 && a.pt < 25 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9532;
    if (a.pt >= 20 && a.pt < 25 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.9605;
    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9755;
    if (a.pt >= 25 && a.pt < 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9818;
    if (a.pt >= 25 && a.pt < 30 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.9632;
    if (a.pt >= 30 && fabs(a.eta) < 0.8) return 0.9956;
    if (a.pt >= 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9644;
    if (a.pt >= 30 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 2.1) return 0.9530;
    return 1.0;
}

float Corr_Trg_Ele_2012_53X(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9639;
    if (a.pt >= 10 && a.pt < 15 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.479) return 0.8898;
    if (a.pt >= 10 && a.pt < 15 && 1.479 <= fabs(a.eta) && fabs(a.eta) < 2.3) return 0.9228;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9762;
    if (a.pt >= 15 && a.pt < 20 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.479) return 0.9647;
    if (a.pt >= 15 && a.pt < 20 && 1.479 <= fabs(a.eta) && fabs(a.eta) < 2.3) return 0.9199;
    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9683;
    if (a.pt >= 20 && a.pt < 25 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.479) return 0.9666;
    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.3) return 0.9679;
    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9756;
    if (a.pt >= 25 && a.pt < 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.479) return 0.9896;
    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.3) return 0.9473;
    if (a.pt > 30 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 1.0035;
    if (a.pt > 30 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.479) return 0.9977;
    if (a.pt > 30 && fabs(a.eta) >= 1.479 && fabs(a.eta) < 2.3) return 0.9885;
    return 1;
}


//float Corr_Trg_Mu_2012_53X(myobject const& a)
//{
//    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 0.8) return 0.9841;
//    if (a.pt >= 10 && a.pt < 15 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.2) return 0.9742;
//    if (a.pt >= 10 && a.pt < 15 && 1.2 <= fabs(a.eta) &&  fabs(a.eta) < 1.6) return 0.9955;
//    if (a.pt >= 10 && a.pt < 15 && 1.6 <= fabs(a.eta) &&  fabs(a.eta) < 2.1) return 0.9151;
//    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 0.8) return 0.9846;
//    if (a.pt >= 15 && a.pt < 20 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.2) return 0.9834;
//    if (a.pt >= 15 && a.pt < 20 && 1.2 <= fabs(a.eta) &&  fabs(a.eta) < 1.6) return 0.9793;
//    if (a.pt >= 15 && a.pt < 20 && 1.6 <= fabs(a.eta) &&  fabs(a.eta) < 2.1) return 0.9257;    
//    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) < 0.8) return 0.9937;
//    if (a.pt >= 20 && a.pt < 25 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.2) return 0.9594;
//    if (a.pt >= 20 && a.pt < 25 && 1.2 <= fabs(a.eta) &&  fabs(a.eta) < 1.6) return 0.9692;
//    if (a.pt >= 20 && a.pt < 25 && 1.6 <= fabs(a.eta) &&  fabs(a.eta) < 2.1) return 0.9438;
//    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9856;
//    if (a.pt >= 25 && a.pt < 30 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.2) return 0.9818;
//    if (a.pt >= 25 && a.pt < 30 && 1.2 <= fabs(a.eta) &&  fabs(a.eta) < 1.6) return 0.9684;
//    if (a.pt >= 25 && a.pt < 30 && 1.6 <= fabs(a.eta) &&  fabs(a.eta) < 2.1) return 0.9642;
//    if (a.pt >= 30 && a.pt < 35 && fabs(a.eta) < 0.8) return 0.9930;
//    if (a.pt >= 30 && a.pt < 35 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.2) return 0.9800;
//    if (a.pt >= 30 && a.pt < 35 && 1.2 <= fabs(a.eta) &&  fabs(a.eta) < 1.6) return 0.9958;
//    if (a.pt >= 30 && a.pt < 35 && 1.6 <= fabs(a.eta) &&  fabs(a.eta) < 2.1) return 0.9428;
//    if (a.pt >= 35 && fabs(a.eta) < 0.8) return 0.9991;
//    if (a.pt >= 35 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.2) return 0.9626;
//    if (a.pt >= 35 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.6) return 0.9611;
//    if (a.pt >= 35 && 1.6 <= fabs(a.eta) &&  fabs(a.eta) < 2.1) return 0.9314;
//    return 1.0;
//}
//
//float Corr_Trg_Ele_2012_53X(myobject const& a)
//{
//    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) >= 0 && fabs(a.eta)<0.8) return 0.9529;
//    if (a.pt >= 10 && a.pt < 15 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.5) return 0.8858;
//    if (a.pt >= 10 && a.pt < 15 && 1.5 <= fabs(a.eta) &&  fabs(a.eta) < 2.3) return 0.9259;
//    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9841;
//    if (a.pt >= 15 && a.pt < 20 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.5) return 0.9699;
//    if (a.pt >= 15 && a.pt < 20 && 1.5 <= fabs(a.eta) &&  fabs(a.eta) < 2.3) return 0.9286;
//    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta)>=0 && fabs(a.eta) < 0.8) return 0.9716;
//    if (a.pt >= 20 && a.pt < 25 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.5) return 0.9702;
//    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta)>=1.5 && fabs(a.eta) < 2.3) return 0.9726;
//    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta)>=0 && fabs(a.eta) < 0.8) return 0.9772;
//    if (a.pt >= 25 && a.pt < 30 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.5) return 0.9916;
//    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta)>=1.5 && fabs(a.eta) < 2.3) return 0.9609;
//	if (a.pt >= 30 && a.pt < 35 && fabs(a.eta)>=0 && fabs(a.eta) < 0.8) return 1.0084;
//    if (a.pt >= 30 && a.pt < 35 && 0.8 <= fabs(a.eta) &&  fabs(a.eta) < 1.5) return 0.9900;
//    if (a.pt >= 30 && a.pt < 35 && fabs(a.eta)>=1.5 && fabs(a.eta) < 2.3) return 0.9817;
//    if (a.pt >= 35 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 1.0069;
//    if (a.pt >= 35 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 1.0049;
//    if (a.pt >= 35 && fabs(a.eta) >= 1.5 && fabs(a.eta) < 2.3) return 0.9989;
//    return 1.0;
//}

// from Alexei and Lucia end

float Trg_Mu_2011(myobject const & a, myobject const& b) {
    return Corr_Trg_Mu_2011(a) * Corr_Trg_Mu_2011(b);
}

float Trg_Mu_2012(myobject const & a, myobject const& b) {
    return Corr_Trg_Mu_2012_53X(a) * Corr_Trg_Mu_2012_53X(b);
}

float Trg_Ele_2011(myobject const & a, myobject const& b) {
    return Corr_Trg_Ele_2011(a) * Corr_Trg_Ele_2011(b);
}

float Trg_Ele_2012(myobject const & a, myobject const& b) {
    return Corr_Trg_Ele_2012_53X(a) * Corr_Trg_Ele_2012_53X(b);
}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float getCorrFactor(std::string channel, std::string type, myobject const& a, myobject const& b, myobject const& c) {

    // return 1;


    if (type == "mc12") {
        if (channel == "mm")
            return Trg_Mu_2012(a, b) * Cor_ID_Iso_Mu_Loose_2012_53X(a) * Cor_ID_Iso_Mu_Loose_2012_53X(b);
        if (channel == "mmtt")
            return Trg_Mu_2012(a, b) * Cor_ID_Iso_Mu_Loose_2012_53X(a) * Cor_ID_Iso_Mu_Loose_2012_53X(b);
        if (channel == "mmet")
            return Trg_Mu_2012(a, b) * Cor_ID_Iso_Mu_Loose_2012_53X(a) * Cor_ID_Iso_Mu_Loose_2012_53X(b) * Cor_ID_Iso_Ele_Tight_2012_53X(c);
        if (channel == "mmmt")
            return Trg_Mu_2012(a, b) * Cor_ID_Iso_Mu_Loose_2012_53X(a) * Cor_ID_Iso_Mu_Loose_2012_53X(b) * Cor_ID_Iso_Mu_Tight_2012_53X(c);
        if (channel == "mmme")
            return Trg_Mu_2012(a, b) * Cor_ID_Iso_Mu_Loose_2012_53X(a) * Cor_ID_Iso_Mu_Loose_2012_53X(b) * Cor_ID_Iso_Mu_Loose_2012_53X(c) * Cor_ID_Iso_Ele_Loose_2012_53X(d);
        if (channel == "ee")
            return Trg_Ele_2012(a, b) * Cor_ID_Iso_Ele_Loose_2012_53X(a) * Cor_ID_Iso_Ele_Loose_2012_53X(b);
        if (channel == "eett")
            return Trg_Ele_2012(a, b) * Cor_ID_Iso_Ele_Loose_2012_53X(a) * Cor_ID_Iso_Ele_Loose_2012_53X(b);
        if (channel == "eemt")
            return Trg_Ele_2012(a, b) * Cor_ID_Iso_Ele_Loose_2012_53X(a) * Cor_ID_Iso_Ele_Loose_2012_53X(b) * Cor_ID_Iso_Mu_Tight_2012_53X(c);
        if (channel == "eeet")
            return Trg_Ele_2012(a, b) * Cor_ID_Iso_Ele_Loose_2012_53X(a) * Cor_ID_Iso_Ele_Loose_2012_53X(b) * Cor_ID_Iso_Ele_Tight_2012_53X(c);
        if (channel == "eeem")
            return Trg_Ele_2012(a, b) * Cor_ID_Iso_Ele_Loose_2012_53X(a) * Cor_ID_Iso_Ele_Loose_2012_53X(b) * Cor_ID_Iso_Ele_Loose_2012_53X(c) * Cor_ID_Iso_Mu_Loose_2012_53X(d);

    }


    if (type == "mc11") {
        if (channel == "mm")
            return Trg_Mu_2011(a, b) * Cor_ID_Iso_Mu_Loose_2011(a) * Cor_ID_Iso_Mu_Loose_2011(b);
        if (channel == "mmtt")
            return Trg_Mu_2011(a, b) * Cor_ID_Iso_Mu_Loose_2011(a) * Cor_ID_Iso_Mu_Loose_2011(b);
        if (channel == "mmet")
            return Trg_Mu_2011(a, b) * Cor_ID_Iso_Mu_Loose_2011(a) * Cor_ID_Iso_Mu_Loose_2011(b) * Cor_ID_Iso_Ele_Tight_2011(c);
        if (channel == "mmmt")
            return Trg_Mu_2011(a, b) * Cor_ID_Iso_Mu_Loose_2011(a) * Cor_ID_Iso_Mu_Loose_2011(b) * Cor_ID_Iso_Mu_Tight_2011(c);
        if (channel == "mmme")
            return Trg_Mu_2011(a, b) * Cor_ID_Iso_Mu_Loose_2011(a) * Cor_ID_Iso_Mu_Loose_2011(b) * Cor_ID_Iso_Mu_Loose_2011(c) * Cor_ID_Iso_Ele_Loose_2011(d);
        if (channel == "ee")
            return Trg_Ele_2011(a, b) * Cor_ID_Iso_Ele_Loose_2011(a) * Cor_ID_Iso_Ele_Loose_2011(b);
        if (channel == "eett")
            return Trg_Ele_2011(a, b) * Cor_ID_Iso_Ele_Loose_2011(a) * Cor_ID_Iso_Ele_Loose_2011(b);
        if (channel == "eemt")
            return Trg_Ele_2011(a, b) * Cor_ID_Iso_Ele_Loose_2011(a) * Cor_ID_Iso_Ele_Loose_2011(b) * Cor_ID_Iso_Mu_Tight_2011(c);
        if (channel == "eeet")
            return Trg_Ele_2011(a, b) * Cor_ID_Iso_Ele_Loose_2011(a) * Cor_ID_Iso_Ele_Loose_2011(b) * Cor_ID_Iso_Ele_Tight_2011(c);
        if (channel == "eeem")
            return Trg_Ele_2011(a, b) * Cor_ID_Iso_Ele_Loose_2011(a) * Cor_ID_Iso_Ele_Loose_2011(b) * Cor_ID_Iso_Ele_Loose_2011(c) * Cor_ID_Iso_Mu_Loose_2011(d);

    }




    if (type == "data11" || type == "data12")
        return 1;



}




