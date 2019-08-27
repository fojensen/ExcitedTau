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
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TH2D.h>

class MuonProducer : public edm::stream::EDFilter<> {
   public:
      explicit MuonProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Muon>> muonToken;
      edm::EDGetTokenT<std::vector<reco::Vertex>> primaryVertexColl_;
      TH2D * h_nMuons;
      TH1D * h_nCollection;
      bool applyFilter;
};

MuonProducer::MuonProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Muon>>("goodMuons");
   muonToken = consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muonCollection")); 
   primaryVertexColl_ = consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexCollection"));
   applyFilter = iConfig.getParameter<bool>("applyFilter");
   edm::Service<TFileService> fs;
   h_nMuons = fs->make<TH2D>("h_nMuons", ";# of #mu^{+};# of #mu^{-}", 4, -0.5, 3.5, 4, -0.5, 3.5);
   h_nCollection = fs->make<TH1D>("h_nCollection", ";number of muons;events / bin", 4, -0.5, 3.5);
}

bool MuonProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<reco::Vertex>> goodVertices;
   iEvent.getByToken(primaryVertexColl_, goodVertices);
   //https://twiki.cern.ch/CMS/SWGuideMuonIdRun2
   auto goodMuons = std::make_unique<std::vector<pat::Muon>>();
   edm::Handle<std::vector<pat::Muon>> muons;
   iEvent.getByToken(muonToken, muons);
   h_nCollection->Fill(muons->size());
   int nPlus = 0;
   int nMinus = 0;
   for (auto i = muons->begin(); i != muons->end(); ++i) {
      if (i->pt()>=35. && std::abs(i->eta())<2.4) {
         const double iso = i->isolationR03().sumPt/i->pt();
         if (iso < 0.1) {
            const bool id = i->isHighPtMuon(goodVertices->at(0));
            //const bool id = i->isTightMuon(goodVertices->at(0));
            if (id) {
               goodMuons->push_back(*i);
               if (i->charge()>0) ++nPlus;
               if (i->charge()<0) ++nMinus;
            }
         }
      }
   }
   iEvent.put(std::move(goodMuons), std::string("goodMuons"));
   h_nMuons->Fill(nPlus, nMinus);
   if (applyFilter) return (nPlus+nMinus>=1);
   return true;
}

DEFINE_FWK_MODULE(MuonProducer);
