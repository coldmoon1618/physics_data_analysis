##########################################################################
# Build output candidate variable list
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

vm.addAlias('pstar', 'useCMSFrame(p)')
vm.addAlias('thetaStar','useCMSFrame(theta)')
vm.addAlias('SigProb', 'extraInfo(SignalProbability)')
vm.addAlias('decMode', 'extraInfo(decayModeID)')
B_Tag_vars = vc.kinematics + ['PDG','M','theta','phi','pstar','thetaStar','Mbc','deltaE']
B_Tag_vars+= ['mcPDG','isSignal','isSignalAcceptMissing','isSignalAcceptMissingGamma','isSignalAcceptWrongFSPs','mcErrors']
B_Tag_vars+= ['SigProb','decMode']

# CleonConeCS can't be calculated for just reco side, and KSFWvars are always for reco side (primary daughters vs final state particles)
B_Tag_vars+= ['CleoConeCS(1)','CleoConeCS(1,ROE)']+['KSFWVariables(hso00)','KSFWVariables(hso00,FS1)']
# cleoCone(i,axisName) is for EventShape (whole event)
B_Tag_vars+= ['cleoCone(1,thrust)','cleoCone(1,collision)']

##########################################################################
# Analysis modules
import basf2 as b2
import fei
import modularAnalysis as ma
import vertex as vt

##########################################################################
# Basic setups
import sys
#if len(sys.argv) != 2:
#    print('Usage: basf2 Reco_MC.py index')
#    print('')
#    sys.exit(1)

mcType      = 'charged' #sys.argv[1]
BGx         = '0'       #sys.argv[2]
ele         = '0'       #sys.argv[1]
feiTagList  = 'B+:generic'
feiSigList  = 'B+:generic'

input_list = []
input_list.append('/group/belle2/users/tenchini/prerelease-05-00-00a/'+str(mcType)+'/'+str(mcType)+'_eph3_BGx'+str(BGx)+'_'+str(ele)+'.root')

file_prefix='output'
max_event = 500

path = b2.create_path()
ma.inputMdstList('default', input_list, path=path)

##########################################################################
# Tag side: F E I
b2.conditions.prepend_globaltag(ma.getAnalysisGlobaltag())

exclChnls = 'and decMode!=5 '
exclChnls+= 'and decMode!=6 '
exclChnls+= 'and decMode!=7 '
exclChnls+= 'and decMode!=8 '
exclChnls+= 'and decMode!=9 '
exclChnls+= 'and decMode!=10 '
exclChnls+= 'and decMode!=11 '
exclChnls+= 'and decMode!=12 '
exclChnls+= 'and decMode!=13 '
exclChnls+= 'and decMode!=33 '
exclChnls+= 'and decMode!=34 '
# Get FEI default channels.
# Utilise the arguments to toggle on and off certain channels
tagParticles = fei.get_default_channels(
    B_extra_cut  = 'Mbc>5 and abs(deltaE)<0.9 '+exclChnls,
    chargedB     = True,
    neutralB     = False,
    hadronic     = True,
    semileptonic = False,
    baryonic     = True,
)

# Set up FEI configuration specifying the FEI prefix, monitor is for internal characteristics
configuration = fei.FeiConfiguration(prefix='FEIv4_2021_MC14_release_05_01_12', monitor=False)

# Add FEI path to the path to be processed
tagFEI = fei.get_path(tagParticles, configuration)
path.add_path(tagFEI.path)


Btag_cuts = 'Mbc>5.24 and abs(deltaE)<0.5 and SigProb>0.001'
ma.cutAndCopyList('B+:tagList',feiTagList, Btag_cuts, path=path)

ma.rankByHighest("B+:tagList", "SigProb", path=path)
vm.addAlias("Rank",'extraInfo(SigProb_rank)')
ma.cutAndCopyList('B+:tag','B+:tagList', 'Rank==1', path=path)

skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B+:tag'])
path.add_module(skimfilter1)
emptypath = b2.create_path()
skimfilter1.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath,b2.AfterConditionPath.END)

##########################################################################
# ROE
ma.buildEventShape(inputListNames='B+:tag', path=path)
ma.buildRestOfEvent(target_list_name='B+:tag',fillWithMostLikely=True, path=path)
trkSelection = 'nCDCHits > 0 and useCMSFrame(p)<=3.2'
eclSelection = 'p >= 0.05 and useCMSFrame(p)<=3.2'
ma.appendROEMask('B+:tag', 'cleanMask',trkSelection,eclSelection, path=path)
# Build event shape vars for selected candidates
ma.buildContinuumSuppression('B+:tag', 'cleanMask', path=path)
ma.fillParticleListFromROE('B+:sig -> B+:tag','','cleanMask',path=path)

ma.matchMCTruth('B+:tag', path=path)
ma.matchMCTruth('B+:sig', path=path)

##########################################################################
# Save B+ reco candidate outputs
# usual candidate reconstruction info
ma.variablesToNtuple('B+:tag',B_Tag_vars,treename='B_Tag', filename=file_prefix+'_mainOut.root',path=path)
ma.variablesToNtuple('B+:sig',B_Tag_vars,treename='B_Sig', filename=file_prefix+'_mainOut.root',path=path)
# What FEI thinks the reco decay chain is
path.add_module('ParticleMCDecayString', listName='B+:tag',fileName=file_prefix+'_feiHash.root')

ma.fillParticleListFromMC('B+','',addDaughters=True, skipNonPrimaryDaughters = True, path=path)
ma.cutAndCopyList('B+:MC1','B+','mdstIndex==1', path=path)
path.add_module('ParticleMCDecayString', listName='B+:MC1',fileName=file_prefix+'_mc1Hash.root')
ma.cutAndCopyList('B+:MC2','B+','mdstIndex==2', path=path)
path.add_module('ParticleMCDecayString', listName='B+:MC2',fileName=file_prefix+'_mc2Hash.root')
 
##########################################################################
b2.process(path,max_event)
##########################################################################
