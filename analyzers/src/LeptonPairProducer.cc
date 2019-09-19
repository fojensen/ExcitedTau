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
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/Candidate/interface/Candidate.h"

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
      edm::EDGetTokenT<std::vector<pat::Photon>> photonToken_;
      int q1q2;
      double maxeta_lepton, minpt_lepton;
      double maxeta_tau, minpt_tau;
      double maxeta_photon, minpt_photon;
      TTree * tree;
      bool haveTriplet, havePair, havePhoton;
};

LeptonPairProducer::LeptonPairProducer(const edm::ParameterSet& iConfig)
{
   produces<std::vector<pat::PackedCandidate>>("visibleTaus");
   produces<std::vector<pat::PackedCandidate>>("collinearTaus");
   produces<std::vector<pat::PackedCandidate>>("selectedPhoton");
   
   tauToken_ = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("tauCollection"));
   leptonToken_ = consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonCollection")); 
   metToken_ = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection")); 
   photonToken_ = consumes<std::vector<pat::Photon>>(iConfig.getParameter<edm::InputTag>("photonCollection"));
   
   minpt_lepton = iConfig.getParameter<double>("minpt_lepton");
   maxeta_lepton = iConfig.getParameter<double>("maxeta_lepton");
   maxeta_tau = iConfig.getParameter<double>("maxeta_tau");
   minpt_tau = iConfig.getParameter<double>("minpt_tau");
   maxeta_photon = iConfig.getParameter<double>("maxeta_photon");
   minpt_photon = iConfig.getParameter<double>("minpt_photon");
   q1q2 = iConfig.getParameter<int>("q1q2");
   
   edm::Service<TFileService> fs;
   tree = fs->make<TTree>("tree", "tree");   
   tree->Branch("haveTriplet", &haveTriplet, "haveTriplet/O");
   tree->Branch("havePair", &havePair, "havePair/O");
   tree->Branch("havePhoton", &havePhoton, "havePhoton/O");
}

void LeptonPairProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto visibleTaus = std::make_unique<std::vector<pat::PackedCandidate>>();
   auto collinearTaus = std::make_unique<std::vector<pat::PackedCandidate>>();
   auto outPhotons = std::make_unique<std::vector<pat::PackedCandidate>>();

   haveTriplet = havePair = havePhoton = false;

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken_, taus);

   edm::Handle<edm::View<reco::Candidate>> leptons;
   iEvent.getByToken(leptonToken_, leptons);
 
   edm::Handle<std::vector<pat::Photon>> inPhotons;
   iEvent.getByToken(photonToken_, inPhotons);
 
//   PolarLorentzVector p4(0., 0., 0., 0); //v.SetPt(0.); v.SetEta(0.); v.SetPhi(0.); v.SetM(0.);
   
   pat::PackedCandidate tau_vis;//(p4, taus->at(0).vertex(), 0,0,0,0,0, taus->at(0).vertexRef());
   pat::PackedCandidate tau_col;
   pat::PackedCandidate lepton_vis;
   pat::PackedCandidate lepton_col;
   pat::PackedCandidate thePhoton;
   
//   thePhoton.setP4(p4);
  // lepton_col.setP4(p4);
  // tau_col.setP4(p4);
  // tau_vis.setP4(p4);
  // lepton_vis.setP4(p4);

   for (auto i = taus->begin(); i != taus->end(); ++i) {
      if (i->pt()>=minpt_tau && std::abs(i->eta())<maxeta_tau) {
         for (auto j = leptons->begin(); j != leptons->end(); ++j) {
            if (j->pt()>=minpt_lepton && std::abs(j->eta())<maxeta_lepton) {
               if (i->charge()*j->charge()==q1q2) {
                  for (auto k = inPhotons->begin(); k != inPhotons->end(); ++k) {
                     if (k->pt()>=minpt_photon && std::abs(k->eta())<maxeta_photon) {
                        if (reco::deltaR(i->p4(),j->p4())>=0.4 && reco::deltaR(i->p4(),k->p4())>=0.4 && reco::deltaR(j->p4(),k->p4())>=0.4) {
                           tau_vis.setP4(i->p4());
                           tau_vis.setCharge(i->charge());
                           tau_vis.setPdgId(i->pdgId());
                           lepton_vis.setP4(j->p4());
                           lepton_vis.setCharge(j->charge());
                           lepton_vis.setPdgId(j->pdgId());
                           thePhoton.setP4(k->p4());
                           thePhoton.setCharge(k->charge());
                           thePhoton.setPdgId(k->pdgId());
                           //tau_vis = dynamic_cast<pat::PackedCandidate*>(i->clone());
                           //lepton_vis = dynamic_cast<pat::PackedCandidate*>(j->clone());
                           //thePhoton = i->clone();
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
   if (!haveTriplet) {
      for (auto i = taus->begin(); i != taus->end(); ++i) {
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
                        //tau_vis = dynamic_cast<pat::PackedCandidate*>(i->clone());
                        //lepton_vis = dynamic_cast<pat::PackedCandidate*>(j->clone());
                        havePair = true;
                        break;
                     }
                  }
               }
            }
         }
      }
      for (auto i = inPhotons->begin(); i != inPhotons->end(); ++i) {
         if (i->pt()>=minpt_photon && std::abs(i->eta())<maxeta_photon) {  
            thePhoton.setP4(i->p4());
            thePhoton.setCharge(i->charge());
            thePhoton.setPdgId(i->pdgId()); 
            //thePhoton = i->clone();        
            havePhoton = true;
            break;
         }
      }
   }

   if (havePair) {
      edm::Handle<std::vector<pat::MET>> met;
      iEvent.getByToken(metToken_, met);
      const double METpt = met->at(0).pt();
      const double METphi = met->at(0).phi();

      const double cos0M = cos(dphi(tau_vis.phi(), METphi));
      const double cos1M = cos(dphi(lepton_vis.phi(), METphi));
      const double cos01 = cos(dphi(tau_vis.phi(), lepton_vis.phi()));
      double nu0mag = METpt * (cos0M-cos1M*cos01) / (1.-cos01*cos01);
      double nu1mag = (METpt*cos1M) - (nu0mag*cos01);
   
      double nu0mag_check = METpt*sin(METphi-lepton_vis.phi())/sin(tau_vis.phi()-lepton_vis.phi());
      double nu1mag_check = METpt*sin(METphi-tau_vis.phi())/sin(lepton_vis.phi()-tau_vis.phi());

      //It has been verified analytically that these methods give the same answer, so the next two lines do nothing
      if (nu0mag_check != nu0mag) nu0mag = nu0mag_check;
      if (nu1mag_check != nu1mag) nu1mag = nu1mag_check;

      PolarLorentzVector nu0;
      nu0.SetEta(tau_vis.eta());
      nu0.SetPhi(tau_vis.phi());
      nu0.SetPt(std::abs(nu0mag)); // abs hack to avoid nu0mag<0
      nu0.SetM(0.);

      tau_col.setP4(tau_vis.p4()+nu0);
      tau_col.setPdgId(tau_vis.pdgId());
      tau_col.setCharge(tau_vis.charge());
 
      PolarLorentzVector nu1;
      nu1.SetEta(lepton_vis.eta());
      nu1.SetPhi(lepton_vis.phi());
      nu1.SetPt(std::abs(nu1mag)); // abs hack to avoid nu1mag<0
      nu1.SetM(0.);
   
      lepton_col.setP4(lepton_vis.p4()+nu1);
      lepton_col.setPdgId(lepton_vis.pdgId());
      lepton_col.setCharge(lepton_vis.charge());
   }   
   if (havePair) {
      visibleTaus->push_back(tau_vis);
      visibleTaus->push_back(lepton_vis);
      collinearTaus->push_back(tau_col);
      collinearTaus->push_back(lepton_col);
   }
   if (havePhoton) {
      outPhotons->push_back(thePhoton);
   }

   tree->Fill();
   iEvent.put(std::move(outPhotons), std::string("selectedPhoton"));
   iEvent.put(std::move(visibleTaus), std::string("visibleTaus"));
   iEvent.put(std::move(collinearTaus), std::string("collinearTaus"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(LeptonPairProducer);
