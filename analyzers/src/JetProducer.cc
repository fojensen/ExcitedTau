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
#include "DataFormats/PatCandidates/interface/Jet.h"

class JetProducer : public edm::stream::EDFilter<> {
   public:
      explicit JetProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Jet>> jetToken;
      TH1D * h_nJets;
};

JetProducer::JetProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Jet>>("goodJets");
   jetToken = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jetCollection"));
   edm::Service<TFileService> fs;
   h_nJets = fs->make<TH1D>("h_nJets", ";# of jets;events / 1", 10, -0.5, 9.5);
}

bool JetProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto goodJets = std::make_unique<std::vector<pat::Jet>>();
   edm::Handle<std::vector<pat::Jet>> jets;
   iEvent.getByToken(jetToken, jets);
   for (auto i = jets->begin(); i != jets->end(); ++i) {
      if (i->pt()>=30. && std::abs(i->eta())<2.5) {
         goodJets->push_back(*i);
      }
   }
   const int nJets = goodJets->size();
   h_nJets->Fill(nJets);
   iEvent.put(std::move(goodJets), std::string("goodJets"));
   return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetProducer);
