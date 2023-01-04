# contain some long list of variables that later deprecated

from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

vm.addAlias('pstar', 'useCMSFrame(p)')
vm.addAlias('thetaStar','useCMSFrame(theta)')
vm.addAlias('nDaus', 'nDaughters')
vm.addAlias('motherDr','mcMother(dr)')

def setRecTrkVars():
    print('Building reco track variables')
    
    trks_vars    = vc.kinematics + ['thetaStar','mcPhi','mcTheta','mcPT','mcP','M','theta','phi','pstar'] + vc.track + vc.track_hits + vc.pid 
    trks_vars   += ['thetaInCDCAcceptance','inCDCAcceptance','lastCDCLayer','isCloneTrack', 'nMCMatches']
    trks_vars   += ['genParticleID','motherDr','mcProdVertexDX','mcProdVertexDY']

    return trks_vars

def setMCDauVars():
    print('Building MC daughter variables')
    
    daus_vars   = vc.kinematics + ['M','theta','phi','pstar','thetaStar'] + ['dr','dx','dy','dz']
    daus_vars  += ['thetaInCDCAcceptance','inCDCAcceptance', 'isReconstructible', 'seenInCDC']
    daus_vars  += ['PDG','mdstIndex','mcSecPhysProc']
    daus_vars  += ['genParticleID','motherDr','mcProdVertexDX','mcProdVertexDY']

    return daus_vars

def setPhiRecVars():
    print('Building phi_Rec variables')
    
    Phi_Rec_vars = vc.kinematics + ['M','theta','phi','pstar'] + ['chiProb','isSignal', 'isOrHasCloneTrack', 'nMCMatches']
    Phi_Rec_vars+= ['genParticleID']

    trks_vars = setRecTrkVars() 
    
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Phi_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Phi_Rec_vars

def setKsRecVars():
    print('Building Ks_Rec variables')

    Ks_Rec_vars = vc.kinematics + ['M','theta','phi','pstar'] + ['chiProb','goodBelleKshort','isSignal', 'isOrHasCloneTrack', 'nMCMatches']
    Ks_Rec_vars+= ['distance','significanceOfDistance'] + vc.flight_info
    Ks_Rec_vars+= ['genParticleID']
    
    trks_vars = setRecTrkVars() 
    
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(0,{variable})','dau_0')
    Ks_Rec_vars+= vu.create_aliases(trks_vars, 'daughter(1,{variable})','dau_1')

    return Ks_Rec_vars

def setPhiKsMCVars():

    vm.addAlias('pstar', 'useCMSFrame(p)')
    My_MC_vars  = vc.kinematics + ['M','theta','phi','pstar'] + ['nDaus','mdstIndex','mcSecPhysProc']
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
    B0_Rec_vars = vc.kinematics + ['M','theta','phi','pstar','isSignal','Mbc','deltaE','chiProb', 'nMCMatches']
    B0_Rec_vars+= ['genParticleID','flvrTag_FBDT','flvrTag_FANN','DeltaZ','DeltaZErr']
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

    return B0_CS_vars

def setB0MCVars():
    print('Building B0_MC variables')

    B0_MC_vars  = vc.kinematics + ['PDG','M','theta','phi','pstar','mdstIndex','Mbc','deltaE']
    B0_MC_vars += ['genParticleID'] 
    Phi_MC_vars = setPhiMCVars()
    Ks_MC_vars  = setKsMCVars()

    B0_MC_vars += vu.create_aliases(Phi_MC_vars, 'daughter(0,{variable})','phi')
    B0_MC_vars += vu.create_aliases(Ks_MC_vars,  'daughter(1,{variable})','Ks')

    return B0_MC_vars

def setY4Schain():
    print('Building Y4S variables')
    n=3
    
    list_of_indices = [str(i) for i in range(n)]

    decayChainVarList = ['PDG','E','M','px','py','pz','pstar']

    myCollection = []

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
'''
def setY4Schain():
    print('Building Y4S variables')
    n=10
    
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
    decayChainVarList += ['dx','dy','dz','dr','dcosTheta','dphi','distance','mcSecPhysProc']

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
'''
