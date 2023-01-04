#!/bin/csh

scp kekcc:DX_BR/Tag_side_reco/Data_73ifb/Round1_cand.root .
mkdir full_output
scp 'kekcc:DX_BR/Tag_side_reco/Data_73ifb/full_output/*root' full_output/.
mkdir output
scp 'kekcc:DX_BR/Tag_side_reco/Data_73ifb/output/*root' output/.
