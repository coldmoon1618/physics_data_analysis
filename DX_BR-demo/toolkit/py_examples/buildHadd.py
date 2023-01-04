import sys, os
import glob

ratio_list = [560,529,1000,338,303,1000]

#Didn't work for its original purposes, but the method still applies to other things
print('saving to hadd.sh')
with open('hadd.sh','w') as f:
    for iDir in range(0,6):#max=6
        print('cd',os.path.join('hashtagNov30_{}'.format(iDir+1),'sub00'), file=f)

        print('bsub hadd -f FeiHash_{}.root'.format(iDir+1),end=' ', file=f)
        for i in range(0,ratio_list[iDir]):
            print('feiHash_{:0=5d}*.root'.format(i),end=' ', file=f)
        print('', file=f)

        print('bsub hadd -f MainOut_{}.root'.format(iDir+1),end=' ', file=f)
        for i in range(0,ratio_list[iDir]):
            print('mainOut_{:0=5d}*.root'.format(i),end=' ', file=f)
        print('', file=f)

        print('cd ../../.', file=f)
with open('mv.sh','w') as f:
    for iDir in range(0,6):#max=6
        print('cd',os.path.join('hashtagNov30_{}'.format(iDir+1),'sub00'), file=f)

        print('mv -f FeiHash_{}.root ../../.'.format(iDir+1), file=f)
        print('mv -f MainOut_{}.root ../../.'.format(iDir+1), file=f)

        print('cd ../../.', file=f)
