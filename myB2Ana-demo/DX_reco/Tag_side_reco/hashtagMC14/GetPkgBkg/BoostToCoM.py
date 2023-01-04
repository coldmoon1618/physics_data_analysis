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

iMcTypeLst=np.empty((1), dtype='int')
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

h_eps = list()
H_eps = list()
h_epsStar = list()
H_epsStar = list()
for index in range(n):
    histo   = TH1D('h_{:0=2d}'.format(index),'',100,0,1)
    h_eps.append(histo)
    histo   = TH1D('H_{:0=2d}'.format(index),'',2000,0,1)
    H_eps.append(histo)
    histo   = TH1D('h_{:0=2d}_CM'.format(index),'',100,0,1)
    h_epsStar.append(histo)
    histo   = TH1D('H_{:0=2d}_CM'.format(index),'',2000,0,1)
    H_epsStar.append(histo)

nStop = 0
labToCM = -.28
for i in range(Entries):
    mcTree.GetEntry(i)
    if(iMcTypeLst[0]!=1):
        continue
    #if(nStop>5):
    #    continue
    #else:
    #    nStop+=1
    mcPx = mcPxLst[0]
    mcPy = mcPyLst[0]
    mcPz = mcPzLst[0]
    mcE  = mcELst[0]
    px = pxLst[0]
    py = pyLst[0]
    pz = pzLst[0]
    E  = ELst[0]
    decMode = decModeLst[0]
    subMode = subModeLst[0]
    Mbc     = MbcLst[0]

    p3_MC   = TVector3(mcPx,mcPy,mcPz)
    p3_rec  = TVector3(  px,  py,  pz)
    epsilon = (p3_MC-p3_rec).Mag()
    
    p4_MC   = TLorentzVector(mcPx,mcPy,mcPz,mcE)
    p4_rec  = TLorentzVector(  px,  py,  pz,  E)
    p4_MC   .Boost(0,0,labToCM)
    p4_rec  .Boost(0,0,labToCM)
    
    pstar_MC  = TVector3(p4_MC .Px(),p4_MC .Py(),p4_MC .Pz())
    pstar_rec = TVector3(p4_rec.Px(),p4_rec.Py(),p4_rec.Pz())
    epsStar = (pstar_MC-pstar_rec).Mag()
   
    for index in range(n):
        if(eval(cutStrings[index])):
            h_eps[index].Fill(epsilon)
            H_eps[index].Fill(epsilon)
            h_epsStar[index].Fill(epsStar)
            H_epsStar[index].Fill(epsStar)
            continue

'''
gStyle.SetOptStat(0)
c1 = TCanvas('c1','',400,300)
h_epsStar[0].SetLineStyle(kDashed)
h_epsStar[0].SetLineColor(kRed)
h_epsStar[0].Draw()
h_eps[0].Draw('same')
legend = TLegend(0.5,0.7,0.9,0.9);
legend.AddEntry(h_eps[0],'Lab frame');
legend.AddEntry(h_epsStar[0],'CM frame')
legend.Draw()
c1.SaveAs('h00.pdf')
'''
fOut = TFile('boostedBack.root','recreate')
fOut.cd()
for index in range(n):
    h_eps[index].Write()
    H_eps[index].Write()
    h_epsStar[index].Write()
    H_epsStar[index].Write()
fOut.Close()
