import sys
import decayHash
import root_numpy
from ROOT import Belle2
from printTree import *

if len(sys.argv) != 2:
    print('Usage: basf2 flag.py index')
    print('')
    sys.exit(1)
mcType  = 'charged' #sys.argv[1]
BGx     = '0'       #sys.argv[2]
ele     = sys.argv[1]

file_prefix='output/'+str(mcType)+'_'+str(ele)

mainOut = root_numpy.root2array(file_prefix+'_mainOut.root')
feiHash = root_numpy.root2array(file_prefix+'_feiHash.root')
mc1Hash = root_numpy.root2array(file_prefix+'_mc1Hash.root')
mc2Hash = root_numpy.root2array(file_prefix+'_mc2Hash.root')

feiString = []
for decayHash, decayHashExtended, decayString in feiHash:
    feiString.append(decayString.split('|')[0])

Y4SString = []
MC1String = []
for decayHash, decayHashExtended, decayString in mc1Hash:
    MC1String.append(decayString.split('|')[0])
    Y4SString.append(decayString.split('|')[1])
MC2String = []
for decayHash, decayHashExtended, decayString in mc2Hash:
    MC2String.append(decayString.split('|')[0])

Exp_Num_List  = []
Run_Num_List  = []
Evt_Num_List  = []
Mbc_List      = []
deltaE_List   = []
isSignl_List  = []
MissGam_List  = []
MissPar_List  = []
wrngFSP_List  = []
SigProb_List  = []
decMode_List  = []

import math
for myList in mainOut:
    isSignl = myList[22]
    if(math.isnan(isSignl)):
        isSignl_List.append(int(-1))
    else:
        isSignl_List.append(int(isSignl))
    MissGam = myList[24]
    if(math.isnan(MissGam)):
        MissGam_List.append(int(-1))
    else:
        MissGam_List.append(int(MissGam))
    MissPar = myList[23]
    if(math.isnan(MissPar)):
        MissPar_List.append(int(-1))
    else:
        MissPar_List.append(int(MissPar))
    wrngFSP = myList[25]
    if(math.isnan(wrngFSP)):
        wrngFSP_List.append(int(-1))
    else:
        wrngFSP_List.append(int(wrngFSP))
    Exp_Num_List.append(int(myList[0]))
    Run_Num_List.append(int(myList[1]))
    Evt_Num_List.append(int(myList[2]))
    Mbc_List.append(float(myList[19]))
    deltaE_List.append(float(myList[20]))
    SigProb_List.append(float(myList[27]))
    decMode_List.append(float(myList[28]))

import ROOT
import numpy as np
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

for i in range(len(feiString)):
    mySignal[0]     = -1 
    expNum[0]       = Exp_Num_List[i]
    runNum[0]       = Run_Num_List[i]
    evtNum[0]       = Evt_Num_List[i]
    Mbc[0]          = Mbc_List[i]
    deltaE[0]       = deltaE_List[i]
    isSignal[0]     = isSignl_List[i]
    accMissGamma[0] = MissGam_List[i]
    accMiss[0]      = MissPar_List[i]
    accWrongFSP[0]  = wrngFSP_List[i]
    SigProb[0]      = SigProb_List[i]
    decMode[0]      = decMode_List[i]

    feiList = listFSPs(feiString[i])
    MC1List = listFSPs(MC1String[i])
    MC2List = listFSPs(MC2String[i])
    if(feiList==MC1List or feiList==MC2List):
        mySignal[0]=1
    outTree.Fill()

root_file = ROOT.TFile(file_prefix+"_decFlag.root", "RECREATE")
root_file.cd()
outTree.Write()

sys.exit(0)
