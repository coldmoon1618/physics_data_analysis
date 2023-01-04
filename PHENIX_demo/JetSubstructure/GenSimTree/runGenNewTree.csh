#!/bin/csh
source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
     source $i
end
source /opt/phenix/bin/phenix_setup.csh

cd /sphenix/user/yzhai/JetSubStructure/GenSimTree

#perl runGenNewTree.pl 0		>& 5.out
#perl runGenNewTree.pl 1		>& 10.out
#perl runGenNewTree.pl 2		>& 20.out
#perl runGenNewTree.pl 3		>& 30.out
#perl runGenNewTree.pl 4		>& 50.out
#perl runGenNewTree.pl 5		>& 80.out
#perl runGenNewTree.pl 6		>& 100.out
#perl runGenNewTree.pl 7		>& 120.out
#perl runGenNewTree.pl 8		>& 135.out
#perl runGenNewTree.pl 9		>& 150.out
#perl runGenNewTree.pl 10		>& 170_early.out
#perl runGenNewTree.pl 11	>& 190_early.out
#perl runGenNewTree.pl 12	>& 210_early.out
#perl runGenNewTree.pl 13	>& 225_early.out
perl runGenNewTree.pl 14	>& 240_early.out
#perl runGenNewTree.pl 15	>& 170_late.out
#perl runGenNewTree.pl 16	>& 190_late.out
#perl runGenNewTree.pl 17	>& 210_late.out
#perl runGenNewTree.pl 18	>& 225_late.out
perl runGenNewTree.pl 19	>& 240_late.out
