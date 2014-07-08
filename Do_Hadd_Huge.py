#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "abdollahmohammadi"
__date__ = "$Mar 18, 2013 3:02:47 PM$"

if __name__ == "__main__":
    import os
    TextSamples = open("mmm.txt", "r")

    #########################################   Making the multicrab.cfg ############################
    location = os.getcwd()
    _files_Root = open("source_Hadd_Huge.txt", "w")
    for samples in TextSamples.readlines():
        command = ""
        for i in range(0, 10):
            name = samples[:-14]+"_"+str(i)+".root "
            command = command + "hadd   "+ name +samples[0:-1]+ "/*" + "Ntuples_*"+str(i) + "_*_*.root" + "\n"
        command = command + "hadd   tot" + samples[:-14] +".root " +samples[:-14]+"_*" +".root \n"
        command = command +  "rm   "+ samples[:-14]+"_*" +".root \n\n"

        _files_Root.write(command)
    _files_Root.close()
