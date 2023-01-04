from ROOT import *
import ROOT.RooFit as rf
import argparse
import numpy as np
import math

exec(open("titleStr.py").read())
parser      = argparse.ArgumentParser()
parser.add_argument('--nPltBin', nargs='?', type=int,   default =  60,                         help='Num plot bins for unbinned fit')
parser.add_argument('--fInName', nargs='?', type=str,   default = 'B0_fixSig_group00_CB',   help='Fit result root')

arguments   = parser.parse_args()
nBins       = arguments.nPltBin
stringArray = arguments.fInName.split('_')
title       = stringArray[0] + ' '
myGroupID   = int(stringArray[2].split('group')[1])
title      += ' <'+str(myGroupID)+'> ' 
title      +=  titleStr[myGroupID]

fInName = 'FitGroupsResults/'+arguments.fInName+'.root'

myfile = TFile.Open(fInName)

frame       = myfile.Get('frame')
frame2      = myfile.Get('frame2')

c1          = TCanvas('c1','c1',800,700)
pad1        = TPad('pad1','pad1',0.0,.30,1.0,1.0)
pad2        = TPad('pad2','pad2',0.0,.05,1.0,.3 )
pad1.Draw()
frame.getAttText().SetX1(.2)
frame.getAttText().SetX2(.4)
frame.getAttText().SetY1(.9)
frame.getAttText().SetY2(.7)
pad1.cd()
frame.SetTitle(title)
frame.Draw()

mcTree = TChain('tree')
mcTree.Add('Groups_MC.root')
Entries = mcTree.GetEntries()
iMcTypeLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('iMcType',iMcTypeLst)
groupIDLst=np.empty((1), dtype='i')
mcTree.SetBranchAddress('groupID',groupIDLst)
decModeLst=np.empty((1), dtype='float')
MbcLst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('Mbc',MbcLst)
eps1Lst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('eps1',eps1Lst)
eps2Lst=np.empty((1), dtype='float')
mcTree.SetBranchAddress('eps2',eps2Lst)

lowBound    = 5.21  # Lower limit of both
upBound     = 5.3   # Upper limit of plot range
h_Mbc = TH1F('h_Mbc','',nBins,lowBound,upBound)

central = 0.17
loBound = 0.8*0.17
upBound = 1.2*0.17

nTot    = 0
nCentral= 0
nMoreBkg= 0
nLessBkg= 0

for i in range(Entries):
    mcTree.GetEntry(i)
    iMcType = iMcTypeLst[0]
    groupID = groupIDLst[0]
    eps1    = eps1Lst[0]
    eps2    = eps2Lst[0]
    Mbc     = MbcLst[0]
    if( groupID==myGroupID and iMcType==0):
        if( 5.27 < Mbc < 5.29 ):
            nTot+=1
        if( eps1>upBound and eps2>upBound):
            if( 5.27 < Mbc < 5.29 ):
                nLessBkg+=1
        if( eps1>central and eps2>central):
            h_Mbc.Fill(MbcLst[0])
            if( 5.27 < Mbc < 5.29 ):
                nCentral+=1
        if( eps1>loBound and eps2>loBound):
            if( 5.27 < Mbc < 5.29 ):
                nMoreBkg+=1
h_Mbc.SetLineColor(kGreen+2)
h_Mbc.SetLineWidth(2)
h_Mbc.Draw('HIST same')
result = '{} , {:.0f}±{:.0f} , {:.0f} , {:.0f}'.format(arguments.fInName, nTot-nCentral, int(round(math.sqrt(nTot-nCentral))) , -nMoreBkg+nCentral, -nLessBkg+nCentral)

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

x = open('count.csv', 'a')
#           fInName  nSigTotal     
x.writelines(result+'\n')
x.close()

fOutPlot    = '/Users/zhai/Desktop/AnaNote/fit_plots/' + arguments.fInName+ '.pdf'
c1.SaveAs(fOutPlot)
