import sys, os
import glob

ratio_list = [270,286,546,137,131,576]

print('saving to save.sh')
with open('save.sh','w') as f:
    for iDir in range(0,18):#max=60
        for i in range(0,ratio_list[iDir%6]):
            print('bsub -o hashtagJan08_output/log -e hashtagJan08_output/err basf2 SaveOutput.py {} {} 0 hashtagJan08'.format(iDir+1,i), file=f)
