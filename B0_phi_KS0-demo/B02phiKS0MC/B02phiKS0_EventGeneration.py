import sys
import os

if __name__=='__main__':
    if len(sys.argv) < 2 :
        sys.exit(1)
    numEvent = int(sys.argv[1])

import basf2 as b2
path = b2.create_path()
setter = path.add_module("EventInfoSetter", evtNumList=[numEvent])
b2.print_params(setter)

generator = path.add_module("EvtGenInput", userDECFile='/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/Bd_phiKS_K+K-pi+pi-.dec')
b2.print_params(generator)

from simulation import add_simulation
add_simulation(path)

from reconstruction import add_reconstruction
add_reconstruction(path)

from reconstruction import add_mdst_output
add_mdst_output(path, filename='signalevents_'+str(numEvent)+'.mdst.root')

b2.process(path)

#usage
#bsub -o log -e err basf2 /home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/B02phiKS0_EventGeneration.py 2000
