#!/bin/Csh

# Uniform 0.1 cut for baseline
# Update each decMode along the way

set i = 1
while ($i < 2)
    root -l -b -q 'PlotDecMode.C('$i', 0,  .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 1,  .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 2,  .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 3,  .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 4,  .5)' 

    root -l -b -q 'PlotDecMode.C('$i', 14, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 15, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 16, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 17, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 18, .5)' 

    root -l -b -q 'PlotDecMode.C('$i', 19, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 20, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 21, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 22, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 23, .5)' 

    root -l -b -q 'PlotDecMode.C('$i', 24, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 25, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 26, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 27, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 28, .5)' 

    root -l -b -q 'PlotDecMode.C('$i', 29, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 30, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 31, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 32, .5)' 
    root -l -b -q 'PlotDecMode.C('$i', 35, .5)' 
    @ i++
end
