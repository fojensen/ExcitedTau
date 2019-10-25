// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TTree.h>
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;
#include "DataFormats/Math/interface/deltaR.h"
#include <TVector2.h>
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

double dphi(double phi1, double phi2)
{
   const double pi = 3.14159265359;
   if (phi1<2.*pi) phi1 = (phi1+2.*pi);
   if (phi2<2.*pi) phi2 = (phi2+2.*pi);
   double dphi_ = std::abs(phi2-phi1);
   if (dphi_>=pi) dphi_ = 2.*pi - dphi_;
   return dphi_;
}

class LeptonPairProducer : public edm::stream::EDProducer<> {
   public:
      explicit LeptonPairProducer(const edm::ParameterSet&);
   private:
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Tau>> tauToken_;
      edm::EDGetTokenT<edm::View<reco::Candidate>> leptonToken_;  
      edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
      edm::EDGetTokenT<edm::View<reco::Candidate>> photonToken_;
   //   edm::EDGetTokenT<pat::CompositeCandidateCollection> genVisTauToken_;
      int q1q2;
      double maxeta_lepton, minpt_lepton;
      double maxeta_tau, minpt_tau;
      std::string eVeto_tau, muVeto_tau;
      double maxeta_photon, minpt_photon;
      TTree * tree;
      bool haveTriplet, havePair, havePhoton;
      double deltapt, deltaphi;
     // double gendr_tau, gendr_lepton;
};

LeptonPairProducer::LeptonPairProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::PackedCandidate>>("visibleTaus");
   produces<std::vector<pat::CompositeCandidate>>("collinearTaus");
   produces<std::vector<pat::PackedCandidate>>("selectedPhoton");
   
   tauToken_ =    consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   leptonToken_ = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonCollection")); 
   metToken_ =    consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection")); 
   photonToken_ = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("photonCollection"));

   minpt_lepton = iConfig.getParameter<double>("minpt_lepton");
   maxeta_lepton = iConfig.getParameter<double>("maxeta_lepton");
   minpt_tau = iConfig.getParameter<double>("minpt_tau");
   maxeta_tau = iConfig.getParameter<double>("maxeta_tau");
   eVeto_tau = iConfig.getParameter<std::string>("eVeto_tau");
   muVeto_tau = iConfig.getParameter<std::string>("muVeto_tau");
   minpt_photon = iConfig.getParameter<double>("minpt_photon");
   maxeta_photon = iConfig.getParameter<double>("maxeta_photon");
   q1q2 = iConfig.getParameter<int>("q1q2");
   
   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");   
   tree->Branch("haveTriplet", &haveTriplet, "haveTriplet/O");
   tree->Branch("havePair", &havePair, "havePair/O");
   tree->Branch("havePhoton", &havePhoton, "havePhoton/O");
   tree->Branch("deltaphi", &deltaphi, "deltaphi/D");
   tree->Branch("deltapt", &deltapt, "deltapt/D");
   //gen matching
   //genVisTauToken_ = consumes<pat::CompositeCandidateCollection>(iConfig.getParameter<edm::InputTag>("genVisTauCollection"));
   //tree->Branch("gendr_tau", &gendr_tau, "gendr_tau/D");
   //tree->Branch("gendr_lepton", &gendr_lepton, "gendr_lepton/D");
}

void LeptonPairProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto visibleTaus = std::make_unique<std::vector<pat::PackedCandidate>>();
   auto collinearTaus = std::make_unique<std::vector<pat::CompositeCandidate>>();
   auto photonsOut = std::make_unique<std::vector<pat::PackedCandidate>>();

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken_, taus);

   edm::Handle<edm::View<reco::Candidate>> leptons;
   iEvent.getByToken(leptonToken_, leptons);
 
   edm::Handle<edm::View<reco::Candidate>> photonsIn;
   iEvent.getByToken(photonToken_, photonsIn);
 
   pat::PackedCandidate tau_vis;
   pat::PackedCandidate lepton_vis;
   pat::PackedCandidate photon;

   haveTriplet = havePair = havePhoton = false;
   for (auto i = taus->begin(); i != taus->end(); ++i) {
      if (i->tauID(eVeto_tau) && i->tauID(muVeto_tau)) {
         if (i->pt()>=minpt_tau && std::abs(i->eta())<maxeta_tau) {
            for (auto j = leptons->begin(); j != leptons->end(); ++j) {
               if (j->pt()>=minpt_lepton && std::abs(j->eta())<maxeta_lepton) {
                  if (i->charge()*j->charge()==q1q2) {
                     for (auto k = photonsIn->begin(); k != photonsIn->end(); ++k) {
                        if (k->pt()>=minpt_photon && std::abs(k->eta())<maxeta_photon) {
                           if (reco::deltaR(*i, *j)>=0.4 && reco::deltaR(*i, *k)>=0.4 && reco::deltaR(*j, *k)>=0.4) {
                              tau_vis.setP4(i->p4());
                              tau_vis.setCharge(i->charge());
                              tau_vis.setPdgId(i->pdgId());
                              lepton_vis.setP4(j->p4());
                              lepton_vis.setCharge(j->charge());
                              lepton_vis.setPdgId(j->pdgId());
                              photon.setP4(k->p4());
                              photon.setCharge(k->charge());
                              photon.setPdgId(k->pdgId());
                              haveTriplet = true;
                              havePair = true;
                              havePhoton = true;
                              break;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
   
   if (!haveTriplet) {
      for (auto i = taus->begin(); i != taus->end(); ++i) {
         if (i->tauID(eVeto_tau) && i->tauID(muVeto_tau)) {
            if (i->pt()>=minpt_tau && std::abs(i->eta())<maxeta_tau) {
               for (auto j = leptons->begin(); j != leptons->end(); ++j) {
                  if (j->pt()>=minpt_lepton && std::abs(j->eta())<maxeta_lepton) {
                     if (i->charge()*j->charge()==q1q2) {
                        if (reco::deltaR(*i, *j)>=0.4) {
                           tau_vis.setP4(i->p4());
                           tau_vis.setCharge(i->charge());
                           tau_vis.setPdgId(i->pdgId());
                           lepton_vis.setP4(j->p4());
                           lepton_vis.setCharge(j->charge());
                           lepton_vis.setPdgId(j->pdgId());
                           havePair = true;
                           break;
                        }
                     }
                  }
               }
            }
         }
      }
      for (auto i = photonsIn->begin(); i != photonsIn->end(); ++i) {
         if (i->pt()>=minpt_photon && std::abs(i->eta())<maxeta_photon) {  
            photon.setP4(i->p4());
            photon.setCharge(i->charge());
            photon.setPdgId(i->pdgId()); 
            havePhoton = true;
            break;
         }
      }
   }

   if (havePhoton) photonsOut->push_back(photon);

   //gendr_tau = gendr_lepton = 9.;
   if (havePair) {
      /*edm::Handle<pat::CompositeCandidateCollection> genVisTaus;
      iEvent.getByToken(genVisTauToken_, genVisTaus);
      for (auto i = genVisTaus->begin(); i != genVisTaus->end(); ++i) {
         const int id = std::abs(i->pdgId());
         if (std::abs(tau_vis.pdgId())==id) { 
            if (reco::deltaR(*i, tau_vis)<gendr_tau) {
               gendr_tau = reco::deltaR(*i, tau_vis);
            }
         }
         if (std::abs(lepton_vis.pdgId())==id) {
            if (reco::deltaR(*i, lepton_vis)<gendr_lepton) {
               gendr_lepton = reco::deltaR(*i, lepton_vis);
            }
         }
      }*/
   
      edm::Handle<std::vector<pat::MET>> met;
      iEvent.getByToken(metToken_, met);
      PolarLorentzVector MET;
      MET.SetPt(met->at(0).pt());
      MET.SetEta(met->at(0).eta());
      MET.SetPhi(met->at(0).phi());
      MET.SetM(met->at(0).mass());

      const double cos0M = cos(dphi(tau_vis.phi(), met->at(0).phi()));
      const double cos1M = cos(dphi(lepton_vis.phi(), met->at(0).phi()));
      const double cos01 = cos(dphi(tau_vis.phi(), lepton_vis.phi()));

      const double nu0mag = MET.pt() * (cos0M-cos1M*cos01) / (1.-cos01*cos01);
      const double nu1mag = (MET.pt()*cos1M) - (nu0mag*cos01);
 
      //It has been verified analytically that the expressions above are the same as the
      //expressions below by trig function manipulation
      //const double nu0mag = METpt*sin(METphi-lepton_vis.phi())/sin(tau_vis.phi()-lepton_vis.phi());
      //const double nu1mag = METpt*sin(METphi-tau_vis.phi())/sin(lepton_vis.phi()-tau_vis.phi());

      reco::LeafCandidate tau_inv = reco::LeafCandidate(0, PolarLorentzVector(0., 0., 0., 0.));
      PolarLorentzVector nu0_v;
      nu0_v.SetEta(tau_vis.eta());
      nu0_v.SetPhi(tau_vis.phi());
      nu0_v.SetPt(std::abs(nu0mag)); // abs hack to avoid nu0mag<0
      nu0_v.SetM(0.);
      tau_inv.setP4(nu0_v);
   
      pat::CompositeCandidate tau_col;
      tau_col.addDaughter(tau_vis);
      tau_col.addDaughter(tau_inv);
      const PolarLorentzVector tau_v = tau_vis.polarP4()+tau_inv.polarP4();
      tau_col.setP4(tau_v);
 
      reco::LeafCandidate lepton_inv = reco::LeafCandidate(0, PolarLorentzVector(0., 0., 0., 0.));
      PolarLorentzVector nu1_v;
      nu1_v.SetEta(lepton_vis.eta());
      nu1_v.SetPhi(lepton_vis.phi());
      nu1_v.SetPt(std::abs(nu1mag)); // abs hack to avoid nu1mag<0
      nu1_v.SetM(0.);
      lepton_inv.setP4(nu1_v);
   
      pat::CompositeCandidate lepton_col;
      lepton_col.addDaughter(lepton_vis);
      lepton_col.addDaughter(lepton_inv);
      const PolarLorentzVector lepton_v = lepton_vis.polarP4()+lepton_inv.polarP4();
      lepton_col.setP4(lepton_v);

      /*const PolarLorentzVector inv = tau_inv.polarP4()+lepton_inv.polarP4();
      std::cout << " *** " << std::endl;
      std::cout << "cos01: "  << cos01 << " cos0M: " << cos0M << " cos1M: " << cos1M << std::endl;
      //std::cout << "tau_inv_mag: "  << tau_inv_mag << " lepton_inv_mag: "  << lepton_inv_mag << std::endl;
      std::cout << "MET: "          << " px: " << met->at(0).px()    << " py: " << met->at(0).py() << " pt: " << met->at(0).pt() << " phi: " << met->at(0).phi()  << std::endl;
      std::cout << "inv: "          << " px: " << inv.px()           << " py: " << inv.py()        << " pt: " << inv.pt()        << " phi: " << inv.phi()         << std::endl;
      std::cout << "tau_vis: "      << " px: " << tau_vis.px()       << " py: " << tau_vis.py()    << " pt: " << tau_vis.pt()    << " phi: " << tau_vis.phi()     << " eta: " << tau_vis.eta() << std::endl;
      std::cout << "tau_inv: "      << " px: " << tau_inv.px()       << " py: " << tau_inv.py()    << " pt: " << tau_inv.pt()    << " phi: " << tau_inv.phi()     << " eta: " << tau_inv.eta() << std::endl;
      std::cout << "tau_col: "      << " px: " << tau_col.px()       << " py: " << tau_col.py()    << " pt: " << tau_col.pt()    << " phi: " << tau_col.phi()     << " eta: " << tau_col.eta() << std::endl;
      std::cout << "lepton_vis: "   << " px: " << lepton_vis.px()    << " py: " << lepton_vis.py() << " pt: " << lepton_vis.pt() << " phi: " << lepton_vis.phi()  << " eta: " << lepton_vis.eta() << std::endl;  
      std::cout << "lepton_inv: "   << " px: " << lepton_inv.px()    << " py: " << lepton_inv.py() << " pt: " << lepton_inv.pt() << " phi: " << lepton_inv.phi()  << " eta: " << lepton_inv.eta() << std::endl;
      std::cout << "lepton_col: "   << " px: " << lepton_col.px()    << " py: " << lepton_col.py() << " pt: " << lepton_col.pt() << " phi: " << lepton_col.phi()  << " eta: " << lepton_col.eta() << std::endl;
      std::cout << " *** " << std::endl;*/

      visibleTaus->push_back(tau_vis);
      visibleTaus->push_back(lepton_vis);
      collinearTaus->push_back(tau_col);
      collinearTaus->push_back(lepton_col);
   } 

   tree->Fill();
   iEvent.put(std::move(photonsOut), std::string("selectedPhoton"));
   iEvent.put(std::move(visibleTaus), std::string("visibleTaus"));
   iEvent.put(std::move(collinearTaus), std::string("collinearTaus"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(LeptonPairProducer);
