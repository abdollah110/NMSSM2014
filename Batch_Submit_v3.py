#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "abdollah"
__date__ = "$Feb 23, 2010 5:15:54 PM$"

import os
import shutil

    ########################################################################################
Run_Over = {

    ##### Unfiltered
    1:("/pnfs/iihe/cms/store/user/abdollah/HTTNtuples/53X/MC", "mc12", "45:00:00"),
    2:("/pnfs/iihe/cms/store/user/abdollah/HTTNtuples/53X/Data", "data12", "45:00:00"),
    3:("/pnfs/iihe/cms/store/user/ccaillol/HTTNtuples/53X/MC", "mc12", "45:00:00"),
    4:("/pnfs/iihe/cms/store/user/abdollah/HTTNtuples/53X/Embedded", "embed12", "45:00:00"),
    
}

    ########################################################################################
def make_submit_form(order, pnfn, data_year, timing):

    location = os.getcwd()
#    location = location.replace("/localgrid_mnt", "") #nedd to remove the first part of address while submitting
#    Sample = os.popen(("ls " + pnfn + " | sort "))
    name_submitFile = "Submit_" + data_year + "_"  + order + ".sh"
    name_haddFile = "Hadd_" + data_year + "_" +  order + ".sh"
    submit_File = open(name_submitFile, 'w')
    Hadd_File = open(name_haddFile, 'w')

    #Make loop over the files in the given directories
    for files in pnfn.readlines():
        print files[0:-1]
        for numMod in xrange(0,10):
            f = os.popen("ls " + pnfn + "/" + files[0:-1] + " | sort ")
            dir = "dcap://maite.iihe.ac.be" + pnfn + "/" + files[0:-1] + "/"
            name_out = "_" + data_year + "_" +  files[0:-1] +"_"+str(numMod)+ ".sh"
            outFile = open(name_out, 'w')
            command1 = "source $VO_CMS_SW_DIR/cmsset_default.sh " + "\n"
            command1 = command1 + "cd " + location + "\n"
            command1 = command1 + "eval `scram runtime -sh` " + "\n\n"
            command1 = command1 + "mkdir    Out_" + files[0:-1] +"_"+str(numMod)+ "\n"
            outFile.write(command1)
            #Make loop over the rootfiles in the given file
            for i in f.readlines():
                QName=i[0:-1]
                XName=int(float(QName[15:-11]))
                outName= files[0:-1] + "_"+str(XName) +".root"
                if (XName % 10 == numMod):
                    command2 = "\n" + "./nMSSM_Analysis.exe " + data_year + " "   +outName + " " + dir + "/" + i[0:-1]
                    command2 = command2 + " \n" + " mv  " + data_year + "_" +  outName + "\t" + "Out_" + files[0:-1]+"_"+str(numMod)
                    command2 = command2 + " \n\n\n"
                    outFile.write(command2)

            #Writing on out Files
#            command3 = "qsub -q localgrid@cream02.wn -o " + files[0:-1] + ".stdout -e " + files[0:-1] + ".stderr -l walltime=" + timing + "  " + name_out + "\n"
            Name1=files[0:-1]
            shortName=Name1[0:-10]+"_"+str(numMod)
            command3 = "qsub -q localgrid@cream02 -o " + shortName + ".stdout -e " + shortName + ".stderr -l walltime=" + timing + "  " + name_out + "\n"
            command4 = "hadd -f ROOT/" + data_year + "/" + files[0:-1] +"_"+str(numMod)+ ".root\t" + "Out_" + files[0:-1] +"_"+str(numMod)+ "/*.root" + "\n"
            submit_File.write(command3)
            Hadd_File.write(command4)

    outFile.close()
    submit_File.close()
    Hadd_File.close()
def make_Ready(order, pnfn, data_year, timing,outFile):
	
	Sample = os.popen(("ls " + pnfn + " | sort "))
	for files in Sample.readlines():
		command= ""
		command = command +  '"'+files[0:-1] + ','+ data_year+ ','  + timing+'"\n'
		outFile.write(command)
	

		


    ########################################################################################
if __name__ == "__main__":
     outFile = open("SelectSample.txt", 'w')
     for i in Run_Over:
        R1, R2, R3 = Run_Over[i]
        print "preparing the submission files for-->  " + R1 + "  which is " + R2 + "\n" 
		
	make_Ready(str(i), R1, R2, R3, outFile)
     outFile.close()
       # make_submit_form(str(i), R1, R2, R3)

#    rm TotalSubmit.sh; ls Submit_* | xargs -n 1 -I {} echo 'sh {}' >> TotalSubmit.sh
#    rm TotalHadd.sh;   ls Hadd_* | xargs -n 1 -I {} echo 'sh {}' >> TotalHadd.sh
    
