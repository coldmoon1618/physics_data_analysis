#!/bin/csh

set i = 1
while ($i < 41)
    echo 'Apr04_'"$i"
    gb2_job_kill -p Apr04_$i --force  
    @ i++

end

