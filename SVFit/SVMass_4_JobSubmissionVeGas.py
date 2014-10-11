#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.
#    ************************************************************************************
# To run the svMass one need the following steps
# 1) put all the root files in a directory: InitialRootLocation = "/afs/cern.ch/work/a/abdollah/SVFit/8TeV_ROOT/"
# 2) ls *.root > textSamples.txt  (this is the name of all rootfiles to be processes)
# 3) put the textSamples.txt in the current directory
# 4) Run the python code :   python SVMass_Batch_Submit.py
# 5) the above python command produces a txt file (Prepare_JobsSubmission.sh) and a directory (OutRoot)
#  where all the submission files are there
# 6) Please first check them to be sure
# 7) Last step is soursing the script t submit the jobs ion lxbatch  "src Prepare_JobsSubmission.sh"
#    ************************************************************************************


__author__ = "abdollah"
__date__ = "$Feb 23, 2010 5:15:54 PM$"

import os
import shutil
if __name__ == "__main__":

    location = os.getcwd()

    OutLocation = "OutSubmit"
    if os.path.exists(OutLocation):
        shutil.rmtree(OutLocation)
    if not os.path.exists(OutLocation):
        os.makedirs(OutLocation)

    OutRoot = "OutRoot"
    submit_File = open("Prepare_JobsSubmission.sh", 'w')
    TextSamples = open("textSamples.txt", "r")
    FinalState = ['mmtt_tot','mmet_tot','mmmt_tot','mmme_tot','eett_tot','eemt_tot','eeet_tot','eeem_tot']
    ChannelNumber = [1,2,3,4,5,6,7,8]
    ScaleValue = [-0.03, 0, 0.03]
#    FinalState = ['mmtt','mmet','mmmt','mmme','eett','eemt','eeet','eeem','mmtt_tot','mmet_tot','mmmt_tot','mmme_tot','eett_tot','eemt_tot','eeet_tot','eeem_tot']
#    ChannelNumber = [1,2,3,4,5,6,7,8,91,92,93,94,95,96,97,98]
    for Sample in TextSamples.readlines():



#        RootLocation =  "Results/" + Sample    # For M Machines
#        InitialRootLocation = "/afs/cern.ch/work/a/abdollah/SVFit/8TeV_ROOT/"+ Sample   # for lxplus
        InitialRootLocation =  "outRoot_V3/" + Sample   # for lxplus

        Tag = Sample[:-6]
        for chanl in range(len(FinalState)):
            for scale in ScaleValue:
                name_out = OutLocation + "/" + "submit_" + Tag +"_" +str(FinalState[chanl]) +"_" +str(scale) +".sh"
                outFile = open(name_out, 'w')
                command1 = ""
    #            command1 = "source $VO_CMS_SW_DIR/cmsset_default.sh " + "\n"  # Just for M Machines
                command1 = command1 + "cd " + location + "\n"
                command1 = command1 + "eval `scram runtime -sh` " + "\n\n"
                command1 = command1 + "nsvfitStandalone " +  str(ChannelNumber[chanl]) + " "+ str(scale)  +" "+  Tag+str(FinalState[chanl]) +str(scale)+ ".root"
                command1 = command1 +  " "+ InitialRootLocation + "\n"
    #            command1 = command1 +  "mv " + Tag+FinalState[chanl] + ".root   " + OutRoot
                outFile.write(command1)
                outFile.close()
    #            command2 = "qsub -q localgrid@cream02 -o " + Tag + ".stdout -e "  + Tag + ".stderr  " + name_out + "\n"  # for Ma mchines
                command2 = "bsub  -q 8nh -J  " + Tag + " < " + name_out + "\n"  # for lxplus
                submit_File.write(command2)
    submit_File.close()
