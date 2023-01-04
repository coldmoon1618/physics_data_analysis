#!/bin/csh

set i = 1
while ($i < 61)
    mv Mar421_$i/sub00/Reco_* .
    @ i++

end
