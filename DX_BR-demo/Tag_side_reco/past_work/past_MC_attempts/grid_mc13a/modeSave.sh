#!/bin/csh

set i = 0 
while ($i < 36)
    bsub -o mixed_output/log.$i -e mixed_output/err.$i root -l -b -q 'SaveModeFiles.C('$i',"Rec_1.root","mixed")' 
    @ i++
end

set i = 0 
while ($i < 36)
    bsub -o charged_output/log.$i -e charged_output/err.$i root -l -b -q 'SaveModeFiles.C('$i',"Rec_2.root","charged")' 
    @ i++
end

set i = 0 
while ($i < 36)
    bsub -o uubar_output/log.$i -e uubar_output/err.$i root -l -b -q 'SaveModeFiles.C('$i',"Rec_3.root","uubar")' 
    @ i++
end

set i = 0 
while ($i < 36)
    bsub -o ddbar_output/log.$i -e ddbar_output/err.$i root -l -b -q 'SaveModeFiles.C('$i',"Rec_4.root","ddbar")' 
    @ i++
end

set i = 0 
while ($i < 36)
    bsub -o ssbar_output/log.$i -e ssbar_output/err.$i root -l -b -q 'SaveModeFiles.C('$i',"Rec_5.root","ssbar")' 
    @ i++
end

set i = 0 
while ($i < 36)
    bsub -o ccbar_output/log.$i -e ccbar_output/err.$i root -l -b -q 'SaveModeFiles.C('$i',"Rec_6.root","ccbar")' 
    @ i++
end
