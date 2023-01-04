#!/bin/csh

set i = 0 
while ($i < 36)
    bsub -o full_output/log.$i -e full_output/err.$i root -l -b -q 'FullSaveModeFiles.C('$i')' 
    @ i++
end
