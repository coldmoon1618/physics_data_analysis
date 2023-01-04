import sys
import math
import glob
import root_pandas
import decayHash
import root_numpy
import numpy as np
import ROOT
from ROOT import Belle2
from readDecayTree import *
from decayHash import DecayHashMap
from collections import Counter

#--------------------------------------------------------------------------------
# Parameters for this script
# basf2 SaveOutput.py 2 0 > temp.txt; vi temp.txt
if len(sys.argv) != 3:
    print('Usage: basf2 SaveOutput.py iFile iMode')
    print('')
    sys.exit(1)
iFile   = int(sys.argv[1])
iMode   = int(sys.argv[2])

#--------------------------------------------------------------------------------
# GRID outputs as fIns
prefix  = '/home/belle2/zhai/extraSpace/HashDataJan08/Data/'
# This glob.glob('filename')[0] approach avoids repeated file index

# reco hashmap
fFeiHash= glob.glob(prefix+'feiHash_{:0=5d}*.root'.format(iFile))[0]
feiHash = root_numpy.root2array(fFeiHash)
feiMap  = DecayHashMap(fFeiHash, True)

# mainOut
fMainOut= glob.glob(prefix+'mainOut_{:0=5d}*.root'.format(iFile))[0]
# For TTree entry reading 
mainOut = root_numpy.root2array(fMainOut, 'B_Tag')
# For hashmap lookup
myRec_B = root_pandas.read_root(fMainOut, 'B_Tag')
#TODO: [low priority] switch both pandas and numpy to uproot

# mainOut to script output
Exp_Num_List    = []
Run_Num_List    = []
Evt_Num_List    = []
Mbc_List        = []
deltaE_List     = []
isSignl_List    = []
MissGam_List    = []
MissPar_List    = []
wrngFSP_List    = []
SigProb_List    = []
decMode_List    = []
beamE_List      = []

for myList in mainOut:
    Exp_Num_List.append(int  (myList[ 0]))
    Run_Num_List.append(int  (myList[ 1]))
    Evt_Num_List.append(int  (myList[ 2]))
    Mbc_List    .append(float(myList[19]))
    deltaE_List .append(float(myList[20]))
    SigProb_List.append(float(myList[22]))
    decMode_List.append(float(myList[23]))
    m_beamE = .5*float(myList[21])
    beamE_List  .append(m_beamE)

'''
*Br    0 :__experiment__ : __experiment__/I                                  *
*Br    1 :__run__   : __run__/I                                              *
*Br    2 :__event__ : __event__/I                                            *
*Br   19 :Mbc       : Mbc/D                                                  *
*Br   20 :deltaE    : deltaE/D                                               *
*Br   21 :Ecms      : Ecms/D                                                 *
*Br   22 :SigProb   : SigProb/D                                              *
*Br   23 :decMode   : decMode/D                                              *
*Br   24 :decHash   : decHash/D                                              *
*Br   25 :decHashEx : decHashEx/D                                            *
'''
#--------------------------------------------------------------------------------
# TTree for fOut
outTree = ROOT.TTree("tree","")

expNum=np.empty((1), dtype="int")
outTree.Branch("expNum",expNum,"expNum/I")
runNum=np.empty((1), dtype="int")
outTree.Branch("runNum",runNum,"runNum/I")
evtNum=np.empty((1), dtype="int")
outTree.Branch("evtNum",evtNum,"evtNum/I")

# N.B.: Python float is C++ double, thus double type branches here
Mbc=np.empty((1), dtype="float")
outTree.Branch("Mbc",Mbc,"Mbc/D")
deltaE=np.empty((1), dtype="float")
outTree.Branch("deltaE",deltaE,"deltaE/D")
SigProb=np.empty((1), dtype="float")
outTree.Branch("SigProb",SigProb,"SigProb/D")
decMode=np.empty((1), dtype="float")
outTree.Branch("decMode",decMode,"decMode/D")
subMode=np.empty((1), dtype="float")
outTree.Branch("subMode",subMode,"subMode/D")
beamE=np.empty((1), dtype="float")
outTree.Branch("beamE",beamE,"beamE/D")

#--------------------------------------------------------------------------------
# Read in subMode lists to flag subMode values
B_pos_lst = np.empty(36, dtype=np.object)
B_neg_lst = np.empty(36, dtype=np.object)
for ind in range(36):
    B_pos_lst[ind] =list()
    B_neg_lst[ind] =list()

if(iMode<99):
    readInLsts('decSets/posSet_mode{}.txt'.format(iMode),B_pos_lst[iMode])
    readInLsts('decSets/negSet_mode{}.txt'.format(iMode),B_neg_lst[iMode])
else:
    for ind in range(36):
        readInLsts('decSets/posSet_mode{}.txt'.format(ind),B_pos_lst[ind])
        readInLsts('decSets/negSet_mode{}.txt'.format(ind),B_neg_lst[ind])

# Read from other lists built from mainOut
for i in range(len(mainOut)):
    SigProb[0]      = SigProb_List[i]
    if(SigProb[0]<=0.5):
        continue
    decMode[0]      = decMode_List[i]
    if(iMode<99 and decMode[0]!=iMode*1.0):
        continue

    expNum[0]       = Exp_Num_List[i]
    runNum[0]       = Run_Num_List[i]
    evtNum[0]       = Evt_Num_List[i]
    Mbc[0]          = Mbc_List[i]
    deltaE[0]       = deltaE_List[i]
    beamE[0]        = beamE_List[i]
    subMode[0]      = -999
    iDecMode        = int(decMode[0])
    feiCand         = myRec_B.iloc[i][["decHash", "decHashEx"]].values
    feiString       = feiMap.get_string(*feiCand).split('|')[0]
    [FSPs,setVal]   = listFSPs(feiString)
    if  (setVal>0):
        if(str(FSPs) in B_pos_lst[iDecMode]):
            index       = B_pos_lst[iDecMode].index(str(FSPs))
            subMode[0]  = index
        else:
            subMode[0]  = -999
    elif(setVal<0):
        if(str(FSPs) in B_neg_lst[iDecMode]): 
            index       = B_neg_lst[iDecMode].index(str(FSPs))
            subMode[0]  = index
        else:
            subMode[0]  = -999
    outTree.Fill()

#--------------------------------------------------------------------------------
# Write out info
if(iMode<99):
    outName = 'output/decFlag_{:0=5d}_mode{:0=2d}.root'.format(iFile,iMode)
else:
    outName = 'output/skimOut_{:0=5d}.root'.format(iFile)

root_file = ROOT.TFile(outName, "RECREATE")
root_file.cd()
outTree.Write()
print('root -l',outName)

sys.exit(0)
