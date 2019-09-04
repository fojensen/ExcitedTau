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
#include "DataFormats/PatCandidates/interface/Electron.h"

class ElectronProducer : public edm::stream::EDFilter<> {
   public:
      explicit ElectronProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Electron>> electronToken;
      bool applyFilter;
      TH1D * h_nCollection, *h_nElectrons;
};

ElectronProducer::ElectronProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Electron>>("goodElectrons");
   electronToken = consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electronCollection")); 
   applyFilter = iConfig.getParameter<bool>("applyFilter");
   edm::Service<TFileService> fs;
   h_nCollection = fs->make<TH1D>("h_nCollection", ";# of electrons;events / 1", 5, -0.5, 4.5);
   h_nElectrons = fs->make<TH1D>("h_nElectrons", ";# of electons;events / 1", 5, -0.5, 4.5);
}

bool ElectronProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto goodElectrons = std::make_unique<std::vector<pat::Electron>>();

   edm::Handle<std::vector<pat::Electron>> electrons;
   iEvent.getByToken(electronToken, electrons);
   h_nCollection->Fill(electrons->size());

   for (auto i = electrons->begin(); i != electrons->end(); ++i) {
      if (i->pt()>=35. && std::abs(i->eta())<2.5) {
         goodElectrons->push_back(*i);
      }
   }

   const int nElectrons = goodElectrons->size();
   h_nElectrons->Fill(nElectrons);
   iEvent.put(std::move(goodElectrons), std::string("goodElectrons"));

   if (applyFilter) return nElectrons;
   return true;
}

DEFINE_FWK_MODULE(ElectronProducer);
