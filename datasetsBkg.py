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
   'xs': '6025.2',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'DYJetsToLL_Zpt150',
   'das': '/DYJetsToLL_M-50_Zpt-150toInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '2135787',
   'nfiles': '60',
   'xs': '27.150108', ###derived from inclusive sample
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'TTGJets',
   'das': '/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '4691915',
   'nfiles': '133',
   'xs': '3.697',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'TTGamma_SingleLeptFromT',
   'das': '/TTGamma_SingleLeptFromT_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4840000',
   'nfiles': '140',
   'xs': '0.704',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'TTGamma_SingleLeptFromTbar',
   'das': '/TTGamma_SingleLeptFromTbar_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4945000',
   'nfiles': '141',
   'xs': '0.704',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'TTGamma_DiLept',
   'das': '/TTGamma_Dilept_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4940000',
   'nfiles': '151',
   'xs': '0.5804',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'TTGamma_Hadronic',
   'das': '/TTGamma_Hadronic_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4940000',
   'nfiles': '143',
   'xs': '1.7086', #3.697-0.704-0.704-0.5804
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'  
})
data['people'].append({
   'name': 'TTTo2L2Nu',
   'das': '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '64310000',
   'nfiles': '968',
   'xs': '88.29',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'TTToSemiLeptonic',
   'das': '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '101550000',
   'nfiles': '1523',
   'xs': '365.34',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'TTToHadronic',
   'das': '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '133808000', ###extension exists
   'nfiles': '3313',
   'xs': '377.96',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
#data['people'].append({
#   'name': 'TTToHadronic_ext',
#   'das': '',
#   'nevents': '',
#   'nfiles': '',
#   'xs': '377.96',
#   'isSignalMC': isSignalMC_,
#   'applyFilter': 'False'
#}
data['people'].append({
   'name': 'ST_tW_antitop',
   'das': '/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM',
   'nevents': '5823328',
   'nfiles': '148',
   'xs': '19.4674',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'ST_tW_top',
   'das': '/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM',
   'nevents': '7636887',
   'nfiles': '184',
   'xs': '19.4674',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'ZGToLLG',
   'das': '/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '13946364',
   'nfiles': '266',
   'xs': '16.837021', #(32.3/0.2)*(0.033632+0.033662+0.03696)
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'WGToLNuG',
   'das': '/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '6108186',
   'nfiles': '117',
   'xs': '405.271',###
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'WJetsToLNu',
   'das': '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM',
   'nevents': '71026861',
   'nfiles': '990',
   'xs': '61334.9',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
'''data['people'].append({
   'name': 'GJets_HT-40To100',
   'das': '/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '9371355',
   'nfiles': '130',
   'xs': '9226.0',###FIXME
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_HT-100To200',
   'das': '/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-4cores5k_102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '9798176',
   'nfiles': '163',
   'xs': '9226.0',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_HT-200To400',
   'das': '/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '19062809',
   'nfiles': '312',
   'xs': '2300.0',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_HT-400To600',
   'das': '/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '4655985',
   'nfiles': '124',
   'xs': '277.4',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_HT-600ToInf',
   'das': '/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'nevents': '4981121',
   'nfiles': '165',
   'xs': '93.38',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_DR-0p4_HT-100To200',
   'das': '/GJets_DR-0p4_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '15424758',
   'nfiles': '245',
   'xs': '5000.0',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_DR-0p4_HT-200To400',
   'das': '/GJets_DR-0p4_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '49457520',
   'nfiles': '928',
   'xs': '1079.0',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_DR-0p4_HT-400To600',
   'das': '/GJets_DR-0p4_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '13717985',
   'nfiles': '431',
   'xs': '125.9',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'GJets_DR-0p4_HT-600ToInf',
   'das': '/GJets_DR-0p4_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '12456593',
   'nfiles': '390',
   'xs': '43.36',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'False'
})
data['people'].append({
   'name': 'QCD_HT50to100',
   'das': '/QCD_HT50to100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '38754230',
   'nfiles': '547',
   'xs': '19380000.', #FIXME
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT100to200',
   'das': '/QCD_HT100to200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '93972378',
   'nfiles': '1296',
   'xs': '19380000.', ###
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT200to300',
   'das': '/QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '54289442',
   'nfiles': '780',
   'xs': '1717000.',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT300to500',
   'das': '/QCD_HT300to500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '54661579',
   'nfiles': '829',
   'xs': '351300.',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT500to700',
   'das': '/QCD_HT500to700_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '55152960',
   'nfiles': '1355',
   'xs': '31630.',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT700to1000',
   'das': '/QCD_HT700to1000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '48158738',
   'nfiles': '1257',
   'isMC': isMC_,
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT1000to1500',
   'das': '/QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '15466225',
   'nfiles': '420',
   'xs': '1206.',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT1500to2000',
   'das': '/QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '10955087',
   'nfiles': '302',
   'xs': '120.4',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
data['people'].append({
   'name': 'QCD_HT2000toInf',
   'das': '/QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'nevents': '5475677',
   'nfiles': '166',
   'xs': '25.24',
   'isSignalMC': isSignalMC_,
   'applyFilter': 'True'
})
'''
with open('datasetsBkg.json', 'w') as outfile:
   json.dump(data, outfile)
