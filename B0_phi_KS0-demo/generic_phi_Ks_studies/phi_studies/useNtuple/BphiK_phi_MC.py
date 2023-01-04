input_files_list = []
output_file = 'MCBphiK_phi_MC.root' 

input_dir = '/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/'
input_file = ''

for ele in range(0,100):
    input_file = input_dir + str(ele) + '/signalevents_2000.mdst.root'
    input_files_list.append(input_file)

import basf2 as b2
import modularAnalysis as ma

path = b2.create_path()
ma.inputMdstList("default", input_files_list, path=path)

from modularAnalysis import fillParticleListFromMC
fillParticleListFromMC('phi:generator', '', writeOut = True, path=path) 

ma.variablesToNtuple('phi:generator', 
        ['InvM','M','p','E','phi','theta',
        'useCMSFrame(p)','useCMSFrame(E)','useCMSFrame(phi)','useCMSFrame(theta)',
        'mcMother(PDG)','mcPDG','mcDaughter(0,PDG)','mcDaughter(1,PDG)','mcDaughter(2,PDG)',
        
        'mcDaughter(0,p)','mcDaughter(0,phi)','mcDaughter(0,theta)','mcDaughter(0,pt)',
        'mcDaughter(0,useCMSFrame(p))','mcDaughter(0,useCMSFrame(phi))','mcDaughter(0,useCMSFrame(theta))',

        'mcDaughter(1,p)','mcDaughter(1,phi)','mcDaughter(1,theta)','mcDaughter(1,pt)',
        'mcDaughter(1,useCMSFrame(p))','mcDaughter(1,useCMSFrame(phi))','mcDaughter(1,useCMSFrame(theta))',

        ], filename=output_file, treename="tree", path=path)

b2.process(path, max_event=0)
