#!/bin/csh

set i = 1
while ($i < 61)
    sed "$i"'q;d' eph3_1iab.list > temp.txt
    gbasf2 Reco_MC.py -p hashtagJan08_$i -s light-2106-rhea --input_dslist temp.txt --force
    @ i++

end

rm -f temp.txt
