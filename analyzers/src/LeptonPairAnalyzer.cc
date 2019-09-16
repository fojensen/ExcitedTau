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
//#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include <TTree.h>
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;
#include <TLorentzVector.h>
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

class LeptonPairAnalyzer : public edm::EDAnalyzer {
public:
	explicit LeptonPairAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<edm::View<reco::Candidate>> photonToken_;
   edm::EDGetTokenT<edm::View<reco::Candidate>> visibleTauToken_;
   edm::EDGetTokenT<edm::View<reco::Candidate>> collinearTauToken_;

   TTree * tree;
   double lead_pt, lead_eta, lead_phi, lead_mass;
   double sublead_pt, sublead_eta, sublead_phi, sublead_mass;
   double ll_pt, ll_eta, ll_phi, ll_mass, ll_dr;
   double photon_pt, photon_eta, photon_phi;
   double m_lead, dr_lead, m_sublead, dr_sublead;
   double ll_collinearmass, collmass_glead, collmass_gsublead;
   double photon_restpt_lead, photon_restpt_sublead;
   double photon_restp_lead, photon_restp_sublead;
   bool havePair, havePhoton, haveCollinearPair;
};

LeptonPairAnalyzer::LeptonPairAnalyzer(const edm::ParameterSet& iConfig)
{
   visibleTauToken_ = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("visibleTauCollection"));
   collinearTauToken_ = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("collinearTauCollection"));
   photonToken_ = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("photonCollection"));

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
  
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
   tree->Branch("ll_dr", &ll_dr, "ll_dr/D");
   tree->Branch("photon_pt", &photon_pt, "photon_pt/D");
   tree->Branch("photon_eta", &photon_eta, "photon_eta/D");
   tree->Branch("photon_phi", &photon_phi, "photon_phi/D");  
   tree->Branch("ll_collinearmass", &ll_collinearmass, "ll_collinearmass/D");
   tree->Branch("collmass_glead", &collmass_glead, "collmass_glead/D");
   tree->Branch("collmass_gsublead", &collmass_gsublead, "collmass_gsublead/D");
   tree->Branch("photon_restpt_lead", &photon_restpt_lead, "photon_restpt_lead/D");
   tree->Branch("photon_restpt_sublead", &photon_restpt_sublead, "photon_restpt_sublead/D");
   tree->Branch("photon_restp_lead", &photon_restp_lead, "photon_restp_lead/D");
   tree->Branch("photon_restp_sublead", &photon_restp_sublead, "photon_restp_sublead/D");
   tree->Branch("havePair", &havePair, "havePair/O");
   tree->Branch("havePhoton", &havePhoton, "havePhoton/O");
   tree->Branch("haveCollinearPair", &haveCollinearPair, "haveCollinearPair/O");
}

void LeptonPairAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<edm::View<reco::Candidate>> visibleTaus;
   iEvent.getByToken(visibleTauToken_, visibleTaus);
 
   havePair = (visibleTaus->size()>=2);  
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

   edm::Handle<edm::View<reco::Candidate>> photons;
   iEvent.getByToken(photonToken_, photons);
   havePhoton = (photons->size()>=1);
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

   edm::Handle<edm::View<reco::Candidate>> collinearTaus;
   iEvent.getByToken(collinearTauToken_, collinearTaus);
   haveCollinearPair = (collinearTaus->size()>=2);
   if (haveCollinearPair) {
      ll_collinearmass = (collinearTaus->at(0).p4()+collinearTaus->at(1).p4()).mass();    
   } else {
      ll_collinearmass = 0.;
   }
   if (haveCollinearPair && havePhoton) {
      const PolarLorentzVector p4_lead = collinearTaus->at(0).polarP4()+photons->at(0).polarP4();
      collmass_glead = p4_lead.mass();
      const PolarLorentzVector p4_sublead = collinearTaus->at(1).polarP4()+photons->at(0).polarP4();
      collmass_gsublead = p4_sublead.mass();

      // find photon momentum in the rest frame of the excited tau
      TLorentzVector photon_lead;
      photon_lead.SetPtEtaPhiM(photons->at(0).pt(), photons->at(0).eta(), photons->at(0).phi(), photons->at(0).mass());;
      TLorentzVector photon_sublead = photon_lead;
      const math::XYZVector vboost_leadtemp = p4_lead.BoostToCM();
      TVector3 vboost_lead;
      vboost_lead.SetXYZ(vboost_leadtemp.x(), vboost_leadtemp.y(), vboost_leadtemp.z());
      photon_lead.Boost(vboost_lead);
      photon_restpt_lead = photon_lead.Pt();
      photon_restp_lead = photon_lead.P();
      const math::XYZVector vboost_subleadtemp = p4_sublead.BoostToCM();
      TVector3 vboost_sublead;
      vboost_sublead.SetXYZ(vboost_subleadtemp.x(), vboost_subleadtemp.y(), vboost_subleadtemp.z());
      photon_sublead.Boost(vboost_sublead);
      photon_restpt_sublead = photon_sublead.Pt();
      photon_restp_sublead = photon_sublead.P();

   } else {
      collmass_glead = collmass_gsublead = 0.;
      photon_restpt_lead = photon_restpt_sublead = 0.;
      photon_restp_lead = photon_restp_sublead = 0.;
   }

   tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(LeptonPairAnalyzer);

