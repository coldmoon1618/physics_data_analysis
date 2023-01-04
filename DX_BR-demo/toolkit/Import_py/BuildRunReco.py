import sys, os
import glob

BGx = 1
print('Creating bsub commands to runReco.sh')
with open('runReco.sh','w') as f:
    print('#!/bin/csh\n', file=f)
    for tagType in ['had','SL']:
        for mcType in ['mixed','charged','uubar','ddbar','ssbar','ccbar']: 
            for index in range(0,100):
                outputName = os.path.join('MC_out_'+str(tagType),str(mcType)+'_Rec_BGx'+str(BGx)+'_'+str(index)+'.root')
                count = len(glob.glob(outputName))
                # creat bsub command if output of that specification doesn't already exist
                if(count==0):
                    print('bsub -o log/{}_{}_log.{}.{}'.format(tagType,mcType,BGx,index)+' -e log/{}_{}_err.{}.{}'.format(tagType,mcType,BGx,index)+' basf2 Reco_MC.py {} {} {} '.format(mcType,BGx,index),str(tagType), file=f)
                    print('sleep .01s', file=f)
