#!/bin/csh

gb2_job_status -p Oct27_1 Oct27_2 Oct27_3 Oct27_4 Oct27_5 Oct27_6 --status Failed > fail.txt

echo -n 'gb2_job_reschedule -f -j '
awk '{if($2 == "Failed") print $1}' fail.txt > failed.txt
awk 1 ORS=' ' failed.txt
echo ''

rm -rf fail.txt
rm -rf failed.txt
