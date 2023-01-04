#!/bin/csh

set i = 0
while ($i<36)
    #./build.sh $i
    #sleep 20s
    #./combine.sh $i
    basf2 SetToNames.py $i
    @ i++
end
