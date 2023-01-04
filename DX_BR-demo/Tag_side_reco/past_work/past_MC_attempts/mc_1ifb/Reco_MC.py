##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

from buildVars import *
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
import sys
if len(sys.argv) != 5:
    print('Usage: basf2 DX_reco.py mcType BGx index tagType')
    print('')
    sys.exit(1)

mcType  = sys.argv[1]
BGx     = sys.argv[2]
ele     = sys.argv[3]
tagType = sys.argv[4]

if  (tagType=='had'):
    useHad = True
    useSL  = False
    useBay = True
    feiList= 'B+:generic'
elif(tagType=='SL'):
    useHad = False
    useSL  = True
    useBay = False
    feiList='B+:semileptonic'
else:
    print('Usage: basf2 DX_reco.py mcType BGx index tagType{had,SL}')
    print('')
    sys.exit(1)

input_list = []
input_list.append('/group/belle2/users/tenchini/prerelease-05-00-00a/'+str(mcType)+'/'+str(mcType)+'_eph3_BGx'+str(BGx)+'_'+str(ele)+'.root')

output_file = 'MC_out_'+str(tagType)+'/'+str(mcType)+'_Rec_BGx'+str(BGx)+'_'+str(ele)+'.root'
max_event = 0

path = b2.create_path()
ma.inputMdstList('default', input_list, path=path)

##########################################################################
# Tag side: F E I
# Add the database with the classifier weight files for the FEI
b2.conditions.prepend_globaltag('analysis_tools_release-04-02')
#b2.conditions.expert_settings(stalled_timeout=0, connection_timeout=600)

# Get FEI default channels.
# Utilise the arguments to toggle on and off certain channels
particles = fei.get_default_channels(
    #B_extra_cut  = 'Mbc>5 and abs(deltaE)<0.9',
    B_extra_cut  = 'Mbc>5',
    chargedB     = True,
    neutralB     = False,
    hadronic     = useHad,
    semileptonic = useSL,
    baryonic     = useBay,
)

# Set up FEI configuration specifying the FEI prefix, monitor is for internal characteristics
configuration = fei.FeiConfiguration(prefix='FEIv4_2020_MC13_release_04_01_01', monitor=False)

# Add FEI path to the path to be processed
feistate = fei.get_path(particles, configuration)
path.add_path(feistate.path)

ma.matchMCTruth(list_name=feiList, path=path)

ma.buildRestOfEvent(target_list_name=feiList, path=path)
cleanMask = ("cleanMask", "nCDCHits > 0 and useCMSFrame(p)<=3.2", "p >= 0.05 and useCMSFrame(p)<=3.2")
ma.appendROEMasks(list_name=feiList, mask_tuples=[cleanMask], path=path)

ma.buildContinuumSuppression(list_name=feiList, roe_mask="cleanMask", path=path)

#Btag_cuts = 'Mbc>5.2 and abs(deltaE)<0.5 and extraInfo(SignalProbability)>0.001'
Btag_cuts = 'Mbc>5.2 and extraInfo(SignalProbability)>0.001'
ma.cutAndCopyList('B+:tag',feiList, Btag_cuts, path=path)

vt.treeFit('B+:tag', conf_level=-1, updateAllDaughters = False, path=path)

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
