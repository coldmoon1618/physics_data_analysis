#!/bin/csh

set i = 60
while ($i < 61)
    [ -d "Mar421_$i" ] && cd Mar421_$i/sub00
    bsub hadd Reco_$i.root M*root
    cd ../..
    @ i++

end
