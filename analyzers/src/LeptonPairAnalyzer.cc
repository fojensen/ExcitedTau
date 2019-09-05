// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <TTree.h>

class LeptonPairAnalyzer : public edm::EDAnalyzer {
public:
	explicit LeptonPairAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<pat::Photon>> photonToken_;
   edm::EDGetTokenT<std::vector<pat::PackedCandidate>> visibleTauToken_;
   edm::EDGetTokenT<std::vector<pat::PackedCandidate>> collinearTauToken_;

   TTree * tree;
   bool havePair, havePhoton;
   double lead_pt, lead_eta, lead_phi, lead_mass;
   double sublead_pt, sublead_eta, sublead_phi, sublead_mass;
   double ll_pt, ll_eta, ll_phi, ll_mass, ll_dr;
   double photon_pt, photon_eta, photon_phi;
   double m_lead, dr_lead, m_sublead, dr_sublead;
   double collinearmass_ll, collinearmass_photonl_min, collinearmass_photonl_max;
};

LeptonPairAnalyzer::LeptonPairAnalyzer(const edm::ParameterSet& iConfig)
{
   photonToken_ = consumes<std::vector<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photonCollection"));
   visibleTauToken_ = consumes<std::vector<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("visibleTauCollection"));
   collinearTauToken_ = consumes<std::vector<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("collinearTauCollection"));
 
   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   tree->Branch("havePair", &havePair, "havePair/O");
   tree->Branch("havePhoton", &havePhoton, "havePhoton/O");
   tree->Branch("lead_pt", &lead_pt, "lead_pt/D");
   tree->Branch("lead_eta", &lead_eta, "lead_eta/D");
   tree->Branch("lead_phi", &lead_phi, "lead_phi/D");
   tree->Branch("lead_mass", &lead_mass, "lead_mass/D");
   tree->Branch("sublead_pt", &sublead_pt, "sublead_pt/D");
   tree->Branch("sublead_eta", &sublead_eta, "sublead_eta/D");
   tree->Branch("sublead_phi", &sublead_phi, "sublead_phi/D");
   tree->Branch("sublead_mass", &sublead_mass, "sublead_mass/D");
   tree->Branch("ll_pt", &ll_pt, "ll_pt/D");
   tree->Branch("ll_eta", &ll_eta, "ll_eta/D");
   tree->Branch("ll_phi", &ll_phi, "ll_phi/D");
   tree->Branch("ll_mass", &ll_mass, "ll_mass/D"); 
   tree->Branch("ll_dr", &ll_mass, "ll_dr/D");
   tree->Branch("photon_pt", &photon_pt, "photon_pt/D");
   tree->Branch("photon_eta", &photon_eta, "photon_eta/D");
   tree->Branch("photon_phi", &photon_phi, "photon_phi/D");
   // collinear approximation
   tree->Branch("collinearmass_ll", &collinearmass_ll, "collinearmass_ll/D");
   tree->Branch("collinearmass_photonl_max", &collinearmass_photonl_max, "collinearmass_photonl_max/D");
   tree->Branch("collinearmass_photonl_min", &collinearmass_photonl_min, "collinearmass_photonl_min/D");
}

void LeptonPairAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<pat::PackedCandidate>> visibleTaus;
   iEvent.getByToken(visibleTauToken_, visibleTaus);
   havePair = visibleTaus->size();
   if (havePair) {
      lead_pt = visibleTaus->at(0).pt();
      lead_eta = visibleTaus->at(0).eta();
      lead_phi = visibleTaus->at(0).phi();
      lead_mass = visibleTaus->at(0).mass();
      sublead_pt = visibleTaus->at(1).pt();
      sublead_eta = visibleTaus->at(1).eta();
      sublead_phi = visibleTaus->at(1).phi();
      sublead_mass = visibleTaus->at(1).mass();
      ll_pt = (visibleTaus->at(0).p4()+visibleTaus->at(1).p4()).pt();
      ll_eta = (visibleTaus->at(0).p4()+visibleTaus->at(1).p4()).eta();
      ll_phi = (visibleTaus->at(0).p4()+visibleTaus->at(1).p4()).phi();
      ll_mass = (visibleTaus->at(0).p4()+visibleTaus->at(1).p4()).mass();
      ll_dr = reco::deltaR(visibleTaus->at(0), visibleTaus->at(1));
   } else {
      lead_pt = lead_eta = lead_phi = lead_mass = 0.;
      sublead_pt = sublead_eta = sublead_phi = sublead_mass = 0.;
      ll_pt = ll_eta = ll_phi = ll_mass = ll_dr = 0.;
   }

   edm::Handle<std::vector<pat::Photon>> photons;
   iEvent.getByToken(photonToken_, photons);
   havePhoton = photons->size();
   if (havePhoton){
      photon_pt = photons->at(0).pt();
      photon_eta = photons->at(0).eta();
      photon_phi = photons->at(0).phi();
   } else {
      photon_pt = photon_eta = photon_phi = 0.;
   }

   if (havePair && havePhoton) {
      m_lead = (visibleTaus->at(0).p4()+photons->at(0).p4()).mass();
      dr_lead = reco::deltaR(visibleTaus->at(0), photons->at(0));
      m_sublead = (visibleTaus->at(1).p4()+photons->at(0).p4()).mass();
      dr_sublead = reco::deltaR(visibleTaus->at(1), photons->at(0));
   } else {
      m_lead = dr_lead = m_sublead = dr_sublead = 0.;
   }

   edm::Handle<std::vector<pat::PackedCandidate>> collinearTaus;
   iEvent.getByToken(collinearTauToken_, collinearTaus);
   if (collinearTaus->size()>=2) {
      collinearmass_ll = (collinearTaus->at(0).p4()+collinearTaus->at(1).p4()).mass();
   } else {
      collinearmass_ll = 0.;
   }
   if (collinearTaus->size()>=2 && havePhoton) {
      const double collinearmass_lead = (collinearTaus->at(0).p4()+photons->at(0).p4()).mass();
      const double collinearmass_sublead = (collinearTaus->at(1).p4()+photons->at(0).p4()).mass();
      collinearmass_photonl_max = TMath::Max(collinearmass_lead, collinearmass_sublead);
      collinearmass_photonl_min = TMath::Min(collinearmass_lead, collinearmass_sublead);
   } else {
      collinearmass_photonl_max = collinearmass_photonl_min = 0.;
   }

   tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(LeptonPairAnalyzer);

