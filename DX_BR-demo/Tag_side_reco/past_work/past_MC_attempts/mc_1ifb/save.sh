#!/bin/csh

set i = 0 
set BG = 1
while ($i < 100)
    bsub -o log/log -e log/err root -l -b -q 'SaveSingleCand.C("MC_out_had/mixed_Rec_BGx'$BG'_'$i'.root","cand_had/mixed_cand_BGx'$BG'_'$i'.root","mixed")' 
    bsub -o log/log -e log/err root -l -b -q 'SaveSingleCand.C("MC_out_had/charged_Rec_BGx'$BG'_'$i'.root","cand_had/charged_cand_BGx'$BG'_'$i'.root","charged")' 
    bsub -o log/log -e log/err root -l -b -q 'SaveSingleCand.C("MC_out_had/uubar_Rec_BGx'$BG'_'$i'.root","cand_had/uubar_cand_BGx'$BG'_'$i'.root","uubar")' 
    bsub -o log/log -e log/err root -l -b -q 'SaveSingleCand.C("MC_out_had/ddbar_Rec_BGx'$BG'_'$i'.root","cand_had/ddbar_cand_BGx'$BG'_'$i'.root","ddbar")' 
    bsub -o log/log -e log/err root -l -b -q 'SaveSingleCand.C("MC_out_had/ssbar_Rec_BGx'$BG'_'$i'.root","cand_had/ssbar_cand_BGx'$BG'_'$i'.root","ssbar")' 
    bsub -o log/log -e log/err root -l -b -q 'SaveSingleCand.C("MC_out_had/ccbar_Rec_BGx'$BG'_'$i'.root","cand_had/ccbar_cand_BGx'$BG'_'$i'.root","ccbar")' 
    @ i++
end

