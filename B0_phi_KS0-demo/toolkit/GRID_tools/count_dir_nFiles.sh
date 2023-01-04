#!/bin/csh

set i = 1
while ($i < 61)
    [ -d "Apr28_$i" ] && echo 'Apr28_'"$i" && ls Apr28_$i/sub00 | wc -l
    @ i++
end

