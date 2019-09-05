// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include <TVector2.h>
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;

double dphi(const double phi1, const double phi2)
{
   return phi2-phi1;
}

class LeptonPairProducer : public edm::stream::EDProducer<> {
   public:
      explicit LeptonPairProducer(const edm::ParameterSet&);
   private:
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Tau>> tauToken_;
      edm::EDGetTokenT<edm::View<reco::Candidate>> leptonToken_;
      edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
      int q1q2;
};

LeptonPairProducer::LeptonPairProducer(const edm::ParameterSet& iConfig)
{
   produces<pat::PackedCandidateCollection>("visibleTaus");
   produces<pat::PackedCandidateCollection>("collinearTaus");
   tauToken_ = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   leptonToken_ = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonCollection")); 
   metToken_ = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection")); 
   q1q2 = iConfig.getParameter<int>("q1q2");
}

void LeptonPairProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto visibleTaus = std::make_unique<pat::PackedCandidateCollection>();
   auto collinearTaus = std::make_unique<pat::PackedCandidateCollection>();
   
   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken_, taus);

   edm::Handle<edm::View<reco::Candidate>> leptons;
   iEvent.getByToken(leptonToken_, leptons);
 
   bool havePair = false;
   PolarLorentzVector cand0_vis, cand1_vis;
   for (auto i = taus->begin(); i != taus->end(); ++i) {
      if (!havePair) {
         for (auto j = leptons->begin(); j != leptons->end(); ++j) {
            if (i->charge()*j->charge()==q1q2) {
               if (reco::deltaR(*i, *j)>0.4) {
                  cand0_vis = i->p4();
                  cand1_vis = j->p4();
                  havePair = true;
                  break;
               }
            }
         }
      }
   }

   if (havePair) {

   edm::Handle<std::vector<pat::MET>> met;
   iEvent.getByToken(metToken_, met);
   PolarLorentzVector MET;
   MET.SetPt(met->at(0).pt());
   MET.SetEta(met->at(0).eta());
   MET.SetPhi(met->at(0).phi());
   MET.SetM(met->at(0).mass());

   const double cos0M = cos(dphi(cand0_vis.phi(), MET.phi()));
   const double cos1M = cos(dphi(cand1_vis.phi(), MET.phi()));
   const double cos01 = cos(dphi(cand0_vis.phi(), cand1_vis.phi()));
   const double nu0mag = MET.pt() * (cos0M-cos1M*cos01) / (1.-cos01*cos01);
   const double nu1mag = (MET.pt()*cos1M) - (nu0mag*cos01);
   
   PolarLorentzVector nu0;
   nu0.SetEta(cand0_vis.eta());
   nu0.SetPhi(cand0_vis.phi());
   nu0.SetPt(std::abs(nu0mag)); // abs hack to avoid nu0mag<0
   nu0.SetM(0.);
 
   PolarLorentzVector nu1;
   nu1.SetEta(cand1_vis.eta());
   nu1.SetPhi(cand1_vis.phi());
   nu1.SetPt(std::abs(nu1mag)); // abs hack to avoid nu1mag<0
   nu1.SetM(0.);
   
   pat::PackedCandidate visTau0, visTau1;
   pat::PackedCandidate collinearTau0, collinearTau1;
   visTau0.setP4(cand0_vis);
   visTau1.setP4(cand1_vis);
   collinearTau0.setP4(cand0_vis+nu0);
   collinearTau1.setP4(cand1_vis+nu1);

   if (visTau0.pt()>=visTau1.pt()) {
      visibleTaus->push_back(visTau0);
      visibleTaus->push_back(visTau1);     
      collinearTaus->push_back(collinearTau0);
      collinearTaus->push_back(collinearTau1);
   } else {
      visibleTaus->push_back(visTau1);
      visibleTaus->push_back(visTau0); 
      collinearTaus->push_back(collinearTau1);
      collinearTaus->push_back(collinearTau0);
   }

   }

   iEvent.put(std::move(visibleTaus), std::string("visibleTaus"));
   iEvent.put(std::move(collinearTaus), std::string("collinearTaus"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(LeptonPairProducer);
