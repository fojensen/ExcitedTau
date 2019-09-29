import json
import os

infile = 'datasetsBkg.json'
#infile = 'datasetsSig.json'
#infile = 'datasetsData.json'

with open(infile) as json_file:

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
      #if str(p['isMC']=="isMC"):
      f.write("config.JobType.pyCfgParams = ['isSignalMC="+str(p['isSignalMC'])+"', 'xs="+str(eval(p['xs']))+"', 'nevents="+str(p['nevents'])+"']")
      f.write("\n")
      f.write("config.Data.inputDataset = '"+p['das']+"'\n")
      f.write("config.Data.inputDBS = 'global'\n")
      f.write("config.Data.splitting = 'FileBased'\n")
      f.write("config.Data.unitsPerJob = 1\n")
      f.write("config.Data.allowNonValidInputDataset = True\n")
      f.write("config.Data.outLFNDirBase = '/store/user/fjensen/ExcitedTauProduction_v0'")
      f.write("\n")
      f.write("config.Site.storageSite = 'T3_US_FNALLPC'\n")
      f.close()
      # comment the next line for a dry run
      os.system("crab submit " + f.name)

