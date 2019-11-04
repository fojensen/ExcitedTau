import json

isMC_ = 'False'

data = {}
data['people'] = []

data['people'].append({
   'name': 'Tau2018D-PromptReco-v2/MINIAOD',
   'das': '/Tau/Run2018D-PromptReco-v2/MINIAOD',
   'nevents': '167889795',
   'nfiles': '2372',
   'isMC': isMC_
})

with open('datasetsData.json', 'w') as outfile:
   json.dump(data, outfile)
