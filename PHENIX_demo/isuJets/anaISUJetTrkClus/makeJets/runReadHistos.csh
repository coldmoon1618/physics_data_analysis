#!/bin/csh
source /etc/csh.login
    foreach i (/etc/profile.d/*.csh)
     source $i
    end
    source /opt/phenix/bin/phenix_setup.csh

    setenv LD_LIBRARY_PATH /sphenix/user/yzhai/ISUJetAna/install/lib:${LD_LIBRARY_PATH}
    setenv TSEARCHPATH /sphenix/user/yzhai/ISUJetAna/install

set i = 0
while ($i <= 4)
  root -l -q -b "readHistos.C(1,$i)" 
  root -l -q -b "readHistos.C(2,$i)" 
  echo $i
  @ i++
end
