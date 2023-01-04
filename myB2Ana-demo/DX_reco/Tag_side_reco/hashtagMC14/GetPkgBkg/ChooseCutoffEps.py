from ROOT import *
import numpy as np

mcTree = TChain('tree')
mcTree.Add('mixed.root')
Entries = mcTree.GetEntries()

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


cutoff = 0.05

h1 = TH1D('h1','',60,5.21,5.3)
h2 = TH1D('h2','',60,5.21,5.3)

for i in range(Entries):
    if(groupID!=3):
        continue
    if(eps1[0] < cutoff or eps2[0] < cutoff):
        h1.Fill(Mbc[0])
    else:
        h2.Fill(Mbc[0])

c = TCanvas('c','',500,400)
h2.SetLineColor(kRed+1)
h2.Draw()
h1.SetLineColor(kGreen+2)
h1.Draw("same")
gPad.SetLogy()
gPad.Update()
    
