import json

isMC_ = 'False'
applyFilter_ = 'True'
doSS_ = 'False'

data = {}
data['people'] = []

data['people'].append({
   'name': 'SingleMuon_2018A',
   'das': '/SingleMuon/Run2018A-PromptReco-v3/MINIAOD',
   'nevents': '68383706',
   'nfiles': '765',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'SingleMuon_2018B',
   'das': '/SingleMuon/Run2018B-PromptReco-v2/MINIAOD',
   'nevents': '7574181',
   'nfiles': '105',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'SingleMuon_2018C',
   'das': '/SingleMuon/Run2018C-PromptReco-v3/MINIAOD',
   'nevents': '47948288',
   'nfiles': '537',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'SingleMuon_2018D',
   'das': '/SingleMuon/Run2018D-PromptReco-v2/MINIAOD',
   'nevents': '506717754',
   'nfiles': '5533',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'EGamma_2018A',
   'das': '/EGamma/Run2018A-PromptReco-v3/MINIAOD',
   'nevents': '82136296',
   'nfiles': '957',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'EGamma_2018B',
   'das': '/EGamma/Run2018B-PromptReco-v2/MINIAOD',
   'nevents': '9234833',
   'nfiles': '160',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'EGamma_2018C',
   'das': '/EGamma/Run2018C-PromptReco-v3/MINIAOD',
   'nevents': '65529767',
   'nfiles': '770',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'EGamma_2018D',
   'das': '/EGamma/Run2018D-PromptReco-v2/MINIAOD',
   'nevents': '738976078',
   'nfiles': '8663',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
'''data['people'].append({
   'name': 'EGamma_2018E',
   'das': '/EGamma/Run2018E-PromptReco-v1/MINIAOD',
   'nevents': '112215',
   'nfiles': '2',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})'''
data['people'].append({
   'name': 'JetHT_Run2018A',
   'das': '/JetHT/Run2018A-PromptReco-v3/MINIAOD',
   'nevents': '44331925',
   'nfiles': '623',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'JetHT_Run2018B',
   'das': '/JetHT/Run2018B-PromptReco-v2/MINIAOD',
   'nevents': '4476872',
   'nfiles': '94',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'JetHT_Run2018C',
   'das': '/JetHT/Run2018C-PromptReco-v3/MINIAOD',
   'nevents': '31413719',
   'nfiles': '460',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'JetHT_Run2018D',
   'das': '/JetHT/Run2018D-PromptReco-v2/MINIAOD',
   'nevents': '358853510',
   'nfiles': '5157',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
'''data['people'].append({
   'name': 'JetHT_Run2018E',
   'das': '/JetHT/Run2018E-PromptReco-v1/MINIAOD',
   'nevents': '63022',
   'nfiles': '2',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})'''
data['people'].append({
   'name': 'Tau_2018A',
   'das': '/Tau/Run2018A-PromptReco-v3/MINIAOD',
   'nevents': '17972951',
   'nfiles': '258',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'Tau_2018B',
   'das': '/Tau/Run2018B-PromptReco-v2/MINIAOD',
   'nevents': '2368587',
   'nfiles': '53',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'Tau_2018C',
   'das': '/Tau/Run2018C-PromptReco-v3/MINIAOD',
   'nevents': '235',
   'nfiles': '235',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
data['people'].append({
   'name': 'Tau_2018D',
   'das': '/Tau/Run2018D-PromptReco-v2/MINIAOD',
   'nevents': '167889795',
   'nfiles': '2372',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})
'''data['people'].append({
   'name': 'Tau_2018E',
   'das': '/Tau/Run2018E-PromptReco-v2/MINIAOD',
   'nevents': '0',
   'nfiles': '2',
   'isMC': isMC_,
   'applyFilter': applyFilter_,
   'doSS': doSS_
})'''
with open('datasetsData_2018.json', 'w') as outfile:
   json.dump(data, outfile)
