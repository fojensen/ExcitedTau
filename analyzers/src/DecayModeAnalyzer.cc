// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// new includes
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

class DecayModeAnalyzer : public edm::EDAnalyzer {
public:
	explicit DecayModeAnalyzer(const edm::ParameterSet&);
private:
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_;
   TH1I * h_decayMode;
};

DecayModeAnalyzer::DecayModeAnalyzer(const edm::ParameterSet& iConfig)
{
   genParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));
   edm::Service<TFileService> fs;
   h_decayMode = fs->make<TH1I>("h_decayMode", ";decay mode;events / bin", 7, -0.5, 6.5);
   const TString xlabels[6] = {"#mu+#mu", "e+e", "e+#mu", "#mu+#tau_{had}", "e+#tau_{had}", "#tau_{had}+#tau_{had}"};
   for (int i = 0; i < 6; ++i) h_decayMode->GetXaxis()->SetBinLabel(i+2, xlabels[i]);
}

void DecayModeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<std::vector<reco::GenParticle>> genParticles;
   iEvent.getByToken(genParticleToken_, genParticles); 
   int dm = 0;
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

   h_decayMode->Fill(dm);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DecayModeAnalyzer);

