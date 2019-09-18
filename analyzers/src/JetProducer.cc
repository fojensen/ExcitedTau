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
#include "DataFormats/PatCandidates/interface/Tau.h"

class JetProducer : public edm::EDProducer {
   public:
      explicit JetProducer(const edm::ParameterSet&);
   private:
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Jet>> jetToken;
      edm::EDGetTokenT<std::vector<pat::Tau>> tauToken;
      double maxeta, minpt;
      TH1I *h_nCollection, *h_nJets;
      TH1D *h_jetpt, *h_HT;
};

JetProducer::JetProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::Jet>>("goodJets");
   jetToken = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jetCollection"));
   tauToken = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   maxeta = iConfig.getParameter<double>("maxeta");
   minpt = iConfig.getParameter<double>("minpt");
   edm::Service<TFileService> fs;
   h_nCollection = fs->make<TH1I>("h_nCollection", ";# of jets;events / 1", 10, -0.5, 9.5);
   h_nJets = fs->make<TH1I>("h_nJets", ";# of jets;events / 1", 10, -0.5, 9.5);
   // study QCD
   const double x1[16] = {0., 15., 30., 50., 80., 120., 170., 300., 470., 600., 800., 1000., 1400., 1800., 2400., 3200.};
   h_jetpt = fs->make<TH1D>("h_jetpt", ";jet p_{T} [GeV];jets / bin", 15, x1);
   const double x2[9] = {50., 100., 200., 300., 500., 700., 1000., 1500., 2000.};
   h_HT = fs->make<TH1D>("h_HT", ";HT [GeV];events / bin", 8, x2);
}

void JetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto goodJets = std::make_unique<std::vector<pat::Jet>>();

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken, taus);

   double HT = 0.;

   //https://twiki.cern.ch/CMS/JetID13TeVRun2018
   edm::Handle<std::vector<pat::Jet>> jets;
   iEvent.getByToken(jetToken, jets);
   h_nCollection->Fill(jets->size());
   for (auto i = jets->begin(); i != jets->end(); ++i) {
      bool overlap = false;
      for (auto j = taus->begin(); j != taus->end(); ++j) {
         if (reco::deltaR(*i, *j)<0.4) {
            overlap = true;
            break;
         }
      }
      if (i->pt()>=minpt && std::abs(i->eta())<maxeta) {
         const float NHF = i->neutralHadronEnergyFraction();
         const float NEMF = i->neutralEmEnergyFraction();
         const size_t NumConst = i->numberOfDaughters();
         const float MUF = i->muonEnergyFraction();
         const float CHF = i->chargedHadronEnergyFraction();
         const int CHM = i->chargedMultiplicity();
         const float CEMF = i->chargedEmEnergyFraction();
         if (NHF<0.9 && NEMF<0.9 && NumConst>1 && MUF<0.8 && CHF>0. && CHM>0 && CEMF<0.8) {
            h_jetpt->Fill(i->pt());
            HT += i->pt();
            if (!overlap) {
               goodJets->push_back(*i);
            }
         }
      }
   }
   h_HT->Fill(HT);
   const size_t nJets = goodJets->size();
   h_nJets->Fill(nJets);
   iEvent.put(std::move(goodJets), std::string("goodJets"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetProducer);
