cd hashtagJan08_output

hadd -f AllModes_Mixed.root   skimOut_1_*  skimOut_7_* skimOut_13_*
hadd -f AllModes_Charged.root skimOut_2_*  skimOut_8_* skimOut_14_* skimOut_20_* 
hadd -f AllModes_UUbar.root   skimOut_3_*  skimOut_9_* skimOut_15_*
hadd -f AllModes_DDbar.root   skimOut_4_* skimOut_10_* skimOut_16_*
hadd -f AllModes_SSbar.root   skimOut_5_* skimOut_11_* skimOut_17_*
hadd -f AllModes_CCbar.root   skimOut_6_* skimOut_12_* skimOut_18_*

hadd -f AllModes_MC.root      AllModes_Mixed.root AllModes_Charged.root AllModes_UUbar.root AllModes_DDbar.root AllModes_SSbar.root AllModes_CCbar.root

cd ../.
