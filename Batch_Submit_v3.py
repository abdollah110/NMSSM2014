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
def make_Ready(order, pnfn, data_year, timing,outFile):
	
	Sample = os.popen(("ls " + pnfn + " | sort "))
	for files in Sample.readlines():
		command =   pnfn + "/" +files[0:-1] + ','+ data_year+ ','  + timing+'\n'
		outFile.write(command)

    ########################################################################################
if __name__ == "__main__":
     outFile = open("SelectSample.txt", 'w')
     for i in Run_Over:
        R1, R2, R3 = Run_Over[i]
        print "preparing the submission files for-->  " + R1 + "  which is " + R2 + "\n" 
		
	make_Ready(str(i), R1, R2, R3, outFile)
     outFile.close()

    
