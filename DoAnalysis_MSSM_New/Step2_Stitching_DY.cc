#include <string.h>
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include <iostream>
#include <string>

std::string InputFileLocation = "../FileROOT/MSSMROOTFiles/";

void Step2_Stitching_DY() {

    const int numBG = 5;
    // Do stiching for DY Background
    //    Target_DY = TFile::Open("OutFiles/out_DYJetsAll_8TeV.root", "RECREATE");
    //    FileList_DY = new TList();
    //    FileList_DY->Add(TFile::Open("OutFiles/out_DYJetsAll_8TeV_Hadd.root"));
    //    char * Background_DY[numBG] = {"DYJetsToLL", "DY1JetsToLL", "DY2JetsToLL", "DY3JetsToLL", "DY4JetsToLL"};
    //    float XSection_DY[numBG] = {2950, 561, 181, 51.1, 23};
    //    float LOtoNLO_DY = 1.187694915;
    //    MeasureWeight_Submit(Target_DY, FileList_DY, Background_DY, XSection_DY, LOtoNLO_DY);

    //    // Do stiching for W Background
    Target_W = TFile::Open("OutFiles/out_WJetsAll_8TeV.root", "RECREATE");
    FileList_W = new TList();
    FileList_W->Add(TFile::Open("OutFiles/out_WJetsAll_8TeV_Hadd.root"));
    FileList_W->Add(TFile::Open("OutFiles/out_WJetsAll_8TeV_Hadd.root"));
    char * Background_W[numBG] = {"WJetsToLNu", "W1JetsToLNu", "W2JetsToLNu", "W3JetsToLNu", "W4JetsToLNu"};
    float XSection_W[numBG] = {30400, 5400, 1750, 519, 214};
    float LOtoNLO_W = 1.233848684;
    MeasureWeight_Submit(Target_W, FileList_W, Background_W, XSection_W, LOtoNLO_W);
}

void MeasureWeight_Submit(TDirectory *Target, TList * FileList, char ** Background, float * XSection, float LOtoNLO) {
    const int numBG = 5;
    float weight[numBG] = {};

    for (int i = 0; i < numBG; i++) {
        cout << "start Stitching " << FileList->GetName() << "\n";

        // This is to get Number of events in inclusive DY/W sample
        std::string MMM_inc = InputFileLocation + string(Background[0]) + "_8TeV.root";
        TFile * myFile_inc = new TFile(MMM_inc.c_str());
        TH1F * myHisto_inc = (TH1F*) myFile_inc->Get("TotalEventsNumber");

        // This is to get Number of events in exclisive DY/W + njet sample
        std::string MMM = InputFileLocation + string(Background[i]) + "_8TeV.root";
        TFile * myFile = new TFile(MMM.c_str());
        TH1F * myHisto = (TH1F*) myFile->Get("TotalEventsNumber");

        //Here we measure the weight for each event
        weight[i] = LOtoNLO / (myHisto->Integral() / XSection[i] + myHisto_inc->Integral() / XSection[0]); // for events with more than 0 jet
        if (i == 0) weight[i] = LOtoNLO / (myHisto->Integral() / XSection[i]); // for 0 jet events

        //        cout << "numBG= " << i << " XSection= " << XSection[i] << "   Integral=" << myHisto->Integral() << "   weight=" << weight[i] << endl;
        myFile->Close();
    }

    dostitch(Target, FileList, weight);
}

void dostitch(TDirectory *target, TList *sourcelist, float* weight) {

    TString path((char*) strstr(target->GetPath(), ":"));
    path.Remove(0, 2);

    TFile *first_source = (TFile*) sourcelist->First();
    first_source->cd(path);
    TDirectory *current_sourcedir = gDirectory;
    //gain time, do not add the objects in the list in memory
    Bool_t status = TH1::AddDirectoryStatus();
    TH1::AddDirectory(kFALSE);


    // loop over all keys in this directory
    TChain *globChain = 0;
    TIter nextkey(current_sourcedir->GetListOfKeys());
    TKey *key, *oldkey = 0;
    while ((key = (TKey*) nextkey())) {
        //keep only the highest cycle number for each key
        if (oldkey && !strcmp(oldkey->GetName(), key->GetName())) continue;
        //        cout<<"Name is  "<<key->GetName()<<endl;

        // read object from first source file
        first_source->cd(path);
        TObject *obj = key->ReadObj();
        if (obj->IsA()->InheritsFrom(TH1::Class())) {
            //            int weightCounter = 0;
            // descendant of TH1 -> merge it


            TH1 *BaseHisto = (TH1*) obj;
            std::string name = BaseHisto->GetName();
            std::string nameEnd;
            for (std::string::iterator it = name.begin(); it != name.end(); ++it)
                nameEnd = *it;

            //################################################################################
            //    For Nominal
            //################################################################################
            if (nameEnd != "j") {
                //                cout << key->GetName() << "   " << BaseHisto->Integral() << endl;
                BaseHisto->Scale(0.0000000000001);

                TH1 * h0j = (TH1*) first_source->Get((name + "0j").c_str());
                TH1 * h1j = (TH1*) first_source->Get((name + "1j").c_str());
                TH1 * h2j = (TH1*) first_source->Get((name + "2j").c_str());
                TH1 * h3j = (TH1*) first_source->Get((name + "3j").c_str());
                TH1 * h4j = (TH1*) first_source->Get((name + "4j").c_str());

                if (h0j) {
                    h0j->Scale(weight[0]);
                    BaseHisto->Add(h0j);
                }

                if (h1j) {
                    h1j->Scale(weight[1]);
                    BaseHisto->Add(h1j);
                }

                if (h2j) {
                    h2j->Scale(weight[2]);
                    BaseHisto->Add(h2j);
                }

                if (h3j) {
                    h3j->Scale(weight[3]);
                    BaseHisto->Add(h3j);
                }

                if (h4j) {
                    h4j->Scale(weight[4]);
                    BaseHisto->Add(h4j);
                }
            }

            //################################################################################
            //    For Down
            //################################################################################
            //                if (nameEnd != "p" && nameEnd != "e"&& nameEnd != "g") {
            TH1 *BaseHisto_d = (TH1*) obj;
            std::string name = BaseHisto_d->GetName();
            int lenName = strlen(name.c_str());
            std::string DownName = name.erase(lenName - 4);
            std::string nameEnd_d;
            for (std::string::iterator it_d = DownName.begin(); it_d != DownName.end(); it_d++)
                nameEnd_d = *it_d;
            if (nameEnd_d != "j" && nameEnd == "n") {
                //                cout << " ............................ name is=" << BaseHisto_d->GetName() << "       DownName= " << DownName << "   " << BaseHisto_d->Integral() << endl;
                BaseHisto_d->Scale(0.0000000000001);

                TH1 * h0j = (TH1*) first_source->Get((DownName + "0jDown").c_str());
                TH1 * h1j = (TH1*) first_source->Get((DownName + "1jDown").c_str());
                TH1 * h2j = (TH1*) first_source->Get((DownName + "2jDown").c_str());
                TH1 * h3j = (TH1*) first_source->Get((DownName + "3jDown").c_str());
                TH1 * h4j = (TH1*) first_source->Get((DownName + "4jDown").c_str());

                if (h0j) {
                    h0j->Scale(weight[0]);
                    BaseHisto_d->Add(h0j);
                }

                if (h1j) {
                    h1j->Scale(weight[1]);
                    BaseHisto_d->Add(h1j);
                }

                if (h2j) {
                    h2j->Scale(weight[2]);
                    BaseHisto_d->Add(h2j);
                }

                if (h3j) {
                    h3j->Scale(weight[3]);
                    BaseHisto_d->Add(h3j);
                }

                if (h4j) {
                    h4j->Scale(weight[4]);
                    BaseHisto_d->Add(h4j);
                }
            }
            //################################################################################
            //    For Up
            //################################################################################
            //                if (nameEnd != "p" && nameEnd != "e"&& nameEnd != "g") {
            TH1 *BaseHisto_up = (TH1*) obj;
            std::string nameU = BaseHisto_up->GetName();
            int lenNameU = strlen(nameU.c_str());
            std::string UpName = nameU.erase(lenNameU - 2);
            std::string nameEnd_u;
            for (std::string::iterator it_u = UpName.begin(); it_u != UpName.end(); it_u++)
                nameEnd_u = *it_u;
            if (nameEnd_u != "j" && nameEnd == "p") {
                //                cout << " ............................ name is=" << BaseHisto_up->GetName() << "       UpName= " << UpName << "   " << BaseHisto_up->Integral() << endl;
                BaseHisto_up->Scale(0.0000000000001);

                TH1 * h0j = (TH1*) first_source->Get((UpName + "0jUp").c_str());
                TH1 * h1j = (TH1*) first_source->Get((UpName + "1jUp").c_str());
                TH1 * h2j = (TH1*) first_source->Get((UpName + "2jUp").c_str());
                TH1 * h3j = (TH1*) first_source->Get((UpName + "3jUp").c_str());
                TH1 * h4j = (TH1*) first_source->Get((UpName + "4jUp").c_str());

                if (h0j) {
                    h0j->Scale(weight[0]);
                    BaseHisto_up->Add(h0j);
                }

                if (h1j) {
                    h1j->Scale(weight[1]);
                    BaseHisto_up->Add(h1j);
                }

                if (h2j) {
                    h2j->Scale(weight[2]);
                    BaseHisto_up->Add(h2j);
                }

                if (h3j) {
                    h3j->Scale(weight[3]);
                    BaseHisto_up->Add(h3j);
                }

                if (h4j) {
                    h4j->Scale(weight[4]);
                    BaseHisto_up->Add(h4j);
                }
            }
            //################################################################################



        }
        if (obj) {
            target->cd();

            //!!if the object is a tree, it is stored in globChain...
            if (obj->IsA()->InheritsFrom(TTree::Class()))
                globChain->Merge(target->GetFile(), 0, "keep");
            else
                obj->Write(key->GetName());
        }

    } // while ( ( TKey *key = (TKey*)nextkey() ) )

    // save modifications to target file
    target->SaveSelf(kTRUE);
    TH1::AddDirectory(status);

    //                nextsource = (TFile*) sourcelist->After(nextsource);
}
//            std::cout << "\n";




