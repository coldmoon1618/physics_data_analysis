import sys, os
import glob

# Finds missing outputs, completed the ones due to undownloaded GRID outputs
# Ones still missing caused by empty GRID outputs
nMissSkim = 0
for index in range(0,6115): 
    outputName = os.path.join('output','skimOut_{:0=5d}.root'.format(index))
    count = len(glob.glob(outputName))
    if(count==0):
        print('bsub -e err -o log basf2 SaveOutput.py {} 99'.format(index))
        nMissSkim+=1
print(nMissSkim,'missing')
