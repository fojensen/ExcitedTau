import json
import os

with open('datasets.json') as json_file:
   data = json.load(json_file)
   
   for p in data['people']:
      f = open("./crabSubmits/"+p['name']+".py","w+")
      f.write("from CRABClient.UserUtilities import config, getUsernameFromSiteDB\n")
      f.write("config = config()\n")
      f.write("\n")
      f.write("config.General.requestName = 'excitingAnalyzer_"+p['name']+"'\n")
      f.write("config.General.workArea = 'crab_projects'\n")
      f.write("config.General.transferOutputs = True\n")
      f.write("config.General.transferLogs = True\n")
      f.write("\n")
      f.write("config.JobType.pluginName = 'Analysis'\n")
      f.write("config.JobType.psetName = './excitingAnalyzer_cfg.py'\n")
      f.write("config.JobType.pyCfgParams = ['nevents="+p['nevents']+"', 'xs="+str(eval(p['xs']))+"']\n")
      f.write("\n")
      f.write("config.Data.inputDataset = '"+p['das']+"'\n")
      f.write("config.Data.inputDBS = 'global'\n")
      f.write("config.Data.splitting = 'FileBased'\n")
      f.write("config.Data.unitsPerJob = 1\n")
      f.write("config.Data.allowNonValidInputDataset = True\n")
      f.write("\n")
      f.write("config.Site.storageSite = 'T3_US_FNALLPC'\n")
      f.close()
      os.system("crab submit " + f.name)

