#!/bin/csh

cd mcTypes
mkdir mixed_output
scp 'kekcc:DX_BR/Tag_side_reco/MC14ri_c_GRID/mixed_output/*root' mixed_output/.
mkdir charged_output
scp 'kekcc:DX_BR/Tag_side_reco/MC14ri_c_GRID/charged_output/*root' charged_output/.
mkdir uubar_output
scp 'kekcc:DX_BR/Tag_side_reco/MC14ri_c_GRID/uubar_output/*root' uubar_output/.
mkdir ddbar_output
scp 'kekcc:DX_BR/Tag_side_reco/MC14ri_c_GRID/ddbar_output/*root' ddbar_output/.
mkdir ssbar_output
scp 'kekcc:DX_BR/Tag_side_reco/MC14ri_c_GRID/ssbar_output/*root' ssbar_output/.
mkdir ccbar_output
scp 'kekcc:DX_BR/Tag_side_reco/MC14ri_c_GRID/ccbar_output/*root' ccbar_output/.
cd ../.
