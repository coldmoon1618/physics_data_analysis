##########################################################################
# Build output candidate variable list
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

vm.addAlias('pstar', 'useCMSFrame(p)')
vm.addAlias('thetaStar','useCMSFrame(theta)')
vm.addAlias('SigProb', 'extraInfo(SignalProbability)')
vm.addAlias('decMode', 'extraInfo(decayModeID)')
vm.addAlias('decHash', 'extraInfo(DecayHash)')
vm.addAlias('decHashEx', 'extraInfo(DecayHashExtended)')
B_Tag_vars = vc.kinematics + ['PDG','M','theta','phi','pstar','thetaStar','Mbc','deltaE','Ecms']
B_Tag_vars+= ['SigProb','decMode']
B_Tag_vars+= ['decHash','decHashEx']

##########################################################################
# Analysis modules
import basf2 as b2
import fei
import modularAnalysis as ma
import vertex as vt

##########################################################################
input_list = ['in.root']

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
feistate = fei.get_path(particles, configuration)
path.add_path(feistate.path)

Btag_cuts = 'Mbc>5.2 and abs(deltaE)<0.5 and SigProb>0.001'
ma.cutAndCopyList('B+:myList','B+:generic', Btag_cuts, path=path)

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
# What FEI thinks the reco decay chain is
path.add_module('ParticleMCDecayString', listName='B+:tag',fileName='feiHash.root')
# usual candidate reconstruction info
ma.variablesToNtuple('B+:tag',B_Tag_vars,treename='B_Tag', filename='mainOut.root',path=path)

##########################################################################
b2.process(path,max_event)
##########################################################################
