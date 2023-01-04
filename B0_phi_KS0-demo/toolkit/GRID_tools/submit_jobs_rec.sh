#!/bin/csh

set i = 1
while ($i < 37)
    sed "$i"'q;d' ep3_1iab.txt > temp.txt
    gbasf2 MC13a_B0_select.py -p B0_reco_eff_$i -s light-2002-ichep --input_dslist temp.txt --force
    @ i++

end
