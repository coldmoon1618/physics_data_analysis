from ROOT import *
import numpy as np

exec(open("cutStrings.py").read())
#-------------------------------------------------- INPUT
mcTree = TChain('tree')
mcTree.Add('AllModes_Data.root')
Entries = mcTree.GetEntries()

MbcLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('Mbc',MbcLst)
beamELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('beamE',beamELst)
decModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('decMode',decModeLst)
subModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('subMode',subModeLst)

fOut = TFile('Groups_Data.root','recreate')
outTree = TTree("tree","")
Mbc=np.empty((1), dtype='float')
outTree.Branch("Mbc",Mbc,"Mbc/D")
beamE=np.empty((1), dtype='float')
outTree.Branch("beamE",beamE,"beamE/D")
groupID=np.empty((1), dtype='i')
outTree.Branch("groupID",groupID,"groupID/I")

for i in range(Entries):
    mcTree.GetEntry(i)
    decMode     = decModeLst[0]
    subMode     = subModeLst[0]
    Mbc[0]      = MbcLst[0]
    beamE[0]    = beamELst[0]
    groupID[0]  = -999
    for index in range(n):
        if(eval(cutStrings[index])):
            groupID[0] = index
            continue
    outTree.Fill()

fOut.cd()
outTree.Write()
fOut.Close()
