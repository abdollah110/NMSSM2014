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
    1:("/pnfs/iihe/cms/store/user/abdollah/HTTNtuples_v3/53X/MC", "mc12", "75:00:00"),
    2:("/pnfs/iihe/cms/store/user/abdollah/HTTNtuples_v3/53X/Data", "data12", "75:00:00"),
   # 3:("/pnfs/iihe/cms/store/user/abdollah/HTTNtuples_v3/53X/Embed_MC", "embedmc12", "75:00:00"),
   # 4:("/pnfs/iihe/cms/store/user/abdollah/HTTNtuples_v3/53X/Embed_Data", "embeddata12", "75:00:00"),

    5:("/pnfs/iihe/cms/store/user/ccaillol/HTTNtuples_v3/53X/MC", "mc12", "75:00:00"),
    6:("/pnfs/iihe/cms/store/user/ccaillol/HTTNtuples_v3/53X/Embed_MC_v2", "embedmc12", "75:00:00"),
    7:("/pnfs/iihe/cms/store/user/ccaillol/HTTNtuples_v3/53X/Embed_Data_v3", "embeddata12", "75:00:00"),

}
    ########################################################################################
def make_Ready(order, pnfn, data_year, timing,outFile):

	Sample = os.popen(("ls " + pnfn + " | sort "))
	command = ""
	for files in Sample.readlines():
                DIR = pnfn + '/' +files[0:-1]
		FileSize= len([name for name in os.listdir(DIR) if os.path.isfile(os.path.join(DIR, name))])
		outFile.write('%s%s%s%s%s%s%d%s' %(DIR , ',', data_year, ','  , timing, ',',FileSize,'\n'))
		#command =   DIR + ','+ data_year+ ','  + timing+ ','+str(FileSize)+'\n'
		#outFile.write(command)

    ########################################################################################
if __name__ == "__main__":
     outFile = open("SelectSample.txt", 'w')
     for i in Run_Over:
        R1, R2, R3 = Run_Over[i]
        print "preparing the submission files for-->  " + R1 + "  which is " + R2 + "\n"

	make_Ready(str(i), R1, R2, R3, outFile)
     outFile.close()


