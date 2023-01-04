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
eps1Lst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('eps1',eps1Lst)
eps2Lst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('eps2',eps2Lst)

groups = [1     ,2      ,3      ,6      ,7      ,9      ]
fcns   = ['CB','Johnson','Johnson','CB','CB','Gaussian' ]
#lo_cut = [0.18  ,0.2    ,0.11   ,0.21   ,0.21   ,0.21   ] 
#mi_cut = [0.18  ,0.4    ,0.15   ,0.21   ,0.21   ,0.21   ]
#hi_cut = [1.    ,1      ,0.19   ,0.21   ,0.21   ,0.21   ]
lo_cut = [0.16  ,0.16   ,0.11   ,0.16   ,0.16   ,0.16   ] 
mi_cut = [0.18  ,0.18   ,0.15   ,0.18   ,0.18   ,0.18   ]
hi_cut = [0.24  ,0.24   ,0.19   ,0.24   ,0.24   ,0.24   ]
nFitTot= [421.42,292.29 ,1211.69,849.47 ,693.27 ,303.92 ]
h_Mbc_lo = list()
h_Mbc_mi = list()
h_Mbc_hi = list()
nTot = [0,0,0,0,0,0]
for j in range(len(groups)):
    h_Mbc_lo.append(TH1D("h_Mbc_lo_{}".format(groups[j]),"",60,5.21,5.3))
    h_Mbc_mi.append(TH1D("h_Mbc_mi_{}".format(groups[j]),"",60,5.21,5.3))
    h_Mbc_hi.append(TH1D("h_Mbc_hi_{}".format(groups[j]),"",60,5.21,5.3))
    for i in range(Entries):
        mcTree.GetEntry(i)
        if(iMcTypeLst[0]==0 and groupIDLst[0]==groups[j]):
            nTot[j]+=1
            if(eps1Lst[0]>lo_cut[j] and eps2Lst[0]>lo_cut[j]):
                h_Mbc_lo[j].Fill(MbcLst[0])
                if(eps1Lst[0]>mi_cut[j] and eps2Lst[0]>mi_cut[j]):
                    h_Mbc_mi[j].Fill(MbcLst[0])
                    if(eps1Lst[0]>hi_cut[j] and eps2Lst[0]>hi_cut[j]):
                        h_Mbc_hi[j].Fill(MbcLst[0])

import math
for j in range(len(groups)):
    myfile = TFile.Open('../FitGroupsResults/B0B0bar_Group{:0=2d}_{}.root'.format(groups[j],fcns[j])) 
    frame       = myfile.Get('frame')
    c = TCanvas('c_{}'.format(groups[j]),'',400,300)
    frame.Draw()
    h_Mbc_lo[j].SetLineColor(kGreen+2)
    h_Mbc_lo[j].SetLineWidth(2)
    h_Mbc_lo[j].SetLineStyle(kDashed)
    h_Mbc_lo[j].Draw('HIST same')
    h_Mbc_mi[j].SetLineColor(kGreen+2)
    h_Mbc_mi[j].SetLineWidth(2)
    h_Mbc_mi[j].Draw('HIST same')
    h_Mbc_hi[j].SetLineColor(kGreen+2)
    h_Mbc_hi[j].SetLineWidth(2)
    h_Mbc_hi[j].SetLineStyle(kDashed)
    h_Mbc_hi[j].Draw('HIST same')
    gPad.Update()
    lo = nTot[j]-h_Mbc_lo[j].GetEntries()
    mi = nTot[j]-h_Mbc_mi[j].GetEntries()
    hi = nTot[j]-h_Mbc_hi[j].GetEntries()
    print(hi)
    print(mi)
    print(lo)
    err = math.sqrt(mi)
    print(mi,'±',int(round(err)),'+',hi-mi,'',lo-mi)
    c.SaveAs('fitVsCut_{}.pdf'.format(groups[j]))
    print(mi/nFitTot[j],'±',err/nFitTot[j],'+',(hi-mi)/nFitTot[j],'',(lo-mi)/nFitTot[j])
