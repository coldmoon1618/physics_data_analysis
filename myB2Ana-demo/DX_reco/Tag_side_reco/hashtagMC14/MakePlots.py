from ROOT import *
import ROOT.RooFit as rf
import argparse

exec(open("titleStr.py").read())
parser      = argparse.ArgumentParser()
parser.add_argument('--nPltBin', nargs='?', type=int,   default = 180,                         help='Num plot bins for unbinned fit')
parser.add_argument('--fInName', nargs='?', type=str,   default = 'Matched_Group00_Johnson',   help='Fit result root')
parser.add_argument('--hasHist', nargs='?', type=int,   default = 0,                           help='The input has h_Mbc')

arguments   = parser.parse_args()
nBins       = arguments.nPltBin
stringArray = arguments.fInName.split('_')
title       = stringArray[0] + ' '
groupID     = int(stringArray[1].split('group')[1])
title      += ' <'+str(groupID)+'> ' 
title      +=  titleStr[groupID]

fInName = 'FitGroupsResults/'+arguments.fInName+'.root'

myfile = TFile.Open(fInName)

frame       = myfile.Get('frame')
frame2      = myfile.Get('frame2')
if(arguments.hasHist>.5):
    h_Mbc       = myfile.Get('h_Mbc')

c1          = TCanvas('c1','c1',800,700)
pad1        = TPad('pad1','pad1',0.0,.30,1.0,1.0)
pad2        = TPad('pad2','pad2',0.0,.05,1.0,.3 )
pad1.Draw()
frame.getAttText().SetX1(.755)
frame.getAttText().SetX2(.999)
frame.getAttText().SetY1(.9)
frame.getAttText().SetY2(.6)
pad1.cd()
frame.SetTitle(title)
frame.Draw()
if(arguments.hasHist>.5):
    h_Mbc.Draw('HIST same')
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

fOutPlot    = '/Users/zhai/Desktop/AnaNote/fit_plots/' + arguments.fInName+ '.pdf'
c1.SaveAs(fOutPlot)

pad1.cd()
gPad.SetLogy()
fOutPlot    = '/Users/zhai/Desktop/AnaNote/fit_plots/' + arguments.fInName+ '_LOG.pdf'
