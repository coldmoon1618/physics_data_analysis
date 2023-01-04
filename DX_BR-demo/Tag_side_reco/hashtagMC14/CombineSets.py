from readDecayTree import *

import sys
if len(sys.argv) != 4:
    print('Usage: basf2 BuildDecSet.py iMode iDir iFile')
    print('')
    sys.exit(1)

iMode   = int(sys.argv[1])
iDir    = sys.argv[2]
iFile   = int(sys.argv[3])

B_pos_set = set()
B_neg_set = set()

readInSets('decSets/posSet_mode{}.txt'.format(iMode),B_pos_set)
readInSets('decSets/subsets/posSet_mode{}_{}_{:0=5d}.txt'.format(iMode,iDir,iFile),B_pos_set)
updateSets('decSets/posSet_mode{}.txt'.format(iMode),B_pos_set)

readInSets('decSets/negSet_mode{}.txt'.format(iMode),B_neg_set)
readInSets('decSets/subsets/negSet_mode{}_{}_{:0=5d}.txt'.format(iMode,iDir,iFile),B_neg_set)
updateSets('decSets/negSet_mode{}.txt'.format(iMode),B_neg_set)
