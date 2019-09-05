import json

data = {}
data['people'] = []

data['people'].append({
   'name': 'DYJetsToLL',
   'das': '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '6025.2',
   'nevents': '100194597',
   'nfiles': '1254',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTGJets',
   'das': '/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '3.697',
   'nevents': '4691915',
   'nfiles': '133',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTGamma_SingleLeptFromT',
   'das': '/TTGamma_SingleLeptFromT_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'xs': '0.704',
   'nevents': '4840000',
   'nfiles': '140',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTGamma_SingleLeptFromTbar',
   'das': '/TTGamma_SingleLeptFromTbar_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'xs': '0.704',
   'nevents': '4945000',
   'nfiles': '141',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTGamma_DiLept',
   'das': '/TTGamma_Dilept_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'xs': '0.5804',
   'nevents': '4940000',
   'nfiles': '151',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTGamma_Hadronic',
   'das': '/TTGamma_Hadronic_TuneCP5_13TeV_madgraph_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'xs': '1.689440272', ##FIXME
   'nevents': '4940000',
   'nfiles': '143',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTTo2L2Nu',
   'das': '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '88.29',
   'nevents': '64310000',
   'nfiles': '968',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'TTToSemiLeptonic',
   'das': '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '365.34',
   'nevents': '101550000',
   'nfiles': '1523',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'ZGToLLG',
   'das': '/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM',
   'xs': '117.864',
   'nevents': '13946364',
   'nfiles': '266',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'WJetsToLNu',
   'das': '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM',
   'xs': '61334.9',
   'nevents': '71026861',
   'nfiles': '990',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT50to100',
   'das': '/QCD_HT50to100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '19380000.', #FIXME
   'nevents': '38754230',
   'nfiles': '547',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT100to200',
   'das': '/QCD_HT100to200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '19380000.',
   'nevents': '93972378',
   'nfiles': '1296',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT200to300',
   'das': '/QCD_HT200to300_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '1717000.',
   'nevents': '54289442',
   'nfiles': '780',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT300to500',
   'das': '/QCD_HT300to500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '351300.',
   'nevents': '54661579',
   'nfiles': '829',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT500to700',
   'das': '/QCD_HT500to700_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '31630.',
   'nevents': '55152960',
   'nfiles': '1355',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT700to1000',
   'das': '/QCD_HT700to1000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '6802.',
   'nevents': '48158738',
   'nfiles': '1257',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT1000to1500',
   'das': '/QCD_HT1000to1500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '1206.',
   'nevents': '15466225',
   'nfiles': '420',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT1500to2000',
   'das': '/QCD_HT1500to2000_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '120.4',
   'nevents': '10955087',
   'nfiles': '302',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'QCD_HT2000toInf',
   'das': '/QCD_HT2000toInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM',
   'xs': '25.24',
   'nevents': '5475677',
   'nfiles': '166',
   'isSignalMC': 'False',
})
data['people'].append({
   'name': 'Taustar_m250',
   'das': '/Taustar_TauG_L10000_m250_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '0.0177 * 1.285',
   'nevents': '20000',
   'nfiles': '9',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m500',
   'das': '/Taustar_TauG_L10000_m500_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '0.0108 * 1.284',
   'nevents': '20000',
   'nfiles': '9',
   'isSignalMC': 'True'
})
data['people'].append({
    'name': 'Taustar_m750',
    'das': '/Taustar_TauG_L10000_m750_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v2/MINIAODSIM',
    'xs': '6.639e-3 * 1.277',
    'nevents': '20000',
    'nfiles': '1',
    'isSignalMC': 'True'
})
data['people'].append({
   'name':'Taustar_m1000',
   'das': '/Taustar_TauG_L10000_m1000_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '4.069e-3 * 1.270',
   'nevents': '20000',
   'nfiles': '',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m1250',
   'das': '/Taustar_TauG_L10000_m1250_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '2.494e-3 * 1.264',
   'nevents': '15000',
   'nfiles': '3',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m1500',
   'das': '/Taustar_TauG_L10000_m1500_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '1.529e-3 * 1.259',
   'nevents': '14718',
   'nfiles': '16',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m1750',
   'das': '/Taustar_TauG_L10000_m1750_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '9.371e-4 * 1.256',
   'nevents': '14188',
   'nfiles': '',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m2000',
   'das': '/Taustar_TauG_L10000_m2000_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '5.744e-4 * 1.255',
   'nevents': '14766',
   'nfiles': '2',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m2250',
   'das': '/Taustar_TauG_L10000_m2250_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '3.521e-4 * 1.256',
   'nevents': '10000',
   'nfiles': '8',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m2500',
   'das': '/Taustar_TauG_L10000_m2500_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '2.159e-4 * 1.260',
   'nevents': '10000',
   'nfiles': '15',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m2750',
   'das': '/Taustar_TauG_L10000_m2750_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '1.323e-4 * 1.266',
   'nevents': '10000',
   'nfiles': '8',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m3000',
   'das': '/Taustar_TauG_L10000_m3000_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '8.12e-5 * 1.275',
   'nevents': '10000',
   'nfiles': '',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m3250',
   'das': '/Taustar_TauG_L10000_m3250_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '4.97e-5 * 1.285',
   'nevents': '10000',
   'nfiles': '3',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m3500',
   'das': '/Taustar_TauG_L10000_m3500_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v2/MINIAODSIM',
   'xs': '3.05e-5 * 1.297',
   'nevents': '10000',
   'nfiles': '1',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m3750',
   'das': '/Taustar_TauG_L10000_m3750_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '1.87e-5 * 1.310',
   'nevents': '10000',
   'nfiles': '',
   'isSignalMC': 'True'
})
data['people'].append({
   'name': 'Taustar_m4000',
   'das': '/Taustar_TauG_L10000_m4000_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '1.14e-5 * 1.324',
   'nevents': '10000',
   'nfiles': '',
   'isSignalMC': 'True'
})
#data['people'].append({
#   'name': 'Taustar_m4250',
#   'das': '', 
#   'xs': '7.02e-6 * 1.340',
#   'nevents': '',
#   'isSignalMC': 'True'
#})
data['people'].append({
   'name': 'Taustar_m4500',
   'das': '/Taustar_TauG_L10000_m4500_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '4.30e-6 * 1.356',
   'nevents': '9408',
   'nfiles': '',
   'isSignalMC': 'True'
})
#data['people'].append({
#   'name': 'Taustar_m4750',
#   'das': '',
#   'xs': '2.64e-6 * 1.372',
#   'nevents': '',
#   'isSignalMC': 'True'
#})
data['people'].append({
   'name': 'Taustar_m5000',
   'das': '/Taustar_TauG_L10000_m5000_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
   'xs': '1.62e-6 * 1.389',
   'nevents': '10000',
   'nfiles': '2',
   'isSignalMC': 'True'
})

with open('datasets.json', 'w') as outfile:
   json.dump(data, outfile)
