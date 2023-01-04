import sys, os
import glob

# MC14_c, nph3
#ratio_list = [560,529,1000,338,303,1000]
# MC14_c and _d, eph3
ratio_list = [270,286,546,137,131,576]

print('saving to missing.sh')
with open('missing.sh','w') as f:
    print('#!/bin/csh\n', file=f)
    for outName in ['mainOut','feiHash','mc1Hash','mc2Hash']:
        for iDir in range(0,60):#min=0,max=60
            for index in range(0,ratio_list[iDir%6]): #Group by 1 block of 6 mcTypes before the next block
                outputName = os.path.join('hashtagJan08_{}'.format(iDir+1),'sub00', outName+'_{:0=5d}*'.format(index))
                count = len(glob.glob(outputName+'.root'))
                if(count==0):
                    print('gb2_ds_get '+outputName+' --force  ', file=f)
                elif(count>1):
                    print('rm 1 out of ',count,' ',outputName)
