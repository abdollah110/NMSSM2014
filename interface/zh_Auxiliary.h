

#ifndef _DELTA_H
#define	_DELTA_H


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
#include "LinkDef.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
//#include "myobject.h"
//#include "Leptons_PreSelection.h"
#include <TLorentzVector.h>

using namespace std;

float TauESWeight(int mcdata, int DM, float eta) {
    if (mcdata == 2 || mcdata == 4)
        return 1;
    else {
        if (DM == 0 && fabs(eta) < 1.5) return 0.87;
        if (DM == 0 && fabs(eta) > 1.5) return 0.96;
        if (DM == 1 && fabs(eta) < 1.5) return 1.06;
        if (DM == 1 && fabs(eta) > 1.5) return 1.00;
        if (DM == 10 && fabs(eta) < 1.5) return 1.02;
        if (DM == 10 && fabs(eta) > 1.5) return 1.06;
    }
    return 1;
}

float TMass_F(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    return sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2));
}

bool WZ_Rej_B(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    bool b_met = met < 20;
    bool b_tmass = sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2)) < 30;
    //    if (b_met && b_tmass)
    if (b_tmass)
        return true;
    else
        return false;
}

bool justTmass(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    bool b_tmass = sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2)) < 30;
    if (b_tmass)
        return true;
    else
        return false;
}

float deltaPhi(float a, float b) {
    float result = a - b;
    while (result > M_PI) result -= 2 * M_PI;
    while (result <= -M_PI) result += 2 * M_PI;
    return fabs(result);
}

float dR(float l1eta, float l1phi, float l2eta, float l2phi) {
    float deta = l1eta - l2eta;
    float dphi = deltaPhi(l1phi, l2phi);
    //    cout<< "Here is dR = "<<"l1eta " <<l1eta << " l2eta"<<l2eta  << "     ---dR="<< sqrt(deta * deta + dphi * dphi) <<"\n";
    return sqrt(deta * deta + dphi * dphi);
}

bool NewOverLap(float l1eta, float l1phi, float l2eta, float l2phi, float l3eta, float l3phi, float l4eta, float l4phi) {
    bool over_12 = dR(l1eta, l1phi, l2eta, l2phi) > 0.3;
    bool over_13 = dR(l1eta, l1phi, l3eta, l3phi) > 0.3;
    bool over_14 = dR(l1eta, l1phi, l4eta, l4phi) > 0.3;
    bool over_23 = dR(l2eta, l2phi, l3eta, l3phi) > 0.3;
    bool over_24 = dR(l2eta, l2phi, l4eta, l4phi) > 0.3;
    bool over_34 = dR(l3eta, l3phi, l4eta, l4phi) > 0.3;

    if (over_12 && over_13 && over_14 && over_23 && over_24 && over_34)
        return true;
    else
        return false;
}

float TMass_F(float et1, float et2, float px1, float px2, float py1, float py2) {
    return sqrt(pow(et1 + et2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2));

}

float InvarMass_F(float e1, float e2, float px1, float px2, float py1, float py2, float pz1, float pz2) {
    return sqrt(pow(e1 + e2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2) - pow(pz1 + pz2, 2));
}

inline double deltaPhi(myobject const& a, myobject const& b) {
    double result = a.phi - b.phi;
    while (result > M_PI) result -= 2 * M_PI;
    while (result <= -M_PI) result += 2 * M_PI;
    return fabs(result);
}

inline double deltaPhi(myGenobject const& a, myobject const& b) {
    double result = a.phi - b.phi;
    while (result > M_PI) result -= 2 * M_PI;
    while (result <= -M_PI) result += 2 * M_PI;
    return fabs(result);
}

inline double deltaR2(myobject const& a, myobject const& b) {
    double deta = a.eta - b.eta;
    double dphi = deltaPhi(a, b);
    return deta * deta + dphi*dphi;
}

inline double deltaR2(myGenobject const& a, myobject const& b) {
    double deta = a.eta - b.eta;
    double dphi = deltaPhi(a, b);
    return deta * deta + dphi*dphi;
}
//inline double deltaR2(double eta1, double phi1, double eta2, double phi2) {
//    double deta = eta1 - eta2;
//    double dphi = deltaPhi(phi1, phi2);
//    return deta * deta + dphi*dphi;
//}

inline double deltaR(myobject const& a, myobject const& b) {
    return sqrt(deltaR2(a, b));
}

inline double deltaR(myGenobject const& a, myobject const& b) {
    return sqrt(deltaR2(a, b));
}
//inline double deltaR(double eta1, double phi1, double eta2, double phi2) {
//    return sqrt(deltaR2(eta1, phi1, eta2, phi2));
//}

//double delta(myevent *m, myobject * tau) {
//    vector<myobject> jet;
//    jet = m->RecPFJetsAK5;
//    double minDis = 1000;
//    double mydeltaR;
//    for (vector<myobject>::iterator itjet = jet.begin(); itjet != jet.end(); itjet++) {
//        if (itjet->pt > 20) {
//            mydeltaR = deltaR(*tau, *itjet);
//            if (mydeltaR > 0.1)
//                if (mydeltaR < minDis) minDis = mydeltaR;
//        }
//    }
//
//    return minDis;
//}

//bool findThegenVisibleTauH()

bool checkGenRecodR(myGenobject const& Gen1, myGenobject const& Gen2, myobject const& Reco1, myobject const& Reco2) {
//    cout<<"------------dR11_22="<<Gen1.eta<< " " <<Gen1.phi<< " " << Reco1.eta<< " " << Reco1.phi <<"  " <<Gen2.eta<< " " << Gen2.phi<< " " << Reco2.eta<< " " << Reco2.phi<< " "  <<"\n";
//    cout<<"------------dR11_22="<<dR(Gen1.eta, Gen1.phi, Reco1.eta, Reco1.phi) <<"  " <<dR(Gen2.eta, Gen2.phi, Reco2.eta, Reco2.phi) <<"\n";
//    cout<<"------------dR12_12="<<dR(Gen1.eta, Gen1.phi, Reco2.eta, Reco2.phi) <<"  " <<dR(Gen2.eta, Gen2.phi, Reco1.eta, Reco1.phi) <<"\n";
    bool check_11_22 = dR(Gen1.eta, Gen1.phi, Reco1.eta, Reco1.phi) < 0.5 && dR(Gen2.eta, Gen2.phi, Reco2.eta, Reco2.phi) < 0.5;
    bool check_12_21 = dR(Gen1.eta, Gen1.phi, Reco2.eta, Reco2.phi) < 0.5 && dR(Gen2.eta, Gen2.phi, Reco1.eta, Reco1.phi) < 0.5;
    if (check_11_22 || check_12_21)
        return true;
    else return false;

}

int ZCategory(myevent *m, myobject const& obj1, myobject const& obj2) {

    vector<myGenobject> genTausFromZ;
    vector<myGenobject> genMuFromZ;
    vector<myGenobject> genEleFromZ;
    vector<myGenobject> genLepFromTaus;
    genTausFromZ.clear();
    genMuFromZ.clear();
    genEleFromZ.clear();
    genLepFromTaus.clear();
    vector<myGenobject> genPar = m->RecGenParticle;
    int gen_ditau = 6;
    bool TherIsZGamma= 0;
    if (genPar.size() != 0) {
        for (int a = 0; a < genPar.size(); ++a) {
            if ((fabs(genPar[a].mod_pdgId) == 23 || fabs(genPar[a].mod_pdgId) == 22) && fabs(genPar[a].pdgId) == 15) genTausFromZ.push_back(genPar[a]);
            if (genPar[a].pt > 8.0 && (fabs(genPar[a].mod_pdgId) == 23 || fabs(genPar[a].mod_pdgId) == 22) && fabs(genPar[a].pdgId) == 13) genMuFromZ.push_back(genPar[a]);
            if (genPar[a].pt > 8.0 && (fabs(genPar[a].mod_pdgId) == 23 || fabs(genPar[a].mod_pdgId) == 22) && fabs(genPar[a].pdgId) == 11) genEleFromZ.push_back(genPar[a]);
            if (genPar[a].pt > 8.0 && fabs(genPar[a].mod_pdgId) == 15 &&  (fabs(genPar[a].Gmod_pdgId) == 15 || fabs(genPar[a].Gmod_pdgId) == 22|| fabs(genPar[a].Gmod_pdgId) == 23) &&(fabs(genPar[a].pdgId) == 11 || fabs(genPar[a].pdgId) == 13)) genLepFromTaus.push_back(genPar[a]);
            if (fabs(genPar[a].mod_pdgId) == 23 || fabs(genPar[a].mod_pdgId) == 22)  TherIsZGamma=1;

//    if(fabs(genPar[a].mod_pdgId) == 15) cout << "-------> genPar[a].pdgId "<<genPar[a].pdgId << "   genPar[a].Gmod_pdgId "<< genPar[a].Gmod_pdgId <<"\n";
        
    }
    }
    
    
   // if (! TherIsZGamma) cout<< "TherIsZGamma "<< TherIsZGamma<<"\n";
//        if (genTausFromZ.size() == 2) {
//            cout <<  " genTausFromZ.size()=" << genTausFromZ.size() << "\n";
//            cout << "status= " << genTausFromZ[0].status<< " pdgId= " << genTausFromZ[0].pdgId <<  " MotherpdgId= " << genTausFromZ[0].mod_pdgId << " genTausFromZ[0].eta= " << genTausFromZ[0].eta << " genTausFromZ[0].phi= " << genTausFromZ[0].phi << " genTausFromZ[0].pt= " << genTausFromZ[0].pt << "\n";
//            cout << "status= " << genTausFromZ[1].status<<"pdgId= " << genTausFromZ[1].pdgId <<  " MotherpdgId= " << genTausFromZ[1].mod_pdgId << "genTausFromZ[1].eta= " << genTausFromZ[1].eta << " genTausFromZ[1].phi= " << genTausFromZ[1].phi << " genTausFromZ[1].pt= " << genTausFromZ[1].pt << "\n";
//            cout << "Electron---> recoObj1.eta= " << obj1.eta << " recoObj1.phi= " << obj1.phi << " recoObj1.pt= " << obj1.pt << "\n";
//            cout << "Tau ---> recoObj2.eta= " << obj2.eta << " recoObj2.phi= " << obj2.phi << " recoObj2.pt= " << obj2.pt << "\n";
//        }
    
    
//        cout<<"+++++++++++++++++++ genLepFromTaus.size()=   "<<genLepFromTaus.size() <<"\n";
//        if (genLepFromTaus.size() > 0) {
//            cout << "genLepFromTaus.size()=" << genLepFromTaus.size() << "\n";
//            
//            cout << "status= " << genLepFromTaus[0].status<<" pdgId= " << genLepFromTaus[0].pdgId <<  " MotherpdgId= " << genLepFromTaus[0].mod_pdgId << " GrandMotherpdgId= " << genLepFromTaus[0].Gmod_pdgId << " genLepFromTaus[0].eta= " << genLepFromTaus[0].eta << " genLepFromTaus[0].phi= " << genLepFromTaus[0].phi << " genLepFromTaus[0].pt= " << genLepFromTaus[0].pt << "\n";
//            
//            cout << "status= " << genLepFromTaus[1].status<<" pdgId= " << genLepFromTaus[1].pdgId <<  " MotherpdgId= " << genLepFromTaus[1].mod_pdgId <<  " GrandMotherpdgId= " << genLepFromTaus[1].Gmod_pdgId <<" genLepFromTaus[0].eta= " << genLepFromTaus[1].eta << " genLepFromTaus[0].phi= " << genLepFromTaus[1].phi << " genLepFromTaus[0].pt= " << genLepFromTaus[1].pt << "\n";
//            cout << "status= " << genLepFromTaus[2].status<<" pdgId= " << genLepFromTaus[2].pdgId <<  " MotherpdgId= " << genLepFromTaus[2].mod_pdgId << " genLepFromTaus[0].eta= " << genLepFromTaus[2].eta << " genLepFromTaus[0].phi= " << genLepFromTaus[2].phi << " genLepFromTaus[0].pt= " << genLepFromTaus[2].pt << "\n";
//            cout << "status= " << genLepFromTaus[3].status<<" pdgId= " << genLepFromTaus[3].pdgId <<  " MotherpdgId= " << genLepFromTaus[3].mod_pdgId << " genLepFromTaus[0].eta= " << genLepFromTaus[3].eta << " genLepFromTaus[0].phi= " << genLepFromTaus[3].phi << " genLepFromTaus[0].pt= " << genLepFromTaus[3].pt << "\n";
//        }
//        cout << "genMuFromZ.size() = "<<genMuFromZ.size() << "   genEleFromZ.size()=" << genEleFromZ.size() << "    genTausFromZ.size() ="<<genTausFromZ.size() << "   genLepFromTaus.size()= "<<genLepFromTaus.size() <<"\n";
//    cout <<"-------   "<<genTausFromZ.size() << genLepFromTaus.size() << checkGenRecodR(genTausFromZ[0], genTausFromZ[1], obj1, obj2)<<checkGenRecodR(genLepFromTaus[0], genLepFromTaus[1], obj1, obj2)<<"------\n";
   
    
    if (genMuFromZ.size() == 2 && checkGenRecodR(genMuFromZ[0], genMuFromZ[1], obj1, obj2)) gen_ditau = 1; // THis is ZLL (ZMuMU)
    
    else if (genEleFromZ.size() == 2 && checkGenRecodR(genEleFromZ[0], genEleFromZ[1], obj1, obj2)) gen_ditau = 2; // THis is ZLL (ZEE)
    
    else if (genTausFromZ.size() == 2 && genLepFromTaus.size() == 1 && checkGenRecodR(genTausFromZ[0], genTausFromZ[1], obj1, obj2)) gen_ditau = 3; //This is ZTT (ZLepTau)
    
    else if (genTausFromZ.size() == 2 && genLepFromTaus.size() == 2 && checkGenRecodR(genLepFromTaus[0], genLepFromTaus[1], obj1, obj2)) gen_ditau = 4; //This is ZL (ZTauTau_LepLep)
//    else if (genTausFromZ.size() == 2 && genLepFromTaus.size() == 2 && checkGenRecodR(genTausFromZ[0], genTausFromZ[1], obj1, obj2)) gen_ditau = 4; //This is ZL (ZTauTau_LepLep) BUG Found December 5th
    else gen_ditau = 5; //This is ZJ (ZTauTau_LepLep)
    return gen_ditau;
    // 1 or 2 will be  ZLL
    // 3 or  will be  ZTT
    // 4 or  will be  ZL
    //  5 will be  ZJ
}



//int ZCategory(myevent *m, myobject const& tau) {
//    int numGenTau = 0;
//    bool RecoTauMatchToGenLep = false;
//
//    vector<myGenobject> genPar = m->RecGenParticle;
//    for (int gg = 0; gg < genPar.size(); gg++) {
//
//        if (abs(genPar[gg].mod_pdgId) == 23 && abs(genPar[gg].pdgId) == 15) numGenTau++;
//        if (genPar[gg].pt > 8 && (abs(genPar[gg].pdgId) == 11 || abs(genPar[gg].pdgId) == 13) && deltaR(genPar[gg], tau) < 0.5) RecoTauMatchToGenLep = true;
//    }
//    if (numGenTau == 2 && !RecoTauMatchToGenLep) return 1;
//    if (numGenTau == 0 && RecoTauMatchToGenLep) return 2;
//    if (numGenTau == 0 && !RecoTauMatchToGenLep) return 3;
//    if (numGenTau == 2 && RecoTauMatchToGenLep) return 4;
//    else return -10;
//}


//int jetGenpdgid(myevent *m, myobject const& jet) {
//    int pdgGenJet = 0;
//    float minimuDist = 10;
//
//    vector<myGenobject> genPar = m->RecGenParticle;
//    for (int gg = 0; gg < genPar.size(); gg++) {
//        if ((fabs(genPar[gg].pdgId) < 6 || fabs(genPar[gg].pdgId) == 21)  &&( (genPar[gg].pt - jet.pt) /genPar[gg].pt  < 0.3)) {
////        if ((fabs(genPar[gg].pdgId) < 6 || fabs(genPar[gg].pdgId) == 21) && (genPar[gg].status == 3) &&( (genPar[gg].pt - jet.pt) /genPar[gg].pt  < 0.5)) {
////        if (fabs(genPar[gg].pdgId) < 7 ) {
//            if (deltaR(genPar[gg], jet) < minimuDist) {
//                pdgGenJet = abs(genPar[gg].pdgId);
//            }
//        }
//    }
//    return pdgGenJet;
//}





//struct  InvarMass_2{
//    double InvarMass_2 ()(myobject const& a, myobject const& b) const {
//        return (a.pt + b.pt);
//    }
//};

//struct  InvarMass_2{

double InvarMass_2(myobject const& a, myobject const& b) {

    return sqrt(pow(a.E + b.E, 2) - pow(a.px + b.px, 2) - pow(a.py + b.py, 2) - pow(a.pz + b.pz, 2));
}

double InvarMass_2(myGenobject const& a, myGenobject const& b) {
    
    return sqrt(pow(a.E + b.E, 2) - pow(a.px + b.px, 2) - pow(a.py + b.py, 2) - pow(a.pz + b.pz, 2));
}

//double InvarMass_2(myGenobject const& a, myGenobject const& b) {
//    return sqrt(pow(a.E + b.E, 2) - pow(a.px + b.px, 2) - pow(a.py + b.py, 2) - pow(a.pz + b.pz, 2));
//}

double TMass(myobject const& a, myobject const& b) {

    return sqrt(pow(a.et + b.et, 2) - pow(a.px + b.px, 2) - pow(a.py + b.py, 2));
}

double TMass_2(myobject const& a, myobject const& b) {

    return sqrt(pow(a.et + b.et, 2) - pow(a.px + b.px, 2) - pow(a.py + b.py, 2));
}

double InvarMass_4(myobject const& a, myobject const& b, myobject const& c, myobject const& d) {

    return sqrt(pow(a.E + b.E + c.E + d.E, 2) - pow(a.px + b.px + c.px + d.px, 2) - pow(a.py + b.py + c.py + d.py, 2) - pow(a.pz + b.pz + c.pz + d.pz, 2));
    //        return (pow(a.pt+b.pt+c.pt+d.pt,2)-pow(a.px+b.px+c.px+d.px,2)-pow(a.py+b.py+c.py+d.py,2)-pow(a.pz+b.pz+c.pz+d.pz,2));
}
//};

double TMass(double et1, double et2, double px1, double px2, double py1, double py2) {

    return sqrt(pow(et1 + et2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2));

}

double InvarMass(double e1, double e2, double px1, double px2, double py1, double py2, double pz1, double pz2) {
    return sqrt(pow(e1 + e2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2) - pow(pz1 + pz2, 2));
}




#endif	/* _JETVETO_H */






