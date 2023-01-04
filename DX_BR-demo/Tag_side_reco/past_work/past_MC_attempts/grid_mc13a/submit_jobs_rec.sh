#!/bin/csh

set i = 1
while ($i < 7)
    sed "$i"'q;d' ep3_100ifb.txt > temp.txt
    gbasf2 Reco_MC.py -p Oct27_$i -s light-2106-rhea --input_dslist temp.txt --force
    @ i++

end
