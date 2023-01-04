from ROOT import *
import ROOT.RooFit as rf
import argparse
import numpy as np

parser      = argparse.ArgumentParser()
parser.add_argument('--nPltBin', nargs='?', type=int,   default = 180,          help='Num plot bins for unbinned fit')
parser.add_argument('--fInName', nargs='?', type=str,   default = 'fitresult',  help='Fit result root')

arguments   = parser.parse_args()
nBins       = arguments.nPltBin

fInName = arguments.fInName+'.root'

myfile = TFile.Open(fInName)

frame       = myfile.Get('frame')

c1          = TCanvas('c1','c1',800,700)
pad1        = TPad('pad1','pad1',0.0,.30,1.0,1.0)
pad2        = TPad('pad2','pad2',0.0,.05,1.0,.3 )
pad1.Draw()
#frame.getAttText().SetX1(.755)
#frame.getAttText().SetX2(.999)
#frame.getAttText().SetY1(.9)
#frame.getAttText().SetY2(.6)
pad1.cd()
#frame.SetTitle(title)
frame.Draw()
h_Mbc = TH1D("h_Mbc","",180,5.21,5.3)

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

for i in range(Entries):
    mcTree.GetEntry(i)
    if(#iMcTypeLst[0]==1 and 
        epsLst[0]>0.21 
        and 
        (groupIDLst[0]==1 or groupIDLst[0]==2 or groupIDLst[0]==3 or groupIDLst[0]==6 or groupIDLst[0]==7 or groupIDLst[0]==9)):
        h_Mbc.Fill(MbcLst[0])

h_Mbc.SetLineColor(kGreen+2)
h_Mbc.SetLineWidth(2)
h_Mbc.Draw('HIST same')
gPad.SetLogy()
gPad.Update()
'''
c1.cd()
pad2.Draw()
pad2.cd()
frame2.SetTickLength(.1)
frame2.GetXaxis().SetLabelSize(.1)
frame2.GetXaxis().SetTitle('')
frame2.GetYaxis().SetLabelSize(.1)
frame2.GetYaxis().SetNdivisions(10)
frame2.SetTitle('Pull distribution')
frame2.getAttMarker().SetMarkerSize(.7)
frame2.Draw()
'''

