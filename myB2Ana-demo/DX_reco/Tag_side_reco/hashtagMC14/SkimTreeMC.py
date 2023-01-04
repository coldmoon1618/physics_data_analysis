from ROOT import *
import numpy as np

exec(open("cutStrings.py").read())
#-------------------------------------------------- INPUT
mcTree = TChain('tree')
mcTree.Add('AllModes_MC.root')
Entries = mcTree.GetEntries()

iMcTypeLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('iMcType',iMcTypeLst)
MbcLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('Mbc',MbcLst)
beamELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('beamE',beamELst)
decModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('decMode',decModeLst)
SigProbLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('SigProb',SigProbLst)
subModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('subMode',subModeLst)
mySignalLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('mySignal',mySignalLst)

pxLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('px',pxLst)
pyLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('py',pyLst)
pzLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('pz',pzLst)
ELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('E',ELst)

mcPxLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcPx',mcPxLst)
mcPyLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcPy',mcPyLst)
mcPzLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcPz',mcPzLst)
mcELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcE',mcELst)

mc1PxLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc1Px',mc1PxLst)
mc1PyLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc1Py',mc1PyLst)
mc1PzLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc1Pz',mc1PzLst)
mc1ELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc1E',mc1ELst)

mc2PxLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc2Px',mc2PxLst)
mc2PyLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc2Py',mc2PyLst)
mc2PzLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc2Pz',mc2PzLst)
mc2ELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mc2E',mc2ELst)

#-------------------------------------------------- OUTPUT
fOut = TFile('Groups_MC.root','recreate')
outTree = TTree("tree","")
eps=np.empty((1), dtype='float')
outTree.Branch("eps",eps,"eps/D")
eps1=np.empty((1), dtype='float')
outTree.Branch("eps1",eps1,"eps1/D")
eps2=np.empty((1), dtype='float')
outTree.Branch("eps2",eps2,"eps2/D")
Mbc=np.empty((1), dtype='float')
outTree.Branch("Mbc",Mbc,"Mbc/D")
beamE=np.empty((1), dtype='float')
outTree.Branch("beamE",beamE,"beamE/D")
groupID=np.empty((1), dtype='i')
outTree.Branch("groupID",groupID,"groupID/I")
iMcType=np.empty((1), dtype='i')
outTree.Branch("iMcType",iMcType,"iMcType/I")
mySignal=np.empty((1), dtype='i')
outTree.Branch("mySignal",mySignal,"mySignal/I")

for i in range(Entries):
    mcTree.GetEntry(i)
    decMode     = decModeLst[0]
    subMode     = subModeLst[0]
    Mbc[0]      = MbcLst[0]
    beamE[0]    = beamELst[0]
    iMcType[0]  = iMcTypeLst[0]
    mySignal[0] = mySignalLst[0]
    groupID[0]  = -999
    eps[0]      = 999
    eps1[0]     = 999
    eps2[0]     = 999
    for index in range(n):
        if(eval(cutStrings[index])):
            groupID[0] = index
            continue
    if(iMcTypeLst[0]==0):
        mc1Px = mc1PxLst[0]
        mc1Py = mc1PyLst[0]
        mc1Pz = mc1PzLst[0]
        mc2Px = mc2PxLst[0]
        mc2Py = mc2PyLst[0]
        mc2Pz = mc2PzLst[0]
        px = pxLst[0]
        py = pyLst[0]
        pz = pzLst[0]
        p_MC1   = TVector3(mc1Px,mc1Py,mc1Pz)
        p_MC2   = TVector3(mc2Px,mc2Py,mc2Pz)
        p_rec   = TVector3(   px,   py,   pz)
        p_del1  = p_MC1-p_rec
        p_del2  = p_MC2-p_rec
        eps1[0] = p_del1.Mag()
        eps2[0] = p_del2.Mag()
    elif(iMcTypeLst[0]==1):
        mcPx = mcPxLst[0]
        mcPy = mcPyLst[0]
        mcPz = mcPzLst[0]
        px = pxLst[0]
        py = pyLst[0]
        pz = pzLst[0]
        p_MC    = TVector3(mcPx,mcPy,mcPz)
        p_rec   = TVector3(  px,  py,  pz)
        p_del   = p_MC-p_rec
        eps[0]  = p_del.Mag()
    outTree.Fill()

fOut.cd()
outTree.Write()
fOut.Close()
