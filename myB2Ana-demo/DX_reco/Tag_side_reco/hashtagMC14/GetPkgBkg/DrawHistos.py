from ROOT import *
import numpy as np

h_eps = list()
h_epsStar = list()
myfile = TFile.Open('boostedBack.root')
n = 30
gStyle.SetOptStat(0)
for index in range(n):
    histo = myfile.Get('h_{:0=2d}'.format(index))
    h_eps.append(histo)
    histo = myfile.Get('h_{:0=2d}_CM'.format(index))
    h_epsStar.append(histo)
    
for index in range(n):
    c = TCanvas('c','',400,300)
    h1 = h_epsStar[index].Clone('h1')
    h1.Draw()
    h1.SetTitle('GroupID={}'.format(index))
    h1.GetXaxis().SetTitle('GeV')
    h1.GetXaxis().SetRange(0,50)
    h1.SetLineWidth(2)
    h1.SetLineColor(kRed+1)
    h1.SetLineStyle(kDashed)
    h2 = h_eps[index].Clone('h2')
    h2.SetTitle('GroupID={}'.format(index))
    h2.GetXaxis().SetTitle('GeV')
    h2.GetXaxis().SetRange(0,50)
    h2.SetLineWidth(2)
    h2.SetLineColor(kGreen+2)
    h2.Draw('same')
    legend = TLegend(0.7,0.7,0.9,0.9);
    legend.AddEntry(h2,'Lab frame');
    legend.AddEntry(h1,'CM frame')
    legend.Draw()
    c.SaveAs('plots/h{:0=2d}.pdf'.format(index))
