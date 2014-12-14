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

//******************** ETau/MuTau turn-on *****************************

double correctionHighPtTail_Data(float l1Pt, float l1Eta, TF1* TriggerWeightBarrel, TF1* TriggerWeightEndcaps) {
    //Additional corrections for high pT taus in muTau & eTau (Arun, Mar14)
    //    TF1 *TriggerWeightBarrel = new TF1("AddTriggerWeightMuTauBarrel", "1 - 9.01280e-04*(x - 140) + 4.81592e-07*(x - 140)*(x-140)", 0., 800.);
    //    TF1 *TriggerWeightEndcaps = new TF1("AddTriggerWeightMuTauEndcaps", "1 - 1.81148e-03*(x - 60) + 5.44335e-07*(x - 60)*(x-60)", 0., 800.);

    //    Double_t DataValBarrel_pt = OneMinratioABCD +ratioABCD * TriggerWeightBarrel->Eval(l1Pt);
    //    Double_t DataValBarrel_800 = OneMinratioABCD +ratioABCD * TriggerWeightBarrel->Eval(800.);
    //
    //    Double_t DataValEndcaps_pt = OneMinratioABCD +ratioABCD * TriggerWeightEndcaps->Eval(l1Pt);
    //    Double_t DataValEndcaps_400 = OneMinratioABCD +ratioABCD * TriggerWeightEndcaps->Eval(400.);
    float ratioABCD= 0.62457;
    float OneMinratioABCD= 1- ratioABCD;
    if (l1Pt > 140 && l1Pt < 800 && fabs(l1Eta) < 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightBarrel->Eval(l1Pt));
    else if (l1Pt >= 800 && fabs(l1Eta) <= 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightBarrel->Eval(800.));
    else if (l1Pt > 60 && l1Pt < 400 && fabs(l1Eta) > 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightEndcaps->Eval(l1Pt));
    else if (l1Pt >= 400 && fabs(l1Eta) >= 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightEndcaps->Eval(400.));
        //    if (l1Pt < 800 && fabs(l1Eta) < 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightBarrel->Eval(l1Pt));
        //    else if (l1Pt >= 800 && fabs(l1Eta) <= 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightBarrel->Eval(800.));
        //    else if (l1Pt < 400 && fabs(l1Eta) > 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightEndcaps->Eval(l1Pt));
        //    else if (l1Pt >= 400 && fabs(l1Eta) >= 1.5) return (OneMinratioABCD +ratioABCD * TriggerWeightEndcaps->Eval(400.));
    else
        return 1;
}

double efficiency(double m, double m0, double sigma, double alpha, double n, double norm) {
    const double sqrtPiOver2 = 1.2533141373;
    const double sqrt2 = 1.4142135624;
    double sig = fabs((double) sigma);
    double t = (m - m0) / sig;
    if (alpha < 0)
        t = -t;
    double absAlpha = fabs(alpha / sig);
    double a = TMath::Power(n / absAlpha, n) * exp(-0.5 * absAlpha * absAlpha);
    double b = absAlpha - n / absAlpha;
    double ApproxErf;
    double arg = absAlpha / sqrt2;
    if (arg > 5.) ApproxErf = 1;
    else if (arg < -5.) ApproxErf = -1;
    else ApproxErf = TMath::Erf(arg);
    double leftArea = (1 + ApproxErf) * sqrtPiOver2;
    double rightArea = (a * 1 / TMath::Power(absAlpha - b, n - 1)) / (n - 1);
    double area = leftArea + rightArea;
    if (t <= absAlpha) {
        arg = t / sqrt2;
        if (arg > 5.) ApproxErf = 1;
        else if (arg < -5.) ApproxErf = -1;
        else ApproxErf = TMath::Erf(arg);
        return norm * (1 + ApproxErf) * sqrtPiOver2 / area;
    } else {
        return norm * (leftArea + a * (1 / TMath::Power(t - b, n - 1) -
                1 / TMath::Power(absAlpha - b, n - 1)) / (1 - n)) / area;
    }
}


//*****************************************************
//***************** ETau channel **********************
//*****************************************************

float Cor_IDIso_ETau_Ele_2012(float l1Pt, float l1Eta) {
    if (l1Pt >= 24 && l1Pt < 30 && fabs(l1Eta) < 1.479) return 0.8999 * 0.9417;
    else if (l1Pt >= 24 && l1Pt < 30 && fabs(l1Eta) > 1.479) return 0.7945 * 0.9471;
    else if (l1Pt >= 30 && fabs(l1Eta) < 1.479) return 0.9486 * 0.9804;
    else if (l1Pt >= 30 && fabs(l1Eta) > 1.479) return 0.8866 * 0.9900;
    else return 1.0;
}

float Cor_ID_ETau_Ele_2012(float l1Pt, float l1Eta) {
    if (l1Pt >= 24 && l1Pt < 30 && fabs(l1Eta) < 1.479) return 0.8999;
    else if (l1Pt >= 24 && l1Pt < 30 && fabs(l1Eta) > 1.479) return 0.7945;
    else if (l1Pt >= 30 && fabs(l1Eta) < 1.479) return 0.9486;
    else if (l1Pt >= 30 && fabs(l1Eta) > 1.479) return 0.8866;
    else return 1.0;
}

float Cor_Iso_ETau_Ele_2012(float l1Pt, float l1Eta) {
    if (l1Pt >= 24 && l1Pt < 30 && fabs(l1Eta) < 1.479) return 0.9417;
    else if (l1Pt >= 24 && l1Pt < 30 && fabs(l1Eta) > 1.479) return 0.9471;
    else if (l1Pt >= 30 && fabs(l1Eta) < 1.479) return 0.9804;
    else if (l1Pt >= 30 && fabs(l1Eta) > 1.479) return 0.9900;
    else return 1.0;
}


//   Trigger   *****************************************************

float Eff_ETauTrg_Ele_Data_2012(float l1Pt, float l1Eta) {
    if (fabs(l1Eta) < 1.479) return efficiency(l1Pt, 22.9704, 1.0258, 1.26889, 1.31024, 1.06409);
    else return efficiency(l1Pt, 21.9816, 1.40993, 0.978597, 2.33144, 0.937552);
}

float Eff_ETauTrg_Tau_Data_2012(float l1Pt, float l1Eta) {
    if (fabs(l1Eta) < 1.5) return efficiency(l1Pt, 1.83211e+01, -1.89051e+00, 3.71081e+00, 1.06628e+00, 1.28559e+00); //data barrel
    else return efficiency(l1Pt, 1.80812e+01, 1.39482e+00, 1.14305e+00, 1.08989e+01, 8.97087e-01); //data end cap
}




//*****************************************************
//***************** MuTau channel *********************
//*****************************************************

float Cor_IDIso_MuTau_Muon_2012(float l1Pt, float l1Eta) {
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 0.8) return 0.9818 * 0.9494;
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 1.2) return 0.9829 * 0.9835;
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 2.1) return 0.9869 * 0.9923;
    if (l1Pt >= 30 && fabs(l1Eta) < 0.8) return 0.9852 * 0.9883;
    if (l1Pt >= 30 && fabs(l1Eta) < 1.2) return 0.9852 * 0.9937;
    if (l1Pt >= 30 && fabs(l1Eta) < 2.1) return 0.9884 * 0.9996;
    return 1.0;
}

float Cor_ID_MuTau_Muon_2012(float l1Pt, float l1Eta) {
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 0.8) return 0.9818;
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 1.2) return 0.9829;
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 2.1) return 0.9869;
    if (l1Pt >= 30 && fabs(l1Eta) < 0.8) return 0.9852;
    if (l1Pt >= 30 && fabs(l1Eta) < 1.2) return 0.9852;
    if (l1Pt >= 30 && fabs(l1Eta) < 2.1) return 0.9884;
    return 1.0;
}

float Cor_Iso_MuTau_Muon_2012(float l1Pt, float l1Eta) {
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 0.8) return 0.9494;
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 1.2) return 0.9835;
    if (l1Pt >= 20 && l1Pt < 30 && fabs(l1Eta) < 2.1) return 0.9923;
    if (l1Pt >= 30 && fabs(l1Eta) < 0.8) return 0.9883;
    if (l1Pt >= 30 && fabs(l1Eta) < 1.2) return 0.9937;
    if (l1Pt >= 30 && fabs(l1Eta) < 2.1) return 0.9996;
    return 1.0;
}


//   Trigger   *****************************************************

float Eff_MuTauTrg_Mu_Data_2012(float l1Pt, float l1Eta) {
    if (l1Eta < -1.2) return efficiency(l1Pt, 15.9977, 0.0000764004, 6.4951e-8, 1.57403, 0.865325);
    else if (l1Eta < -0.8) return efficiency(l1Pt, 17.3974, 0.804001, 1.47145, 1.24295, 0.928198);
    else if (l1Eta < 0) return efficiency(l1Pt, 16.4307, 0.226312, 0.265553, 1.55756, 0.974462);
    else if (l1Eta < 0.8) return efficiency(l1Pt, 17.313, 0.662731, 1.3412, 1.05778, 1.26624);
    else if (l1Eta < 1.2) return efficiency(l1Pt, 16.9966, 0.550532, 0.807863, 1.55402, 0.885134);
    else if (l1Eta > 1.2) return efficiency(l1Pt, 15.9962, 0.000106195, 4.95058e-8, 1.9991, 0.851294);
    else return 1;
}


// ABCD      antiEMed

float Eff_MuTauTrg_Tau_Data_2012(float l1Pt, float l1Eta) {
    if (fabs(l1Eta) < 1.5) return efficiency(l1Pt, 1.83211e+01, -1.89051e+00, 3.71081e+00, 1.06628e+00, 1.28559e+00); //data barrel
    else return efficiency(l1Pt, 1.80812e+01, 1.39482e+00, 1.14305e+00, 1.08989e+01, 8.97087e-01); //data endcap
}



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//float getCorrFactorMC(int mcdata, int channel, float l1Pt, float l1Eta, float l2Pt, float l2Eta) {
//
//    if (channel == 3) {
//        return Cor_IDIso_ETau_Ele_2012(l1Pt, l1Eta) * Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta);
//    }
//    if (channel == 1) {
//        return Cor_IDIso_MuTau_Muon_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta);
//    }
//    return 1.0;
//
//}

// This is buggy found December 12
//float getCorrFactorEMbed(int mcdata, int channel, float l1Pt, float l1Eta, float l2Pt, float l2Eta, TF1* TriggerWeightBarrel, TF1* TriggerWeightEndcaps) {
//
//    if (mcdata == 2 || mcdata == 4 || mcdata == 5) {  //Data
//        if (channel == 3) {
//            //            return Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l1Pt, l1Eta);
//            return Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l1Pt, l1Eta) * correctionHighPtTail_Data(l1Pt, l1Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
//        }
//        if (channel == 1) {
//            //            return Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l1Pt, l1Eta) ;
//            return Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l1Pt, l1Eta) * correctionHighPtTail_Data(l1Pt, l1Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
//        }
//    } else if (channel == 3) {  //MC
//        //        return Cor_IDIso_ETau_Ele_2012(l1Pt, l1Eta) * Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l2Pt, l2Eta) ;
//        return Cor_IDIso_ETau_Ele_2012(l1Pt, l1Eta) * Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
//    } else if (channel == 1) {
//        //        return Cor_IDIso_MuTau_Muon_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l2Pt, l2Eta) ;
//        return Cor_IDIso_MuTau_Muon_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
//    } else return 1;
//    return 1;
//
//
//}
float getCorrFactorEMbed(int mcdata, int channel, float l1Pt, float l1Eta, float l2Pt, float l2Eta, TF1* TriggerWeightBarrel, TF1* TriggerWeightEndcaps) {

    if (mcdata == 2 || mcdata == 4 || mcdata == 5) {  //Data
        if (channel == 3) {
            //            return Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l1Pt, l1Eta);
            return Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
        }
        if (channel == 1) {
            //            return Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l1Pt, l1Eta) ;
            return Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
        }
    } else if (channel == 3) {  //MC
        //        return Cor_IDIso_ETau_Ele_2012(l1Pt, l1Eta) * Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l2Pt, l2Eta) ;
        return Cor_IDIso_ETau_Ele_2012(l1Pt, l1Eta) * Eff_ETauTrg_Ele_Data_2012(l1Pt, l1Eta) * Eff_ETauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
    } else if (channel == 1) {
        //        return Cor_IDIso_MuTau_Muon_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l2Pt, l2Eta) ;
        return Cor_IDIso_MuTau_Muon_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Mu_Data_2012(l1Pt, l1Eta) * Eff_MuTauTrg_Tau_Data_2012(l2Pt, l2Eta) * correctionHighPtTail_Data(l2Pt, l2Eta, TriggerWeightBarrel, TriggerWeightEndcaps);
    } else return 1;
    return 1;


}











