void findDaughters(const reco::GenParticle* mother, std::vector<const reco::GenParticle*>& daughters, int status) {
    unsigned numDaughters = mother->numberOfDaughters();
    for (unsigned iDaughter = 0; iDaughter < numDaughters; ++iDaughter) {
        const reco::GenParticle* daughter = mother->daughterRef(iDaughter).get();

        if (status == -1 || daughter->status() == status) daughters.push_back(daughter);

        findDaughters(daughter, daughters, status);
    }
}

reco::Candidate::LorentzVector getVisMomentum(const std::vector<const reco::GenParticle*>& daughters, int status) {
    reco::Candidate::LorentzVector p4Vis(0, 0, 0, 0);

    for (std::vector<const reco::GenParticle*>::const_iterator daughter = daughters.begin();
            daughter != daughters.end(); ++daughter) {
        if ((status == -1 || (*daughter)->status() == status) && !isNeutrino(*daughter)) {
            //std::cout << "adding daughter: pdgId = " << (*daughter)->pdgId() << ", Pt = " << (*daughter)->pt() << ","
            //	  << " eta = " << (*daughter)->eta() << ", phi = " << (*daughter)->phi()*180./TMath::Pi() << std::endl;
            p4Vis += (*daughter)->p4();
        }
    }
}

void matchGenParticleFromZdecay(const reco::Candidate::LorentzVector& p4, const reco::GenParticle* genLeptonPlus, const reco::GenParticle* genLeptonMinus,
        bool& isGenHadTau, bool& isGenMuon, bool& isGenElectron, bool& isGenJet) {
    std::vector<int> pdgIds_ranked;
    pdgIds_ranked.push_back(15);
    pdgIds_ranked.push_back(13);
    pdgIds_ranked.push_back(11);
    for (std::vector<int>::const_iterator pdgId = pdgIds_ranked.begin(); pdgId != pdgIds_ranked.end(); ++pdgId) {
        bool matchesGenLeptonPlus = (genLeptonPlus && TMath::Abs(genLeptonPlus->pdgId()) == (*pdgId) && deltaR(p4, getVisMomentum(genLeptonPlus)) < 0.5);
        bool matchesGenLeptonMinus = (genLeptonMinus && TMath::Abs(genLeptonMinus->pdgId()) == (*pdgId) && deltaR(p4, getVisMomentum(genLeptonMinus)) < 0.5);
        if (matchesGenLeptonPlus || matchesGenLeptonMinus) {
            if ((*pdgId) == 15) {
                std::string genTauDecayModePlus = (matchesGenLeptonPlus) ? getGenTauDecayMode(genLeptonPlus) : "undefined";
                std::string genTauDecayModeMinus = (matchesGenLeptonMinus) ? getGenTauDecayMode(genLeptonMinus) : "undefined";
                if ((matchesGenLeptonPlus && genTauDecayModePlus == "electron") ||
                        (matchesGenLeptonMinus && genTauDecayModeMinus == "electron")) {
                    isGenElectron = true;
                }
                if ((matchesGenLeptonPlus && genTauDecayModePlus == "muon") ||
                        (matchesGenLeptonMinus && genTauDecayModeMinus == "muon")) {
                    isGenMuon = true;
                }
                if ((matchesGenLeptonPlus && !(genTauDecayModePlus == "electron" || genTauDecayModePlus == "muon")) ||
                        (matchesGenLeptonMinus && !(genTauDecayModeMinus == "electron" || genTauDecayModeMinus == "muon"))) {
                    isGenHadTau = true;
                }
            } else if ((*pdgId) == 13) {
                isGenMuon = true;
            } else if ((*pdgId) == 11) {
                isGenElectron = true;
            } else assert(0);
            return;
        }
    }
    isGenJet = true;
}

//IN YOUR MAIN

void main() {
    isZdecay_ = false;

    isZthth_ = false;
    isZtt_ = false;
    isZttj_ = false;
    isZttl_ = false;
    isZj_ = false;
    isZee_ = false;
    isZmm_ = false;
    isZll_ = false;

    if (isMC_ || isPFEmb_) {
        //get Z or gamma*
        const reco::GenParticle* genZ_or_Gammastar = 0;
        for (reco::GenParticleCollection::const_iterator genParticle = genParticles->begin(); genParticle != genParticles->end(); ++genParticle) {
            if ((genParticle->pdgId() == 22 || genParticle->pdgId() == 23) && genParticle->mass() > 50.) {
                genZ_or_Gammastar = &(*genParticle);
                break;
            }
        }

        //get Z or gamma* daughters
        std::vector<const reco::GenParticle*> allDaughters;

        //check Z or gamma* decay products
        if (genZ_or_Gammastar)// Z or gamma* found
        {
            isZdecay_ = true;
            findDaughters(genZ_or_Gammastar, allDaughters, -1);
        } else// no Z or gamma* found (due to potential inconsistency in DY MC or EmbedPF): still fill the daughters not to lose in acceptance
        {
            for (reco::GenParticleCollection::const_iterator genParticle = genParticles->begin(); genParticle != genParticles->end(); ++genParticle) {
                allDaughters.push_back(&(*genParticle));
            }
        }

        //loop on daughters to check decay mode
        const reco::GenParticle* genLeptonPlus = 0;
        const reco::GenParticle* genLeptonMinus = 0;
        for (std::vector<const reco::GenParticle*>::const_iterator daughter = allDaughters.begin(); daughter != allDaughters.end(); ++daughter) {
            int absPdgId = TMath::Abs((*daughter)->pdgId());
            //give preference to taus, since electrons and muons may either come from decay of Z-boson or from tau decay
            //taus
            if (absPdgId == 15 && (*daughter)->charge() > +0.5 && (isZdecay_ || !genLeptonPlus)) genLeptonPlus = (*daughter);
            if (absPdgId == 15 && (*daughter)->charge() < -0.5 && (isZdecay_ || !genLeptonMinus)) genLeptonMinus = (*daughter);
            //electrons or muons
            if ((absPdgId == 11 || absPdgId == 13) && (*daughter)->charge() > +0.5 && !genLeptonPlus) genLeptonPlus = (*daughter);
            if ((absPdgId == 11 || absPdgId == 13) && (*daughter)->charge() < -0.5 && !genLeptonMinus) genLeptonMinus = (*daughter);
        }

        //check if leg1 is matched to a Z decay product
        bool leg1isGenHadTau = false;
        bool leg1isGenMuon = false;
        bool leg1isGenElectron = false;
        bool leg1isGenJet = false;
        matchGenParticleFromZdecay(leg1->p4(), genLeptonPlus, genLeptonMinus, leg1isGenHadTau, leg1isGenMuon, leg1isGenElectron, leg1isGenJet);

        //check if leg2 is matched to a Z decay product
        bool leg2isGenHadTau = false;
        bool leg2isGenMuon = false;
        bool leg2isGenElectron = false;
        bool leg2isGenJet = false;
        matchGenParticleFromZdecay(leg2->p4(), genLeptonPlus, genLeptonMinus, leg2isGenHadTau, leg2isGenMuon, leg2isGenElectron, leg2isGenJet);

        //check decay modes
        //Ztt --> both leg1 and leg2 matched to generator taus
        if (genLeptonPlus && TMath::Abs(genLeptonPlus->pdgId()) == 15 && genLeptonMinus && TMath::Abs(genLeptonMinus->pdgId()) == 15) {
            //Hadronic decay --> both legs matched to gen had tau
            if (leg1isGenHadTau && leg2isGenHadTau) isZthth_ = true;
                //Semi-leptonic taus decays --> leg1 matched to lepton, leg2 matched to gen had tau
            else if ((leg1isGenElectron || leg1isGenMuon) && leg2isGenHadTau) isZtt_ = true;
                //Fully-leptonic taus decays --> leg1 matched to gen lepton, leg2 matched to gen lepton
            else if ((leg1isGenMuon || leg1isGenElectron) && (leg2isGenMuon || leg2isGenElectron)) isZttl_ = true;
                //Other cases (most are when a jet fakes a had. tau)
            else isZttj_ = true;
        }//Zee --> both leg1 and leg2 matched to generator electrons
        else if (genLeptonPlus && TMath::Abs(genLeptonPlus->pdgId()) == 11 && leg1isGenElectron && genLeptonMinus && TMath::Abs(genLeptonMinus->pdgId()) == 11 && leg2isGenElectron) {
            isZee_ = true;
        }//Zmm --> both leg1 and leg2 matched to generator muons
        else if (genLeptonPlus && TMath::Abs(genLeptonPlus->pdgId()) == 13 && leg1isGenMuon && genLeptonMinus && TMath::Abs(genLeptonMinus->pdgId()) == 13 && leg2isGenMuon) {
            isZmm_ = true;
        }//Zj --> at least one leg matched to a jet, or legs not matched to same flavor lepton, or a leg is not matched at all
        else {
            isZj_ = true;
        }
    }

    isZll_ = isZee_ || isZmm_;

    TString flag = "";
    if (isZtt_) flag = "ZTT";
    else if (isZll_ || isZttl_) flag = "ZL";
    else flag = "ZJ";

    cout << "flag = " << flag << endl;

}
