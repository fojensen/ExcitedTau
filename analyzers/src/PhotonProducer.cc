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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class PhotonProducer : public edm::stream::EDFilter<> {
   public:
      explicit PhotonProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Photon>> photonToken_;
      edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
      bool applyFilter;
      double maxeta, minpt;
      TH1I *h_nCollection, *h_nPhotons;
      TH1I *h_nGen;
};

PhotonProducer::PhotonProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Photon>>("goodPhotons");
   photonToken_ = consumes<std::vector<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photonCollection")); 
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));
   applyFilter = iConfig.getParameter<bool>("applyFilter");
   maxeta = iConfig.getParameter<double>("maxeta");
   minpt = iConfig.getParameter<double>("minpt");
   edm::Service<TFileService> fs;
   h_nCollection = fs->make<TH1I>("h_nCollection", ";# of photons;events / 1", 5, -0.5, 4.5);
   h_nPhotons = fs->make<TH1I>("h_nPhotons", ";# of photons;events / 1", 5, -0.5, 4.5);
   h_nGen = fs->make<TH1I>("h_nGen", ";# of status=1 photons [truth];events / 1", 5, -0.5, 4.5);
}

bool PhotonProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   int nGen = 0;
   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles);
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->pdgId()==22 && i->status()==1) {
         ++nGen;
      }
   }
   h_nGen->Fill(nGen);

   auto goodPhotons = std::make_unique<std::vector<pat::Photon>>();

   edm::Handle<std::vector<pat::Photon>> photons;
   iEvent.getByToken(photonToken_, photons);
   h_nCollection->Fill(photons->size());

   //https://twiki.cern.ch/CMS/EgammaIDRecipesRun2
   for (auto i = photons->begin(); i != photons->end(); ++i) {
      const double eta = std::abs(i->eta());
      //const double eta = std::abs(i->superCluster().eta());
      if (i->pt()>=minpt && eta<maxeta) {
         //if (eta<1.479||eta>=1.653) {
            if (i->passElectronVeto()) {
               if (i->photonID("mvaPhoID-RunIIFall17-v1p1-wp90")) {
                  goodPhotons->push_back(*i);
               }
            }
         //}
      }
   }
   const size_t nPhotons = goodPhotons->size();
   h_nPhotons->Fill(nPhotons);

   iEvent.put(std::move(goodPhotons), std::string("goodPhotons"));  
   if (applyFilter) return nPhotons;
   return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(PhotonProducer);
