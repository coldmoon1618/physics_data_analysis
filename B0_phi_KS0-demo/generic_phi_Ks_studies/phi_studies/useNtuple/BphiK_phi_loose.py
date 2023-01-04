input_files_list = []
output_file = 'A_BphiK_phi_loose.root' 

input_dir = '/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/'
input_file = ''

for ele in range(0,100):
    input_file = input_dir + str(ele) + '/signalevents_2000.mdst.root'
    input_files_list.append(input_file)

import basf2 as b2
import modularAnalysis as ma
import vertex as vt

path = b2.create_path()
ma.inputMdstList("default", input_files_list, path=path)

from stdCharged import stdK

stdK("all", path=path)
ma.reconstructDecay('phi:all -> K+:all K-:all', '0.97 < M < 1.1', path=path)
vt.treeFit('phi:all',     conf_level=0, updateAllDaughters = True, path=path)
ma.matchMCTruth("phi:all", path=path)

ma.variablesToNtuple('phi:all', 
        ['M','p','E','phi','theta',
        'isSignal','chiProb','pt',
        'useCMSFrame(p)',

        'daughter(0,M)','daughter(0,E)','daughter(0,p)','daughter(0,phi)','daughter(0,theta)',
        'daughter(0,useCMSFrame(p))','daughter(0,pt)',
        'daughter(0,electronID)','daughter(0,muonID)','daughter(0,pionID)',
        'daughter(0,kaonID)','daughter(0,protonID)','daughter(0,deuteronID)',
        'daughter(0,thetaInCDCAcceptance)','daughter(0,nCDCHits)','daughter(0,lastCDCLayer)',
        'daughter(0,d0)','daughter(0,z0)',
        'daughter(0,dr)','daughter(0,dz)',

        'daughter(1,M)','daughter(1,E)','daughter(1,p)','daughter(1,phi)','daughter(1,theta)',
        'daughter(1,useCMSFrame(p))','daughter(1,pt)',
        'daughter(1,electronID)','daughter(1,muonID)','daughter(1,pionID)',
        'daughter(1,kaonID)','daughter(1,protonID)','daughter(1,deuteronID)',
        'daughter(1,thetaInCDCAcceptance)','daughter(1,nCDCHits)','daughter(1,lastCDCLayer)',
        'daughter(1,d0)','daughter(1,z0)',
        'daughter(1,dr)','daughter(1,dz)',
        ], filename=output_file, treename="tree", path=path)

b2.process(path, max_event=0)
