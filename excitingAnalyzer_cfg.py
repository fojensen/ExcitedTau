import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
options.register('isMC',
   False,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.bool,
   "Is MC?"
) 
options.register('nevents',
   1,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.int,
   "Number of events in the dataset."
)
options.register('xs',
   1.,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.float,
   "Cross section of the dataset process."
)
options.register('isSignalMC',
   False,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.bool,
   "Is signal MC?")

options.parseArguments()

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'
#process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v19'

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#import FWCore.Utilities.FileUtils as FileUtils
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m250_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m500_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m750_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m1000_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m1500_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m2000_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m3000_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m4000_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m5000_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/DYJetsToLL_M-50_Zpt-150toInf_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#readFiles = cms.untracked.vstring(*mylist)
process.source = cms.Source("PoolSource",
   #fileNames = readFiles
   fileNames = cms.untracked.vstring("/store/mc/RunIIFall15MiniAODv2/Taustar_TauG_L10000_m250_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E4F9CE0A-5BCE-E511-8E00-002590D60026.root")
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("output.root")
)

process.goodPhotons = cms.EDFilter("PhotonProducer",
   photonCollection = cms.InputTag("slimmedPhotons"),
   minpt = cms.double(20.),
   maxeta = cms.double(2.5),
   applyFilter = cms.bool(False),
)

process.goodJets = cms.EDProducer("JetProducer",
   jetCollection = cms.InputTag("slimmedJets"),
   tauCollection = cms.InputTag("goodTaus:goodTaus")
)

process.goodElectrons = cms.EDFilter("ElectronProducer",
   electronCollection = cms.InputTag("slimmedElectrons"),
   applyFilter = cms.bool(False),
   minpt = cms.double(20.),
   maxeta = cms.double(2.5),
)

process.goodVertices = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlineSlimmedPrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"),
   filter = cms.bool(False)
)

process.goodMuons = cms.EDFilter("MuonProducer",
   muonCollection = cms.InputTag("slimmedMuons"),
   vertexCollection = cms.InputTag("goodVertices"),
   minpt = cms.double(20.),
   maxeta = cms.double(2.4),
   applyFilter = cms.bool(False)
)

process.goodTaus = cms.EDFilter("TauProducer",
   tauCollection = cms.InputTag("slimmedTaus"),
   minpt = cms.double(20.),
   maxeta = cms.double(2.3),
   applyFilter = cms.bool(False)
)

process.triggerProducer = cms.EDFilter("TriggerProducer",
   bits = cms.InputTag("TriggerResults", "", "HLT"),
   #prescales = cms.InputTag("patTrigger"),
   #triggers = cms.vstring(
   #   "HLT_Photon200_v",
   #   "HLT_Photon175_v",
   #),
   #applyFilter = cms.bool(False)
)

xsWeight_ = options.xs / options.nevents
process.EventAnalyzer = cms.EDAnalyzer("EventAnalyzer",
   electronCollection = cms.InputTag("goodElectrons:goodElectrons"),
   muonCollection = cms.InputTag("goodMuons:goodMuons"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   vertexCollection = cms.InputTag("goodVertices"),
   metCollection = cms.InputTag("slimmedMETs"),
   jetCollection = cms.InputTag("goodJets:goodJets"),
   xsWeight = cms.double(xsWeight_)
)

### e + tau ###
process.osETauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodElectrons:goodElectrons"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(20.),
   maxeta_lepton = cms.double(2.5),
   minpt_tau = cms.double(20.),
   maxeta_tau = cms.double(2.3),
   minpt_photon = cms.double(20.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False),
   isMC = cms.bool(options.isMC),
)
process.osETauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osETauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osETauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osETauPairProducer:selectedPhoton")
)

### mu + tau ###
process.osMuTauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodMuons:goodMuons"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(20.),
   maxeta_lepton = cms.double(2.4),
   minpt_tau = cms.double(25.),
   maxeta_tau = cms.double(2.1),
   minpt_photon = cms.double(20.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False),   
   isMC = cms.bool(options.isMC),
)
process.osMuTauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osMuTauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osMuTauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osMuTauPairProducer:selectedPhoton")
)

### tau + tau ###
process.osTauTauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodTaus:goodTaus"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   genVisTauCollection = cms.InputTag("genVisTauProducer:genVisTaus"),
   minpt_lepton = cms.double(20.),
   maxeta_lepton = cms.double(2.3),
   minpt_tau = cms.double(20.),
   maxeta_tau = cms.double(2.3),
   minpt_photon = cms.double(20.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   isMC = cms.bool(options.isMC),
)
process.osTauTauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osTauTauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osTauTauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osTauTauPairProducer:selectedPhoton")
)

### REQUIRES GEN INFORMATION
process.TauTauFilter = cms.EDFilter("TauTauFilter",
   genParticleCollection = cms.InputTag("prunedGenParticles")
)
process.genVisTauProducer = cms.EDProducer("GenVisTauProducer",
   genParticleCollection = cms.InputTag("prunedGenParticles")
)
process.genSignalAnalyzer = cms.EDAnalyzer("GenSignalAnalyzer",
   genParticleCollection = cms.InputTag("prunedGenParticles"),
   metCollection = cms.InputTag("slimmedMETs")
)

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(True),
)

mypath = cms.Sequence(
   process.TauTauFilter
   * process.genVisTauProducer
   * process.goodTaus
   * process.goodVertices
   * process.goodElectrons
   * process.goodMuons
   * process.goodPhotons
   * process.goodJets
   * process.triggerProducer
   * process.EventAnalyzer
   * process.osETauPairProducer * process.osETauPairAnalyzer
   * process.osMuTauPairProducer * process.osMuTauPairAnalyzer
   * process.osTauTauPairProducer * process.osTauTauPairAnalyzer
)

if options.isMC:
   mypath *= cms.Sequence(process.genVisTauProducer)
if options.isSignalMC:
   mypath *= cms.Sequence(process.genSignalAnalyzer)

#process.Tracer = cms.Service("Tracer")
process.p = cms.Path(mypath)

