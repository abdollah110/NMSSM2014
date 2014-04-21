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
    1:("/pnfs/iihe/cms/store/user/ccaillol/ZHttNtuples_v2/53X/Data", "data11", "Ele", "25:00:00"),
    2:("/pnfs/iihe/cms/store/user/ccaillol/ZHttNtuples_v2/53X/Data", "data11", "Mu",  "25:00:00"),
    3:("/pnfs/iihe/cms/store/user/lperrini/ZHttNtuples_v2/42X/Data", "data11", "Ele", "25:00:00"),
    4:("/pnfs/iihe/cms/store/user/lperrini/ZHttNtuples_v2/42X/Data", "data11", "Mu",  "25:00:00"),
    5:("/pnfs/iihe/cms/store/user/abdollah/ZHttNtuples_v3/53X/Data", "data12", "Ele", "25:00:00"),
    6:("/pnfs/iihe/cms/store/user/abdollah/ZHttNtuples_v3/53X/Data", "data12", "Mu", "25:00:00"),
    7:("/pnfs/iihe/cms/store/user/jez/ZHttNtuples_v3/53X/Data", "data12", "Ele", "25:00:00"),
    8:("/pnfs/iihe/cms/store/user/jez/ZHttNtuples_v3/53X/Data", "data12", "Mu", "25:00:00"),
    9:("/pnfs/iihe/cms/store/user/ccaillol/ZHttNtuples_v2/53X/MC", "mc11", "Tot", "25:00:00"),
    10:("/pnfs/iihe/cms/store/user/lperrini/ZHttNtuples_v2/42X/MC", "mc11", "Tot",  "45:00:00"),
    11:("/pnfs/iihe/cms/store/user/ccaillol/ZHttNtuples_v3/53X/MC", "mc12", "Tot", "25:00:00"),
    12:("/pnfs/iihe/cms/store/user/jez/ZHttNtuples_v3/53X/MC", "mc12", "Tot", "45:00:00"),

    
#   ##### Filtered
#    1:("/pnfs/iihe/cms/store/user/ccaillol/ZHttNtuples_v2_Filtered/42X/Data", "data11", "Ele", "15:00:00"),
#    2:("/pnfs/iihe/cms/store/user/ccaillol/ZHttNtuples_v2_Filtered/42X/Data", "data11", "Mu",  "15:00:00"),
#    3:("/pnfs/iihe/cms/store/user/lperrini/FILTER2/ZHttNtuples_v2/42X/Data", "data11", "Ele", "15:00:00"),
#    4:("/pnfs/iihe/cms/store/user/lperrini/FILTER2/ZHttNtuples_v2/42X/Data", "data11", "Mu",  "15:00:00"),
#    5:("/pnfs/iihe/cms/store/user/ccaillol/FILTER2/ZHttNtuples_v2/53X/Data", "data12", "Ele", "15:00:00"),
#    6:("/pnfs/iihe/cms/store/user/ccaillol/FILTER2/ZHttNtuples_v2/53X/Data", "data12", "Mu", "15:00:00"),
#    7:("/pnfs/iihe/cms/store/user/jez/FILTER2/ZHttNtuples_v2/53X/Data", "data12", "Ele", "15:00:00"),
#    8:("/pnfs/iihe/cms/store/user/jez/FILTER2/ZHttNtuples_v2/53X/Data", "data12", "Mu", "15:00:00"),
#    9:("/pnfs/iihe/cms/store/user/ccaillol/ZHttNtuples_v2/53X/MC", "mc11", "Tot", "15:00:00"),
#    10:("/pnfs/iihe/cms/store/user/lperrini/FILTER2/ZHttNtuples_v2/42X/MC", "mc11", "Tot",  "25:00:00"),
#    11:("/pnfs/iihe/cms/store/user/abdollah/ZHttNtuples_v2/53X/MC", "mc12", "Tot", "15:00:00"),
#    12:("/pnfs/iihe/cms/store/user/jez/FILTER2/ZHttNtuples_v2/53X/MC", "mc12", "Tot", "25:00:00"),
    
}

    ########################################################################################
def make_submit_form(order, pnfn, data_year, lepton_type, timing):

    location = os.getcwd()
    location = location.replace("/localgrid_mnt", "") #nedd to remove the first part of address while submitting
    Sample = os.popen(("ls " + pnfn + " | sort | grep " + lepton_type))
    if lepton_type == "Tot":
        Sample = os.popen(("ls " + pnfn + " | sort "))
    #output name, one for submitting jobs, one for hadding root files
    name_submitFile = "Submit_" + data_year + "_" + lepton_type + "_" + order + ".sh"
    name_haddFile = "Hadd_" + data_year + "_" + lepton_type + "_" + order + ".sh"
    submit_File = open(name_submitFile, 'w')
    Hadd_File = open(name_haddFile, 'w')

    #Make loop over the files in the given directories
    for files in Sample.readlines():
        f = os.popen("ls " + pnfn + "/" + files[0:-1] + " | sort")
        dir = "dcap://maite.iihe.ac.be" + pnfn + "/" + files[0:-1] + "/"
        name_out = "__" + data_year + "_" + files[0:-1] + ".sh"
        outFile = open(name_out, 'w')
        command1 = "source $VO_CMS_SW_DIR/cmsset_default.sh " + "\n"
        command1 = command1 + "cd " + location + "\n"
        command1 = command1 + "eval `scram runtime -sh` " + "\n\n"
        command1 = command1 + "mkdir    Out_" + files[0:-1] + "\n"
        outFile.write(command1)
        #Make loop over the rootfiles in the given file
        for i in f.readlines():
            command2 = "\n" + "./ZH_Analysis.exe " + data_year + " " + lepton_type + " "  + files[0:-1] + i[0:-1] + " " + dir + "/" + i[0:-1]
            command2 = command2 + " \n" + " mv  " + data_year + "_" + lepton_type + "_" + files[0:-1] + i[0:-1] + "\t" + "Out_" + files[0:-1]
            command2 = command2 + " \n\n\n"
            outFile.write(command2)

        #Writing on out Files
        command3 = "qsub -q localgrid@cream02 -o " + files[0:-1] + ".stdout -e " + files[0:-1] + ".stderr -l walltime=" + timing + "  " + name_out + "\n"
        command4 = "hadd -f ROOT/" + data_year + "/" + files[0:-1] + ".root\t" + "Out_" + files[0:-1] + "/*.root" + "\n"
        submit_File.write(command3)
        Hadd_File.write(command4)

    outFile.close()
    submit_File.close()
    Hadd_File.close()
    ########################################################################################
if __name__ == "__main__":
    for i in Run_Over:
        R1, R2, R3, R4 = Run_Over[i]
        print "preparing the submission files for-->  " + R1+R2+R3
	make_submit_form(str(i), R1, R2, R3, R4)

    maindir = 'ROOT'
    dirs = ['data11','data12','mc11','mc12']
    if  os.path.exists(maindir):
	shutil.rmtree(maindir)
    for i in dirs:
	if not os.path.exists(maindir+"/"+i):
		os.makedirs(maindir+'/'+i)
    	     
    _Total = open('Do_total.txt', 'w')	     
    command = "rm TotalSubmit.sh\n"
    command = command + 'ls Submit_* | xargs -n 1 -I {} echo "sh {}" >> TotalSubmit.sh\n'
    command = command + 'rm TotalHadd.sh\n'
    command = command + 'ls Hadd_* | xargs -n 1 -I {} echo "sh {}" >> TotalHadd.sh\n'
    _Total.write(command)
    		

#    rm TotalSubmit.sh; ls Submit_* | xargs -n 1 -I {} echo 'sh {}' >> TotalSubmit.sh
#    rm TotalHadd.sh;   ls Hadd_* | xargs -n 1 -I {} echo 'sh {}' >> TotalHadd.sh
    
