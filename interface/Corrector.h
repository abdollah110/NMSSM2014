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
#include "myevent.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include "myobject.h"
#include <TLorentzVector.h>
using namespace std;

//******************** ETau/MuTau turn-on *****************************

double correctionHighPtTail_Data(myobject const& a) {
    //Additional corrections for high pT taus in muTau & eTau (Arun, Mar14)
    TF1 *TriggerWeightBarrel = new TF1("AddTriggerWeightMuTauBarrel", "1 - 9.01280e-04*(x - 140) + 4.81592e-07*(x - 140)*(x-140)", 0., 800.);
    TF1 *TriggerWeightEndcaps = new TF1("AddTriggerWeightMuTauEndcaps", "1 - 1.81148e-03*(x - 60) + 5.44335e-07*(x - 60)*(x-60)", 0., 800.);

    //    Double_t DataValBarrel_pt = 0.3 + 0.7 * TriggerWeightBarrel->Eval(a.pt);
    //    Double_t DataValBarrel_800 = 0.3 + 0.7 * TriggerWeightBarrel->Eval(800.);
    //
    //    Double_t DataValEndcaps_pt = 0.3 + 0.7 * TriggerWeightEndcaps->Eval(a.pt);
    //    Double_t DataValEndcaps_400 = 0.3 + 0.7 * TriggerWeightEndcaps->Eval(400.);

    if (a.pt > 140 && a.pt < 800 && fabs(a.eta) < 1.5) return (0.3 + 0.7 * TriggerWeightBarrel->Eval(a.pt));
    else if (a.pt >= 800 && fabs(a.eta) <= 1.5) return (0.3 + 0.7 * TriggerWeightBarrel->Eval(800.));
    else if (a.pt > 60 && a.pt < 400 && fabs(a.eta) > 1.5) return (0.3 + 0.7 * TriggerWeightEndcaps->Eval(a.pt));
    else if (a.pt >= 400 && fabs(a.eta) >= 1.5) return (0.3 + 0.7 * TriggerWeightEndcaps->Eval(400.));
        //    if (a.pt < 800 && fabs(a.eta) < 1.5) return (0.3 + 0.7 * TriggerWeightBarrel->Eval(a.pt));
        //    else if (a.pt >= 800 && fabs(a.eta) <= 1.5) return (0.3 + 0.7 * TriggerWeightBarrel->Eval(800.));
        //    else if (a.pt < 400 && fabs(a.eta) > 1.5) return (0.3 + 0.7 * TriggerWeightEndcaps->Eval(a.pt));
        //    else if (a.pt >= 400 && fabs(a.eta) >= 1.5) return (0.3 + 0.7 * TriggerWeightEndcaps->Eval(400.));
    else return 1;
}

double correctionHighPtTail_MC(myobject const& a) {
    //Additional corrections for high pT taus in muTau & eTau (Arun, Mar14)
    TF1 *TriggerWeightBarrel = new TF1("AddTriggerWeightMuTauBarrel", "1 - 9.01280e-04*(x - 140) + 4.81592e-07*(x - 140)*(x-140)", 0., 800.);
    TF1 *TriggerWeightEndcaps = new TF1("AddTriggerWeightMuTauEndcaps", "1 - 1.81148e-03*(x - 60) + 5.44335e-07*(x - 60)*(x-60)", 0., 800.);

    //    Double_t MCValBarrel_pt = TriggerWeightBarrel->Eval(a.pt);
    //    Double_t MCValBarrel_800 = TriggerWeightBarrel->Eval(800.);
    //
    //    Double_t MCValEndcaps_pt = TriggerWeightEndcaps->Eval(a.pt);
    //    Double_t MCValEndcaps_400 = TriggerWeightEndcaps->Eval(400.);


    if (a.pt > 140 && a.pt < 800 && fabs(a.eta) < 1.5) return TriggerWeightBarrel->Eval(a.pt);
    else if (a.pt >= 800 && fabs(a.eta) <= 1.5) return TriggerWeightBarrel->Eval(800.);
    else if (a.pt > 60 && a.pt < 400 && fabs(a.eta) > 1.5) return TriggerWeightEndcaps->Eval(a.pt);
    else if (a.pt >= 400 && fabs(a.eta) >= 1.5) return TriggerWeightEndcaps->Eval(400.);
    else return 1;

}

//double efficiency(double m, double m0, double sigma, double alpha, double n, double norm) {
//    const double sqrtPiOver2 = 1.2533141373;
//    const double sqrt2 = 1.4142135624;
//    double sig = fabs((double) sigma);
//    double t = (m - m0) / sig;
//    if (alpha < 0)
//        t = -t;
//    double absAlpha = fabs(alpha / sig);
//    double a = TMath::Power(n / absAlpha, n) * exp(-0.5 * absAlpha * absAlpha);
//    double b = absAlpha - n / absAlpha;
//    double ApproxErf;
//    double arg = absAlpha / sqrt2;
//    if (arg > 5.) ApproxErf = 1;
//    else if (arg < -5.) ApproxErf = -1;
//    else ApproxErf = TMath::Erf(arg);
//    double leftArea = (1 + ApproxErf) * sqrtPiOver2;
//    double rightArea = (a * 1 / TMath::Power(absAlpha - b, n - 1)) / (n - 1);
//    double area = leftArea + rightArea;
//    if (t <= absAlpha) {
//        arg = t / sqrt2;
//        if (arg > 5.) ApproxErf = 1;
//        else if (arg < -5.) ApproxErf = -1;
//        else ApproxErf = TMath::Erf(arg);
//        return norm * (1 + ApproxErf) * sqrtPiOver2 / area;
//    } else {
//        return norm * (leftArea + a * (1 / TMath::Power(t - b, n - 1) -
//                1 / TMath::Power(absAlpha - b, n - 1)) / (1 - n)) / area;
//    }
//
//}

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


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// *** 2011 *** //
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float Cor_ID_Iso_Mu_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 1.5) return 0.9895;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) >= 1.5) return 1.0303;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 1.5) return 1.0168;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) >= 1.5) return 1.0247;
    if (a.pt > 20 && fabs(a.eta) < 1.5) return 1.0061;
    if (a.pt > 20 && fabs(a.eta) >= 1.5) return 1.0144;
    return 1.0;
}

float Cor_ID_Iso_Ele_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 1.479) return 1.0396;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) >= 1.479) return 0.9758;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 1.479) return 0.9622;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) >= 1.479) return 1.1483;
    if (a.pt >= 20 && fabs(a.eta) < 1.479) return 0.9849;
    if (a.pt >= 20 && fabs(a.eta) >= 1.479) return 1.0117;
    return 1.0;
}

//*****************************************************
//***************** ETau channel **********************
//*****************************************************

float Cor_IDIso_ETau_Ele_2012(myobject const& a) {
    if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) < 1.479) return 0.8999 * 0.9417;
    else if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) > 1.479) return 0.7945 * 0.9471;
    else if (a.pt >= 30 && fabs(a.eta) < 1.479) return 0.9486 * 0.9804;
    else if (a.pt >= 30 && fabs(a.eta) > 1.479) return 0.8866 * 0.9900;
    else return 1.0;
}

float Cor_ID_ETau_Ele_2012(myobject const& a) {
    if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) < 1.479) return 0.8999;
    else if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) > 1.479) return 0.7945;
    else if (a.pt >= 30 && fabs(a.eta) < 1.479) return 0.9486;
    else if (a.pt >= 30 && fabs(a.eta) > 1.479) return 0.8866;
    else return 1.0;
}

float Cor_Iso_ETau_Ele_2012(myobject const& a) {
    if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) < 1.479) return 0.9417;
    else if (a.pt >= 24 && a.pt < 30 && fabs(a.eta) > 1.479) return 0.9471;
    else if (a.pt >= 30 && fabs(a.eta) < 1.479) return 0.9804;
    else if (a.pt >= 30 && fabs(a.eta) > 1.479) return 0.9900;
    else return 1.0;
}
//   Trigger   *****************************************************

float Eff_ETauTrg_Ele_MC_2012(myobject const& a) {
    if (fabs(a.eta) < 1.479) return efficiency(a.pt, 21.7243, 0.619015, 0.739301, 1.34903, 1.02594);
    else return efficiency(a.pt, 22.1217, 1.34054, 1.8885, 1.01855, 4.7241);
}

float Eff_ETauTrg_Ele_Data_2012(myobject const& a) {
    if (fabs(a.eta) < 1.479) return efficiency(a.pt, 22.9704, 1.0258, 1.26889, 1.31024, 1.06409);
    else return efficiency(a.pt, 21.9816, 1.40993, 0.978597, 2.33144, 0.937552);
}

//float Eff_ETauTrg_Tau_MC_2012(myobject const& a) {
//    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 18.525766, 0.275904, 0.126185, 4.957594, 0.915910);
//    else return efficiency(a.pt, 18.552006, 0.632002, 0.426891, 133.934952, 0.866543);
//}
//
//float Eff_ETauTrg_Tau_Data_2012(myobject const& a) {
//    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 18.538229, 0.651562, 0.324869, 13.099048, 0.902365);
//    else return efficiency(a.pt, 18.756548, 0.230732, 0.142859, 3.358497, 0.851919);
//}
// ABCD

float Eff_ETauTrg_Tau_Data_2012(myobject const& a) {
    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 1.83211e+01, -1.89051e+00, 3.71081e+00, 1.06628e+00, 1.28559e+00); //data barrel
    else return efficiency(a.pt, 1.80812e+01, 1.39482e+00, 1.14305e+00, 1.08989e+01, 8.97087e-01); //data end cap
}
// MC ABCD

float Eff_ETauTrg_Tau_MC_2012(myobject const& a) {
    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 1.83709e+01, 1.37806e-01, 1.64478e-01, 1.44798e+00, 9.92673e-01); //MC barrel
    else return efficiency(a.pt, 1.83074e+01, 1.43406e+00, 1.40743e+00, 1.41501e+02, 9.19457e-01); //MC end cap
}

//*****************************************************
//***************** MuTau channel *********************
//*****************************************************

float Cor_IDIso_MuTau_Muon_2012(myobject const& a) {
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9818 * 0.9494;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 1.2) return 0.9829 * 0.9835;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 2.1) return 0.9869 * 0.9923;
    if (a.pt >= 30 && fabs(a.eta) < 0.8) return 0.9852 * 0.9883;
    if (a.pt >= 30 && fabs(a.eta) < 1.2) return 0.9852 * 0.9937;
    if (a.pt >= 30 && fabs(a.eta) < 2.1) return 0.9884 * 0.9996;
    return 1.0;
}

float Cor_ID_MuTau_Muon_2012(myobject const& a) {
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9818;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 1.2) return 0.9829;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 2.1) return 0.9869;
    if (a.pt >= 30 && fabs(a.eta) < 0.8) return 0.9852;
    if (a.pt >= 30 && fabs(a.eta) < 1.2) return 0.9852;
    if (a.pt >= 30 && fabs(a.eta) < 2.1) return 0.9884;
    return 1.0;
}

float Cor_Iso_MuTau_Muon_2012(myobject const& a) {
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9494;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 1.2) return 0.9835;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) < 2.1) return 0.9923;
    if (a.pt >= 30 && fabs(a.eta) < 0.8) return 0.9883;
    if (a.pt >= 30 && fabs(a.eta) < 1.2) return 0.9937;
    if (a.pt >= 30 && fabs(a.eta) < 2.1) return 0.9996;
    return 1.0;
}
//   Trigger   *****************************************************

float Eff_MuTauTrg_Mu_Data_2012(myobject const& a) {
    if (a.eta < -1.2) return efficiency(a.pt, 15.9977, 0.0000764004, 6.4951e-8, 1.57403, 0.865325);
    else if (a.eta < -0.8) return efficiency(a.pt, 17.3974, 0.804001, 1.47145, 1.24295, 0.928198);
    else if (a.eta < 0) return efficiency(a.pt, 16.4307, 0.226312, 0.265553, 1.55756, 0.974462);
    else if (a.eta < 0.8) return efficiency(a.pt, 17.313, 0.662731, 1.3412, 1.05778, 1.26624);
    else if (a.eta < 1.2) return efficiency(a.pt, 16.9966, 0.550532, 0.807863, 1.55402, 0.885134);
    else if (a.eta > 1.2) return efficiency(a.pt, 15.9962, 0.000106195, 4.95058e-8, 1.9991, 0.851294);
    else return 1;
}

float Eff_MuTauTrg_Mu_MC_2012(myobject const& a) {
    if (a.eta < -1.2) return efficiency(a.pt, 16.0051, 2.45144e-5, 4.3335e-9, 1.66134, 0.87045);
    else if (a.eta < -0.8) return efficiency(a.pt, 17.3135, 0.747646, 1.21803, 1.40611, 0.934983);
    else if (a.eta < 0) return efficiency(a.pt, 15.9556, 0.0236127, 0.00589832, 1.75409, 0.981338);
    else if (a.eta < 0.8) return efficiency(a.pt, 15.9289, 0.0271317, 0.00448573, 1.92101, 0.978625);
    else if (a.eta < 1.2) return efficiency(a.pt, 16.5678, 0.328333, 0.354533, 1.67085, 0.916992);
    else if (a.eta > 1.2) return efficiency(a.pt, 15.997, 7.90069e-5, 4.40039e-8, 1.66272, 0.884502);
    else return 1;
}

//float Eff_MuTauTrg_Tau_MC_2012(myobject const& a) {
//    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 18.537441, 1.385790, 3.102076, 1.002486, 6.275127);
//    else return efficiency(a.pt, 18.393366, 1.526254, 2.021678, 124.741631, 0.893280);
//}
//
//float Eff_MuTauTrg_Tau_Data_2012(myobject const& a) {
//    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 18.604910, 0.276042, 0.137039, 2.698437, 0.94721);
//    else return efficiency(a.pt, 18.701715, 0.216523, 0.148111, 2.245081, 0.895320);
//}


// ABCD      antiEMed

float Eff_MuTauTrg_Tau_Data_2012(myobject const& a) {
    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 1.83211e+01, -1.89051e+00, 3.71081e+00, 1.06628e+00, 1.28559e+00); //data barrel
    else return efficiency(a.pt, 1.80812e+01, 1.39482e+00, 1.14305e+00, 1.08989e+01, 8.97087e-01); //data endcap
}

// MC ABCD   antiEMed

float Eff_MuTauTrg_Tau_MC_2012(myobject const& a) {
    if (fabs(a.eta) < 1.5) return efficiency(a.pt, 1.83709e+01, 1.37806e-01, 1.64478e-01, 1.44798e+00, 9.92673e-01); //MC barrel
    else return efficiency(a.pt, 1.83074e+01, 1.43406e+00, 1.40743e+00, 1.41501e+02, 9.19457e-01); //MC endcap
}

//*****************************************************
//****************** EMu channel **********************
//*****************************************************

float Cor_IDIso_EMu_Mu_2012(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 0.8) return 0.9771;
    if (a.pt >= 10 && a.pt < 15 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9746;
    if (a.pt >= 10 && a.pt < 15 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9644;
    if (a.pt >= 10 && a.pt < 15 && 1.6 <= fabs(a.eta)) return 0.9891;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 0.8) return 0.9548;
    if (a.pt >= 15 && a.pt < 20 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9701;
    if (a.pt >= 15 && a.pt < 20 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9766;
    if (a.pt >= 15 && a.pt < 20 && 1.6 <= fabs(a.eta)) return 0.9892;
    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) < 0.8) return 0.9648;
    if (a.pt >= 20 && a.pt < 25 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9836;
    if (a.pt >= 20 && a.pt < 25 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9820;
    if (a.pt >= 20 && a.pt < 25 && 1.6 <= fabs(a.eta)) return 0.9909;
    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9676;
    if (a.pt >= 25 && a.pt < 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9817;
    if (a.pt >= 25 && a.pt < 30 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9886;
    if (a.pt >= 25 && a.pt < 30 && 1.6 <= fabs(a.eta)) return 0.9883;
    if (a.pt >= 30 && a.pt < 35 && fabs(a.eta) < 0.8) return 0.9730;
    if (a.pt >= 30 && a.pt < 35 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9833;
    if (a.pt >= 30 && a.pt < 35 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9910;
    if (a.pt >= 30 && a.pt < 35 && 1.6 <= fabs(a.eta)) return 0.9900;
    if (a.pt >= 35 && fabs(a.eta) < 0.8) return 0.9826;
    if (a.pt >= 35 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9841;
    if (a.pt >= 35 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9900;
    if (a.pt >= 35 && 1.6 <= fabs(a.eta)) return 0.9886;
    return 1.0;
}

float Cor_IDIso_EMu_Ele_2012(myobject const& a) {
    if (a.pt > 10.0 && a.pt <= 15.0 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 0.7654;
    if (a.pt > 10.0 && a.pt <= 15.0 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 0.7693;
    if (a.pt > 10.0 && a.pt <= 15.0 && fabs(a.eta) >= 1.5) return 0.5719;
    if (a.pt > 15.0 && a.pt <= 20.0 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 0.8394;
    if (a.pt > 15.0 && a.pt <= 20.0 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 0.8457;
    if (a.pt > 15.0 && a.pt <= 20.0 && fabs(a.eta) >= 1.5) return 0.7024;
    if (a.pt > 20.0 && a.pt <= 25.0 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 0.8772;
    if (a.pt > 20.0 && a.pt <= 25.0 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 0.8530;
    if (a.pt > 20.0 && a.pt <= 25.0 && fabs(a.eta) >= 1.5) return 0.7631;
    if (a.pt > 25.0 && a.pt <= 30.0 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 0.9006;
    if (a.pt > 25.0 && a.pt <= 30.0 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 0.8874;
    if (a.pt > 25.0 && a.pt <= 30.0 && fabs(a.eta) >= 1.5) return 0.8092;
    if (a.pt > 30.0 && a.pt <= 35.0 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 0.9261;
    if (a.pt > 30.0 && a.pt <= 35.0 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 0.9199;
    if (a.pt > 30.0 && a.pt <= 35.0 && fabs(a.eta) >= 1.5) return 0.8469;
    if (a.pt > 35.0 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 0.9514;
    if (a.pt > 35.0 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 0.9445;
    if (a.pt > 35.0 && fabs(a.eta) >= 1.5) return 0.9078;
    return 1.0;
}

float Corr_EMuTrg_Mu_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) <= 1.5) return 1.01;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) > 1.5) return 1.03;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) <= 1.5) return 0.99;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) > 1.5) return 1.07;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) <= 1.5) return 0.99;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) > 1.5) return 1.04;
    if (a.pt > 30 && fabs(a.eta) <= 1.5) return 0.992;
    if (a.pt > 30 && fabs(a.eta) > 1.5) return 1.06;
    return 1;
}

float Corr_EMuTrg_Ele_2011(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) <= 1.479) return 0.98;
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) > 1.479) return 0.97;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) <= 1.479) return 1.00;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) > 1.479) return 1.05;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) <= 1.479) return 1.001;
    if (a.pt >= 20 && a.pt < 30 && fabs(a.eta) > 1.479) return 1.00;
    if (a.pt > 30 && fabs(a.eta) <= 1.479) return 1.003;
    if (a.pt > 30 && fabs(a.eta) > 1.479) return 1.008;
    return 1;
}

float Corr_EMuTrg_MuLeg_2012(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) < 0.8) return 0.9829;
    if (a.pt >= 10 && a.pt < 15 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9745;
    if (a.pt >= 10 && a.pt < 15 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9943;
    if (a.pt >= 10 && a.pt < 15 && 1.6 <= fabs(a.eta)) return 0.9158;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) < 0.8) return 0.9850;
    if (a.pt >= 15 && a.pt < 20 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9852;
    if (a.pt >= 15 && a.pt < 20 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9743;
    if (a.pt >= 15 && a.pt < 20 && 1.6 <= fabs(a.eta)) return 0.9333;
    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) < 0.8) return 0.9951;
    if (a.pt >= 20 && a.pt < 25 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9610;
    if (a.pt >= 20 && a.pt < 25 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9716;
    if (a.pt >= 20 && a.pt < 25 && 1.6 <= fabs(a.eta)) return 0.9459;
    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) < 0.8) return 0.9869;
    if (a.pt >= 25 && a.pt < 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9779;
    if (a.pt >= 25 && a.pt < 30 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9665;
    if (a.pt >= 25 && a.pt < 30 && 1.6 <= fabs(a.eta)) return 0.9501;
    if (a.pt >= 30 && a.pt < 35 && fabs(a.eta) < 0.8) return 0.9959;
    if (a.pt >= 30 && a.pt < 35 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9881;
    if (a.pt >= 30 && a.pt < 35 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9932;
    if (a.pt >= 30 && a.pt < 35 && 1.6 <= fabs(a.eta)) return 0.9391;
    if (a.pt >= 35 && fabs(a.eta) < 0.8) return 0.9986;
    if (a.pt >= 35 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.2) return 0.9540;
    if (a.pt >= 35 && 1.2 <= fabs(a.eta) && fabs(a.eta) < 1.6) return 0.9549;
    if (a.pt >= 35 && 1.6 <= fabs(a.eta)) return 0.9386;
    return 1.0;
}

float Corr_EMuTrg_EleLeg_2012(myobject const& a) {
    if (a.pt >= 10 && a.pt < 15 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9548;
    if (a.pt >= 10 && a.pt < 15 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.5) return 0.9015;
    if (a.pt >= 10 && a.pt < 15 && 1.5 <= fabs(a.eta)) return 0.9017;
    if (a.pt >= 15 && a.pt < 20 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9830;
    if (a.pt >= 15 && a.pt < 20 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.5) return 0.9672;
    if (a.pt >= 15 && a.pt < 20 && 1.5 <= fabs(a.eta)) return 0.9463;
    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9707;
    if (a.pt >= 20 && a.pt < 25 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.5) return 0.9731;
    if (a.pt >= 20 && a.pt < 25 && fabs(a.eta) >= 1.5) return 0.9691;
    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 0.9768;
    if (a.pt >= 25 && a.pt < 30 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.5) return 0.9870;
    if (a.pt >= 25 && a.pt < 30 && fabs(a.eta) >= 1.5) return 0.9727;
    if (a.pt >= 30 && a.pt < 35 && fabs(a.eta) >= 0 && fabs(a.eta) < 0.8) return 1.0047;
    if (a.pt >= 30 && a.pt < 35 && 0.8 <= fabs(a.eta) && fabs(a.eta) < 1.5) return 0.9891;
    if (a.pt >= 30 && a.pt < 35 && fabs(a.eta) >= 1.5) return 0.9858;
    if (a.pt > 35 && fabs(a.eta) >= 0.0 && fabs(a.eta) < 0.8) return 1.0063;
    if (a.pt > 35 && fabs(a.eta) >= 0.8 && fabs(a.eta) < 1.5) return 1.0047;
    if (a.pt > 35 && fabs(a.eta) >= 1.5) return 1.0015;
    return 1;
}

//*****************************************************
//***************** TauTau channel ********************
//*****************************************************

double eff2012IsoParkedTau19fb_Simone(double pt, double eta) {
    return ( 0.826969 * 0.5 * (TMath::Erf((pt - 42.2274) / 2. / 0.783258 / sqrt(pt)) + 1.)); // only one eta bin
}

double eff2012IsoParkedTau19fbMC_Simone(double pt, double eta) {
    double data_plateau = 0.826969;
    if (pt < 140.)
        return ( 0.813769 * 0.5 * (TMath::Erf((pt - 39.9322) / 2. / 0.819354 / sqrt(pt)) + 1.)); // only one eta bin
    else if (pt > 400) return data_plateau / 2.03467;
    else if (pt > 300) return data_plateau / 1.31593;
    else if (pt > 250) return data_plateau / 1.25698;
    else if (pt > 200) return data_plateau / 1.18941;
    else if (pt > 180) return data_plateau / 1.17448;
    else if (pt > 160) return data_plateau / 1.0964;
    else return data_plateau / 1.09279;
}

double eff2012IsoTau19fb_Simone(double pt, double eta) {
    // for real Taus mT<20
    if (fabs(eta) < 1.4) {
        return ( 808.411 * (0.764166 * 0.5 * (TMath::Erf((pt - 33.2236) / 2. / 0.97289 / sqrt(pt)) + 1.)) // 2012A by Bastian not split in eta
                + 4428.0 * (0.75721 * 0.5 * (TMath::Erf((pt - 39.0836) / 2. / 1.07753 / sqrt(pt)) + 1.)) // 2012B
                + 6892.158 * (0.791464 * 0.5 * (TMath::Erf((pt - 38.4932) / 2. / 1.01232 / sqrt(pt)) + 1.)) // 2012C measured in v2 only
                + 7274. * (0.779446 * 0.5 * (TMath::Erf((pt - 38.4603) / 2. / 1.01071 / sqrt(pt)) + 1.))) // 2012D measured in one go
                / (808.411 + 4428.0 + 6892.158 + 7274.);
        //                if(a.pt >= 17 && a.pt < 20 && fabs(a.eta) < 0.8) return;
    } else {
        return ( 808.411 * (0.764166 * 0.5 * (TMath::Erf((pt - 33.2236) / 2. / 0.97289 / sqrt(pt)) + 1.)) // 2012A by Bastian not split in eta
                + 4428.0 * (0.693788 * 0.5 * (TMath::Erf((pt - 37.7719) / 2. / 1.09202 / sqrt(pt)) + 1.)) // 2012B
                + 6892.158 * (0.698909 * 0.5 * (TMath::Erf((pt - 36.5533) / 2. / 1.05743 / sqrt(pt)) + 1.)) // 2012C measured in v2 only
                + 7274. * (0.703532 * 0.5 * (TMath::Erf((pt - 38.8609) / 2. / 1.05514 / sqrt(pt)) + 1.))) // 2012D measured in one go
                / (808.411 + 4428.0 + 6892.158 + 7274.);
    }
}

double eff2012IsoTau19fbMC_Simone(double pt, double eta) {
    if (fabs(eta) < 1.4) {
        return ( 0.807425 * 0.5 * (TMath::Erf((pt - 35.2214) / 2. / 1.04214 / sqrt(pt)) + 1.));
    } else {
        return ( 0.713068 * 0.5 * (TMath::Erf((pt - 33.4584) / 2. / 0.994692 / sqrt(pt)) + 1.));
    }
}

double eff2012Jet19fb(double pt, double eta) {
    return (fabs(eta) <= 2.1)*
            ((808.411 * (0.99212 * 0.5 * (TMath::Erf((pt - 31.3706) / 2. / 1.22821 / sqrt(pt)) + 1.))
            + 4428.0 * (0.99059 * 0.5 * (TMath::Erf((pt - 32.1104) / 2. / 1.23292 / sqrt(pt)) + 1.))
            + 1783.003 * (0.988256 * 0.5 * (TMath::Erf((pt - 31.3103) / 2. / 1.18766 / sqrt(pt)) + 1.))
            + 5109.155 * (0.988578 * 0.5 * (TMath::Erf((pt - 31.6391) / 2. / 1.22826 / sqrt(pt)) + 1.))
            + 4131. * (0.989049 * 0.5 * (TMath::Erf((pt - 31.9836) / 2. / 1.23871 / sqrt(pt)) + 1.))
            + 3143. * (0.988047 * 0.5 * (TMath::Erf((pt - 31.6975) / 2. / 1.25372 / sqrt(pt)) + 1.)))
            / (808.411 + 4428.0 + 1783.003 + 5109.155 + 4131 + 3143))+
            (fabs(eta) > 2.1)*
            ((808.411 * (0.969591 * 0.5 * (TMath::Erf((pt - 36.8179) / 2. / 0.904254 / sqrt(pt)) + 1.))
            + 4428.0 * (0.975932 * 0.5 * (TMath::Erf((pt - 37.2121) / 2. / 0.961693 / sqrt(pt)) + 1.))
            + 1783.003 * (0.990305 * 0.5 * (TMath::Erf((pt - 36.3096) / 2. / 0.979524 / sqrt(pt)) + 1.))
            + 5109.155 * (0.971612 * 0.5 * (TMath::Erf((pt - 36.2294) / 2. / 0.871726 / sqrt(pt)) + 1.))
            + 4131. * (0.977958 * 0.5 * (TMath::Erf((pt - 37.131) / 2. / 0.987523 / sqrt(pt)) + 1.))
            + 3143. * (0.968457 * 0.5 * (TMath::Erf((pt - 36.3159) / 2. / 0.895031 / sqrt(pt)) + 1.)))
            / (808.411 + 4428.0 + 1783.003 + 5109.155 + 4131 + 3143));
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float getCorrTriggerLep(std::string channel, std::string type, myobject const& a) {

    if (type == "mc12" || type == "embedmc12") {

        if (channel == "eltau") {
            return (Eff_ETauTrg_Ele_Data_2012(a) / Eff_ETauTrg_Ele_MC_2012(a));
        }
        if (channel == "mutau") {
            return (Eff_MuTauTrg_Mu_Data_2012(a) / Eff_MuTauTrg_Mu_MC_2012(a));
        }
    } else if (type == "data11" || type == "data12")
        return 1;
    return 1.0;

}

float getCorrTriggerTau(std::string channel, std::string type, myobject const& b) {

    if (type == "mc12" || type == "embedmc12") {
        if (channel == "eltau") {
            return ((Eff_ETauTrg_Tau_Data_2012(b) / Eff_ETauTrg_Tau_MC_2012(b)))*(correctionHighPtTail_Data(b) / correctionHighPtTail_MC(b));
        }
        if (channel == "mutau") {
            return ((Eff_MuTauTrg_Tau_Data_2012(b) / Eff_MuTauTrg_Tau_MC_2012(b)))*(correctionHighPtTail_Data(b) / correctionHighPtTail_MC(b));
        }

    } else
        return 1;
    return 1.0;
}


float getCorrIDIsoLep(std::string channel, std::string type, myobject const& a) {

    if (type == "mc12" || type == "embedmc12") {

        if (channel == "eltau") {
            return Cor_IDIso_ETau_Ele_2012(a);
        }
        if (channel == "mutau") {
            return Cor_IDIso_MuTau_Muon_2012(a);
        }
    } else if (type == "data11" || type == "data12")
        return 1;
    return 1.0;

}

float getCorrTriggerLepTau(std::string channel, std::string type, myobject const& a, myobject const& b) {

    return getCorrTriggerLep(channel, type, a) * getCorrTriggerTau(channel, type, b);
}
float getCorrIDLep(std::string channel, std::string type, myobject const& a) {

    if (type == "mc12" || type == "embedmc12") {

        if (channel == "eltau") {
            return Cor_ID_ETau_Ele_2012(a);
        }
        if (channel == "mutau") {
            return Cor_ID_MuTau_Muon_2012(a);
        }
    } else if (type == "data11" || type == "data12")
        return 1;
    return 1.0;

}

float getCorrIsoLep(std::string channel, std::string type, myobject const& a) {

    if (type == "mc12" || type == "embedmc12") {

        if (channel == "eltau") {
            return Cor_Iso_ETau_Ele_2012(a);
        }
        if (channel == "mutau") {
            return Cor_Iso_MuTau_Muon_2012(a);
        }
    } else if (type == "data11" || type == "data12")
        return 1;
    return 1.0;

}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

float getCorrFactor(std::string channel, std::string type, myobject const& a, myobject const& b, myobject const& c) {

    if (type == "mc12" || type == "embedmc12") {
        if (channel == "emu") {
            return (Cor_IDIso_EMu_Ele_2012(b) * Cor_IDIso_EMu_Mu_2012(a) * Corr_EMuTrg_MuLeg_2012(a) * Corr_EMuTrg_EleLeg_2012(b));
        }
        if (channel == "eltau") {
            //            return (Eff_ETauTrg_Ele_Data_2012(a) / Eff_ETauTrg_Ele_MC_2012(a))*(Eff_ETauTrg_Tau_Data_2012(b) / Eff_ETauTrg_Tau_MC_2012(b)) * Cor_IDIso_ETau_Ele_2012(a);
            return getCorrTriggerLepTau(channel, type, a, b) * Cor_IDIso_ETau_Ele_2012(a);
        }
        if (channel == "mutau") {
            //            return (Eff_MuTauTrg_Mu_Data_2012(a) / Eff_MuTauTrg_Mu_MC_2012(a))*(Eff_MuTauTrg_Tau_Data_2012(b) / Eff_MuTauTrg_Tau_MC_2012(b)) * Cor_IDIso_MuTau_Muon_2012(a);
            return getCorrTriggerLepTau(channel, type, a, b) * Cor_IDIso_MuTau_Muon_2012(a);
        }
        if (channel == "tautau_ditau") {
            return (eff2012IsoParkedTau19fb_Simone(a.pt, a.eta) / eff2012IsoParkedTau19fbMC_Simone(a.pt, a.eta))*(eff2012IsoParkedTau19fb_Simone(b.pt, b.eta) / eff2012IsoParkedTau19fbMC_Simone(b.pt, b.eta));
        }
        if (channel == "tautau_ditaujet") {
            return (eff2012IsoTau19fb_Simone(a.pt, a.eta) / eff2012IsoTau19fbMC_Simone(a.pt, a.eta))*(eff2012IsoTau19fb_Simone(b.pt, b.eta) / eff2012IsoTau19fbMC_Simone(b.pt, b.eta))*(eff2012Jet19fb(c.pt, c.eta));
        }
    } else if (type == "data11" || type == "data12")
        return 1;
    return 1.0;

}











