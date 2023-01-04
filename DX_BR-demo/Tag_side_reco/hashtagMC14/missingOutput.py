import sys, os
import glob

# MC14_c and _d, eph3
ratio_list = [270,286,546,137,131,576]

dirList = [1,7,13,19]
for iDir in dirList:#min=0,max=60
    nMissing = 0
    for index in range(0,ratio_list[iDir%6]): #Group by 1 block of 6 mcTypes before the next block
        outputName = os.path.join('hashtagJan08_output','skimOut_{}_{:0=5d}.root'.format(iDir+1,index))
        count = len(glob.glob(outputName))
        if(count==0 and (iDir+1)%6 ==2):
            print('basf2 SaveOutput.py {} {} 0 hashtagJan08'.format(iDir+1,index))
            nMissing+=1
    print(nMissing,'missing B+B- files')
