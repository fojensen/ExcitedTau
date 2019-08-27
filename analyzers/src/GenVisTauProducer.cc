// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;
#include <TH2D.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"

class GenVisTauProducer : public edm::global::EDProducer<> {
public:
	explicit GenVisTauProducer(const edm::ParameterSet&);
private:
   void produce(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleTok_;
   TH2D *h_leadpt, *h_subleadpt;
   TH1D *h_scale_lead, *h_scale_sublead;
   TH1D *h_dr_lead, *h_dr_sublead;
   TH1D * h_photonpt;
};

GenVisTauProducer::GenVisTauProducer(const edm::ParameterSet& iConfig)
{
   edm::InputTag genParticleTag_ = iConfig.getParameter<edm::InputTag>("genParticleCollection");
   genParticleTok_ = consumes<std::vector<reco::GenParticle>>(genParticleTag_);
   produces<pat::CompositeCandidateCollection>("genVisTaus");
   produces<pat::CompositeCandidateCollection>("genInvTaus");

   edm::Service<TFileService> fs;
   h_leadpt = fs->make<TH2D>("h_leadpt", ";leading #tau_{vis} p_{T} [GeV];#tau_{inv} p_{T} [GeV]", 10, 0., 200., 10, 0., 200.);
   h_subleadpt = fs->make<TH2D>("h_subleadpt", ";subleading #tau_{vis} p_{T} [GeV];#tau_{inv} p_{T} [GeV]", 10, 0., 200., 10, 0., 200.);
   h_scale_lead = fs->make<TH1D>("h_scale_lead", ";;", 20, 0., 2.);
   h_scale_sublead = fs->make<TH1D>("h_scale_sublead", ";;", 20, 0., 2.);
   h_dr_lead = fs->make<TH1D>("h_dr_lead", ";#deltaR(leading #tau_{vis}, #tau_{inv});events / 0.05", 24, 0., 1.2);
   h_dr_sublead = fs->make<TH1D>("h_dr_sublead", ";#deltaR(subleading #tau_{vis}, #tau_{inv});events / 0.05", 24, 0., 1.2);
}

void GenVisTauProducer::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const
{
   pat::CompositeCandidate visPlus, visMinus;
   PolarLorentzVector vPlus(0., 0., 0., 0.);
   PolarLorentzVector vMinus(0., 0., 0., 0.);
   bool hadPlus = true;
   bool hadMinus = true;

   pat::CompositeCandidate invPlus, invMinus;
   PolarLorentzVector iPlus(0., 0., 0., 0.);
   PolarLorentzVector iMinus(0., 0., 0., 0.);

   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleTok_, genParticles);
 
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->mother()) {
         if (i->mother()->pdgId()!=i->pdgId()) { // isLastCopy()
            const int id = std::abs(i->pdgId());
            if (i->mother()->pdgId()==15) {
               if (id==12||id==14||id==16) {
                  invMinus.addDaughter(*i);
                  iMinus += i->polarP4();
               } else {
                  visMinus.addDaughter(*i);
                  vMinus += i->polarP4();
               }
               if (id==11||id==13) hadMinus=false;
            }
            if (i->mother()->pdgId()==-15) {
               if (id==12||id==14||id==16) {
                  invPlus.addDaughter(*i);
                  iPlus += i->polarP4();
               } else {
                  visPlus.addDaughter(*i);
                  vPlus += i->polarP4();
               }
               if (id==11||id==13) hadPlus=false;
            }
         }
      }
   }

   //(condition) ? (if_true) : (if_false)
   (hadMinus) ? (visMinus.setName("hadronic")) : visMinus.setName("leptonic");
   (hadPlus) ? (visPlus.setName("hadronic")) : visPlus.setName("leptonic");
   invMinus.setName("minus");   
   invPlus.setName("plus");

   visMinus.setP4(vMinus);
   visPlus.setP4(vPlus);
   invMinus.setP4(iMinus);
   invPlus.setP4(iPlus);

   auto genVisTaus   = std::make_unique<pat::CompositeCandidateCollection>();
   auto genInvTaus = std::make_unique<pat::CompositeCandidateCollection>(); 
   if ( vMinus.Pt()>0. || vPlus.Pt()>0. || iMinus.Pt()>0. || iPlus.Pt()>0. ) {
      if (visPlus.pt()>=visMinus.pt()) {
         genVisTaus->push_back(visPlus);
         genVisTaus->push_back(visMinus);
         genInvTaus->push_back(invPlus);
         genInvTaus->push_back(invMinus);
         h_leadpt->Fill(visPlus.pt(), invPlus.pt());
         h_subleadpt->Fill(visMinus.pt(), invMinus.pt());
         h_scale_lead->Fill(invPlus.p()/visPlus.p());
         h_scale_sublead->Fill(invMinus.p()/visMinus.p());
         h_dr_lead->Fill(reco::deltaR(visPlus, invPlus));
         h_dr_sublead->Fill(reco::deltaR(visMinus, invMinus));
      } else {
         genVisTaus->push_back(visMinus);
         genVisTaus->push_back(visPlus);
         genInvTaus->push_back(invMinus);
         genInvTaus->push_back(invPlus);
         h_leadpt->Fill(visMinus.pt(), invMinus.pt());
         h_subleadpt->Fill(visPlus.pt(), invPlus.pt());
         h_scale_lead->Fill(invMinus.p()/visMinus.p());
         h_scale_sublead->Fill(invPlus.p()/visPlus.p());
         h_dr_lead->Fill(reco::deltaR(visMinus, invMinus));
         h_dr_sublead->Fill(reco::deltaR(visPlus, invPlus));
      }
   }
   iEvent.put(std::move(genVisTaus), std::string("genVisTaus"));
   iEvent.put(std::move(genInvTaus), std::string("genInvTaus"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenVisTauProducer);

