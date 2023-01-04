#!/bin/csh

set i = 200
while ($i < 500)
    [ -d "$i" ] && echo "$i" && rm -rf $i
    @ i++
end

