import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
      '/store/mc/RunIIFall15MiniAODv2/Taustar_TauG_L10000_m1000_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/CA54CCDC-7DCD-E511-B5CB-00266CF250C4.root'
   )
)

process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(10)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

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

process.p = cms.Path(
   process.printTree
)

