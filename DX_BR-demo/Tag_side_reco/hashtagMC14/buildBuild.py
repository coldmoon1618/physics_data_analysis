import sys, os
import glob

ratio_list = [560,529,1000,338,303,1000]

print('saving to run.sh')
with open('run.sh','w') as f:
    for iDir in range(0,6):#max=6
        for i in range(0,ratio_list[iDir]):
            print('bsub basf2 BuildDecSet.py {} {}'.format(iDir+1,i), file=f)
