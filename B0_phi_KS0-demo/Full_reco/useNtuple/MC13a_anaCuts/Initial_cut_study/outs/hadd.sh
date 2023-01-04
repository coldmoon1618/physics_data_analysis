#!/bin/csh

hadd mixed.root      MC_cand_1.root  MC_cand_2.root  MC_cand_3.root  MC_cand_4.root  MC_cand_5.root  MC_cand_6.root  MC_cand_7.root  MC_cand_8.root  MC_cand_9.root MC_cand_10.root
hadd charged.root   MC_cand_11.root MC_cand_12.root MC_cand_13.root MC_cand_14.root MC_cand_15.root MC_cand_16.root MC_cand_17.root MC_cand_18.root MC_cand_19.root MC_cand_20.root
hadd uubar.root     MC_cand_21.root MC_cand_22.root MC_cand_23.root MC_cand_24.root MC_cand_25.root MC_cand_26.root MC_cand_27.root MC_cand_28.root MC_cand_29.root MC_cand_30.root
hadd ddbar.root     MC_cand_31.root MC_cand_32.root MC_cand_33.root MC_cand_34.root MC_cand_35.root MC_cand_36.root MC_cand_37.root MC_cand_38.root MC_cand_39.root MC_cand_40.root
hadd ssbar.root     MC_cand_41.root MC_cand_42.root MC_cand_43.root MC_cand_44.root MC_cand_45.root MC_cand_46.root MC_cand_47.root MC_cand_48.root MC_cand_49.root MC_cand_50.root
hadd ccbar.root     MC_cand_51.root MC_cand_52.root MC_cand_53.root MC_cand_54.root MC_cand_55.root MC_cand_56.root MC_cand_57.root MC_cand_58.root MC_cand_59.root MC_cand_60.root

mv -f mixed.root ../.
mv -f charged.root ../.
mv -f uubar.root ../.
mv -f ddbar.root ../.
mv -f ssbar.root ../.
mv -f ccbar.root ../.
