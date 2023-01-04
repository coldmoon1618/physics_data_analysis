#!/bin/csh

set i = 1 
while ($i < 201)
    bsub -o log -e err root -l -b -q 'SaveSingleCand.C(1,0,"/home/belle2/zhai/B0_phi_KS0/Full_reco/getNtuple/SigMC_poseidon/output/SigMC_B0_'$i'.root","/home/belle2/zhai/B0_phi_KS0/Full_reco/getNtuple/SigMC_poseidon/mcDeltaTau/SigMC_mcDeT_'$i'.root","reco_cand_outs/SigMC_cand_'$i'.root",.025)' #i=1...201
    @ i++
end

