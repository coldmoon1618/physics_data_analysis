from ROOT import *
import numpy as np

cutStrings =[
'decMode==0&&subMode==0',
'decMode==0&&subMode==2',
'decMode==0&&(subMode==3||subMode==6||subMode==8||subMode==10)',
'decMode==0&&(subMode==5||subMode==13)',
'decMode==0&&subMode==11',
'decMode==0&&subMode==14',
'decMode==1&&(subMode==1||subMode==2||subMode==5)',
'decMode==1&&(subMode==3||subMode==6||subMode==10||subMode==13||subMode==14)',
'decMode==1&&(subMode==8||subMode==11)',
'decMode==3&&(subMode==2||subMode==13)',
'decMode==3&&subMode==5',
'decMode==15&&(subMode==0||subMode==3)',
'decMode==15&&(subMode==1||subMode==6||subMode==7||subMode==9)',
'decMode==15&&subMode==2',
'decMode==15&&(subMode==5||subMode==8||subMode==12)',
'decMode==15&&(subMode==10||subMode==11)',
'decMode==25&&subMode==0',
'decMode==25&&subMode==1',
'decMode==26&&subMode==0',
'decMode==26&&subMode==1',
'(decMode==27&&subMode==0)||(decMode==28&&subMode==0)',
'(decMode==27&&subMode==1)||(decMode==28&&subMode==1)',
'decMode==4',
'decMode==14',
'decMode==16',
'decMode==20',
'decMode==22',
'decMode==23',
'(decMode==24||decMode==30)',
'decMode==31'
]

# So far only lab frame version saved
# TODO include boosted to CM frame 
n = len(cutStrings)
for index in range(n):
    cutStrings[index]=cutStrings[index].replace('&&',' and ').replace('||',' or ')

mcTree = TChain('tree')
mcTree.Add('../Mixed.root')
Entries = mcTree.GetEntries()

iMcTypeLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('iMcType',iMcTypeLst)
MbcLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('Mbc',MbcLst)
decModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('decMode',decModeLst)
SigProbLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('SigProb',SigProbLst)
subModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('subMode',subModeLst)

pxLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('px',pxLst)
pyLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('py',pyLst)
pzLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('pz',pzLst)
ELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('E',ELst)

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

fOut = TFile('mixed.root','recreate')
outTree = TTree("tree","")
eps1=np.empty((1), dtype='float')
outTree.Branch("eps1",eps1,"eps1/D")
eps2=np.empty((1), dtype='float')
outTree.Branch("eps2",eps2,"eps2/D")
minEps=np.empty((1), dtype='float')
outTree.Branch("minEps",minEps,"minEps/D")
Mbc=np.empty((1), dtype='float')
outTree.Branch("Mbc",Mbc,"Mbc/D")
groupID=np.empty((1), dtype='i')
outTree.Branch("groupID",groupID,"groupID/I")

for i in range(Entries):
    mcTree.GetEntry(i)
    mc1Px = mc1PxLst[0]
    mc1Py = mc1PyLst[0]
    mc1Pz = mc1PzLst[0]
    mc2Px = mc2PxLst[0]
    mc2Py = mc2PyLst[0]
    mc2Pz = mc2PzLst[0]
    px = pxLst[0]
    py = pyLst[0]
    pz = pzLst[0]
    decMode = decModeLst[0]
    subMode = subModeLst[0]
    Mbc[0]  = MbcLst[0]
    p_MC1   = TVector3(mc1Px,mc1Py,mc1Pz)
    p_MC2   = TVector3(mc2Px,mc2Py,mc2Pz)
    p_rec   = TVector3(   px,   py,   pz)
    p_del1  = p_MC1-p_rec
    p_del2  = p_MC2-p_rec
    eps1[0] = p_del1.Mag()
    eps2[0] = p_del2.Mag()
    if(eps1[0]<eps1[0]):
        minEps[0]=eps1[0]
    else:
        minEps[0]=eps2[0]
    for index in range(n):
        if(eval(cutStrings[index])):
            groupID[0] = index
            continue
    outTree.Fill()

fOut.cd()
outTree.Write()
fOut.Close()
