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
#include "DataFormats/PatCandidates/interface/Tau.h"
#include <TH2D.h>

class TauProducer : public edm::stream::EDFilter<> {
   public:
      explicit TauProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Tau>> tauToken;
      bool applyFilter;
      TH2D * h2_nTaus;
};

TauProducer::TauProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Tau>>("goodTaus");
   tauToken = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection")); 
   applyFilter = iConfig.getParameter<bool>("applyFilter");
   edm::Service<TFileService> fs;
   h2_nTaus = fs->make<TH2D>("h2_nTaus", ";# of #tau^{+};# of #tau^{-}", 5, -0.5, 4.5, 5, -0.5, 4.5);
}

bool TauProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   //https://twiki.cern.ch/CMS/TauIDRecommendation13TeV
   auto goodTaus = std::make_unique<std::vector<pat::Tau>>();
   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken, taus);
   int nPlus = 0;
   int nMinus = 0;
   for (auto i = taus->begin(); i != taus->end(); ++i) {
      if (i->pt()>=35. && std::abs(i->eta())<2.1) {
         if (i->tauID("againstElectronVLooseMVA6") && i->tauID("againstMuonLoose3")) {
            if (i->tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") && i->tauID("decayModeFinding")) {
               goodTaus->push_back(*i);
               if (i->charge()>0) ++nPlus;
               if (i->charge()<0) ++nMinus;
            }
         }
      }
   }
   iEvent.put(std::move(goodTaus), std::string("goodTaus"));
   h2_nTaus->Fill(nPlus, nMinus);  
   if (applyFilter) return (nPlus+nMinus>=2);
   return true;
}

DEFINE_FWK_MODULE(TauProducer);
