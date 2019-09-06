import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')

options.register('nevents',
   1,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.int,
   "Number of events in the dataset.")

options.register('xs',
   1.,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.float,
   "Cross section of the dataset process.")

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
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

#import FWCore.Utilities.FileUtils as FileUtils
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m250_13TeV-pythia8.list')
#readFiles = cms.untracked.vstring(*mylist)
process.source = cms.Source("PoolSource",
   #fileNames = readFiles
   fileNames = cms.untracked.vstring("/store/mc/RunIIFall15MiniAODv2/Taustar_TauG_L10000_m250_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E4F9CE0A-5BCE-E511-8E00-002590D60026.root")
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string('output.root')
)

process.goodPhotons = cms.EDFilter("PhotonProducer",
   photonCollection = cms.InputTag("slimmedPhotons"),
   applyFilter = cms.bool(False),
)

process.goodJets = cms.EDProducer("JetProducer",
   jetCollection = cms.InputTag("slimmedJets"),
   tauCollection = cms.InputTag("goodTaus:goodTaus")
)

process.goodElectrons = cms.EDFilter("ElectronProducer",
   electronCollection = cms.InputTag("slimmedElectrons"),
   applyFilter = cms.bool(False)
)

process.goodVertices = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlineSlimmedPrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"),
   filter = cms.bool(False)
)

process.goodMuons = cms.EDFilter("MuonProducer",
   muonCollection = cms.InputTag("slimmedMuons"),
   vertexCollection = cms.InputTag("goodVertices"),
   applyFilter = cms.bool(False)
)

process.goodTaus = cms.EDFilter("TauProducer",
   tauCollection = cms.InputTag("slimmedTaus"),
   applyFilter = cms.bool(False)
)

process.triggers = cms.EDFilter("TriggerProducer",
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
   minpt_lepton = cms.double(24.),
   maxeta_lepton = cms.double(2.1),
   minpt_tau = cms.double(30.),
   maxeta_tau = cms.double(2.1),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False)
)
process.osETauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osETauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osETauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons")
)

### mu + tau ###
process.osMuTauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodMuons:goodMuons"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   minpt_lepton = cms.double(20.),
   maxeta_lepton = cms.double(2.1),
   minpt_tau = cms.double(27.),
   maxeta_tau = cms.double(2.1),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False)
)
process.osMuTauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osMuTauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osMuTauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons")
)

### tau + tau ###
process.osTauTauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodTaus:goodTaus"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   minpt_lepton = cms.double(35.),
   maxeta_lepton = cms.double(2.1),
   minpt_tau = cms.double(35.),
   maxeta_tau = cms.double(2.1),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False)
)
process.osTauTauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osTauTauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osTauTauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons")
)

### REQUIRES GEN INFORMATION
#process.TauTauFilter = cms.EDFilter("TauTauFilter",
#   genParticleCollection = cms.InputTag("prunedGenParticles")
#)
#process.GenVisTauProducer = cms.EDProducer("GenVisTauProducer",
#   genParticleCollection = cms.InputTag("prunedGenParticles")
#)
process.GenSignalAnalyzer = cms.EDAnalyzer("GenSignalAnalyzer",
   genParticleCollection = cms.InputTag("prunedGenParticles"),
   metCollection = cms.InputTag("slimmedMETs")
)
#process.decayMode = cms.EDAnalyzer("DecayModeAnalyzer",
#   genParticleCollection = cms.InputTag("prunedGenParticles"),
#)
#process.decayMode_selection = cms.EDAnalyzer("DecayModeAnalyzer", 
#   genParticleCollection = cms.InputTag("prunedGenParticles"),
#)

#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.printTree = cms.EDAnalyzer("ParticleTreeDrawer",
#   src = cms.InputTag("prunedGenParticles"),
#   printP4 = cms.untracked.bool(False),
#   printPtEtaPhi = cms.untracked.bool(False),
#   printVertex = cms.untracked.bool(False),
#   printStatus = cms.untracked.bool(True),
#   printIndex = cms.untracked.bool(False),
   #status = cms.untracked.vint32(3),
#)

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(True),
)

mypath = cms.Sequence(
   #process.TauTauFilter
   process.goodTaus
   * process.goodElectrons
   * process.goodVertices
   * process.goodMuons
   * process.goodPhotons
   * process.goodJets
   * process.triggers
   * process.EventAnalyzer
   * process.osETauPairProducer * process.osETauPairAnalyzer
   * process.osMuTauPairProducer * process.osMuTauPairAnalyzer
   * process.osTauTauPairProducer * process.osTauTauPairAnalyzer
)
if options.isSignalMC:
   mypath *= cms.Sequence(process.GenSignalAnalyzer)

process.p = cms.Path(mypath)

