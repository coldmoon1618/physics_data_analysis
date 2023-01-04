import sys
import math
import decayHash
import root_numpy
import numpy as np
import ROOT
from ROOT import Belle2
from readDecayTree import *

#--------------------------------------------------------------------------------
# Parameters for this script
# basf2 SaveOutput.py 2 0 0 hashtagNov30 > temp.txt; vi temp.txt
if len(sys.argv) != 4:
    print('Usage: basf2 SaveOutput.py iDir iFile iEvt')
    print('')
    sys.exit(1)
iDir    = sys.argv[1]
iFile   = int(sys.argv[2])
iEvt    = int(sys.argv[3])

#--------------------------------------------------------------------------------
# GRID outputs as fIns
prefix = '/home/belle2/zhai/extraSpace/hashMC14ri/hashtagNov30_'+str(iDir)+'/sub00/'
fFeiHash = prefix+'feiHash_{:0=5d}*'.format(iFile)
feiHash = root_numpy.root2array(fFeiHash)
fMainOut = prefix+'mainOut_{:0=5d}*'.format(iFile)
mainOut = root_numpy.root2array(fMainOut)

fMC1Hash = prefix+'mc1Hash_{:0=5d}*'.format(iFile)
mc1Hash = root_numpy.root2array(fMC1Hash)
fMC2Hash = prefix+'mc2Hash_{:0=5d}*'.format(iFile)
mc2Hash = root_numpy.root2array(fMC2Hash)

#--------------------------------------------------------------------------------
# Read info from fIns
# feiHash
feiString = []
for decayHash, decayHashExtended, decayString in feiHash:
    feiString.append(decayString.split('|')[0])

MC1String = []
MC2String = []
for decayHash, decayHashExtended, decayString in mc1Hash:
    MC1String.append(decayString.split('|')[0])
for decayHash, decayHashExtended, decayString in mc2Hash:
    MC2String.append(decayString.split('|')[0])

Evt_Num_List = []
for myList in mainOut:
    Evt_Num_List.append(int  (myList[2]))

# Read from other lists built from mainOut
for i in range(len(feiString)):
    if(Evt_Num_List[i]== iEvt):
        feiList     = listFSPs(feiString[i])[0]
        MC1List     = listFSPs(MC1String[i])[0]
        MC2List     = listFSPs(MC2String[i])[0]
        print(feiString[i])
        print(MC1String[i])
        print(MC2String[i])
        print(feiList)
        print(MC1List)
        print(MC2List)

sys.exit(0)
