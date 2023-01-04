import sys
import glob
import root_pandas
import decayHash
import root_numpy
import numpy as np
from ROOT import Belle2
from decayHash import DecayHashMap
from readDecayTree import *

if len(sys.argv) != 4:
    print('Usage: basf2 BuildDecSet.py iMode iDir iFile')
    print('')
    sys.exit(1)

iMode   = int(sys.argv[1])
iDir    = sys.argv[2]
iFile   = int(sys.argv[3])
prefix = '/home/belle2/zhai/extraSpace/hashMC14ri/hashtagJan08_'+str(iDir)+'/sub00/'

fFeiHash= glob.glob(prefix+'feiHash_{:0=5d}*.root'.format(iFile))[0]
feiHash = root_numpy.root2array(fFeiHash)
feiMap  = DecayHashMap(fFeiHash, True)

fMainOut= glob.glob(prefix+'mainOut_{:0=5d}*.root'.format(iFile))[0]
mainOut = root_numpy.root2array(fMainOut, 'B_Tag')
myRec_B = root_pandas.read_root(fMainOut, 'B_Tag')

decMode_List = []
for myList in mainOut:
    decMode_List.append(int(myList[29]))

B_pos_set = set()
B_neg_set = set()

decMode=np.empty((1), dtype="float")
# add new elements to sets
for i in range(len(mainOut)):
    decMode[0]      = decMode_List[i]
    if(decMode[0]!=iMode*1.0):
        continue
    feiCand         = myRec_B.iloc[i][["decHash", "decHashEx"]].values
    feiString       = feiMap.get_string(*feiCand).split('|')[0]
    [FSPs,setVal]   = listFSPs(feiString)
    if  (setVal>0):
        B_pos_set.add(str(FSPs))
    elif(setVal<0):
        B_neg_set.add(str(FSPs))

import json
B_pos_lst = list(B_pos_set)
B_pos_lst.sort(key=lambda item: (len(json.loads(item)), sum(map(abs, json.loads(item)))))
B_neg_lst = list(B_neg_set)
B_neg_lst.sort(key=lambda item: (len(json.loads(item)), sum(map(abs, json.loads(item)))))
print('Saving dec sets')
with open('decSets/subsets/posSet_mode{}_{}_{:0=5d}.txt'.format(iMode,iDir,iFile),'w') as f:
    for ele in B_pos_lst:
        print(ele, file=f)
with open('decSets/subsets/negSet_mode{}_{}_{:0=5d}.txt'.format(iMode,iDir,iFile),'w') as f:
    for ele in B_neg_lst:
        print(ele, file=f)
