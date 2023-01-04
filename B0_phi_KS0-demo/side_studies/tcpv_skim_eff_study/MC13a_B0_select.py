import basf2 as b2
import modularAnalysis as ma
import vertex as vt
import flavorTagger as ft
import variables.collections as vc
import variables.utils as vu
from variables import variables as vm
from stdV0s import stdKshorts

vm.addAlias('pstar', 'useCMSFrame(p)')
vm.addAlias('nDaus', 'nDaughters')

def setRecTrkVars():
    print('Building reco track variables')
    
    trks_vars    = vc.kinematics + ['M','theta','phi','pstar'] + vc.track + vc.track_hits + vc.pid 
    trks_vars   += ['thetaInCDCAcceptance','inCDCAcceptance','lastCDCLayer']

    return trks_vars

def setMCDauVars():
    print('Building MC daughter variables')
    
    daus_vars   = vc.kinematics + ['M','theta','phi','pstar'] + ['dr','dx','dy','dz']
    daus_vars  += ['thetaInCDCAcceptance','inCDCAcceptance']
    daus_vars  += ['PDG','mdstIndex']

    return daus_vars

def setPhiRecVars():
    print('Building phi_Rec variables')
    
    Phi_Rec_vars = vc.kinematics + ['M','theta','phi','pstar'] + ['chiProb','isSignal']

    trks_vars = setRecTrkVars() 
    
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Phi_Rec_vars

def setKsRecVars():
    print('Building Ks_Rec variables')

    Ks_Rec_vars = vc.kinematics + ['M','theta','phi','pstar'] + ['chiProb','goodBelleKshort','isSignal']
    Ks_Rec_vars+= ['distance','significanceOfDistance'] + vc.flight_info
    
    trks_vars = setRecTrkVars() 
    
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Ks_Rec_vars

def setPhiKsMCVars():

    vm.addAlias('pstar', 'useCMSFrame(p)')
    My_MC_vars  = vc.kinematics + ['M','theta','phi','pstar'] + ['nDaus','mdstIndex']
    My_MC_vars += vc.mc_flight_info 

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

    B0_Rec_vars = vc.kinematics + ['M','theta','phi','pstar','isSignal','Mbc','deltaE','chiProb']
    Phi_Rec_vars= setPhiRecVars()
    Ks_Rec_vars = setKsRecVars()

    B0_Rec_vars+= vu.create_aliases(Phi_Rec_vars, 'daughter(0,{variable})','phi')
    B0_Rec_vars+= vu.create_aliases(Ks_Rec_vars,  'daughter(1,{variable})','Ks')

    return B0_Rec_vars

def setB0MCVars():
    print('Building B0_MC variables')

    B0_MC_vars  = vc.kinematics + ['M','theta','phi','pstar','mdstIndex','Mbc','deltaE']
    Phi_MC_vars = setPhiMCVars()
    Ks_MC_vars  = setKsMCVars()

    B0_MC_vars += vu.create_aliases(Phi_MC_vars, 'daughter(0,{variable})','phi')
    B0_MC_vars += vu.create_aliases(Ks_MC_vars,  'daughter(1,{variable})','Ks')

    return B0_MC_vars

def setY4Schain():
    print('Building Y4S variables')
    n=200
    
    list_of_indices = [str(i) for i in range(n)]

    myCollection = []
    wrapper     = 'genParticle({variable}, varForMCGen(mcMother(mdstIndex)))'
    prefix      = 'MothIndex'
    myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

    wrapper     = 'genParticle({variable}, varForMCGen(useCMSFrame(p)))'
    prefix      = 'pstar'
    myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

    wrapper     = 'genParticle({variable}, mcDaughter(0,mdstIndex))'
    prefix      = 'firstDau'
    myCollection.append(vu.create_aliases(list_of_indices, wrapper, prefix))

    decayChainVarList = ['PDG','E','p','px','py','pz','pt','mcFSR','mcISR','thetaInCDCAcceptance']
    decayChainVarList += ['dx','dy','dz','dr','dcosTheta','dphi','distance']

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

##########################################################################
# Basic setups
input_file_list = []
#for ele in range(0,1):
#    input_file_list.append('/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/' + str(ele) + '/signalevents_2000.mdst.root')

output_file = 'MC13a_B0_Rec.root'
max_event = 0

path1 = b2.create_path()
ma.inputMdstList("default", input_file_list, path=path1)

phi_Rec_vars = setPhiRecVars()
phi_MC_vars  = setPhiMCVars()
Ks_Rec_vars  = setKsRecVars()
Ks_MC_vars   = setKsMCVars()
B0_Rec_vars  = setB0RecVars()
B0_MC_vars   = setB0MCVars()
Y4Schain_vars= setY4Schain()

##########################################################################
# Extract signal events
# skip the rest of path1 if no signal decay chain
B2phiKsCut  = 'nDaughters==2 and daughter(0,mcPDG)==333 and daughter(1,mcPDG)==310'
ma.fillParticleListFromMC('B0:MC', B2phiKsCut, addDaughters = True, skipNonPrimaryDaughters = True, path = path1)

skimfilter1 = b2.register_module('SkimFilter')
skimfilter1.param('particleLists', ['B0:MC'])
path1.add_module(skimfilter1)
emptypath1 = b2.create_path()
skimfilter1.if_true(emptypath1,b2.AfterConditionPath.CONTINUE)
skimfilter1.if_false(emptypath1,b2.AfterConditionPath.END)

##########################################################################
# Saving in the order Y4S_chain, B0_MC, B0_rec, phi_rec, Ks_rec
##########################################################################
# Save entire decay chain
ma.fillParticleListFromMC('Upsilon(4S):MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

path1.add_module('VariablesToEventBasedTree',
    particleList= 'Upsilon(4S):MC',
    variables   = Y4Schain_vars,
    treeName    = 'Y4S_chain',
    fileName    = output_file)

##########################################################################
# Save MC B0 info

path1.add_module('VariablesToEventBasedTree',
    particleList='B0:MC',
    variables= B0_MC_vars, 
    treeName = 'B0_MC',
    fileName = output_file)

##########################################################################
# Full reco to assess cut efficiency
# phi reco
ma.fillParticleList('K-', '', path=path1)
ma.reconstructDecay('phi:reco -> K+ K-', '0.97<M<1.1',  path=path1)
vt.treeFit('phi:reco',     conf_level=-1, updateAllDaughters = True,  path=path1)
ma.matchMCTruth('phi:reco',      path=path1)

# K_S0 reco
stdKshorts(path=path1)
vt.treeFit('K_S0:merged', conf_level=-1, updateAllDaughters = True,  path=path1)
ma.matchMCTruth('K_S0:merged',      path=path1)

# B0 -> phi K_S0 reco
ma.reconstructDecay('B0:reco -> phi:reco K_S0:merged', '', path=path1)
vt.treeFit('B0:reco',     conf_level=-1, updateAllDaughters = True,  path=path1)
ma.matchMCTruth('B0:reco',      path=path1)

##########################################################################
# Save B0 reco candidate outputs
path1.add_module('VariablesToEventBasedTree',
    particleList='B0:reco',
    variables = B0_Rec_vars,
    treeName = 'B0_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
# Save phi reco candidate outputs
path1.add_module('VariablesToEventBasedTree',
    particleList='phi:reco',
    variables= phi_Rec_vars, 
    treeName = 'phi_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
# Save phi MC candidate outputs
ma.fillParticleListFromMC('phi:MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

path1.add_module('VariablesToEventBasedTree',
    particleList='phi:MC',
    variables= phi_MC_vars,
    treeName = 'phi_MC',
    fileName = output_file)

##########################################################################
# Save Ks  reco candidate outputs

path1.add_module('VariablesToEventBasedTree',
    particleList='K_S0:merged',
    variables= Ks_Rec_vars,
    treeName = 'Ks_Rec',
    fileName = output_file,
    maxCandidates = 9999)

##########################################################################
# Save Ks  MC candidate outputs
ma.fillParticleListFromMC('K_S0:MC','',addDaughters=True, skipNonPrimaryDaughters = False, path=path1)

path1.add_module('VariablesToEventBasedTree',
    particleList='K_S0:MC',
    variables= Ks_MC_vars,
    treeName = 'Ks_MC',
    fileName = output_file)

##########################################################################
b2.process(path1,max_event)
##########################################################################
