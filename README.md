# ExcitedTau

the search for excited tau leptons...

## To install the package:

### Install stuff to calculate DeepTauv2p1
`https://twiki.cern.ch/CMSPublic/SWGuidePFTauID#Running_of_the_DeepTauIDs_ver_20` 
* `cmsrel CMSSW_10_2_16`
* `cd CMSSW_10_2_16/src/`
* `cmsenv`
* `git cms-merge-topic -u cms-tau-pog:CMSSW_10_2_X_tau-pog_DeepTau2017v2p1_nanoAOD`
* `scram b -j 4`

### Then install stuff for EGAMMA MVA ID
`https://twiki.cern.ch/CMS/EgammaPostRecoRecipes#2018_Data_MC`
* `git cms-merge-topic cms-egamma:EgammaPostRecoTools`
* `git cms-merge-topic cms-egamma:PhotonIDValueMapSpeedup1029`
* `git cms-merge-topic cms-egamma:slava77-btvDictFix_10210`
* `git cms-addpkg EgammaAnalysis/ElectronTools`
* `rm EgammaAnalysis/ElectronTools/data -rf`
* `git clone git@github.com:cms-data/EgammaAnalysis-ElectronTools.git EgammaAnalysis/ElectronTools/data`
* `scram build`

### Then install the jet toolbox
`https://twiki.cern.ch/CMS/JetToolbox`
* `git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_102X_v2`
* `scram build`

### Then install the ExcitedTau package
`https://github.com/fojensen/ExcitedTau` 
* `git clone https://github.com/fojensen/ExcitedTau`
* `scram build`
* `cd ExcitedTau/`

### Do a test run on a signal file:
* `cmsRun excitingAnalyzer_cfg.py isSignalMC=True >& log &`

## To submit jobs/datasets to CRAB:
`datasetsBkg.py` and `datasetSig.py` contain entries which will eventually be submitted to crab.  

`python datasetsBkg.py`  
`python datasetSig.py`  
generates JSON files to give to the submission script `submitToCrab.py`.  

Comment/uncomment the appropriate line for signal or background:  
`https://github.com/fojensen/ExcitedTau/blob/master/submitToCrab.py#L4-L5`, then run:  
`python submitToCrab.py`
If you actually want to submit, uncomment the following line  
`https://github.com/fojensen/ExcitedTau/blob/master/submitToCrab.py#L36`

