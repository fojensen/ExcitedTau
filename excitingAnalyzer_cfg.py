import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
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
   "Is this signal MC?"
)
options.register('applyFilter',
   False,
   VarParsing.VarParsing.multiplicity.singleton,
   VarParsing.VarParsing.varType.bool,
   "Apply skim requiring at least 1 tau?"
)
options.parseArguments()

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v18'
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Geometry.CaloEventSetup.CaloTopology_cfi')
from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
setupEgammaPostRecoSeq(process,era='2018-Prompt')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

#infile = './filelists/Taustar_TauG_L10000_m250_13TeV-pythia8.list'
#outfile = "./mcsamples/Taustar_m250.root"
#infile = './filelists/Taustar_TauG_L10000_m500_13TeV-pythia8.list'
#outfile = "./mcsamples/Taustar_m500.root"
#infile = './filelists/Taustar_TauG_L10000_m1000_13TeV-pythia8.list'
#outfile = "./mcsamples/Taustar_m1000.root"
#infile = './filelists/Taustar_TauG_L10000_m1250_13TeV-pythia8.list'
#outfile = "./mcsamples/Taustar_m1250.root"
#infile = './filelists/Taustar_TauG_L10000_m3000_13TeV-pythia8.list'
#outfile = "./mcsamples/Taustar_m3000.root"
#infile = './filelists/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.list'
#outfile = './mcsamples/DYJetsToLL.root'
#infile = './filelists/DYJetsToLL_M-50_Zpt-150toInf_TuneCP5_13TeV-madgraphMLM-pythia8.list'
#outfile = "./mcsamples/DYJetsToLL_Zpt150.root"

#import FWCore.Utilities.FileUtils as FileUtils
#mylist = FileUtils.loadListFromFile(infile)
#readFiles = cms.untracked.vstring(*mylist)
process.source = cms.Source("PoolSource",
   #fileNames = cms.untracked.vstring("")
   #fileNames = readFiles
   fileNames = cms.untracked.vstring("/store/mc/RunIIFall15MiniAODv2/Taustar_TauG_L10000_m250_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E4F9CE0A-5BCE-E511-8E00-002590D60026.root")
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(-1)
)

process.TFileService = cms.Service("TFileService",
   #fileName = cms.string(outfile)
   fileName = cms.string("output.root")
)

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool(True),
)

updatedTauName = "slimmedTausNewID" #name of pat::Tau collection with new tau-Ids
import RecoTauTag.RecoTau.tools.runTauIdMVA as tauIdConfig
tauIdEmbedder = tauIdConfig.TauIDEmbedder(process, cms, debug = False,
                    updatedTauName = updatedTauName,
                    toKeep = ["deepTau2017v2p1", #deepTau TauIDs
                               ])
tauIdEmbedder.runTauID()

process.goodTaus = cms.EDFilter("TauProducer",
   tauCollection = cms.InputTag(updatedTauName),
   minpt = cms.double(20.),
   maxeta = cms.double(2.3),
   applyFilter = cms.bool(options.applyFilter)
)

process.goodPhotons = cms.EDFilter("PhotonProducer",
   photonCollection = cms.InputTag("slimmedPhotons"),
   minpt = cms.double(50.),
   maxeta = cms.double(2.5),
   applyFilter = cms.bool(False),
)

jetTag = cms.InputTag("slimmedJets")
if options.isSignalMC:
   jetTag = cms.InputTag("selectedPatJetsAK4PFCHS")
process.goodJets = cms.EDProducer("JetProducer",
   jetCollection = jetTag,
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   minpt = cms.double(30.),
   maxeta = cms.double(2.6)
)

process.goodElectrons = cms.EDFilter("ElectronProducer",
   electronCollection = cms.InputTag("slimmedElectrons"),
   minpt = cms.double(24.),
   maxeta = cms.double(2.5),
   applyFilter = cms.bool(False),
)

process.goodVertices = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlineSlimmedPrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) < 24 && position.Rho < 2"),
   filter = cms.bool(False)
)

process.goodMuons = cms.EDFilter("MuonProducer",
   muonCollection = cms.InputTag("slimmedMuons"),
   vertexCollection = cms.InputTag("goodVertices"),
   minpt = cms.double(19.),
   maxeta = cms.double(2.4),
   isSignalMC = cms.bool(options.isSignalMC),
   applyFilter = cms.bool(False)
)

process.triggerProducer = cms.EDFilter("TriggerProducer",
   bits = cms.InputTag("TriggerResults", "", "HLT"),
   #prescales = cms.InputTag("patTrigger"),
   #applyFilter = cms.bool(False)
)

xsWeight_ = options.xs / options.nevents
process.eventAnalyzer = cms.EDAnalyzer("EventAnalyzer",
   electronCollection = cms.InputTag("goodElectrons:goodElectrons"),
   muonCollection = cms.InputTag("goodMuons:goodMuons"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   vertexCollection = cms.InputTag("goodVertices"),
   genParticleCollection = cms.InputTag("prunedGenParticles"),
   metCollection = cms.InputTag("slimmedMETs"),
   jetCollection = cms.InputTag("goodJets:goodJets"),
   xsWeight = cms.double(xsWeight_)
)

### mu + mu ###
process.osMuMuPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodMuons:goodMuons"),
   tauCollection = cms.InputTag("goodMuons:goodMuons"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(27.),
   maxeta_lepton = cms.double(2.4),
   minpt_tau = cms.double(27.),
   maxeta_tau = cms.double(2.4),
   minpt_photon = cms.double(50.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False),
   isMC = cms.bool(options.isSignalMC),
   genVisTauCollection = cms.InputTag("genVisTauProducer:genVisTaus"),
)
process.osMuMuPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osMuMuPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osMuMuPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osMuMuPairProducer:selectedPhoton")
)

### e + e ###
process.osEEPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodElectrons:goodElectrons"),
   tauCollection = cms.InputTag("goodElectrons:goodElectrons"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(35.),
   maxeta_lepton = cms.double(2.5),
   minpt_tau = cms.double(35.),
   maxeta_tau = cms.double(2.5),
   minpt_photon = cms.double(50.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False),
   isMC = cms.bool(options.isSignalMC),
   genVisTauCollection = cms.InputTag("genVisTauProducer:genVisTaus"),
)
process.osEEPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osEEPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osEEPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osEEPairProducer:selectedPhoton")
)

### e + mu ###
process.osEMuPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodMuons:goodMuons"),
   tauCollection = cms.InputTag("goodElectrons:goodElectrons"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(27.),
   maxeta_lepton = cms.double(2.4),
   minpt_tau = cms.double(35.),
   maxeta_tau = cms.double(2.5),
   minpt_photon = cms.double(50.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False),
   isMC = cms.bool(options.isSignalMC),
   genVisTauCollection = cms.InputTag("genVisTauProducer:genVisTaus"),
)
process.osEMuPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osEMuPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osEMuPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osEMuPairProducer:selectedPhoton")
)

### mu + tau ###
process.osMuTauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodMuons:goodMuons"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(27.),
   maxeta_lepton = cms.double(2.4),
   minpt_tau = cms.double(20.),
   maxeta_tau = cms.double(2.3),
   minpt_photon = cms.double(50.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False),   
   isMC = cms.bool(options.isSignalMC),
   genVisTauCollection = cms.InputTag("genVisTauProducer:genVisTaus"),
)
process.osMuTauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osMuTauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osMuTauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osMuTauPairProducer:selectedPhoton")
)

### e + tau ###
process.osETauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodElectrons:goodElectrons"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(35.),
   maxeta_lepton = cms.double(2.5),
   minpt_tau = cms.double(20.),
   maxeta_tau = cms.double(2.3),
   minpt_photon = cms.double(50.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   applyFilter = cms.bool(False),
   isMC = cms.bool(options.isSignalMC),
   genVisTauCollection = cms.InputTag("genVisTauProducer:genVisTaus"),
)
process.osETauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osETauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osETauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osETauPairProducer:selectedPhoton")
)

### tau + tau ###
process.osTauTauPairProducer = cms.EDProducer("LeptonPairProducer",
   leptonCollection = cms.InputTag("goodTaus:goodTaus"),
   tauCollection = cms.InputTag("goodTaus:goodTaus"),
   metCollection = cms.InputTag("slimmedMETs"),
   photonCollection = cms.InputTag("goodPhotons:goodPhotons"),
   minpt_lepton = cms.double(20.),
   maxeta_lepton = cms.double(2.3),
   minpt_tau = cms.double(180.),
   maxeta_tau = cms.double(2.1),
   minpt_photon = cms.double(50.),
   maxeta_photon = cms.double(2.5),
   q1q2 = cms.int32(-1),
   isMC = cms.bool(options.isSignalMC),
   genVisTauCollection = cms.InputTag("genVisTauProducer:genVisTaus"),
)
process.osTauTauPairAnalyzer = cms.EDAnalyzer("LeptonPairAnalyzer",
   visibleTauCollection = cms.InputTag("osTauTauPairProducer:visibleTaus"),
   collinearTauCollection = cms.InputTag("osTauTauPairProducer:collinearTaus"),
   photonCollection = cms.InputTag("osTauTauPairProducer:selectedPhoton")
)

#process.TauTauFilter = cms.EDFilter("TauTauFilter",
#   genParticleCollection = cms.InputTag("prunedGenParticles")
#)
process.genVisTauProducer = cms.EDProducer("GenVisTauProducer",
   genParticleCollection = cms.InputTag("prunedGenParticles")
)
process.genSignalAnalyzer = cms.EDAnalyzer("GenSignalAnalyzer",
   genParticleCollection = cms.InputTag("prunedGenParticles"),
   metCollection = cms.InputTag("slimmedMETs")
)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printTree = cms.EDAnalyzer("ParticleTreeDrawer",
   src = cms.InputTag("prunedGenParticles"),
   printP4 = cms.untracked.bool(False),
   printPtEtaPhi = cms.untracked.bool(True),
   printVertex = cms.untracked.bool(False),
   printStatus = cms.untracked.bool(True),
   printIndex = cms.untracked.bool(False),
   #status = cms.untracked.vint32(3),
)

mypath = cms.Sequence(
   process.rerunMvaIsolationSequence * getattr(process,updatedTauName)
   * process.goodTaus
   * process.egammaPostRecoSeq
   * process.goodPhotons
   * process.goodVertices
   * process.goodElectrons
   * process.goodMuons
   * process.goodJets
   * process.triggerProducer
   * process.eventAnalyzer
   * process.genVisTauProducer
   * process.osMuMuPairProducer * process.osMuMuPairAnalyzer
   * process.osEEPairProducer * process.osEEPairAnalyzer
   * process.osEMuPairProducer * process.osEMuPairAnalyzer
   * process.osMuTauPairProducer * process.osMuTauPairAnalyzer
   * process.osETauPairProducer * process.osETauPairAnalyzer
   * process.osTauTauPairProducer * process.osTauTauPairAnalyzer
   #* process.printTree
)

if options.isSignalMC:
   #https://twiki.cern.ch/CMS/JetToolbox
   from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox
   jetToolbox(
      process, #process
      'ak4', #jetType
      'jetSequence', #sequence
      'noOutput', #outputModules
      JETCorrPayload='AK4PFchs',
      bTagDiscriminators = ['pfDeepCSVJetTags:probb', 'pfDeepCSVJetTags:probbb']
   )
   mypath = process.genSignalAnalyzer * process.jetSequence * mypath

#process.Tracer = cms.Service("Tracer")

process.p = cms.Path(mypath)

