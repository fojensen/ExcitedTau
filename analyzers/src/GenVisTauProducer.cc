// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;
//#include <TH2D.h>
#include <TTree.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"

//class GenVisTauProducer : public edm::global::EDProducer<> {
class GenVisTauProducer : public edm::EDProducer {
public:
	explicit GenVisTauProducer(const edm::ParameterSet&);
private:
   void produce(edm::Event&, const edm::EventSetup&) override;
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleTok_;
   //TH2D *h_leadpt, *h_subleadpt;
   //TH1D *h_scale_lead, *h_scale_sublead;
   //TH1D *h_dr_lead, *h_dr_sublead;
   //TH1D * h_photonpt;
   TTree * tree;
   double leadvis_pt, leadvis_eta, leadvis_phi, leadvis_mass;
   //double leadinv_pt, leadinv_eta, leadinv_phi, leadinv_mass;
   double sublvis_pt, sublvis_eta, sublvis_phi, sublvis_mass;
   double photon_pt, photon_eta;
   int dm;
   //double sublinv_pt, subleinv_eta, subleinv_phi, leadinv_mass;
};

GenVisTauProducer::GenVisTauProducer(const edm::ParameterSet& iConfig)
{
   edm::InputTag genParticleTag_ = iConfig.getParameter<edm::InputTag>("genParticleCollection");
   genParticleTok_ = consumes<std::vector<reco::GenParticle>>(genParticleTag_);
   produces<pat::CompositeCandidateCollection>("genVisTaus");
   produces<pat::CompositeCandidateCollection>("genInvTaus");

   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");
   tree->Branch("leadvis_pt", &leadvis_pt, "leadvis_pt/D");
   tree->Branch("leadvis_eta", &leadvis_eta, "leadvis_eta/D");
   tree->Branch("sublvis_pt", &sublvis_pt, "sublvis_pt/D");
   tree->Branch("sublvis_eta", &sublvis_eta, "sublvis_eta/D");
   tree->Branch("dm", &dm, "dm/I");
   tree->Branch("photon_pt", &photon_pt, "photon_pt/D");
   tree->Branch("photon_eta", &photon_eta, "photon_eta/D");
   //h_leadpt = fs->make<TH2D>("h_leadpt", ";leading #tau_{vis} p_{T} [GeV];#tau_{inv} p_{T} [GeV]", 10, 0., 200., 10, 0., 200.);
   //h_subleadpt = fs->make<TH2D>("h_subleadpt", ";subleading #tau_{vis} p_{T} [GeV];#tau_{inv} p_{T} [GeV]", 10, 0., 200., 10, 0., 200.);
   //h_scale_lead = fs->make<TH1D>("h_scale_lead", ";;", 20, 0., 2.);
   //h_scale_sublead = fs->make<TH1D>("h_scale_sublead", ";;", 20, 0., 2.);
   //h_dr_lead = fs->make<TH1D>("h_dr_lead", ";#deltaR(leading #tau_{vis}, #tau_{inv});events / 0.05", 24, 0., 1.2);
   //h_dr_sublead = fs->make<TH1D>("h_dr_sublead", ";#deltaR(subleading #tau_{vis}, #tau_{inv});events / 0.05", 24, 0., 1.2);
   
}

void GenVisTauProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   pat::CompositeCandidate visPlus;
   PolarLorentzVector visPlus_(0., 0., 0., 0.);
   bool hadPlus = true;

   pat::CompositeCandidate visMinus;
   PolarLorentzVector visMinus_(0., 0., 0., 0.);
   bool hadMinus = true;

   pat::CompositeCandidate invPlus;
   PolarLorentzVector invPlus_(0., 0., 0., 0.);

   pat::CompositeCandidate invMinus;
   PolarLorentzVector invMinus_(0., 0., 0., 0.);

   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleTok_, genParticles);
 
   photon_pt = 0.;
   photon_eta = 0.;
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->pdgId()==22 && i->mother()) {
         if (std::abs(i->mother()->pdgId())==4000015) {
            photon_pt = i->pt();
            photon_eta = i->eta();
         }
      }
   }

   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->mother()) {
         if (i->mother()->pdgId()!=i->pdgId()) { // isLastCopy()
            const int id = std::abs(i->pdgId());
            if (i->mother()->pdgId()==15) {
               if (id==12||id==14||id==16) {
                  invMinus.addDaughter(*i);
                  invMinus_ += i->polarP4();
               } else {
                  visMinus.addDaughter(*i);
                  visMinus_ += i->polarP4();
               }
               if (id==11||id==13) hadMinus=false;
            }
            if (i->mother()->pdgId()==-15) {
               if (id==12||id==14||id==16) {
                  invPlus.addDaughter(*i);
                  invPlus_ += i->polarP4();
               } else {
                  visPlus.addDaughter(*i);
                  visPlus_ += i->polarP4();
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

   visMinus.setP4(visMinus_);
   visPlus.setP4(visPlus_);
   invMinus.setP4(invMinus_);
   invPlus.setP4(invPlus_);

   auto genVisTaus = std::make_unique<pat::CompositeCandidateCollection>();
   auto genInvTaus = std::make_unique<pat::CompositeCandidateCollection>(); 
   
   if (visPlus.pt()>=visMinus.pt()) {
      leadvis_pt = visPlus.pt();
      leadvis_eta = visPlus.eta();
      sublvis_pt = visMinus.pt();
      sublvis_eta = visMinus.eta();
      genVisTaus->push_back(visPlus);
      genVisTaus->push_back(visMinus);
      genInvTaus->push_back(invPlus);
      genInvTaus->push_back(invMinus);
      //h_leadpt->Fill(visPlus.pt(), invPlus.pt());
      //h_subleadpt->Fill(visMinus.pt(), invMinus.pt());
      //h_scale_lead->Fill(invPlus.p()/visPlus.p());
      //h_scale_sublead->Fill(invMinus.p()/visMinus.p());
      //h_dr_lead->Fill(reco::deltaR(visPlus, invPlus));
      //h_dr_sublead->Fill(reco::deltaR(visMinus, invMinus));
   } else {
      leadvis_pt = visMinus.pt();
      leadvis_eta = visMinus.eta();
      sublvis_pt = visPlus.pt();
      sublvis_eta = visPlus.eta();
      genVisTaus->push_back(visMinus);
      genVisTaus->push_back(visPlus);
      genInvTaus->push_back(invMinus);
      genInvTaus->push_back(invPlus);
      //h_leadpt->Fill(visMinus.pt(), invMinus.pt());
      //h_subleadpt->Fill(visPlus.pt(), invPlus.pt());
      //h_scale_lead->Fill(invMinus.p()/visMinus.p());
      //h_scale_sublead->Fill(invPlus.p()/visPlus.p());
      //h_dr_lead->Fill(reco::deltaR(visMinus, invMinus));
      //h_dr_sublead->Fill(reco::deltaR(visPlus, invPlus));
   }

   // *** dm ***
   dm = 0;
   bool isel[2] = {false, false};
   bool ismu[2] = {false, false};
   for (auto i = genParticles->begin(); i != genParticles->end(); ++i) {
      if (i->mother()) {
         const int mid = i->mother()->pdgId();
         const int id = i->pdgId();
         if ((mid==15||mid==-24) && id==11) isel[0] = true;
         if ((mid==15||mid==-24) && id==13) ismu[0] = true;
         if ((mid==-15||mid==24) && id==-11) isel[1] = true;
         if ((mid==-15||mid==24) && id==-13) ismu[1] = true; 
      }
      // mu+mu->1; e+e->2; e+mu->3; mu+had->4; e+had->5; hadhad->6;
      //mu+mu
      if (ismu[0] && ismu[1]) dm = 1;
      //e+e
      if (isel[0] && isel[1]) dm = 2;
      //e+mu
      if ( (isel[0]&&ismu[1]) || (ismu[0]&&isel[1]) ) dm = 3;
      //mu+had
      if ( (ismu[0]&&!isel[1]&&!ismu[1]) || (!isel[0]&&!ismu[0]&&ismu[1]) ) dm = 4;
      //e+had
      if ( (isel[0]&&!isel[1]&&!ismu[1]) || (!isel[0]&&!ismu[0]&&isel[1]) ) dm = 5;
      //had+had
      if ( !ismu[0] && !isel[0] && !ismu[1] && !isel[1] ) dm = 6;
   }
   
   tree->Fill();

    iEvent.put(std::move(genVisTaus), std::string("genVisTaus"));
    iEvent.put(std::move(genInvTaus), std::string("genInvTaus"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenVisTauProducer);

