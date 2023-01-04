#!/bin/csh

set i = 5
while ($i < 56)
    bsub -o log -e err basf2 Poseidon_reco.py "$i"
    @ i++

end

