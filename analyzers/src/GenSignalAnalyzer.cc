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
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;

class GenSignalAnalyzer : public edm::EDAnalyzer {
public:
   explicit GenSignalAnalyzer(const edm::ParameterSet&);
private:
   void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
   edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
   TTree * tree;
   double photon_pt, photon_eta, photon_phi;
   double excited_pt, excited_eta, excited_phi, excited_mass;
   double deexcited_pt, deexcited_eta, deexcited_phi, deexcited_mass;
   double deexcited_vispt, deexcited_viseta, deexcited_visphi, deexcited_vismass;
   double spectator_pt, spectator_eta, spectator_phi, spectator_mass;
   double spectator_vispt, spectator_viseta, spectator_visphi, spectator_vismass;
   int deexcited_dm, spectator_dm;
   double MET_pt, MET_eta, MET_phi, MET_mass;
};

GenSignalAnalyzer::GenSignalAnalyzer(const edm::ParameterSet& iConfig)
{
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));
   metToken_ = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection"));

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   // photon
   tree->Branch("photon_pt", &photon_pt, "photon_pt/D");
   tree->Branch("photon_eta", &photon_eta, "photon_eta/D");
   tree->Branch("photon_phi", &photon_phi, "photon_phi/D");
   // excited tau
   tree->Branch("excited_pt", &excited_pt, "excited_pt/D");
   tree->Branch("excited_eta", &excited_eta, "excited_eta/D");
   tree->Branch("excited_phi", &excited_phi, "excited_phi/D");
   tree->Branch("excited_mass", &excited_mass, "excited_mass/D");
   // excited tau
   tree->Branch("deexcited_pt", &deexcited_pt, "deexcited_pt/D");
   tree->Branch("deexcited_eta", &deexcited_eta, "deexcited_eta/D");
   tree->Branch("deexcited_phi", &deexcited_phi, "deexcited_phi/D");
   tree->Branch("deexcited_mass", &deexcited_mass, "deexcited_mass/D");
   // excited visible tau
   tree->Branch("deexcited_vispt", &deexcited_vispt, "deexcited_vispt/D");
   tree->Branch("deexcited_viseta", &deexcited_viseta, "deexcited_viseta/D");
   tree->Branch("deexcited_visphi", &deexcited_visphi, "deexcited_visphi/D");
   tree->Branch("deexcited_vismass", &deexcited_vismass, "deexcited_vismass/D");
   tree->Branch("deexcited_dm", &deexcited_dm, "deexcited_dm/I");
   // spectator tau
   tree->Branch("spectator_pt", &spectator_pt, "spectator_pt/D");
   tree->Branch("spectator_eta", &spectator_eta, "spectator_eta/D");
   tree->Branch("spectator_phi", &spectator_phi, "spectator_phi/D");
   tree->Branch("spectator_mass", &spectator_mass, "spectator_mass/D");
   // spectator visible tau
   tree->Branch("spectator_vispt", &spectator_vispt, "spectator_vispt/D");
   tree->Branch("spectator_viseta", &spectator_viseta, "spectator_viseta/D");
   tree->Branch("spectator_visphi", &spectator_visphi, "spectator_visphi/D");
   tree->Branch("spectator_vismass", &spectator_vismass, "spectator_vismass/D");
   tree->Branch("spectator_dm", &spectator_dm, "spectator_dm/I");
   // MET
   tree->Branch("MET_pt", &MET_pt, "MET_pt/D");
   tree->Branch("MET_eta", &MET_eta, "MET_eta/D");
   tree->Branch("MET_phi", &MET_phi, "MET_phi/D");
   tree->Branch("MET_mass", &MET_mass, "MET_mass/D");
}

void GenSignalAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles);
   PolarLorentzVector photon, excited, deexcited, spectator;
   PolarLorentzVector deexcitedvis, spectatorvis;
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      const int id = std::abs(i->pdgId());
      if (id==22 && i->mother()) {
         if (std::abs(i->mother()->pdgId())==4000015) {
            photon = i->p4();
         }
      }
      if (id==15 && i->mother()) {
         if (std::abs(i->mother()->pdgId())==4000015) {
            deexcited = i->p4();
         }
      }
      if (id==4000015) {
         excited = i->p4();
      }
      if (id==15 && i->mother()) {
         if (std::abs(i->mother()->pdgId())!=4000015) {
            spectator = i->p4();
         }
      }
   }

   // photon
   photon_pt = photon.pt();
   photon_eta = photon.eta();
   photon_phi = photon.phi();
   // excited tau
   excited_pt = excited.pt();
   excited_eta = excited.eta();
   excited_phi = excited.pt();
   excited_mass = excited.mass();
   // deexcited tau
   deexcited_pt = excited.pt();
   deexcited_eta = excited.eta();
   deexcited_phi = excited.pt();
   deexcited_mass = excited.mass();
   // deexcited tau
   deexcited_vispt = deexcitedvis.pt();
   deexcited_viseta = deexcitedvis.eta();
   deexcited_visphi = deexcitedvis.pt();
   deexcited_vismass = deexcitedvis.mass();
   // spectator
   spectator_pt = spectator.pt();
   spectator_eta = spectator.eta();
   spectator_phi = spectator.pt();
   spectator_mass = spectator.mass();   
   // spectator vis
   spectator_vispt = spectatorvis.pt();
   spectator_viseta = spectatorvis.eta();
   spectator_visphi = spectatorvis.pt();
   spectator_vismass = spectatorvis.mass();

   edm::Handle<std::vector<pat::MET>> met;
   iEvent.getByToken(metToken_, met);
   MET_pt = met->at(0).genMET()->pt();
   MET_eta = met->at(0).genMET()->eta();
   MET_phi = met->at(0).genMET()->phi();
   MET_mass = met->at(0).genMET()->mass();

   tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenSignalAnalyzer);
