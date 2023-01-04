from ROOT import *
import ROOT.RooFit as rf
import argparse
import numpy as np

mcTree = TChain('tree')
mcTree.Add('../Groups_MC.root')
Entries = mcTree.GetEntries()
mySignalLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('mySignal',mySignalLst)
iMcTypeLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('iMcType',iMcTypeLst)
MbcLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('Mbc',MbcLst)
groupIDLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('groupID',groupIDLst)
epsLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('eps',epsLst)

groups = [1,2,3,6,7,9]
h_eps = list()
for i in range(len(groups)):
    h_eps.append(TH1D('h_{}'.format(groups[i]),'',100,0,1))
for i in range(Entries):
    mcTree.GetEntry(i)
    if(iMcTypeLst[0]==1):
        for j in range(len(groups)):
            if(groupIDLst[0]==groups[j]):
                h_eps[j].Fill(epsLst[0])
                continue

for i in range(len(groups)):
    c = TCanvas('c_{}'.format(groups[i]),'',400,300)
    h_eps[i].Draw()
    gPad.SetLogy()
    c.SaveAs('eps_{}.pdf'.format(groups[i]))
