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
#include "DataFormats/VertexReco/interface/Vertex.h"
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

   TTree * tree;
   unsigned int nPhotons, nLeptons, nVertices;
   unsigned int BTags[3], NJets;
   double HT;
   double MET, MET_phi, metSignificance;
   double pt_photon, pt_lead, pt_sublead;
   double phi_lead, phi_sublead, phi_ll;
   double m_lead, m_sublead;
   double xsWeight;
   // leptons only
   double m_ll, pt_ll, dr_ll;
   int sumQ;
   // leptons+photon
   double m_max, m_min;
   double dr_lead, dr_sublead;
   float photon_hadTowOverEm;
   // collinear
   double collinear_m_ll;
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
 
   const double xs_ = iConfig.getParameter<double>("xs");
   const int nevents_ = iConfig.getParameter<int>("nevents");
   xsWeight = xs_/double(nevents_);
   
   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   // all events
   tree->Branch("nPhotons", &nPhotons, "nPhotons/i");
   tree->Branch("nLeptons", &nLeptons, "nLeptons/i");
   //tree->Branch("nVertices", &nVertices, "nVertices/i");
   tree->Branch("MET", &MET, "MET/D");
   tree->Branch("HT", &HT, "HT/D");
   tree->Branch("NJets", &NJets, "NJets/i");
   tree->Branch("MET_phi", &MET_phi, "MET_phi/D");
   tree->Branch("metSignificance", &metSignificance, "metSignificance/D");
   tree->Branch("BTags", BTags, "BTags[3]/i");
   tree->Branch("xsWeight", &xsWeight, "xsWeight/D");
   // leptons only
   tree->Branch("pt_lead", &pt_lead, "pt_lead/D");
   tree->Branch("pt_sublead", &pt_sublead, "pt_sublead/D");
   tree->Branch("phi_lead", &phi_lead, "phi_lead/D");
   tree->Branch("phi_sublead", &phi_sublead, "phi_sublead/D");
   tree->Branch("m_lead", &m_lead, "m_lead/D");
   tree->Branch("m_sublead", &m_sublead, "m_sublead/D");
   tree->Branch("m_ll", &m_ll, "m_ll/D");
   tree->Branch("pt_ll", &pt_ll, "pt_ll/D");
   tree->Branch("phi_ll", &phi_ll, "phi_ll/D");
   tree->Branch("dr_ll", &dr_ll, "dr_ll/D");
   tree->Branch("sumQ", &sumQ, "sumQ/I");
   // leptons+photon
   tree->Branch("pt_photon", &pt_photon, "pt_photon/D");
   tree->Branch("m_max", &m_max, "m_max/D");
   tree->Branch("m_min", &m_min, "m_min/D");
   tree->Branch("dr_lead", &dr_lead, "dr_lead/D");
   tree->Branch("dr_sublead", &dr_sublead, "dr_sublead/D");
   tree->Branch("photon_hadTowOverEm", &photon_hadTowOverEm, "photon_hadTowOverEm/F");
   // collinear
   tree->Branch("collinear_m_ll", &collinear_m_ll, "collinear_m_ll/D");
   tree->Branch("collinear_m_max", &collinear_m_max, "collinear_m_max/D");
   tree->Branch("collinear_m_min", &collinear_m_min, "collinear_m_min/D");
}

void ExcitingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   nPhotons = nLeptons = nVertices = 0;
   MET = MET_phi = metSignificance = 0.;
   BTags[0] = BTags[1] = BTags[2] = 0;
   pt_photon = pt_lead = pt_sublead = 0.;
   phi_lead = phi_sublead = phi_ll = 0.;;
   m_lead = m_sublead = 0.;
   NJets = 0;
   HT = 0.;
   // leptons only
   m_ll = pt_ll = dr_ll = 0.;
   sumQ = 0;
   // leptons+photon
   m_max = m_min = 0.;
   dr_lead = dr_sublead = 0.;
   photon_hadTowOverEm = 0.;
   // collinear
   collinear_m_ll = collinear_m_min = collinear_m_max = 0.;

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
   MET = met->at(0).pt();
   MET_phi = met->at(0).phi();
   metSignificance = met->at(0).metSignificance();

   edm::Handle<std::vector<pat::Jet>> jets;
   iEvent.getByToken(jetToken_, jets);
   for (auto i = jets->begin(); i != jets->end(); ++i) {
      HT += i->pt();
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

   pt_lead = leptons->at(0).pt();
   pt_sublead = leptons->at(1).pt();

   phi_lead = leptons->at(0).phi();
   phi_sublead = leptons->at(1).phi();

   m_lead = leptons->at(0).mass();
   m_sublead = leptons->at(1).mass();

   m_ll = (leptons->at(0).p4()+leptons->at(1).p4()).mass();
   pt_ll = (leptons->at(0).p4()+leptons->at(1).p4()).pt();
   phi_ll = (leptons->at(0).p4()+leptons->at(1).p4()).phi();
   dr_ll = reco::deltaR(leptons->at(0), leptons->at(1));

   sumQ = leptons->at(0).charge()+leptons->at(1).charge();

   // collinear
   edm::Handle<edm::View<reco::Candidate>> collinearLeptons;
   iEvent.getByToken(collinearLeptonToken, collinearLeptons);
   collinear_m_ll = (collinearLeptons->at(0).p4()+collinearLeptons->at(1).p4()).mass();

   if (nPhotons<1) {
      tree->Fill();
      return;
   }

   pt_photon = photons->at(0).pt();
   photon_hadTowOverEm = photons->at(0).hadTowOverEm();
 
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

