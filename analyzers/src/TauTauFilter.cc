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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class TauTauFilter : public edm::stream::EDFilter<> {
   public:
      explicit TauTauFilter(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken;
};

TauTauFilter::TauTauFilter(const edm::ParameterSet& iConfig)
{
   genParticleToken = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection")); 
}

bool TauTauFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken, genParticles); 
   int nGenTaus = 0;
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (std::abs(i->pdgId())==15 && i->isLastCopy()) {
         ++nGenTaus;
      }
   }
   return (nGenTaus>=2);
}

DEFINE_FWK_MODULE(TauTauFilter);
