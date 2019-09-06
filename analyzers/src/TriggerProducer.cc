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
      //std::vector<std::string> goodTriggers;
      edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
      //edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
      int HLT_Photon200_v, HLT_Photon175_v;
      TTree * tree;
};

TriggerProducer::TriggerProducer(const edm::ParameterSet& iConfig)
{
   //applyFilter = iConfig.getParameter<bool>("applyFilter");
   //goodTriggers = iConfig.getParameter<std::vector<std::string>>("triggers");
   triggerBits_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"));
   //triggerPrescales_ = consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"));
   edm::Service<TFileService> fs; 
   tree = fs->make<TTree>("tree", "tree");
   tree->Branch("HLT_Photon200_v", &HLT_Photon200_v, "HLT_Photon200_v/I");
   tree->Branch("HLT_Photon175_v", &HLT_Photon175_v, "HLT_Photon175_v/I");
}

bool TriggerProducer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<edm::TriggerResults> triggerBits;
   iEvent.getByToken(triggerBits_, triggerBits);
   const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);

   //edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
   //iEvent.getByToken(triggerPrescales_, triggerPrescales);

   HLT_Photon200_v = -1;
   HLT_Photon175_v = -1; 

//   for (auto i = goodTriggers.begin(); i != goodTriggers.end(); ++i) {
  //    std::string trig1 = *i;
    //  std::cout << "looking for trigger: " << trig1 << std::endl;
      for (unsigned int j = 0, n = triggerBits->size(); j < n; ++j) {
         std::string trig2 = names.triggerName(j);
         if (trig2.find("HLT_Photon200_v")!=std::string::npos) HLT_Photon200_v = triggerBits->accept(j);
         if (trig2.find("HLT_Photon175_v")!=std::string::npos) HLT_Photon175_v = triggerBits->accept(j);
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
