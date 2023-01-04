import sys, os
import glob

print('saving to skim.sh')
with open('skim.sh','w') as f:
    for i in range(0,6115):
        print('bsub -o output/log -e output/err basf2 SaveOutput.py {} 99'.format(i), file=f)
