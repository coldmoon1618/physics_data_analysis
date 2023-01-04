#!/bin/csh

set i = 0 
while ($i < 36)
    bsub root -l -b -q 'PlotData.C('$i',.01)' 
    @ i++
end
set i = 0 
while ($i < 36)
    bsub root -l -b -q 'PlotData.C('$i',.1)' 
    @ i++
end
set i = 0 
while ($i < 36)
    bsub root -l -b -q 'PlotData.C('$i',.5)' 
    @ i++
end
