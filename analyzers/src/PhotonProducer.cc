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
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

class PhotonProducer : public edm::stream::EDFilter<> {
   public:
      explicit PhotonProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Photon>> photonToken;
      edm::EDGetTokenT<std::vector<pat::Tau>> tauToken;
      bool applyFilter;
      TH1D *h_nCollection, *h_nPhotons;
};

PhotonProducer::PhotonProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Photon>>("goodPhotons");
   photonToken = consumes<std::vector<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photonCollection")); 
   tauToken = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection")); 
   applyFilter = iConfig.getParameter<bool>("applyFilter");
   edm::Service<TFileService> fs;
   h_nCollection = fs->make<TH1D>("h_nCollection", ";# of photons;events / 1", 5, -0.5, 4.5);
   h_nPhotons = fs->make<TH1D>("h_nPhotons", ";# of photons;events / 1", 5, -0.5, 4.5);
}

bool PhotonProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto goodPhotons = std::make_unique<std::vector<pat::Photon>>();

   edm::Handle<std::vector<pat::Photon>> photons;
   iEvent.getByToken(photonToken, photons);
   h_nCollection->Fill(photons->size());

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken, taus);

   //https://twiki.cern.ch/CMS/EgammaIDRecipesRun2
   for (auto i = photons->begin(); i != photons->end(); ++i) {
      const double eta = std::abs(eta);
      if (i->pt()>=50. && eta<2.5) {
         if (eta<1.479||eta>=1.653) {
            if (i->passElectronVeto()) {
               bool overlap = false;
               for (auto j = taus->begin(); j != taus->end(); ++j) {
                  if (reco::deltaR(*i, *j)<0.14678388) overlap = true;;
               }
               if (!overlap) {
                  goodPhotons->push_back(*i);
               }
            }
         }
      }
   }
   const int nPhotons = goodPhotons->size();
   h_nPhotons->Fill(nPhotons);
   iEvent.put(std::move(goodPhotons), std::string("goodPhotons"));  
   if (applyFilter) return nPhotons;
   return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(PhotonProducer);
