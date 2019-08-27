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
#include "DataFormats/Math/interface/LorentzVector.h"
typedef math::PtEtaPhiMLorentzVectorD PolarLorentzVector;
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include <TH2D.h>
#include "DataFormats/Math/interface/deltaR.h"
#include <TVector2.h>

double dphi(const double phi1, const double phi2)
{
   return phi2-phi1;
}

class CollinearTauProducer : public edm::stream::EDProducer<> {
   public:
      explicit CollinearTauProducer(const edm::ParameterSet&);
   private:
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      edm::EDGetTokenT<std::vector<pat::Tau>> tauToken;
      edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
      TH1D *h_dr_METGenMET, *h_dp_METGenMET;
      TH1D *h_dr_InvGenMET, *h_dp_InvGenMET;
      TH1D *h_dr_InvMET, *h_dp_InvMET;
      TH2D *h_leadpt, *h_subleadpt;
      TH1D *h_dr_ll, *h_dr_lvislvis, *h_dr_nunu;
      TH1D *h_dr_lead, *h_dr_sublead;
};

CollinearTauProducer::CollinearTauProducer(const edm::ParameterSet& iConfig)
{
   produces<pat::CompositeCandidateCollection>("collinearTaus");
   tauToken = consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("leptonCollection")); 
   metToken_ = consumes<std::vector<pat::MET>>(iConfig.getParameter<edm::InputTag>("metCollection")); 
  
   edm::Service<TFileService> fs;
   h_dr_METGenMET = fs->make<TH1D>("h_dr_METGenMET", ";#DeltaR(gen MET, reco MET);events / 0.2", 20, 0., 4.);
   h_dp_METGenMET = fs->make<TH1D>("h_dp_METGenMET", ";#Delta#phi(gen MET, reco MET);events / 0.2", 20, 0., 4.);
   h_dr_InvGenMET = fs->make<TH1D>("h_dr_InvGenMET", ";#DeltaR(gen MET, reco #nu#nu);events / 0.2", 20, 0., 4.);
   h_dp_InvGenMET = fs->make<TH1D>("h_dp_InvGenMET", ";#Delta#phi(gen MET, reco #nu#nu);events / 0.2", 20, 0., 4.);
   h_dr_InvMET = fs->make<TH1D>("h_dr_InvMET", ";#DeltaR(MET, reco #nu#nu);events / 0.2", 20, 0., 4.);
   h_dp_InvMET = fs->make<TH1D>("h_dp_InvMET", ";#Delta#phi(MET, reco #nu#nu);events / 0.2", 20, 0., 4.);
   h_leadpt = fs->make<TH2D>("h_leadpt", ";leading #tau_{vis} p_{T} [GeV];#tau_{inv} p_{T} [GeV]", 10, 0., 200., 10, 0., 200.);
   h_subleadpt = fs->make<TH2D>("h_subleadpt", ";subleading #tau_{vis} p_{T} [GeV];#tau_{inv} p_{T} [GeV]", 10, 0., 200., 10, 0., 200.);
   h_dr_ll = fs->make<TH1D>("h_dr_ll", ";#DeltaR(#ell, #ell);events / 0.2", 20, 0., 4.);
   h_dr_lvislvis = fs->make<TH1D>("h_dr_lvislvis", ";#DeltaR(#ell, #ell);events / 0.2", 20, 0., 4.);
   h_dr_nunu = fs->make<TH1D>("h_dr_nunu", ";#DeltaR(#nu, #nu);events / 0.2", 20, 0., 4.);
   h_dr_lead = fs->make<TH1D>("h_dr_lead", ";#DeltaR();events / 0.1", 20, 0., 2.);
   h_dr_sublead = fs->make<TH1D>("h_dr_sublead", ";#DeltaR();events / 0.1", 20, 0., 2.);
}

void CollinearTauProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   auto collinearTaus = std::make_unique<pat::CompositeCandidateCollection>();

   edm::Handle<std::vector<pat::Tau>> taus;
   iEvent.getByToken(tauToken, taus);

   if (taus->size()>=2) {

   edm::Handle<std::vector<pat::MET>> met;
   iEvent.getByToken(metToken_, met);
   PolarLorentzVector MET;
   MET.SetPt(met->at(0).pt());
   MET.SetEta(met->at(0).eta());
   MET.SetPhi(met->at(0).phi());
   MET.SetM(met->at(0).mass());

   const double cos0M = cos(dphi(taus->at(0).phi(), met->at(0).phi()));
   const double cos1M = cos(dphi(taus->at(1).phi(), met->at(0).phi()));
   const double cos01 = cos(dphi(taus->at(0).phi(), taus->at(1).phi()));
   const double nu0mag = MET.pt() * (cos0M-cos1M*cos01) / (1.-cos01*cos01);
   const double nu1mag = (MET.pt()*cos1M) - (nu0mag*cos01);
   

   const reco::LeafCandidate * tau0_vis = dynamic_cast<reco::LeafCandidate*>(taus->at(0).clone());
   
   reco::LeafCandidate tau0_inv = reco::LeafCandidate(0, PolarLorentzVector(0., 0., 0., 0.));
   PolarLorentzVector nu0_v;
   nu0_v.SetEta(tau0_vis->eta());
   nu0_v.SetPhi(tau0_vis->phi());
   nu0_v.SetPt(std::abs(nu0mag)); // abs hack to avoid nu0mag<0
   nu0_v.SetM(0.);
   tau0_inv.setP4(nu0_v);
   
   pat::CompositeCandidate tau0; 
   tau0.addDaughter(*tau0_vis);
   tau0.addDaughter(tau0_inv);
   const PolarLorentzVector tau0_v = tau0_vis->polarP4()+tau0_inv.polarP4();
   tau0.setP4(tau0_v);

   const reco::LeafCandidate * tau1_vis = dynamic_cast<reco::LeafCandidate*>(taus->at(1).clone());
   
   reco::LeafCandidate tau1_inv = reco::LeafCandidate(0, PolarLorentzVector(0., 0., 0., 0.));
   PolarLorentzVector nu1_v;
   nu1_v.SetEta(tau1_vis->eta());
   nu1_v.SetPhi(tau1_vis->phi());
   nu1_v.SetPt(std::abs(nu1mag)); // abs hack to avoid nu1mag<0
   nu1_v.SetM(0.);
   tau1_inv.setP4(nu1_v);
   
   pat::CompositeCandidate tau1;
   tau1.addDaughter(*tau1_vis);
   tau1.addDaughter(tau1_inv);
   const PolarLorentzVector tau1_v = tau1_vis->polarP4()+tau1_inv.polarP4();
   tau1.setP4(tau1_v);
  
   if (tau0.pt()>=tau1.pt()) {
      collinearTaus->push_back(tau0);
      collinearTaus->push_back(tau1);
      h_leadpt->Fill(tau0_vis->pt(), tau0_inv.pt());
      h_subleadpt->Fill(tau1_vis->pt(), tau1_inv.pt());
      h_dr_lead->Fill(reco::deltaR(*tau0_vis, tau0_inv));
      h_dr_sublead->Fill(reco::deltaR(*tau1_vis, tau1_inv));
   } else {
      collinearTaus->push_back(tau1);
      collinearTaus->push_back(tau0);
      h_leadpt->Fill(tau1_vis->pt(), tau1_inv.pt());
      h_subleadpt->Fill(tau0_vis->pt(), tau0_inv.pt());
      h_dr_lead->Fill(reco::deltaR(*tau1_vis, tau1_inv));
      h_dr_sublead->Fill(reco::deltaR(*tau0_vis, tau0_inv));
   }

   const PolarLorentzVector inv = nu0_v+nu1_v;
   PolarLorentzVector genMET;
   genMET.SetPt(met->at(0).genMET()->pt());
   genMET.SetEta(met->at(0).genMET()->eta());
   genMET.SetPhi(met->at(0).genMET()->phi());
   genMET.SetM(met->at(0).genMET()->mass());
   h_dr_METGenMET->Fill(reco::deltaR(genMET, MET));
   h_dp_METGenMET->Fill(genMET.phi()-MET.phi());
   h_dr_InvGenMET->Fill(reco::deltaR(genMET, inv));
   h_dp_InvGenMET->Fill(genMET.phi()-inv.phi());
   h_dr_InvMET->Fill(reco::deltaR(MET, inv));
   h_dp_InvMET->Fill(MET.phi()-inv.phi());  
   h_dr_ll->Fill(reco::deltaR(tau0, tau1));
   h_dr_lvislvis->Fill(reco::deltaR(*tau0_vis, *tau1_vis));
   h_dr_nunu->Fill(reco::deltaR(tau0_inv, tau1_inv));

   /*if (nu0mag<0. || nu1mag<0.) {
      std::cout << "*** funny event ***" << std::endl;
      std::cout << "nu0 pt: " << nu0_v.pt() << " px: " << nu0_v.px() << " py: " << nu0_v.py() << " pz: " << nu0_v.pz() << std::endl;
      std::cout << "nu1 pt : " << nu1_v.pt() << " px: " << nu1_v.px() << " py: " << nu1_v.py() << " pz: " << nu1_v.pz() << std::endl;
      std::cout << "tauvis0 pt: " << taus->at(0).pt() << " px: " << taus->at(0).px() << " py: " << taus->at(0).py() << " pz: " << taus->at(0).pz() << std::endl;
      std::cout << "tauvis1 pt: " << taus->at(1).pt() << " px: " << taus->at(1).px() << " py: " << taus->at(1).py() <<  " pz: " << taus->at(1).pz() << std::endl;
      std::cout << "met pt : " << met->at(0).pt() << " px: " << met->at(0).px()  << " py: " << met->at(0).py() <<  " pz: " << met->at(0).pz() << std::endl;
      std::cout << "" << std::endl;
   }*/

   }

   iEvent.put(std::move(collinearTaus), std::string("collinearTaus"));
}

//define this as a plug-in
DEFINE_FWK_MODULE(CollinearTauProducer);
