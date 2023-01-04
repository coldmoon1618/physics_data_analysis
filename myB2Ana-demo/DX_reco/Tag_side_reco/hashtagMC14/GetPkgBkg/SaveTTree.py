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
mcTree.Add('../AllModes_MC.root')
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

mcPxLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcPx',mcPxLst)
mcPyLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcPy',mcPyLst)
mcPzLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcPz',mcPzLst)
mcELst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('mcE',mcELst)

fOut = TFile('tree.root','recreate')
outTree = TTree("tree","")
eps=np.empty((1), dtype='float')
outTree.Branch("eps",eps,"eps/D")
Mbc=np.empty((1), dtype='float')
outTree.Branch("Mbc",Mbc,"Mbc/D")
groupID=np.empty((1), dtype='i')
outTree.Branch("groupID",groupID,"groupID/I")

for i in range(Entries):
    mcTree.GetEntry(i)
    if(iMcTypeLst[0]!=1):
        continue
    mcPx = mcPxLst[0]
    mcPy = mcPyLst[0]
    mcPz = mcPzLst[0]
    px = pxLst[0]
    py = pyLst[0]
    pz = pzLst[0]
    decMode = decModeLst[0]
    subMode = subModeLst[0]
    Mbc[0]  = MbcLst[0]
    p_MC    = TVector3(mcPx,mcPy,mcPz)
    p_rec   = TVector3(  px,  py,  pz)
    p_del   = p_MC-p_rec
    eps[0]  = p_del.Mag()
    for index in range(n):
        if(eval(cutStrings[index])):
            groupID[0] = index
            continue
    outTree.Fill()

fOut.cd()
outTree.Write()
fOut.Close()
