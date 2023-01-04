import sys
import decayHash
import root_numpy
from ROOT import Belle2
from printTree import *

#if len(sys.argv) != 4:
#    print('Usage: basf2 DX_reco.py mcType BGx index tagType')
#    print('')
#    sys.exit(1)
mcType  = 'charged' #sys.argv[1]
BGx     = '0'       #sys.argv[2]
ele     = '0'       #sys.argv[3]

file_prefix='output/'+str(mcType)+'_'+str(ele)

mainOut = root_numpy.root2array(file_prefix+'_mainOut.root')
feiHash = root_numpy.root2array(file_prefix+'_feiHash.root')
mc1Hash = root_numpy.root2array(file_prefix+'_mc1Hash.root')
mc2Hash = root_numpy.root2array(file_prefix+'_mc2Hash.root')

#Read in FEI hash
feiString = []
for decayHash, decayHashExtended, decayString in feiHash:
    feiString.append(decayString.split('|')[0])

#Read in MC truth hash
Y4SString = []
MC1String = []
for decayHash, decayHashExtended, decayString in mc1Hash:
    MC1String.append(decayString.split('|')[0])
    Y4SString.append(decayString.split('|')[1])
MC2String = []
for decayHash, decayHashExtended, decayString in mc2Hash:
    MC2String.append(decayString.split('|')[0])

Exp_Num_List = []
Run_Num_List = []
Evt_Num_List = []
isSignl_List = []

import math
for myList in mainOut:
    Exp_Num_List.append(int(myList[0]))
    Run_Num_List.append(int(myList[1]))
    Evt_Num_List.append(int(myList[2]))
    isSignal = myList[23]
    if(math.isnan(isSignal)):
        isSignl_List.append(int(-1))
    else:
        isSignl_List.append(int(isSignal))

import ROOT

cnt1 = 0
cnt2 = 0

for i in range(len(feiString)):
#for i in range(3):
    if(isSignl_List[i]>.5):
        cnt2+=1
    feiList = listFSPs(feiString[i])
    MC1List = listFSPs(MC1String[i])
    MC2List = listFSPs(MC2String[i])
    if(feiList==MC1List):
        print('exp',Exp_Num_List[i],'run',Run_Num_List[i],'evt',Evt_Num_List[i])
        print('MC1\n')
        cnt1+=1
    elif(feiList==MC2List):
        print('exp',Exp_Num_List[i],'run',Run_Num_List[i],'evt',Evt_Num_List[i])
        print('MC2\n')
        cnt1+=1

print(cnt1,' FSPs matched')
print(cnt2,' basf2 isSignal')

sys.exit(0)
