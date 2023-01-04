#!/bin/csh

set i = 1
while ($i < 10)
    echo 'Apr12_'"$i"
    nohup gb2_ds_get Apr12_$i --force & 
    @ i++

end

