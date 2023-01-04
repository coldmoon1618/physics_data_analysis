#!/bin/csh

nohup gb2_ds_get HashDataJan08 --force &

set i = 1
while ($i < 61)
    echo 'hashtagJan08_'"$i"
    nohup gb2_ds_get hashtagJan08_$i --force & 
    @ i++
end

