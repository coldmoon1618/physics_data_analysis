import basf2 as b2
import modularAnalysis as ma
import vertex as vt
import flavorTagger as ft
import variables.collections as vc
import variables.utils as vu
from variables import variables as vm
from stdV0s import stdKshorts

##########################################################################
vm.addAlias('pstar', 'useCMSFrame(p)')
vm.addAlias('thetaStar','useCMSFrame(theta)')
vm.addAlias('nDaus', 'nDaughters')

def setRecTrkVars():
    print('Building reco track variables')
    
    trks_vars    = vc.kinematics + ['thetaStar','M','theta','phi','pstar'] + vc.track + vc.track_hits + vc.pid 
    trks_vars   += ['thetaInCDCAcceptance','inCDCAcceptance','lastCDCLayer','isCloneTrack']

    return trks_vars

def setPhiRecVars():
    print('Building phi_Rec variables')
   
    Phi_Rec_vars = vc.kinematics + ['M','theta','phi','pstar'] + ['chiProb','isSignal', 'isOrHasCloneTrack']

    trks_vars = setRecTrkVars() 
    
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Phi_Rec_vars

def setKsRecVars():
    print('Building Ks_Rec variables')

    Ks_Rec_vars = vc.kinematics + ['M','theta','phi','pstar'] + ['chiProb','goodBelleKshort','isSignal', 'isOrHasCloneTrack']
    Ks_Rec_vars+= ['distance','significanceOfDistance'] + vc.flight_info
    
    trks_vars = setRecTrkVars() 
    
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Ks_Rec_vars

def setB0RecVars():
    print('Building B0_Rec variables')

#   vm.addAlias('flvrTag_FBDT','qrOutput(FBDT)')
#   vm.addAlias('flvrTag_FANN','qrOutput(FANN)')
    vm.addAlias('phi_cosHel','cosHelicityAngle(0,0)')
    B0_Rec_vars = vc.kinematics + ['M','theta','phi','pstar','isSignal','Mbc','deltaE','chiProb']
#    B0_Rec_vars+= ['genParticleID','flvrTag_FBDT','flvrTag_FANN','DeltaZ','DeltaZErr']
    Phi_Rec_vars= setPhiRecVars()
    Ks_Rec_vars = setKsRecVars()

    B0_Rec_vars+= ['phi_cosHel']
    B0_Rec_vars+= vu.create_aliases(Phi_Rec_vars, 'daughter(0,{variable})','phi')
    B0_Rec_vars+= vu.create_aliases(Ks_Rec_vars,  'daughter(1,{variable})','Ks')

    return B0_Rec_vars

def setB0CSVars():
    print('building continuum suppression vars')
    CSvars      = [ 'hso00','hso02','hso04',
                    'hso10','hso12','hso14',
                    'hso20','hso22','hso24',
                    'hoo1' ,'hoo2' ,'hoo3', 'hoo4',
                    'et'   ,'mm2'  ]
    wrapper     = 'KSFWVariables({variable})'
    prefix      = 'CS'

    B0_CS_vars  = vc.event_shape + vu.create_aliases(CSvars,wrapper,prefix)
    B0_CS_vars += ['R2','cosTBTO', 'cosTBz', 'thrustBm', 'thrustOm']

    return B0_CS_vars

##########################################################################
# Basic setups

import sys    
if len(sys.argv) != 3:
    print('Usage: basf2 -i inputFile '+ sys.argv[0] + ' jobID  outName ')
    print('')
    print('jobID: an int indentifying the job number. It will be included in the output file name')
    print('outName: output file name, without .root. The final file name will look like: [outName]_num_[jobID].root')
    sys.exit(1)

jobNum = sys.argv[1]
outName = sys.argv[2] 
output_file = outName+'_'+jobNum+'.root'

max_event = 0
path = b2.create_path()
ma.inputMdstList("default", [], path=path)

##########################################################################
# phi reco
phiCuts  = '0.97<M<1.1 '
phiCuts += 'and daughter(0,thetaInCDCAcceptance)==1 '
phiCuts += 'and daughter(1,thetaInCDCAcceptance)==1 '
phiCuts += 'and daughter(0,nCDCHits)>0 '
phiCuts += 'and daughter(1,nCDCHits)>0 '
phiCuts += 'and daughter(0,kaonID)>.05 '
phiCuts += 'and daughter(1,kaonID)>.05 '
ma.fillParticleList('K-', '', path=path)
ma.reconstructDecay('phi:cuts -> K+ K-', phiCuts,  path=path)
vt.treeFit('phi:cuts',     conf_level=0, updateAllDaughters = True,  path=path)
ma.cutAndCopyList('phi:reco','phi:cuts','chiProb>0', path=path)

# K_S0 reco
Ks0Cuts  = 'goodBelleKshort == 1 and chiProb>0'
stdKshorts(path=path)
vt.treeFit('K_S0:merged', conf_level=0, updateAllDaughters = True,  path=path)
ma.cutAndCopyList('K_S0:reco','K_S0:merged',Ks0Cuts, path=path)

# B0 -> phi K_S0 reco
ma.reconstructDecay('B0:rec -> phi:reco K_S0:reco', '', path=path)
vt.treeFit('B0:rec',     conf_level=-1, updateAllDaughters = False,  path=path)
ma.cutAndCopyList('B0:reco','B0:rec','chiProb>0 and 5.2<Mbc<5.3 and abs(deltaE)<0.2',path=path)

# Only carry on if non-zero candidates in B0:reco
skimfilter = b2.register_module('SkimFilter')
skimfilter.param('particleLists', ['B0:reco'])
path.add_module(skimfilter)
emptypath = b2.create_path()
skimfilter.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter.if_false(emptypath,b2.AfterConditionPath.END)

B0_Rec_vars  = setB0RecVars()
B0_Rec_vars += setB0CSVars()

#vt.TagV(list_name='B0:reco',MCassociation='breco',constraintType='noConstraint',path=path)
#default is pi+ and gamma
ma.buildEventShape(foxWolfram=True, path=path)
ma.buildRestOfEvent(target_list_name='B0:reco', path=path)
ma.appendROEMask('B0:reco', 'IPtracks', 'nCDCHits > 0 and abs(d0) < 0.05 and abs(z0) < 0.1', '', path=path)
ma.buildContinuumSuppression('B0:reco', 'IPtracks', path=path)
##########################################################################
# Save B0 reco candidate outputs
path.add_module('VariablesToEventBasedTree',
    particleList='B0:reco',
    variables = B0_Rec_vars,
    treeName = 'B0_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
b2.process(path,max_event)
##########################################################################
