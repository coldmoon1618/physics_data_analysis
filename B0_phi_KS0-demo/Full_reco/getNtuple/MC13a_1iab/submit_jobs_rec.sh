#!/bin/csh

set i = 1
while ($i < 61)
    sed "$i"'q;d' ep3_1iab.txt > temp.txt
    gbasf2 Reco_MC13a.py -p May14_$i -s light-2002-ichep --input_dslist temp.txt --force
    @ i++

end
