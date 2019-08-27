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
#include "DataFormats/PatCandidates/interface/Tau.h"

class TauPairProducer : public edm::stream::EDFilter<> {
   public:
      explicit TauPairProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Tau>> tauToken;
      int q1q2;
};

TauPairProducer::TauPairProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Tau>>("tauPair");
   tauToken = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection")); 
   q1q2 = iConfig.getParameter<int>("q1q2");
}

bool TauPairProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto goodTaus = std::make_unique<std::vector<pat::Tau>>();
   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken, taus);  
   for (auto i = taus->begin(); i != taus->end(); ++i) {
      for (auto j = i+1; j != taus->end(); ++j) {
         if (i->charge()*j->charge()==q1q2) {
            goodTaus->push_back(*i);
            goodTaus->push_back(*j);
            iEvent.put(std::move(goodTaus), std::string("tauPair"));
            return true;
         }
      }
   }
   iEvent.put(std::move(goodTaus), std::string("tauPair"));
   return true;
}

DEFINE_FWK_MODULE(TauPairProducer);
