#! /bin/bash

cd ~/Desktop/myB2Ana/B0_phi_K0/BDT_response/
root -l -b -q 'GetResponse.C("SigMC")'
ln -f SigMC_resp.root ~/Desktop/myB2Ana/B0_phi_K0/A_CP_study/.
