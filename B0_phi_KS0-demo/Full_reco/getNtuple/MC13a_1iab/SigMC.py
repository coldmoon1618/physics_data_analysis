##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu
import flavorTagger as ft

#from buildVars import *
##########################################################################
vm.addAlias('pstar', 'useCMSFrame(p)')
vm.addAlias('thetaStar','useCMSFrame(theta)')
vm.addAlias('nDaus', 'nDaughters')
vm.addAlias('motherDr','mcMother(dr)')

def setRecTrkVars():
    print('Building reco track variables')
    
    trks_vars    = vc.kinematics + ['thetaStar','M','theta','phi','pstar'] + vc.track + vc.track_hits + vc.pid 
    trks_vars   += ['thetaInCDCAcceptance','inCDCAcceptance','lastCDCLayer','isCloneTrack']
    trks_vars   += ['mcPhi','mcTheta','mcPT','mcP','genParticleID','motherDr','mcProdVertexDX','mcProdVertexDY','mcProdVertexDZ']

    return trks_vars

def setMCDauVars():
    print('Building MC daughter variables')
    
    daus_vars   = vc.kinematics + ['M','theta','phi','pstar','thetaStar'] + vc.track 
    daus_vars  += ['thetaInCDCAcceptance','inCDCAcceptance','lastCDCLayer']
    daus_vars  += ['PDG','mdstIndex','mcSecPhysProc', 'isReconstructible', 'seenInCDC']
    daus_vars  += ['genParticleID','motherDr','mcProdVertexDX','mcProdVertexDY','mcProdVertexDZ']

    return daus_vars

def setPhiRecVars():
    print('Building phi_Rec variables')

    Phi_Rec_vars = vc.kinematics + ['M','theta','phi','pstar','thetaStar'] + ['chiProb', 'isOrHasCloneTrack']
    Phi_Rec_vars+= ['genParticleID','isSignal','nMCMatches']

    trks_vars = setRecTrkVars() 
    
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Phi_Rec_vars

def setKsRecVars():
    print('Building Ks_Rec variables')

    Ks_Rec_vars = vc.kinematics + ['M','theta','phi','pstar','thetaStar'] + ['chiProb','goodBelleKshort','isOrHasCloneTrack']
    Ks_Rec_vars+= ['distance','significanceOfDistance'] + vc.flight_info
    Ks_Rec_vars+= ['genParticleID','isSignal','nMCMatches']
    
    trks_vars = setRecTrkVars() 
    
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Ks_Rec_vars

def setPhiKsMCVars():

    My_MC_vars  = vc.kinematics + ['M','theta','phi','pstar','thetaStar'] + ['nDaus','mdstIndex','mcSecPhysProc']
    My_MC_vars += vc.mc_flight_info + ['genParticleID']

    daus_vars = setMCDauVars()
    
    My_MC_vars += vu.create_aliases(daus_vars, 'daughter(0,{variable})','dau_0')
    My_MC_vars += vu.create_aliases(daus_vars, 'daughter(1,{variable})','dau_1')
    My_MC_vars += vu.create_aliases(daus_vars, 'daughter(2,{variable})','dau_2')
    My_MC_vars += vu.create_aliases(daus_vars, 'daughter(3,{variable})','dau_3')
    
    return My_MC_vars

def setPhiMCVars():
    print('Building phi_MC variables')
    
    return setPhiKsMCVars()

def setKsMCVars():
    print('Building Ks_MC variables')
    
    return setPhiKsMCVars()

def setB0RecVars():
    print('Building B0_Rec variables')

    vm.addAlias('flvrTag_FBDT','qrOutput(FBDT)')
    vm.addAlias('flvrTag_FANN','qrOutput(FANN)')
    vm.addAlias('phi_cosHel','cosHelicityAngle(0,0)')
    vm.addAlias('qrMC','isRelatedRestOfEventB0Flavor')
    vm.addAlias('hasRoeTrks','hasRestOfEventTracks')
    B0_Rec_vars = vc.kinematics + ['M','theta','phi','pstar','thetaStar','Mbc','deltaE','chiProb']
    B0_Rec_vars+= ['flvrTag_FBDT','flvrTag_FANN','DeltaZ','DeltaZErr']
    B0_Rec_vars+= ['DeltaT','DeltaTErr','DeltaBoost','DeltaBoostErr']
    B0_Rec_vars+= ft.flavor_tagging + ['hasRoeTrks']
    B0_Rec_vars+= ['genParticleID','mcPDG','isSignal','nMCMatches']
    B0_Rec_vars+= ['qrMC','MCDeltaT','mcErrors','mcFlavorOfOtherB']
    Phi_Rec_vars= setPhiRecVars()
    Ks_Rec_vars = setKsRecVars()

    B0_Rec_vars+= ['phi_cosHel']
    B0_Rec_vars+= vu.create_aliases(Phi_Rec_vars, 'daughter(0,{variable})','phi')
    B0_Rec_vars+= vu.create_aliases(Ks_Rec_vars,  'daughter(1,{variable})','Ks')

    print('building continuum suppression vars')
    CSvars      = ['hso00','hso02','hso04',
                   'hso10','hso12','hso14',
                   'hso20','hso22','hso24',
                   'hoo1' ,'hoo2' ,'hoo3', 'hoo4',
                   'et'   ,'mm2'  ]
    wrapper     = 'KSFWVariables({variable})'
    prefix      = 'CS'
    
    B0_Rec_vars+= vc.event_shape + vu.create_aliases(CSvars,wrapper,prefix)
    B0_Rec_vars+= ['R2','cosTBTO','cosTBz','thrustOm','thrustBm']
    
    return B0_Rec_vars

def setB0MCVars():
    print('Building B0_MC variables')

    B0_MC_vars  = vc.kinematics + ['PDG','M','theta','phi','pstar','thetaStar','Mbc','deltaE']
    B0_MC_vars += ['mdstIndex','genParticleID'] 
    Phi_MC_vars = setPhiMCVars()
    Ks_MC_vars  = setKsMCVars()

    B0_MC_vars += vu.create_aliases(Phi_MC_vars, 'daughter(0,{variable})','phi')
    B0_MC_vars += vu.create_aliases(Ks_MC_vars,  'daughter(1,{variable})','Ks')

    return B0_MC_vars

def setY4Schain():
    print('Building Y4S variables')
    n=10
    
    list_of_indices = [str(i) for i in range(n)]

    myCollection = []
    wrapper     = 'genParticle({variable}, varForMCGen(mcMother(mdstIndex)))'
    prefix      = 'MothIndex'
    myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

    wrapper     = 'genParticle({variable}, mcDaughter(0,mdstIndex))'
    prefix      = 'firstDau'
    myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

    decayChainVarList = ['PDG']

    for decVars in decayChainVarList:
        wrapper = 'genParticle({variable}, varForMCGen('+decVars+'))'
        prefix  = decVars
        myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

    vm.addAlias('nMCGen', 'nMCParticles')
    Y4Schain_vars = ['nMCGen']

    for i in range(n):
        for ele in myCollection:
            Y4Schain_vars.append(ele[i])

    return Y4Schain_vars

B0_Rec_vars  = setB0RecVars()
B0_MC_vars   = setB0MCVars()
Y4Schain_vars= setY4Schain()
##########################################################################
# Analysis modules
import basf2 as b2
import modularAnalysis as ma
import vertex as vt
import flavorTagger as ft
from stdV0s import stdKshorts

##########################################################################
# Basic setups
import sys
if len(sys.argv) != 2:
    print('Usage: basf2 SigMC_B0_noUpdate.py index')
    print('')
    sys.exit(1)

ele = sys.argv[1]

input_file_list = []
input_file_list.append('/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/' + str(ele) + '/signalevents_2000.mdst.root')
output_file = 'Rec_'+str(ele)+'.root'
max_event = 0

path = b2.create_path()
ma.inputMdstList("default", input_file_list, path=path)

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
ma.cutAndCopyList('B0:reco','B0:rec','chiProb>0',path=path)
ma.matchMCTruth('B0:reco',      path=path)

##########################################################################
# Only carry on if non-zero candidates in B0:reco
skimfilter = b2.register_module('SkimFilter')
skimfilter.param('particleLists', ['B0:reco'])
path.add_module(skimfilter)
emptypath = b2.create_path()
skimfilter.if_true(emptypath,b2.AfterConditionPath.CONTINUE)
skimfilter.if_false(emptypath,b2.AfterConditionPath.END)

#default is pi+ and gamma
ma.buildEventShape(foxWolfram=True, path=path)
ma.buildRestOfEvent(target_list_name='B0:reco', path=path)
ma.appendROEMask('B0:reco', 'IPtracks', 'nCDCHits > 0 and abs(d0) < 0.05 and abs(z0) < 0.1', '', path=path)
ma.buildContinuumSuppression('B0:reco', 'IPtracks', path=path)
ft.flavorTagger('B0:reco',belleOrBelle2='Belle2', weightFiles='B2nunubarBGx1', path=path)
vt.TagV(list_name='B0:reco',MCassociation='breco',constraintType='noConstraint',path=path)

##########################################################################
# Save B0 reco candidate outputs
path.add_module('VariablesToEventBasedTree',
    particleList='B0:reco',
    variables = B0_Rec_vars,
    treeName = 'B0_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
# Save MC B0 with signal flag
B2phiKsCut  = 'nDaughters==2 and daughter(0,mcPDG)==333 and daughter(1,mcPDG)==310 '
#B2phiKsCut += 'and daughter(0,nDaughters)>=2 and daughter(1,nDaughters)>=2 '
#B2phiKsCut += 'and abs(daughter(0,daughter(0,mcPDG)))==321 and abs(daughter(0,daughter(1,mcPDG)))==321 '
#B2phiKsCut += 'and abs(daughter(1,daughter(0,mcPDG)))==211 and abs(daughter(1,daughter(1,mcPDG)))==211'

ma.fillParticleListFromMC('B0:MC', B2phiKsCut, addDaughters = True, skipNonPrimaryDaughters = False, path = path)

path.add_module('VariablesToEventBasedTree',
    particleList='B0:MC',
    variables= B0_MC_vars, 
    treeName = 'B0_MC',
    fileName = output_file)

# Save partial decay chain
ma.fillParticleListFromMC('Upsilon(4S):MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path)

path.add_module('VariablesToEventBasedTree',
    particleList= 'Upsilon(4S):MC',
    variables   = Y4Schain_vars,
    treeName    = 'Y4S_chain',
    fileName    = output_file)

##########################################################################
b2.process(path,max_event)
##########################################################################
