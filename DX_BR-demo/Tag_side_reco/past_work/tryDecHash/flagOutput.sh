#! /bin/csh

set i = 1 
while ($i < 100)
    bsub -o output/log.$i -e output/err.$i basf2 Flag_output.py $i
    @ i++
end
