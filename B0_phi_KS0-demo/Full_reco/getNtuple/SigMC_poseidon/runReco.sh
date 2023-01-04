#!/bin/csh

set i = 1
while ($i < 201)
    bsub -o log -e err basf2 Poseidon_reco.py "$i"
    @ i++

end

