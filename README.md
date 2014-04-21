ZHAnalysis_2013
===============

ZH Analysis  for Summer2013 Publication


rm -rf HiggsAnalysis/HiggsToTauTau/setup/8/*.*
cp add_bbb_errors_1.py HiggsAnalysis/HiggsToTauTau/scripts/ 
cp add_bbb_errors_2.py HiggsAnalysis/HiggsToTauTau/scripts/ 
cp setup-datacards.py HiggsAnalysis/HiggsToTauTau/scripts/
cp cvs2local.py  HiggsAnalysis/HiggsToTauTau/scripts/
cp compareLimits.C  HiggsAnalysis/HiggsToTauTau/macros/ 
cp  -r calcul_limites_7.sh calcul_limites_8.sh calcul_limites_78.sh files  cgs_and_unc copy_cgs.sh .
cp HTT_EM_X_template.C HTT_MT_X_template.C HTT_ET_X_template.C HTT_TT_X_template.C  HiggsAnalysis/HiggsToTauTau/test/templates/
cp produce_macros.py  HiggsAnalysis/HiggsToTauTau/test/
cp produce_macros_2.py draw_postfit.C make_postfit_plots_78.sh  HiggsAnalysis/HiggsToTauTau/test/
scramv1 b

root -l
.L draw_postfit.C++

To do the first time and when you change the files in cgs_and_unc/ or files/:

1. sh copy_cgs.sh #this copies the cgs, unc and root files with special naming conventions to the appropriate directories

To compute limits:

 sh calcul_limites_7.sh 
 sh calcul_limites_8.sh 
 sh calcul_limites_78.sh

These sh files work for expected only limit plots. Just remove all the --expectedOnly and expectedOnly=true to get observed limits also.
The equivalent option to -t -1 is --no-prefit, to add to limit .py ... lines. It should be used with expectedOnly " --no-prefit         Skip th
e fit that is usually performed before the asymptotic limits are calculated. Use this option together with option --expectedOnly to calculate e
xpected limits for data blinding. [Default: False]"

To get the plots from file7.root, file78.root, file8.root:
1. root -l
2.  .x HiggsAnalysis/HiggsToTauTau/macros/compareLimits.C+("Limits78/file78.root", "tt, em, et, mt, cmb", true, false, "sm-xsec", 0., 30.) # To
 compare limits by channel
3.  .x HiggsAnalysis/HiggsToTauTau/macros/plotAsymptotic.C+("Limits78/file78.root", "mt", "LIMIT", 0., -1.,false, "125","CMS Preliminary,  ZH#r
ightarrowllLL,  4.9 fb^{-1} at 7 TeV, 19.8 fb^{-1} at 8 TeV, #mu#tau", "m_{H} [GeV]","95% CL limit on #sigma/#sigma_{SM}", false) #asymptotic l
imit for mt final state. Just change the second argument to have the other channels (mt, et, tt, em or cmb)

To get the postfit plots:
1. cd HiggsAnalysis/HiggsToTauTau/test/
2. sh make_postfit_plots_78.sh #you need to have computed the limits before for 7 and 8 TeV, sh calcul_limites_78.sh, you will get all the post
fit plots per channel, period, combined, ... in Postfit/ directory. The pulls are in fitresults/ .