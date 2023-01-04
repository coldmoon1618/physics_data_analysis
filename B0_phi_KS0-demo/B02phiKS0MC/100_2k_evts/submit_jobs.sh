#!/bin/csh

set i = 1000
while ($i < 1500)
    mkdir $i
    cd $i
    bsub -o log -e err basf2 /home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/B02phiKS0_EventGeneration.py 2000
    cd ..
    @ i++

end
