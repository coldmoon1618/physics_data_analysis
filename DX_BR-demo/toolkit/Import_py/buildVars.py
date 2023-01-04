# Compatible with release-05-02-17
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

##########################################################################

def setB_TagVars(isMC = True):
    print('Building B_Tag variables')

    vm.addAlias('SigProb', 'extraInfo(SignalProbability)')
    vm.addAlias('decMode', 'extraInfo(decayModeID)')
    B_Tag_vars = vc.kinematics + ['PDG','M','theta','phi','Mbc','deltaE','Ecms']
    if(isMC):
        B_Tag_vars+= ['genParticleID','mcPDG','isSignal','isSignalAcceptMissing','isSignalAcceptMissingGamma','isSignalAcceptWrongFSPs','mcErrors','nMCMatches']
    B_Tag_vars+= ['SigProb','decMode']

    return B_Tag_vars
def setB_MCVars():
    print('Building B_MC variables')

    list_of_indices = [str(i) for i in range(6)]
    B_MC_vars  = ['charge','PDG']

    wrapper    = 'daughter({variable},PDG)'
    prefix     = 'PDG'
    B_MC_vars += vu.create_aliases(list_of_indices,wrapper,prefix)

    list_of_indices = [str(i) for i in range(6)]
    for i in range(5):
        wrapper    = 'daughter('+str(i)+',daughter({variable},PDG))'
        prefix     = 'PDG_'+str(i)
        B_MC_vars += vu.create_aliases(list_of_indices,wrapper,prefix)

    return B_MC_vars
