#!/bin/csh

gb2_job_status -p May14_1 May14_2 May14_3 May14_4 May14_5 May14_6 May14_7 May14_8 May14_9 May14_10 May14_11 May14_12 May14_13 May14_14 May14_15 May14_16 May14_17 May14_18 May14_19 May14_20 May14_21 May14_22 May14_23 May14_24 May14_25 May14_26 May14_27 May14_28 May14_29 May14_30 May14_31 May14_32 May14_33 May14_34 May14_35 May14_36 May14_37 May14_38 May14_39 May14_40 May14_41 May14_42 May14_43 May14_44 May14_45 May14_46 May14_47 May14_48 May14_49 May14_50 May14_51 May14_52 May14_53 May14_54 May14_55 May14_56 May14_57 May14_58 May14_59 May14_60 --status Failed > fail.txt

echo -n 'gb2_job_reschedule -f -j '
awk '{if($2 == "Failed") print $1}' fail.txt > failed.txt
awk 1 ORS=' ' failed.txt
echo ''

rm -rf fail.txt
rm -rf failed.txt
