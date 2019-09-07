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
      double maxeta, minpt;
      TH1I *h_nCollection, *h_nElectrons;
};

ElectronProducer::ElectronProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Electron>>("goodElectrons");
   electronToken = consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electronCollection")); 
   applyFilter = iConfig.getParameter<bool>("applyFilter");
   maxeta = iConfig.getParameter<double>("maxeta");
   minpt = iConfig.getParameter<double>("minpt");
   edm::Service<TFileService> fs;
   h_nCollection = fs->make<TH1I>("h_nCollection", ";# of electrons;events / 1", 4, -0.5, 4.5);
   h_nElectrons = fs->make<TH1I>("h_nElectrons", ";# of electrons;events / 1", 4, -0.5, 4.5);
}

bool ElectronProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto goodElectrons = std::make_unique<std::vector<pat::Electron>>();

   edm::Handle<std::vector<pat::Electron>> electrons;
   iEvent.getByToken(electronToken, electrons);
   h_nCollection->Fill(electrons->size());

   for (auto i = electrons->begin(); i != electrons->end(); ++i) {
      if (i->pt()>=minpt && std::abs(i->eta())<maxeta) {
         goodElectrons->push_back(*i);
      }
   }

   const size_t nElectrons = goodElectrons->size();
   h_nElectrons->Fill(nElectrons);
   iEvent.put(std::move(goodElectrons), std::string("goodElectrons"));

   if (applyFilter) return nElectrons;
   return true;
}

DEFINE_FWK_MODULE(ElectronProducer);
