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
#include <TTree.h>
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/MET.h"

class EventAnalyzer : public edm::EDAnalyzer {
public:
	explicit EventAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<pat::Photon>> photonToken_;
   edm::EDGetTokenT<std::vector<pat::Electron>> electronToken_;
   edm::EDGetTokenT<std::vector<pat::Muon>> muonToken_;
   edm::EDGetTokenT<std::vector<pat::Tau>> tauToken_;
   edm::EDGetTokenT<std::vector<reco::Vertex>> vertexToken_;
   edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
   edm::EDGetTokenT<std::vector<pat::Jet>> jetToken_;

   TTree * tree;
   unsigned int nPhotons, nElectrons, nMuons, nTaus;
   unsigned int BTags[3], NJets;
   unsigned int nJets, nVertices;
   double MET_pt, MET_phi, metSignificance;
   double xsWeight;
};

EventAnalyzer::EventAnalyzer(const edm::ParameterSet& iConfig)
{
   photonToken_ = consumes<std::vector<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photonCollection"));
   electronToken_ = consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electronCollection"));
   muonToken_ = consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muonCollection"));
   tauToken_ = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   vertexToken_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));
   metToken_ = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection"));
   jetToken_ = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jetCollection"));

   xsWeight = iConfig.getParameter<double>("xsWeight");
   
   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   tree->Branch("nPhotons", &nPhotons, "nPhotons/b");
   tree->Branch("nElectrons", &nElectrons, "nElectrons/b");
   tree->Branch("nMuons", &nMuons, "nMuons/b");
   tree->Branch("nTaus", &nTaus, "nTaus/b");
   tree->Branch("nJets", &nJets, "nJets/b");
   tree->Branch("MET_pt", &MET_pt, "MET_pt/D");
   tree->Branch("MET_phi", &MET_phi, "MET_phi/D");
   tree->Branch("metSignificance", &metSignificance, "metSignificance/D");
   tree->Branch("BTags", BTags, "BTags[3]/b");
   tree->Branch("xsWeight", &xsWeight, "xsWeight/D");
   tree->Branch("nVertices", &nVertices, "nVertices/b");
}

void EventAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<pat::Photon>> photons;
   iEvent.getByToken(photonToken_, photons);
   nPhotons = photons->size();
   
   edm::Handle<std::vector<pat::Electron>> electrons;
   iEvent.getByToken(electronToken_, electrons);
   nElectrons = electrons->size();

   edm::Handle<std::vector<pat::Muon>> muons;
   iEvent.getByToken(muonToken_, muons);
   nMuons = muons->size();

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken_, taus);
   nTaus = taus->size();

   edm::Handle<std::vector<pat::Jet>> jets;
   iEvent.getByToken(jetToken_, jets);
   nJets = jets->size();

   edm::Handle<std::vector<reco::Vertex>> vertices;
   iEvent.getByToken(vertexToken_, vertices);
   nVertices = vertices->size();

   edm::Handle<std::vector<pat::MET>> met;
   iEvent.getByToken(metToken_, met);
   MET_pt = met->at(0).pt();
   MET_phi = met->at(0).phi();
   metSignificance = met->at(0).metSignificance();

   BTags[0] = BTags[1] = BTags[2] = 0;
   for (auto i = jets->begin(); i != jets->end(); ++i) {
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
      const float disc = i->bDiscriminator("pfDeepCSVJetTags:probb")+i->bDiscriminator("pfDeepCSVJetTags:probbb");
      if (disc>=0.1241) ++BTags[0];
      if (disc>=0.4184) ++BTags[1];
      if (disc>=0.7527) ++BTags[2];
   }

   tree->Fill();
}

//define this as a plug-in
DEFINE_FWK_MODULE(EventAnalyzer);

