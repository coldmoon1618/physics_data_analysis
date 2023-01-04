#!/bin/csh

set i = 1
while ($i < 37)
    echo 'B0_reco_eff_'"$i"
    gb2_ds_list B0_reco_eff_$i/sub00|wc -l 
    @ i++
end

set i = 1
while ($i < 13)
    echo 'TcpvSkimEff_'"$i"
    gb2_ds_list TcpvSkimEff_$i/sub00|wc -l 
    @ i++
end

