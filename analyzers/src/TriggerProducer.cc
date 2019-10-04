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
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
//#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include <TTree.h>
#include <iostream>

class TriggerProducer : public edm::stream::EDFilter<> {
   public:
      explicit TriggerProducer(const edm::ParameterSet&);
   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      bool applyFilter;
      edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
      //edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
      //2018
      int HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v;
      int HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v;
      int HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v;
      int HLT_Photon200_v;
      int HLT_IsoMu24_v;
      int HLT_IsoTkMu24_v;
      int HLT_Mu50_v;
      int HLT_TkMu50_v;
      int HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v;
      int HLT_Ele32_WPTight_Gsf_v;
      int HLT_Ele115_CaloIdVT_GsfTrkIdT_v;
      int HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v;
      //2017
      int HLT_Photon175_v;
      TTree * tree;
};

TriggerProducer::TriggerProducer(const edm::ParameterSet& iConfig)
{
   triggerBits_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"));
   //triggerPrescales_ = consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"));
   edm::Service<TFileService> fs; 
   tree = fs->make<TTree>("tree", "tree");
   //2018
   tree->Branch(
      "HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v",
      &HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v, 
      "HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v/I"
   );
   tree->Branch(
      "HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v",
      &HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v,
      "HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v/I"
   );
   tree->Branch(
      "HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v",
      &HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v,
      "HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v/I"
   );
   tree->Branch("HLT_Photon200_v", &HLT_Photon200_v, "HLT_Photon200_v/I");
   tree->Branch("HLT_IsoMu24_v", &HLT_IsoMu24_v, "HLT_IsoMu24_v/I");
   tree->Branch("HLT_IsoTkMu24_v", &HLT_IsoTkMu24_v, "HLT_IsoTkMu24_v/I");
   tree->Branch("HLT_Mu50_v", &HLT_Mu50_v, "HLT_Mu50_v/I");
   tree->Branch("HLT_TkMu50_v", &HLT_TkMu50_v, "HLT_TkMu50_v/I");
   tree->Branch(
      "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v",
      &HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v,
      "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v/I"
   );
   tree->Branch("HLT_Ele32_WPTight_Gsf_v", &HLT_Ele32_WPTight_Gsf_v, "HLT_Ele32_WPTight_Gsf_v/I");
   tree->Branch("HLT_Ele115_CaloIdVT_GsfTrkIdT_v", &HLT_Ele115_CaloIdVT_GsfTrkIdT_v, "HLT_Ele115_CaloIdVT_GsfTrkIdT_v/I");
   tree->Branch("HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v", &HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v, "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v/I");

   //2017
   tree->Branch("HLT_Photon175_v", &HLT_Photon175_v, "HLT_Photon175_v/I");
}

bool TriggerProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<edm::TriggerResults> triggerBits;
   iEvent.getByToken(triggerBits_, triggerBits);
   const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);

   //edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
   //iEvent.getByToken(triggerPrescales_, triggerPrescales);

   //2018
   HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v = -1;
   HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v = -1;
   HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v = -1;
   HLT_Photon200_v = -1;
   HLT_IsoMu24_v = HLT_IsoTkMu24_v = HLT_Mu50_v = HLT_TkMu50_v = -1;
   HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v = -1;
   HLT_Ele32_WPTight_Gsf_v = HLT_Ele115_CaloIdVT_GsfTrkIdT_v = HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v = -1;
   //2017
   HLT_Photon175_v = -1;

   for (unsigned int j = 0, n = triggerBits->size(); j < n; ++j) {
      std::string trig2 = names.triggerName(j);
      //std::cout << "trigger " << j << ": " << trig2 << std::endl;
      //2018
      if (trig2.find("HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v")!=std::string::npos) {
                      HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v")!=std::string::npos) {
                      HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v")!=std::string::npos) {
                      HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_Photon200_v")!=std::string::npos) {
                      HLT_Photon200_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_IsoMu24_v")!=std::string::npos) {
                      HLT_IsoMu24_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_IsoTkMu24_v")!=std::string::npos) {
                      HLT_IsoTkMu24_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_Mu50_v")!=std::string::npos) {
                      HLT_Mu50_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_TkMu50_v")!=std::string::npos) {
                      HLT_TkMu50_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v")!=std::string::npos) {
                      HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_Ele32_WPTight_Gsf_v")!=std::string::npos) {
                      HLT_Ele32_WPTight_Gsf_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_Ele115_CaloIdVT_GsfTrkIdT_v")!=std::string::npos) {
                      HLT_Ele115_CaloIdVT_GsfTrkIdT_v = triggerBits->accept(j);
      }
      if (trig2.find("HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v")!=std::string::npos) {
                      HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v = triggerBits->accept(j);
      }
      //2017
      if (trig2.find("HLT_Photon175_v")!=std::string::npos) {
                      HLT_Photon175_v = triggerBits->accept(j);
      }
      //   if (trig2.find(trig1)!=std::string::npos) {
        //    std::cout << "found trigger:" << names.triggerName(j) << "; prescale: " << triggerPrescales->getPrescaleForIndex(j) << "; accept: " << triggerBits->accept(j) << std::endl;
         //   break;
         }
      //}
   //}
   tree->Fill();
   return true;
}

DEFINE_FWK_MODULE(TriggerProducer);
