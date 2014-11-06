#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "abdollah"
__date__ = "$Feb 23, 2010 5:15:54 PM$"

import os
import shutil
    ########################################################################################
def make_submit_form():
    TextSamples = open("SelectSample.txt", "r")
    location = os.getcwd()
    location = location.replace("/grid_mnt/volumes__Z2Volume__localgrid", "/localgrid")
    name_submitFile = "Submit_" +  "_"  + ".sh"
    name_haddFile = "Hadd_" +  ".sh"
    submit_File = open(name_submitFile, 'w')
    Hadd_File = open(name_haddFile, 'w')
    for files in TextSamples.readlines():

        pnfn= files[0:-1].split(",")[0]
        data_year= files[0:-1].split(",")[1]
        timing= files[0:-1].split(",")[2]
        FileSize= int(files[0:-1].split(",")[3])

        sampleName2= files[0:-1].split("/")[10]
        sampleName= sampleName2.split(",")[0]
        print "sampleName=", sampleName

        NumberToBedevided= 10
        if FileSize > 500 : NumberToBedevided=20
        if FileSize > 1200 : NumberToBedevided=30
        if FileSize > 2000 : NumberToBedevided=40
        for numMod in xrange(0,NumberToBedevided):
            f = os.popen("ls " + pnfn + "/" + " | sort ")
            dir = "dcap://maite.iihe.ac.be" + pnfn + "/"
            name_out = "_" +  sampleName +"_"+str(numMod)+ ".sh"
            outFile = open(name_out, 'w')
            command1 = "source $VO_CMS_SW_DIR/cmsset_default.sh " + "\n"
            command1 = command1 + "cd " + location + "\n"
            command1 = command1 + "eval `scram runtime -sh` " + "\n\n"
            command1 = command1 + "mkdir    Out_" + sampleName +"_"+str(numMod)+ "\n"
            outFile.write(command1)
            #Make loop over the rootfiles in the given file
            for i in f.readlines():
                QName=i[0:-1]
                XName=int(float(QName[15:-11]))
                outName= sampleName + "_"+str(XName) +".root"
                if (XName % NumberToBedevided == numMod):
                    command2 = "\n" + "./nMSSM_Analysis.exe " + data_year + " "   +outName + " " + dir + "/" + i[0:-1]
                    command2 = command2 + " \n" + " mv  " +  outName + "\t" + "Out_" + sampleName+"_"+str(numMod)
#                    command2 = command2 + " \n" + " mv  " + data_year + "_" +  outName + "\t" + "Out_" + sampleName+"_"+str(numMod)
                    command2 = command2 + " \n\n\n"
                    outFile.write(command2)

            #Writing on out Files
#            command3 = "qsub -q localgrid@cream02.wn -o " + files[0:-1] + ".stdout -e " + files[0:-1] + ".stderr -l walltime=" + timing + "  " + name_out + "\n"
            Name1=sampleName
            shortName=Name1[0:-10]+"_"+str(numMod)
            command3 = "qsub -q localgrid@cream02 -o " + shortName + ".stdout -e " + shortName + ".stderr -l walltime=" + timing + "  " + name_out + "\n"
            command4 = "hadd -f ROOT/" + sampleName +"_"+str(numMod)+ ".root\t" + "Out_" + sampleName +"_"+str(numMod)+ "/*.root" + "\n"
            submit_File.write(command3)
            Hadd_File.write(command4)

    outFile.close()
    submit_File.close()
    Hadd_File.close()
    ########################################################################################
if __name__ == "__main__":
     make_submit_form()

    