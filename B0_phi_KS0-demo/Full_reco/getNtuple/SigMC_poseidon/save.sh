#!/bin/csh

set i = 1
while ($i < 201)
    bsub -o log -e err root -l -b -q 'Fetch_mDST_info.C('$i')'
    @ i++

end

