#!/bin/csh

while( -d "Apr12_1" || -d "Apr12_2" || -d "Apr12_3" || -d "Apr12_4" || -d "Apr12_5" || -d "Apr12_6" || -d "Apr12_7" || -d "Apr12_8" || -d "Apr12_9" ) 
    echo "Delete!"
    rm -rf Apr*
    sleep 30s
end
