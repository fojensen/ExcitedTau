# ExcitedTau
the search for excited tau leptons...

## To install and run on a test file:

# First install packages for EGAMMA MVA ID
These instructions are pulled from:
`https://twiki.cern.ch/CMS/EgammaPostRecoRecipes#2018_Data_MC`

* `cmsrel CMSSW_10_2_10`
* `cd CMSSW_10_2_10/src`
* `cmsenv`
* `git cms-init`
* `git cms-merge-topic cms-egamma:EgammaPostRecoTools`
* `git cms-merge-topic cms-egamma:PhotonIDValueMapSpeedup1029`
* `git cms-merge-topic cms-egamma:slava77-btvDictFix_10210`
* `git cms-addpkg EgammaAnalysis/ElectronTools`
* `rm EgammaAnalysis/ElectronTools/data -rf`
* `git clone git@github.com:cms-data/EgammaAnalysis-ElectronTools.git EgammaAnalysis/ElectronTools/data`

# Then install the ExcitedTau package
* `cd CMSSW_10_2_10/src`
* `git clone https://github.com/fojensen/ExcitedTau`
* `cd ExcitedTau/`
* `scram build`
* `cmsRun excitingAnalyzer_cfg.py isSignalMC=True >& log &`

## To submit jobs/datasets to CRAB:

