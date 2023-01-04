import sys, os
import glob

ratio_list = [255,270,803,201,192,665]

print('saving to missing.sh')
with open('missing.sh','w') as f:
    print('#!/bin/csh\n', file=f)
    for iDir in range(0,60):#max=60
        for index in range(0,ratio_list[iDir//10]):
            outputName = os.path.join('Mar0921_{}'.format(iDir+1),'sub00', 'MC13a_B0_Rec_{:0=5d}*'.format(index))
            count = len(glob.glob(outputName+'.root'))
            if(count==0):
                print('gb2_ds_get '+outputName+' --force &', file=f)
            elif(count>1):
                print('rm 1 out of ',count,' ',outputName)
