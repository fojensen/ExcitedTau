import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')

options.register('nevents',
   1,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.int,
   "Number of events in the dataset")

options.register('xs',
   1.,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.float,
   "cross section of the dataset process")

options.register('isSignalMC',
   True,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.bool,
   "run gen signal analyzer or not")

options.parseArguments()

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_v12'
#process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v19'

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

import FWCore.Utilities.FileUtils as FileUtils
#mylist = FileUtils.loadListFromFile('./filelists/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m250_13TeV-pythia8.list')
mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m500_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m1000_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/Taustar_TauG_L10000_m2000_13TeV-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/QCD_HT50to100_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/QCD_HT100to200_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/QCD_HT300to500_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/QCD_HT500to700_TuneCP5_13TeV-madgraphMLM-pythia8.list')
#mylist = FileUtils.loadListFromFile('./filelists/ZprimeToTauTau_M-1000_TuneCP5_13TeV-pythia8-tauola.list')
#mylist = FileUtils.loadListFromFile('./filelists/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.list')
readFiles = cms.untracked.vstring(*mylist)
process.source = cms.Source("PoolSource",
   fileNames = readFiles
   #fileNames = cms.untracked.vstring("")
   #fileNames = cms.untracked.vstring("file:4EF56E49-0BD5-E511-B16D-0CC47A57CBBC.root")
)
process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(1000000)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string('./mcsamples/Taustar_m500.root')
)
#process.jetHTAnalyzer = cms.EDAnalyzer("JetHTAnalyzer",
#   genjetCollection = cms.InputTag("slimmedGenJets"),
#   jetCollection = cms.InputTag("slimmedJets"),
#   genParticleCollection = cms.InputTag("prunedGenParticles")
#)

process.goodPhotons = cms.EDFilter("PhotonProducer",
   photonCollection = cms.InputTag("slimmedPhotons"),
   applyFilter = cms.bool(False),
   tauCollection = cms.InputTag("goodTaus:goodTaus")
)

process.goodJets = cms.EDProducer("JetProducer",
   jetCollection = cms.InputTag("slimmedJets"),
   tauCollection = cms.InputTag("goodTaus:goodTaus")
)

#process.goodVertices = cms.EDFilter("VertexSelector",
#   src = cms.InputTag("offlineSlimmedPrimaryVertices"),
#   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"),
#   filter = cms.bool(False)
#)

#process.goodMuons = cms.EDFilter("MuonProducer",
#   muonCollection = cms.InputTag("slimmedMuons"),
#   vertexCollection = cms.InputTag("goodVertices"),
#   applyFilter = cms.bool(False)
#)

#process.muonChannel = cms.EDAnalyzer("ExcitingAnalyzer",
#   leptonCollection = cms.InputTag("goodMuons:goodMuons"),
#   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
#   vertexCollection = cms.InputTag("goodVertices"),
#   metCollection = cms.InputTag("slimmedMETs"),
#)

process.goodTaus = cms.EDFilter("TauProducer",
   tauCollection = cms.InputTag("slimmedTaus"),   
   applyFilter = cms.bool(True)
)

#process.visibleTauChannel = cms.EDAnalyzer("ExcitingAnalyzer",
##   leptonCollection = cms.InputTag("goodTaus:goodTaus"),
#   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
#   vertexCollection = cms.InputTag("goodVertices"),
#   metCollection = cms.InputTag("slimmedMETs"),
#   jetCollection = cms.InputTag("goodJets:goodJets"),
#   xs = cms.double(options.xs),
#   nevents = cms.int32(options.nevents),
#)

process.osTauPair = cms.EDFilter("TauPairProducer",
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   q1q2 = cms.int32(-1),
)

process.ssTauPair = cms.EDFilter("TauPairProducer",
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   q1q2 = cms.int32(1),
)

process.osCollinearTaus = cms.EDProducer("CollinearTauProducer",
   leptonCollection = cms.InputTag("osTauPair:tauPair"),
   metCollection = cms.InputTag("slimmedMETs"),
)

process.ssCollinearTaus = cms.EDProducer("CollinearTauProducer",
   leptonCollection = cms.InputTag("ssTauPair:tauPair"),
   metCollection = cms.InputTag("slimmedMETs"),
)

process.osNTupleMaker = cms.EDAnalyzer("ExcitingAnalyzer",
   leptonCollection = cms.InputTag("osTauPair:tauPair"),
   collinearLeptonCollection = cms.InputTag("osCollinearTaus:collinearTaus"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   #vertexCollection = cms.InputTag("goodVertices"),
   metCollection = cms.InputTag("slimmedMETs"),
   jetCollection = cms.InputTag("goodJets:goodJets"),
   xs = cms.double(options.xs),
   nevents = cms.int32(options.nevents),
   genParticleCollection = cms.InputTag("prunedGenParticles")
)

process.ssNTupleMaker = cms.EDAnalyzer("ExcitingAnalyzer",
   leptonCollection = cms.InputTag("ssTauPair:tauPair"),
   collinearLeptonCollection = cms.InputTag("ssCollinearTaus:collinearTaus"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   #vertexCollection = cms.InputTag("goodVertices"),
   metCollection = cms.InputTag("slimmedMETs"),
   jetCollection = cms.InputTag("goodJets:goodJets"),
   xs = cms.double(options.xs),
   nevents = cms.int32(options.nevents),
   genParticleCollection = cms.InputTag("prunedGenParticles"),
)

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(True),
)

#process.DYFilter = cms.EDFilter("DYFilter",
#   genParticleCollection = cms.InputTag("prunedGenParticles")
#)
process.GenVisTauProducer = cms.EDProducer("GenVisTauProducer",
   genParticleCollection = cms.InputTag("prunedGenParticles")
)

process.GenSignalAnalyzer = cms.EDAnalyzer("GenSignalAnalyzer",
   genParticleCollection = cms.InputTag("prunedGenParticles"),
   genVisTauCollection = cms.InputTag("GenVisTauProducer:genVisTaus"),
   metCollection = cms.InputTag("slimmedMETs")
)

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


mypath = cms.Sequence(
   process.goodTaus *
   process.goodPhotons *
   process.goodJets
   * process.osTauPair * process.osCollinearTaus * process.osNTupleMaker
   #* process.ssTauPair * process.ssCollinearTaus * process.ssNTupleMaker
)
if options.isSignalMC:
   mypath *= cms.Sequence(process.GenVisTauProducer * process.GenSignalAnalyzer)
process.p = cms.Path(mypath)

