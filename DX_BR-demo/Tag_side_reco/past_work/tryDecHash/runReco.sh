#! /bin/csh

set i = 0 
while ($i < 100 )
    bsub -o output/log.$i -e output/err.$i basf2 Reco_MC.py $i
    @ i++
end
