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
#include <TH1D.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class GenTauFilter : public edm::stream::EDFilter<> {
   public:
      explicit GenTauFilter(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
      TH1D * h_nGenTaus;
};

GenTauFilter::GenTauFilter(const edm::ParameterSet& iConfig)
{
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));
   edm::Service<TFileService> fs;
   h_nGenTaus = fs->make<TH1D>("h_nGenTaus", ";# of gen #taus;events / 1", 6, -0.5, 5.5);
}

bool GenTauFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   int nGenTaus = 0;
   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles);
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->isLastCopy() && std::abs(i->pdgId())==15) ++nGenTaus;
   }
   h_nGenTaus->Fill(nGenTaus);
   return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenTauFilter);
