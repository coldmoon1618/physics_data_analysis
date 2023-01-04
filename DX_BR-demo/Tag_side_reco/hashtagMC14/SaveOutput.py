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
# basf2 SaveOutput.py 2 0 0 hashtagNov30 > temp.txt; vi temp.txt
if len(sys.argv) != 5:
    print('Usage: basf2 SaveOutput.py iDir iFile iMode myProj')
    print('')
    sys.exit(1)
iDir    = sys.argv[1]
iFile   = int(sys.argv[2])
iMode   = int(sys.argv[3])
myProj  = str(sys.argv[4])

myMcType= (int(iDir)-1)%6

#--------------------------------------------------------------------------------
# GRID outputs as fIns
prefix  = '/home/belle2/zhai/extraSpace/hashMC14ri/'+myProj+'_'+str(iDir)+'/sub00/'
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
# Might also improve branch index lookup to variable name directly to avoid mismatch

if(myMcType==1):
    # For TTree entry reading 
    mcBplus = root_numpy.root2array(fMainOut, 'B_plus')
    mcBminus= root_numpy.root2array(fMainOut, 'B_minus')
    # For hashmap lookup
    myMC1_B = root_pandas.read_root(fMainOut, 'B_plus')
    myMC2_B = root_pandas.read_root(fMainOut, 'B_minus')

# mcHash
if(myMcType==1):
    fMC1Hash= glob.glob(prefix+'mc1Hash_{:0=5d}*.root'.format(iFile))[0]
    mc1Hash = root_numpy.root2array(fMC1Hash)
    mc1Map  = DecayHashMap(fMC1Hash, True)
    fMC2Hash= glob.glob(prefix+'mc2Hash_{:0=5d}*.root'.format(iFile))[0]
    mc2Hash = root_numpy.root2array(fMC2Hash)
    mc2Map  = DecayHashMap(fMC2Hash, True)

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

Bplus_Mbc_Ls  = []
BminusMbc_Ls  = []
Bplus_DeE_Ls  = []
BminusDeE_Ls  = []

for myList in mainOut:
    isSignl = myList[23]
    if(math.isnan(isSignl)):
        isSignl_List.append(int(-1))
    else:
        isSignl_List.append(int(isSignl))
    MissGam = myList[25]
    if(math.isnan(MissGam)):
        MissGam_List.append(int(-1))
    else:
        MissGam_List.append(int(MissGam))
    MissPar = myList[24]
    if(math.isnan(MissPar)):
        MissPar_List.append(int(-1))
    else:
        MissPar_List.append(int(MissPar))
    wrngFSP = myList[26]
    if(math.isnan(wrngFSP)):
        wrngFSP_List.append(int(-1))
    else:
        wrngFSP_List.append(int(wrngFSP))
    Exp_Num_List.append(int  (myList[ 0]))
    Run_Num_List.append(int  (myList[ 1]))
    Evt_Num_List.append(int  (myList[ 2]))
    Mbc_List    .append(float(myList[19]))
    deltaE_List .append(float(myList[20]))
    SigProb_List.append(float(myList[28]))
    decMode_List.append(float(myList[29]))
    m_beamE = .5*float(myList[21])
    beamE_List  .append(m_beamE)

if(myMcType==1):
    for myList in mcBplus:
        Bplus_Mbc_Ls.append(float(myList[19]))
        Bplus_DeE_Ls.append(float(myList[20]))
    for myList in mcBminus:
        BminusMbc_Ls.append(float(myList[19]))
        BminusDeE_Ls.append(float(myList[20]))

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
mcMbc=np.empty((1), dtype="float")
outTree.Branch("mcMbc",mcMbc,"mcMbc/D")
mcDeltaE=np.empty((1), dtype="float")
outTree.Branch("mcDeltaE",mcDeltaE,"mcDeltaE/D")
SigProb=np.empty((1), dtype="float")
outTree.Branch("SigProb",SigProb,"SigProb/D")
decMode=np.empty((1), dtype="float")
outTree.Branch("decMode",decMode,"decMode/D")
subMode=np.empty((1), dtype="float")
outTree.Branch("subMode",subMode,"subMode/D")
nFSgamma=np.empty((1), dtype="float")
outTree.Branch("nFSgamma",nFSgamma,"nFSgamma/D")
beamE=np.empty((1), dtype="float")
outTree.Branch("beamE",beamE,"beamE/D")

#Int representation of mixed charged udsc
iMcType=np.empty((1), dtype="int")
outTree.Branch("iMcType",iMcType,"iMcType/I")
mySignal=np.empty((1), dtype="int")
outTree.Branch("mySignal",mySignal,"mySignal/I")
isSignal=np.empty((1), dtype="int")
outTree.Branch("isSignal",isSignal,"isSignal/I")
accMissGamma=np.empty((1), dtype="int")
outTree.Branch("accMissGamma",accMissGamma,"accMissGamma/I")
accMiss=np.empty((1), dtype="int")
outTree.Branch("accMiss",accMiss,"accMiss/I")
accWrongFSP=np.empty((1), dtype="int")
outTree.Branch("accWrongFSP",accWrongFSP,"accWrongFSP/I")
ithDir=np.empty((1), dtype="int")
outTree.Branch("ithDir",ithDir,"ithDir/I")
ithFile=np.empty((1), dtype="int")
outTree.Branch("ithFile",ithFile,"ithFile/I")
nMissGamma=np.empty((1), dtype="int")
outTree.Branch("nMissGamma",nMissGamma,"nMissGamma/I")
nMissPi0=np.empty((1), dtype="int")
outTree.Branch("nMissPi0",nMissPi0,"nMissPi0/I")

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
    decMode[0]      = decMode_List[i]
    if(iMode<99 and decMode[0]!=iMode*1.0):
        continue

    expNum[0]       = Exp_Num_List[i]
    runNum[0]       = Run_Num_List[i]
    evtNum[0]       = Evt_Num_List[i]
    ithDir[0]       = iDir
    ithFile[0]      = iFile
    Mbc[0]          = Mbc_List[i]
    deltaE[0]       = deltaE_List[i]
    mcMbc[0]        = -999
    mcDeltaE[0]     = -999

    isSignal[0]     = isSignl_List[i]
    accMissGamma[0] = MissGam_List[i]
    accMiss[0]      = MissPar_List[i]
    accWrongFSP[0]  = wrngFSP_List[i]
    SigProb[0]      = SigProb_List[i]
    if(SigProb[0]<=0.5):
        continue
    beamE[0]        = beamE_List[i]

    iMcType[0]      = myMcType
    subMode[0]      = -999
    mySignal[0]     = -1
    nMissGamma[0]   = 0
    nMissPi0[0]     = 0
    feiCand         = myRec_B.iloc[i][["decHash", "decHashEx"]].values
    feiString       = feiMap.get_string(*feiCand).split('|')[0]
    nFSgamma[0]     = getnFSgamma(feiString)
    [FSPs,setVal]   = listFSPs(feiString)
    iDecMode        = int(decMode[0])
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
    if(myMcType==1):
        debugPrint = False
        mc1Cand     = myMC1_B.iloc[i][["decHash", "decHashEx"]].values
        mc1String   = mc1Map.get_string(*mc1Cand).split('|')[0]
        mc2Cand     = myMC2_B.iloc[i][["decHash", "decHashEx"]].values
        mc2String   = mc2Map.get_string(*mc2Cand).split('|')[0]
        feiList     = listFSPs(feiString)[0]
        MC1List     = listFSPs(mc1String)[0]
        MC2List     = listFSPs(mc2String)[0]
        if  (setVal>0):
            mcMbc[0]    = Bplus_Mbc_Ls[i]
            mcDeltaE[0] = Bplus_DeE_Ls[i]
            mcList = MC1List
        elif(setVal<0):
            mcMbc[0]    = BminusMbc_Ls[i]
            mcDeltaE[0] = BminusDeE_Ls[i]
            mcList = MC2List
        # Matching starts
        # 1. The same FSPs
        if(feiList==mcList):
            mySignal[0] = 4
        else:
            [c1,c2,missPars,xtraPars,misPID,nMissPi0[0],nMissGamma[0]]=compareLists(mcList,feiList)
            # 2. accMissGamma
            if(len(xtraPars)==0 and (set(missPars)=={111} or set(missPars)=={22} or set(missPars)=={111,22})):
                mySignal[0] = 3
            # 3. wrongFSPs
            elif(misPID.issubset({321,211,13,11}) and len(misPID)>0):
                mySignal[0] = 2
            # 4. accMiss
            else:
                if(310 in feiList):
                    feiList = decayKshort(feiList)
                if(310 in mcList):
                    mcList  = decayKshort(mcList)
                [c1,c2,missPars,xtraPars,misPID,nMissPi0[0],nMissGamma[0]]=compareLists(mcList,feiList)
                if(feiList==mcList):
                    mySignal[0] = 1
                elif(misPID.issubset({321,211,13,11}) and len(misPID)>0):
                    mySignal[0] = 1
                elif(len(xtraPars)==0 and (set(missPars)=={111} or set(missPars)=={22} or set(missPars)=={111,22})):
                    mySignal[0] = 1
        if(mySignal[0]<.5 and (accWrongFSP[0]>.5 or accMissGamma[0]>.5 or accMissGamma[0]>.5)):#evtNum==15857061):#debugPrint):
            print()
            print('--------------------')
            print('CHECKME')
            print(mySignal[0]<.5 and (accWrongFSP[0]>.5 or accMissGamma[0]>.5 or accMissGamma[0]>.5))
            print('mySignal',mySignal[0])
            print('accWrongFSP=',accWrongFSP[0],', accMissGamma=',accMissGamma[0],', accMiss=',accMiss[0])
            print('FEI')
            print(feiList)
            drawTree(feiString)
            print('MC1')
            print(MC1List)
            drawTree(mc1String)
            print('MC2')
            print(MC2List)
            drawTree(mc2String)
            print('MC      ',c1)
            print('reco    ',c2)
            print('MC-reco ',missPars)
            print('reco-MC ',xtraPars)
            print('misPID  ',misPID)

    outTree.Fill()

#--------------------------------------------------------------------------------
# Write out info
if(iMode<99):
    outName = myProj+'_output/decFlag_{}_{:0=5d}_mode{:0=2d}.root'.format(iDir,iFile,iMode)
else:
    outName = myProj+'_output/skimOut_{}_{:0=5d}.root'.format(iDir,iFile)
root_file = ROOT.TFile(outName, "RECREATE")
root_file.cd()
outTree.Write()
print('root -l',outName)

sys.exit(0)
