import json

isMC_ = 'False'
applyFilter_ = 'True'
doSS_ = 'False'

data = {}
data['people'] = []

data['people'].append({
   'name': 'TauTau_Run2018A',
   'das': '/EmbeddingRun2018A/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '10371109',
   'nfiles': '21606',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doElTau': False,
})
data['people'].append({
   'name': 'TauTau_Run2018B',
   'das': '/EmbeddingRun2018B/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '5227841',
   'nfiles': '9717',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doElTau': False,
})
data['people'].append({
   'name': 'TauTau_Run2018C',
   'das': '/EmbeddingRun2018C/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '4821754',
   'nfiles': '8968',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doElTau': False,
})
data['people'].append({
   'name': 'TauTau_Run2018D',
   'das': '/EmbeddingRun2018D/TauTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '22561187',
   'nfiles': '56451',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doElTau': False,
})
data['people'].append({
   'name': 'ElTau_Run2018A',
   'das': '/EmbeddingRun2018A/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '11735088',
   'nfiles': '21606',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doTauTau': False,
})
data['people'].append({
   'name': 'ElTau_Run2018B',
   'das': '/EmbeddingRun2018B/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '5893097',
   'nfiles': '9717',
   'isMC': isMC_, 
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doTauTau': False,
})
data['people'].append({
   'name': 'ElTau_Run2018C',
   'das': '/EmbeddingRun2018C/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '5422423',
   'nfiles': '8968',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doTauTau': False,
})
data['people'].append({
   'name': 'ElTau_Run2018D',
   'das': '/EmbeddingRun2018D/ElTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '25383778',
   'nfiles': '56450',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doMuTau': False,
   'doTauTau': False,
})
data['people'].append({
   'name': 'MuTau_Run2018A',
   'das': '/EmbeddingRun2018A/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '11735808',
   'nfiles': '21606',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doElTau': False,
   'doTauTau': False,
})
data['people'].append({
   'name': 'MuTau_Run2018B',
   'das': '/EmbeddingRun2018B/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '5892212',
   'nfiles': '9717',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doElTau': False,
   'doTauTau': False,
})
data['people'].append({
   'name': 'MuTau_Run2018C',
   'das': '/EmbeddingRun2018C/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '5420887',
   'nfiles': '8967',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doElTau': False,
   'doTauTau': False,
})
data['people'].append({
   'name': 'MuTau_Run2018D',
   'das': '/EmbeddingRun2018D/MuTauFinalState-inputDoubleMu_102X_miniAOD-v1/USER',
   'nevents': '25379684',
   'nfiles': '56451',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_,
   'doElTau': False,
   'doTauTau': False,
})

with open('datasetsEmbedding.json', 'w') as outfile:
   json.dump(data, outfile)
