// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TTree.h>
//#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/MET.h"

class ExcitingAnalyzer : public edm::EDAnalyzer {
public:
	explicit ExcitingAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<pat::Photon>> photonToken_;
   edm::EDGetTokenT<edm::View<reco::Candidate>> leptonToken;
   edm::EDGetTokenT<edm::View<reco::Candidate>> collinearLeptonToken;
   //edm::EDGetTokenT<std::vector<reco::Vertex>> vertexToken_;
   edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
   edm::EDGetTokenT<std::vector<pat::Jet>> jetToken_;
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
   TTree * tree;

   // leptons
   double lead_pt, lead_eta, lead_phi, lead_mass;
   double sublead_pt, sublead_eta, sublead_phi, sublead_mass;
   double ll_pt, ll_eta, ll_phi, ll_visiblemass;
   double ll_collinearmass, ll_dr;
   double mt_closest, mt_furthest;

   unsigned int nPhotons, nLeptons, nGenTaus;
   unsigned int BTags[3], NJets;
   double MET_pt, MET_phi, metSignificance;
   double photon_pt, photon_eta, photon_phi;
   double xsWeight;
   // leptons only
   double m_ll, pt_ll, dr_ll;
   int sumQ;
   // leptons+photon
   double m_max, m_min;
   double dr_lead, dr_sublead;
   float photon_hadTowOverEm;
   // collinear
   double collinear_m_min;
   double collinear_m_max;
};

ExcitingAnalyzer::ExcitingAnalyzer(const edm::ParameterSet& iConfig)
{
   photonToken_ = consumes<std::vector<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photonCollection"));
   leptonToken = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonCollection"));
   collinearLeptonToken = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("collinearLeptonCollection"));
   //vertexToken_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));
   metToken_ = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection"));
   jetToken_ = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jetCollection"));
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection")); 

   const double xs_ = iConfig.getParameter<double>("xs");
   const int nevents_ = iConfig.getParameter<int>("nevents");
   xsWeight = xs_/double(nevents_);
   
   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   // all events
   tree->Branch("nPhotons", &nPhotons, "nPhotons/i");
   tree->Branch("nLeptons", &nLeptons, "nLeptons/i");
   tree->Branch("nGenTaus", &nGenTaus, "nGenTaus/i");
   tree->Branch("NJets", &NJets, "NJets/i");
   tree->Branch("MET_pt", &MET_pt, "MET_pt/D");
   tree->Branch("MET_phi", &MET_phi, "MET_phi/D");
   tree->Branch("metSignificance", &metSignificance, "metSignificance/D");
   tree->Branch("BTags", BTags, "BTags[3]/i");
   tree->Branch("xsWeight", &xsWeight, "xsWeight/D");
   // leptons
   tree->Branch("lead_pt", &lead_pt, "lead_pt/D");
   tree->Branch("lead_eta", &lead_eta, "lead_eta/D");
   tree->Branch("lead_phi", &lead_phi, "lead_phi/D");
   tree->Branch("lead_mass", &lead_mass, "lead_mass/D");
   tree->Branch("sublead_pt", &sublead_pt, "sublead_pt/D");
   tree->Branch("sublead_eta", &sublead_eta, "sublead_eta/D");
   tree->Branch("sublead_phi", &sublead_phi, "sublead_phi/D");
   tree->Branch("sublead_mass", &sublead_mass, "sublead_mass/D");
   tree->Branch("mt_furthest", &mt_furthest, "mt_furthest/D");
   tree->Branch("mt_closest", &mt_closest, "mt_closest/D");
   tree->Branch("ll_pt", &ll_pt, "ll_pt/D");
   tree->Branch("ll_eta", &ll_eta, "ll_eta/D");
   tree->Branch("ll_phi", &ll_phi, "ll_phi/D");
   tree->Branch("ll_dr", &ll_dr, "ll_dr/D"); 
   tree->Branch("ll_visiblemass", &ll_visiblemass, "ll_visiblemass/D");
   tree->Branch("ll_collinearmass", &ll_collinearmass, "ll_collinearmass/D");
   tree->Branch("sumQ", &sumQ, "sumQ/I");
   // photon
   tree->Branch("photon_pt", &photon_pt, "photon_pt/D");
   tree->Branch("photon_eta", &photon_eta, "photon_eta/D");
   tree->Branch("photon_phi", &photon_phi, "photon_phi/D");

   tree->Branch("m_max", &m_max, "m_max/D");
   tree->Branch("m_min", &m_min, "m_min/D");
   tree->Branch("dr_lead", &dr_lead, "dr_lead/D");
   tree->Branch("dr_sublead", &dr_sublead, "dr_sublead/D");
   // collinear
   tree->Branch("collinear_m_max", &collinear_m_max, "collinear_m_max/D");
   tree->Branch("collinear_m_min", &collinear_m_min, "collinear_m_min/D");
}

void ExcitingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   // global
   nPhotons = nLeptons = nGenTaus = 0;
   MET_pt = MET_phi = metSignificance = 0.;
   BTags[0] = BTags[1] = BTags[2] = 0;
   // leptons
   lead_pt = lead_eta = lead_phi = lead_mass = 0.;
   sublead_pt = sublead_eta = sublead_phi = sublead_mass = 0.;
   ll_pt = ll_eta = ll_phi = ll_visiblemass = 0.;
   ll_collinearmass = ll_dr = 0.;
   sumQ = 0;
   mt_closest = mt_furthest = 0.;
   // leptons+photon
   m_max = m_min = 0.;
   dr_lead = dr_sublead = 0.;
   collinear_m_min = collinear_m_max = 0.;

   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles);
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (std::abs(i->pdgId())==15 && i->isLastCopy()) ++nGenTaus;
   }

   edm::Handle<std::vector<pat::Photon>> photons;
   iEvent.getByToken(photonToken_, photons);
   nPhotons = photons->size();
   
   edm::Handle<edm::View<reco::Candidate>> leptons;
   iEvent.getByToken(leptonToken, leptons);
   nLeptons = leptons->size();

   //edm::Handle<std::vector<reco::Vertex>> vertices;
   //iEvent.getByToken(vertexToken_, vertices);
   //nVertices = vertices->size();

   edm::Handle<std::vector<pat::MET>> met;
   iEvent.getByToken(metToken_, met);
   MET_pt = met->at(0).pt();
   MET_phi = met->at(0).phi();
   metSignificance = met->at(0).metSignificance();

   edm::Handle<std::vector<pat::Jet>> jets;
   iEvent.getByToken(jetToken_, jets);
   for (auto i = jets->begin(); i != jets->end(); ++i) {
      ++NJets;
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
      const float disc = i->bDiscriminator("pfDeepCSVJetTags:probb")+i->bDiscriminator("pfDeepCSVJetTags:probbb");
      if (disc>=0.1241) ++BTags[0];
      if (disc>=0.4184) ++BTags[1];
      if (disc>=0.7527) ++BTags[2];
   }

   if (nLeptons<2) {
      tree->Fill();
      return;
   } 

   lead_pt = leptons->at(0).pt();
   lead_eta = leptons->at(0).eta();
   lead_phi = leptons->at(0).phi();
   lead_mass = leptons->at(0).mass();

   sublead_pt = leptons->at(1).pt();
   sublead_eta = leptons->at(1).eta();
   sublead_phi = leptons->at(1).phi();
   sublead_mass = leptons->at(1).mass();

   TVector2 v2_MET; v2_MET.SetMagPhi(MET_pt, MET_phi);
   TVector2 v2_lead; v2_lead.SetMagPhi(lead_pt, lead_phi);
   TVector2 v2_sublead; v2_sublead.SetMagPhi(sublead_pt, sublead_phi);
   const double cosdphi_lead = cos(v2_MET.DeltaPhi(v2_lead));
   const double cosdphi_sublead = cos(v2_MET.DeltaPhi(v2_sublead));
   const double mt_lead = sqrt(2. * MET_pt * lead_pt * (1. - cosdphi_lead));
   const double mt_sublead = sqrt(2. * MET_pt * sublead_pt * (1. - cosdphi_sublead));
   if (cosdphi_lead>cosdphi_sublead) {
      mt_closest = mt_lead;
      mt_furthest = mt_sublead;
   } else {
      mt_closest = mt_sublead;
      mt_furthest = mt_lead;
   }

   ll_pt = (leptons->at(0).p4()+leptons->at(1).p4()).pt();
   ll_eta = (leptons->at(0).p4()+leptons->at(1).p4()).eta();
   ll_phi = (leptons->at(0).p4()+leptons->at(1).p4()).phi();
   ll_visiblemass = (leptons->at(0).p4()+leptons->at(1).p4()).mass();
   ll_dr = reco::deltaR(leptons->at(0), leptons->at(1));

   sumQ = leptons->at(0).charge()+leptons->at(1).charge();

   // collinear
   edm::Handle<edm::View<reco::Candidate>> collinearLeptons;
   iEvent.getByToken(collinearLeptonToken, collinearLeptons);
   ll_collinearmass = (collinearLeptons->at(0).p4()+collinearLeptons->at(1).p4()).mass();

   if (nPhotons<1) {
      tree->Fill();
      return;
   }

   photon_pt = photons->at(0).pt();
   photon_eta = photons->at(0).eta();
   photon_phi = photons->at(0).phi();
 
   // leptons + photon
   const double m_lead = (leptons->at(0).p4()+photons->at(0).p4()).mass();
   dr_lead = reco::deltaR(leptons->at(0), photons->at(0));
   const double m_sublead = (leptons->at(1).p4()+photons->at(0).p4()).mass();
   dr_sublead = reco::deltaR(leptons->at(1), photons->at(0));
   m_max = TMath::Max(m_lead, m_sublead);
   m_min = TMath::Min(m_lead, m_sublead);

   // collinear and visible pT may be different...
   const double collinear_m_lead = (collinearLeptons->at(0).p4()+photons->at(0).p4()).mass();
   const double collinear_m_sublead = (collinearLeptons->at(1).p4()+photons->at(0).p4()).mass();
   collinear_m_max = TMath::Max(collinear_m_lead, collinear_m_sublead);
   collinear_m_min = TMath::Min(collinear_m_lead, collinear_m_sublead); 

   tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(ExcitingAnalyzer);

