##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

vm.addAlias('pstar', 'useCMSFrame(p)')
vm.addAlias('thetaStar','useCMSFrame(theta)')
vm.addAlias('nDaus', 'nDaughters')
vm.addAlias('mothInd','mcMother(mdstIndex)')

def setB_TagVars(isMC = True):
    print('Building B_Tag variables')

    vm.addAlias('SigProb', 'extraInfo(SignalProbability)')
    vm.addAlias('decMode', 'extraInfo(decayModeID)')
    B_Tag_vars = vc.kinematics + ['PDG','M','theta','phi','pstar','thetaStar','Mbc','deltaE']
    if(isMC):
        B_Tag_vars+= ['genParticleID','mcPDG','isSignal','isSignalAcceptMissing','isSignalAcceptMissingGamma','isSignalAcceptWrongFSPs','mcErrors','nMCMatches']
    B_Tag_vars+= ['SigProb','decMode','charge']

    return B_Tag_vars
B_Tag_vars = setB_TagVars()

##########################################################################
# Analysis modules
import basf2 as b2
import fei
import modularAnalysis as ma
import vertex as vt
from variables import variables as vm

##########################################################################
# Basic setups

input_file  = 'in.root'
output_file = 'Rec.root'

useHad = True
useSL  = False
useBay = True
feiList= 'B+:generic'

input_list = ['']
input_list.append(input_file)

max_event = 0

path = b2.create_path()
ma.inputMdstList('default', input_list, path=path)

##########################################################################
# Tag side: F E I
b2.conditions.prepend_globaltag(ma.getAnalysisGlobaltag())

# Get FEI default channels.
# Utilise the arguments to toggle on and off certain channels
particles = fei.get_default_channels(
    B_extra_cut  = 'Mbc>5.1 and abs(deltaE)<0.6',
    chargedB     = True,
    neutralB     = False,
    hadronic     = useHad,
    semileptonic = useSL,
    baryonic     = useBay,
)

# Set up FEI configuration specifying the FEI prefix, monitor is for internal characteristics
configuration = fei.FeiConfiguration(prefix='FEIv4_2021_MC14_release_05_01_12', monitor=False)

# Add FEI path to the path to be processed
feistate = fei.get_path(particles, configuration)
path.add_path(feistate.path)

ma.matchMCTruth(list_name=feiList, path=path)

Btag_cuts = 'Mbc>5.2 and abs(deltaE)<0.5 and extraInfo(SignalProbability)>0.001'
ma.cutAndCopyList('B+:tag',feiList, Btag_cuts, path=path)

##########################################################################
# Save B+ reco candidate outputs
path.add_module('VariablesToEventBasedTree',
    particleList='B+:tag',
    variables = B_Tag_vars,
    treeName = 'B_Tag',
    fileName = output_file,
    maxCandidates = 9999)
 
##########################################################################
b2.process(path,max_event)
##########################################################################
