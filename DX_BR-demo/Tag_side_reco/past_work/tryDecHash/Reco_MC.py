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

vm.addAlias('decHash', 'extraInfo(DecayHash)')
vm.addAlias('decHashEx', 'extraInfo(DecayHashExtended)')
B_MC_vars  = vc.kinematics + ['PDG','M','theta','phi','pstar','thetaStar','Mbc','deltaE']
B_MC_vars += ['decHash','decHashEx']

##########################################################################
# Analysis modules
import basf2 as b2
import fei
import modularAnalysis as ma
import vertex as vt

##########################################################################
# Basic setups
import sys
if len(sys.argv) != 2:
    print('Usage: basf2 Reco_MC.py index')
    print('')
    sys.exit(1)

mcType  = 'charged' #sys.argv[1]
BGx     = '0'       #sys.argv[2]
ele     = sys.argv[1]
feiList= 'B+:generic'

input_list = []
input_list.append('/group/belle2/users/tenchini/prerelease-05-00-00a/'+str(mcType)+'/'+str(mcType)+'_eph3_BGx'+str(BGx)+'_'+str(ele)+'.root')

file_prefix='output/'+str(mcType)+'_'+str(ele)
max_event = 0

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
particles = fei.get_default_channels(
    B_extra_cut  = 'Mbc>5 and abs(deltaE)<0.9'+exclChnls,
    chargedB     = True,
    neutralB     = False,
    hadronic     = True,
    semileptonic = False,
    baryonic     = True,
)

# Set up FEI configuration specifying the FEI prefix, monitor is for internal characteristics
configuration = fei.FeiConfiguration(prefix='FEIv4_2021_MC14_release_05_01_12', monitor=False)

# Add FEI path to the path to be processed
feistate = fei.get_path(particles, configuration)
path.add_path(feistate.path)

ma.matchMCTruth(list_name=feiList, path=path)

Btag_cuts = 'Mbc>5.24 and abs(deltaE)<0.5 and SigProb>0.01'
ma.cutAndCopyList('B+:myList',feiList, Btag_cuts, path=path)

ma.rankByHighest("B+:myList", "SigProb", path=path)
vm.addAlias("Rank",'extraInfo(SigProb_rank)')
ma.cutAndCopyList('B+:tag','B+:myList', 'Rank==1', path=path)

skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B+:tag'])
path.add_module(skimfilter1)
emptypath = b2.create_path()
skimfilter1.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath,b2.AfterConditionPath.END)

##########################################################################
# Save B+ reco candidate outputs
# usual candidate reconstruction info
ma.variablesToNtuple('B+:tag',B_Tag_vars,treename='B_Tag', filename=file_prefix+'_mainOut.root',path=path)
# What FEI thinks the reco decay chain is
path.add_module('ParticleMCDecayString', listName='B+:tag',fileName=file_prefix+'_feiHash.root')

ma.fillParticleListFromMC('B+','',addDaughters=True, skipNonPrimaryDaughters = True, path=path)
ma.cutAndCopyList('B+:MC1','B+','mdstIndex==1', path=path)
path.add_module('ParticleMCDecayString', listName='B+:MC1',fileName=file_prefix+'_mc1Hash.root')
ma.cutAndCopyList('B+:MC2','B+','mdstIndex==2', path=path)
path.add_module('ParticleMCDecayString', listName='B+:MC2',fileName=file_prefix+'_mc2Hash.root')
 
ma.variablesToNtuple('B+:MC1',B_MC_vars,treename='B_plus', filename=file_prefix+'_mainOut.root',path=path)
ma.variablesToNtuple('B+:MC2',B_MC_vars,treename='B_minus', filename=file_prefix+'_mainOut.root',path=path)
##########################################################################
b2.process(path,max_event)
##########################################################################
