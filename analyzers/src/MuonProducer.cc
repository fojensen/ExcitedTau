// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
// new includes
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class MuonProducer : public edm::stream::EDFilter<> {
   public:
      explicit MuonProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Muon>> muonToken;
      edm::EDGetTokenT<std::vector<reco::Vertex>> primaryVertexColl_;
      TH1I *h_nCollection, *h_nMuons;
      double minpt, maxeta;
      bool applyFilter;
};

MuonProducer::MuonProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Muon>>("goodMuons");
   muonToken = consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muonCollection")); 
   primaryVertexColl_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));
   applyFilter = iConfig.getParameter<bool>("applyFilter");
   maxeta = iConfig.getParameter<double>("maxeta");
   minpt = iConfig.getParameter<double>("minpt"); 
   edm::Service<TFileService> fs;
   h_nCollection = fs->make<TH1I>("h_nCollection", ";# of muons;events / 1", 5, -0.5, 4.5);
   h_nMuons = fs->make<TH1I>("h_nMuons", ";# of muons;events / 1", 5, -0.5, 4.5);
}

bool MuonProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto goodMuons = std::make_unique<std::vector<pat::Muon>>();

   edm::Handle<std::vector<pat::Muon>> muons;
   iEvent.getByToken(muonToken, muons);
   h_nCollection->Fill(muons->size());

   edm::Handle<std::vector<reco::Vertex>> goodVertices;
   iEvent.getByToken(primaryVertexColl_, goodVertices);
   
   //https://twiki.cern.ch/CMS/SWGuideMuonIdRun2
   for (auto i = muons->begin(); i != muons->end(); ++i) {
      if (i->pt()>=minpt && std::abs(i->eta())<maxeta) {
         const float iso = i->trackIso()/i->pt();
         if (iso < 0.1) {
            const bool id = i->isTightMuon(goodVertices->at(0));
            if (id) {
               goodMuons->push_back(*i);
            }
         }
      }
   }
   const size_t nMuons = goodMuons->size();
   h_nMuons->Fill(nMuons);
   iEvent.put(std::move(goodMuons), std::string("goodMuons"));

   if (applyFilter) return nMuons;
   return true;
}

DEFINE_FWK_MODULE(MuonProducer);
