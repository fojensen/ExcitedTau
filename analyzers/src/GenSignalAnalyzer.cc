#include <cmath>
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/GetterOfProducts.h"
#include "FWCore/Framework/interface/ProcessMatch.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes 
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <TTree.h>
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;

class GenSignalAnalyzer : public edm::EDAnalyzer {
public:
   explicit GenSignalAnalyzer(const edm::ParameterSet&);
private:
   void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
   edm::EDGetTokenT<pat::CompositeCandidateCollection> genVisTauToken_;
   edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
   TTree * tree;
   double pt_photon, pt_lead, pt_sublead;
   double eta_photon, eta_lead, eta_sublead;
   double MET, METphi;
   double dr_tautau, dr_lead, dr_sublead;
};

GenSignalAnalyzer::GenSignalAnalyzer(const edm::ParameterSet& iConfig)
{
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));
   genVisTauToken_= consumes<pat::CompositeCandidateCollection>(iConfig.getParameter<edm::InputTag>("genVisTauCollection"));
   metToken_ = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection"));

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   tree->Branch("pt_photon", &pt_photon, "pt_photon/D");
   tree->Branch("eta_photon", &pt_photon, "eta_photon/D");
   tree->Branch("pt_lead", &pt_lead, "pt_lead/D");
   tree->Branch("eta_lead", &pt_lead, "eta_lead/D");
   tree->Branch("pt_sublead", &pt_sublead, "pt_sublead/D");
   tree->Branch("eta_sublead", &eta_sublead, "eta_sublead/D");
   tree->Branch("dr_tautau", &dr_tautau, "dr_tautau/D");
   tree->Branch("dr_lead", &dr_lead, "dr_lead/D");
   tree->Branch("dr_sublead", &dr_sublead, "dr_sublead/D");
   tree->Branch("MET", &MET, "MET/D");
   tree->Branch("METphi", &METphi, "METphi/D");
}

void GenSignalAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   pt_photon = pt_lead = pt_sublead = 0.;
   eta_photon = eta_lead = eta_sublead = 0.;
   MET = METphi = 0.;
   dr_tautau = dr_lead = dr_sublead = 0.;

   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles);
   PolarLorentzVector photon;
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->mother()) {
         if (std::abs(i->mother()->pdgId())==4000015) {
            const int id = std::abs(i->pdgId());
            if (id==22) photon = i->p4();
         }
      }
   }

   edm::Handle<pat::CompositeCandidateCollection> genVisTaus;
   iEvent.getByToken(genVisTauToken_, genVisTaus);
   PolarLorentzVector leading, subleading;
   if (genVisTaus->size()>=2) {
      if (genVisTaus->at(0).pt()>=genVisTaus->at(1).pt()) {
         leading = genVisTaus->at(0).p4();
         subleading = genVisTaus->at(1).p4();
      } else {
         leading = genVisTaus->at(1).p4();
         subleading = genVisTaus->at(0).p4();
      }
   }

   pt_photon = photon.pt();
   eta_photon = photon.eta();
   pt_lead = leading.pt();
   eta_lead = leading.eta();
   pt_sublead = subleading.pt();
   eta_sublead = subleading.eta();
   dr_tautau = reco::deltaR(leading, subleading);
   dr_lead = reco::deltaR(leading, photon);
   dr_sublead = reco::deltaR(photon, subleading);

   edm::Handle<std::vector<pat::MET>> met;
   iEvent.getByToken(metToken_, met);
   MET = met->at(0).genMET()->pt();
   METphi = met->at(0).genMET()->phi();

   tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenSignalAnalyzer);
