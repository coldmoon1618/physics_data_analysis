input_files_list = []
output_file = 'MCBphiK_phi_reco.root' 

input_dir = '/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/'
input_file = ''

for ele in range(0,100):
    input_file = input_dir + str(ele) + '/signalevents_2000.mdst.root'
    input_files_list.append(input_file)

import basf2 as b2
import modularAnalysis as ma

path = b2.create_path()
ma.inputMdstList("default", input_files_list, path=path)

from stdCharged import stdK, stdPi
stdK("all", path=path)

ma.reconstructDecay('phi:all -> K+:all K-:all', 'M < 1.06', path=path)

ma.vertexTree('phi:all',     conf_level=0.001, updateAllDaughters = True, path=path)

ma.variablesToNtuple('phi:all', 
        ['InvM','M','p','E','phi','theta',
        'useCMSFrame(p)','useCMSFrame(E)','useCMSFrame(phi)','useCMSFrame(theta)',
        'isSignal',

        'daughter(0,p)','daughter(0,phi)','daughter(0,theta)','daughter(0,pt)',
        'daughter(0,useCMSFrame(p))','daughter(0,useCMSFrame(phi))','daughter(0,useCMSFrame(theta))',
        'daughter(0,kaonID)',

        'daughter(1,p)','daughter(1,phi)','daughter(1,theta)','daughter(1,pt)',
        'daughter(1,useCMSFrame(p))','daughter(1,useCMSFrame(phi))','daughter(1,useCMSFrame(theta))',
        'daughter(1,kaonID)',

        ], filename=output_file, treename="tree", path=path)

b2.process(path, max_event=0)
