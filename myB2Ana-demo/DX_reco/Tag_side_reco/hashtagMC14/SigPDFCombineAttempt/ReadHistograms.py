from ROOT import *
import numpy as np

def getChi2(histo):
    chi2 = 0.
    for i in range(1,21):
        if(histo.GetBinError(i)==0):
            continue
        chi2 += (histo.GetBinContent(i))**2/histo.GetBinError(i)**2
    return chi2

h_Mbc = list()
Matrix= list()
myfile = TFile.Open('sigPDFs.root')
n = 30
for index in range(n):
    histo = myfile.Get('h_{:0=2d}'.format(index))
    h_Mbc.append(histo)
    Lst = list()
    for inner in range(n):
        Lst.append(-999)
    Matrix.append(Lst)

nTot = 0
nPass = 0
for index in range(n):
    #if(index!=2):
    #    continue
    for inner in range(index):
        if(index==inner):
            continue
        #if(inner!=1):
        #    continue
        nTot+=1
        temp0 = h_Mbc[index].Clone('temp1')
        temp1 = h_Mbc[index].Clone('temp1')
        temp0.Scale(1./temp1.GetEntries())
        temp1.Scale(1./temp1.GetEntries())
        temp2 = h_Mbc[inner].Clone('temp2')
        temp2.Scale(1./temp2.GetEntries())
        temp1.Add(temp2,-1)
        chi2=getChi2(temp1)
        chiProb=TMath.Prob(chi2,19)
        nPass+=1
        Matrix[index][inner]=chiProb
        '''
        c1 = TCanvas('c1','c1',450,900)
        c1.Divide(1,2)
        c1.cd(1)
        temp0.SetLineColor(kGreen+2)
        temp0.Draw()
        temp2.SetLineColor(kRed)
        temp2.Draw('same')
        c1.cd(2)
        temp1.SetTitle('#chi Prob={}'.format(chiProb))
        temp1.Draw()
        c1.SaveAs('temp/h{:0=2d}-h{:0=2d}.pdf'.format(index,inner))
        '''

print ',h0,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11,h12,h13,h14,h15,h16,h17,h18,h19,h20,h21,h22,h23,h24,h25,h26,h27,h28,h29,h30,h31,h32'
for index in range(n):
    lineString = 'h{},'.format(index)
    for inner in range(n):
        value = Matrix[index][inner]
        if(value>0.009):
            lineString+='{:.2f},'.format(value)
        else:
            lineString+=','
    print lineString
