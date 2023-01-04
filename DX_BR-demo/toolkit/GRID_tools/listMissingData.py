import sys, os
import glob

print('saving to missing.sh')
with open('missing.sh','w') as f:
    print('#!/bin/csh\n', file=f)
    for outName in ['mainOut','feiHash']:
        for index in range(0,6115): 
            outputName = os.path.join('sub{:0=2d}'.format(index//1000), outName+'_{:0=5d}*'.format(index))
            search = glob.glob(outputName+'.root')
            count = len(search)
            if(count==0):
                print('gb2_ds_get HashDataJan08/'+outputName+' --force  ', file=f)
            elif(count>1):
                print('rm',search[1])
