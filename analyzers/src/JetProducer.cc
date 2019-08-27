// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

class JetProducer : public edm::EDProducer {
   public:
      explicit JetProducer(const edm::ParameterSet&);
   private:
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Jet>> jetToken;
      TH1D * h_nJets;
      TH1D * h_nCollection;
};

JetProducer::JetProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Jet>>("goodJets");
   jetToken = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jetCollection"));
   edm::Service<TFileService> fs;
   h_nJets = fs->make<TH1D>("h_nJets", ";# of jets;events / 1", 10, -0.5, 9.5);
   h_nCollection = fs->make<TH1D>("h_nCollection", ";# of jets;events / 1", 10, -0.5, 9.5);
}

void JetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   //https://twiki.cern.ch/CMS/JetID13TeVRun2018
   auto goodJets = std::make_unique<std::vector<pat::Jet>>();
   edm::Handle<std::vector<pat::Jet>> jets;
   iEvent.getByToken(jetToken, jets);
   h_nCollection->Fill(jets->size());
   for (auto i = jets->begin(); i != jets->end(); ++i) {
      if (i->pt()>=30. && std::abs(i->eta())<2.6) {
         const float NHF = i->neutralHadronEnergyFraction();
         const float NEMF = i->neutralEmEnergyFraction();
         const int NumConst = i->numberOfDaughters();
         const float MUF = i->muonEnergyFraction();
         const float CHF = i->chargedHadronEnergyFraction();
         const int CHM = i->chargedMultiplicity();
         const float CEMF = i->chargedEmEnergyFraction();
         if (NHF<0.9 && NEMF<0.9 && NumConst>1 && MUF<0.8 && CHF>0. && CHM>0 && CEMF<0.8) {     
            goodJets->push_back(*i);
         }
      }
   }
   const int nJets = goodJets->size();
   h_nJets->Fill(nJets);
   iEvent.put(std::move(goodJets), std::string("goodJets"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetProducer);
