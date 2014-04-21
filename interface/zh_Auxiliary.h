

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

inline double deltaPhi(myobject const& a, myobject const& b) {
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
//inline double deltaR2(double eta1, double phi1, double eta2, double phi2) {
//    double deta = eta1 - eta2;
//    double dphi = deltaPhi(phi1, phi2);
//    return deta * deta + dphi*dphi;
//}

inline double deltaR(myobject const& a, myobject const& b) {
    return sqrt(deltaR2(a, b));
}
//inline double deltaR(double eta1, double phi1, double eta2, double phi2) {
//    return sqrt(deltaR2(eta1, phi1, eta2, phi2));
//}

double delta(myevent *m, myobject * tau) {
    vector<myobject> jet;
    jet = m->RecPFJetsAK5;
    double minDis = 1000;
    double mydeltaR;
    for (vector<myobject>::iterator itjet = jet.begin(); itjet != jet.end(); itjet++) {
        if (itjet->pt > 20) {
            mydeltaR = deltaR(*tau, *itjet);
            if (mydeltaR > 0.1)
                if (mydeltaR < minDis) minDis = mydeltaR;
        }
    }

    return minDis;
}

//struct  InvarMass_2{
//    double InvarMass_2 ()(myobject const& a, myobject const& b) const {
//        return (a.pt + b.pt);
//    }
//};

//struct  InvarMass_2{

double InvarMass_2(myobject const& a, myobject const& b) {
    return sqrt(pow(a.E + b.E, 2) - pow(a.px + b.px, 2) - pow(a.py + b.py, 2) - pow(a.pz + b.pz, 2));
}

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

double InvarMass(double et1, double et2, double px1, double px2, double py1, double py2, double pz1, double pz2) {
    return sqrt(pow(et1 + et2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2) - pow(pz1 + pz2, 2));
}




#endif	/* _JETVETO_H */

