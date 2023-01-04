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

n = len(cutStrings)
for index in range(n):
    cutStrings[index]=cutStrings[index].replace('&&',' and ').replace('||',' or ')

mcTree = TChain('tree')
mcTree.Add('../AllModes_MC.root')
Entries = mcTree.GetEntries()
isSignalLst=np.empty((1), dtype='int')
mcTree.SetBranchAddress('isSignal',isSignalLst)
accMissGammaLst=np.empty((1), dtype='int')
mcTree.SetBranchAddress('accMissGamma',accMissGammaLst)
accMissLst=np.empty((1), dtype='int')
mcTree.SetBranchAddress('accMiss',accMissLst)
accWrongFSPLst=np.empty((1), dtype='int')
mcTree.SetBranchAddress('accWrongFSP',accWrongFSPLst)
iMcTypeLst=np.empty((1), dtype='int')
mcTree.SetBranchAddress('iMcType',iMcTypeLst)
decModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('decMode',decModeLst)
SigProbLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('SigProb',SigProbLst)
subModeLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('subMode',subModeLst)
MbcLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('Mbc',MbcLst)

nBins = 20
lowBound    = 5.27  
upBound     = 5.288 
h_Mbc = list()
for index in range(n):
    histo   = TH1D('h_{:0=2d}'.format(index),'',nBins,lowBound,upBound)
    h_Mbc.append(histo)

for i in range(Entries):
    mcTree.GetEntry(i)
    SigProb = SigProbLst[0]
    decMode = decModeLst[0]
    subMode = subModeLst[0]
    isSignal= isSignalLst[0]
    accMissGamma= accMissGammaLst[0]
    accMiss= accMissLst[0]
    accWrongFSP= accWrongFSPLst[0]
    iMcType= iMcTypeLst[0]
    if(not(isSignal>0.5 or accMissGamma>0.5 or accMiss>0.5 or accWrongFSP>0.5) or iMcType!=1):
        continue
    for index in range(n):
        if(eval(cutStrings[index])):
            h_Mbc[index].Fill(MbcLst[0])
            continue

fOut = TFile('sigPDFs.root','recreate')
fOut.cd()
for index in range(n):
    h_Mbc[index].Write()
fOut.Close()

