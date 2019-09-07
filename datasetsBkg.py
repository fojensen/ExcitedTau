import json

isMC_ = 'True'
isSignalMC_ = 'False'

data = {}
data['people'] = []

data['people'].append({
   'name': 'DYJetsToLL',
   'das': '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '100194597',
   'nfiles': '1254',
   'isMC': isMC_,
   'xs': '6025.2',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'TTGJets',
   'das': '/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '4691915',
   'nfiles': '133',
   'isMC': isMC_,
   'xs': '3.697',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'TTGamma_SingleLeptFromT',
   'das': '/TTGamma_SingleLeptFromT_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4840000',
   'nfiles': '140',
   'isMC': isMC_,
   'xs': '0.704',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'TTGamma_SingleLeptFromTbar',
   'das': '/TTGamma_SingleLeptFromTbar_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4945000',
   'nfiles': '141',
   'isMC': isMC_,
   'xs': '0.704',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTGamma_DiLept',
   'das': '/TTGamma_Dilept_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4940000',
   'nfiles': '151',
   'isMC': isMC_,
   'xs': '0.5804',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'TTGamma_Hadronic',
   'das': '/TTGamma_Hadronic_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4940000',
   'nfiles': '143',
   'isMC': isMC_,
   'xs': '88.29',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'TTTo2L2Nu',
   'das': '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '64310000',
   'nfiles': '968',
   'isMC': isMC_,
   'xs': '88.29',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'TTToSemiLeptonic',
   'das': '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '101550000',
   'nfiles': '1523',
   'isMC': isMC_,
   'xs': '365.34',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'ZGToLLG',
   'das': '/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '13946364',
   'nfiles': '266',
   'isMC': isMC_,
   'xs': '117.864',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'WJetsToLNu',
   'das': '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM',
   'nevents': '71026861',
   'nfiles': '990',
   'isMC': isMC_,
   'xs': '61334.9',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT50to100',
   'das': '/QCD_HT50to100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '38754230',
   'nfiles': '547',
   'isMC': isMC_,
   'xs': '19380000.', #FIXME
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT100to200',
   'das': '/QCD_HT100to200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '93972378',
   'nfiles': '1296',
   'isMC': isMC_,
   'xs': '19380000.',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT200to300',
   'das': '/QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '54289442',
   'nfiles': '780',
   'isMC': isMC_,
   'xs': '1717000.',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT300to500',
   'das': '/QCD_HT300to500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '54661579',
   'nfiles': '829',
   'isMC': isMC_,
   'xs': '351300.',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT500to700',
   'das': '/QCD_HT500to700_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '55152960',
   'nfiles': '1355',
   'isMC': isMC_,
   'xs': '31630.',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT700to1000',
   'das': '/QCD_HT700to1000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '48158738',
   'nfiles': '1257',
   'isMC': isMC_,
   'xs': '6802.',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT1000to1500',
   'das': '/QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '15466225',
   'nfiles': '420',
   'isMC': isMC_,
   'xs': '1206.',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT1500to2000',
   'das': '/QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '10955087',
   'nfiles': '302',
   'isMC': isMC_,
   'xs': '120.4',
   'isSignalMC': isSignalMC_
})
data['people'].append({
   'name': 'QCD_HT2000toInf',
   'das': '/QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '5475677',
   'nfiles': '166',
   'isMC': isMC_,
   'xs': '25.24',
   'isSignalMC': isSignalMC_
})

with open('datasetsBackgrounds.json', 'w') as outfile:
   json.dump(data, outfile)
