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

class NTupleMaker : public edm::EDAnalyzer {
public:
	explicit NTupleMaker(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<pat::Photon>> photonToken;
   edm::EDGetTokenT<std::vector<pat::Electron>> electronToken;
   edm::EDGetTokenT<std::vector<pat::Muon>> muonToken;
   edm::EDGetTokenT<std::vector<pat::Tau>> tauToken;
   edm::EDGetTokenT<std::vector<reco::Vertex>> vertexToken;
   edm::EDGetTokenT<std::vector<pat::MET>> metToken;
   edm::EDGetTokenT<std::vector<pat::Jet>> jetToken;

   TTree * tree;
   unsigned int nPhotons, nElectrons, nMuons, nTaus;
   unsigned int BTags[3], NJets;
   unsigned int nJets, nVertices;
   double MET_pt, MET_phi, metSignificance;
   double xsWeight;
};

NTupleMaker::NTupleMaker(const edm::ParameterSet& iConfig)
{
   photonToken = consumes<std::vector<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photonCollection"));
   electronToken = consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electronCollection"));
   muonToken = consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muonCollection"));
   tauToken = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   vertexToken = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));
   metToken = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection"));
   jetToken = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jetCollection"));

   xsWeight = iConfig.getParameter<double>("xsWeight");
   
   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   tree->Branch("nPhotons", &nPhotons, "nPhotons/i");
   tree->Branch("nElectrons", &nElectrons, "nElectrons/i");
   tree->Branch("nMuons", &nMuons, "nMuons/i");
   tree->Branch("nTaus", &nTaus, "nTaus/i");
   tree->Branch("nJets", &nJets, "nJets/i");
   tree->Branch("MET_pt", &MET_pt, "MET_pt/D");
   tree->Branch("MET_phi", &MET_phi, "MET_phi/D");
   tree->Branch("metSignificance", &metSignificance, "metSignificance/D");
   tree->Branch("BTags", BTags, "BTags[3]/i");
   tree->Branch("xsWeight", &xsWeight, "xsWeight/D");
   tree->Branch("nVertices", &nVertices, "nVertices/i");
}

void NTupleMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<pat::Photon>> photons;
   iEvent.getByToken(photonToken, photons);
   nPhotons = photons->size();
   
   edm::Handle<std::vector<pat::Electron>> electrons;
   iEvent.getByToken(electronToken, electrons);
   nElectrons = electrons->size();

   edm::Handle<std::vector<pat::Muon>> muons;
   iEvent.getByToken(muonToken, muons);
   nMuons = muons->size();

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken, taus);
   nTaus = taus->size();

   edm::Handle<std::vector<pat::Jet>> jets;
   iEvent.getByToken(jetToken, jets);
   nJets = jets->size();

   edm::Handle<std::vector<reco::Vertex>> vertices;
   iEvent.getByToken(vertexToken, vertices);
   nVertices = vertices->size();

   edm::Handle<std::vector<pat::MET>> met;
   iEvent.getByToken(metToken, met);
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
DEFINE_FWK_MODULE(NTupleMaker);

