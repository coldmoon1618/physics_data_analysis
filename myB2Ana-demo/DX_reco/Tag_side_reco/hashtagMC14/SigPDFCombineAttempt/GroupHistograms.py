from ROOT import *
import numpy as np

def getW(histo):
    w = 0.
    nZero = 0 # decrease ndf if no entries for some bins
    for i in range(20):
        if(histo.GetBinError(i)==0):
            nZero+=1
            continue
        w+= 1.0/histo.GetBinError(i)**2
    return w, nZero

def getMu(histo, w):
    Sum = 0.
    for i in range(20):
        if(histo.GetBinError(i)==0):
            continue
        Sum += histo.GetBinContent(i)/histo.GetBinError(i)**2
    return Sum/w

def getChi2(histo,mu):
    chi2 = 0.
    for i in range(20):
        if(histo.GetBinError(i)==0):
            continue
        chi2 += (mu-histo.GetBinContent(i))**2/histo.GetBinError(i)**2
    return chi2

def compare(hGroup, inner):
    for ele in mytuple:
        temp1 = hGroup.Clone('temp1')
        temp1.Sumw2()
        temp1.Scale(1./temp1.GetEntries())
        temp2 = h_Mbc[ele].Clone('temp2')
        temp2.Sumw2()
        temp2.Scale(1./temp2.GetEntries())
        temp1.Add(temp2,-1)
        (w,nZero)=getW(temp1)
        mu  =getMu(temp1,w)
        chi2=getChi2(temp1,mu)
        chiProb=TMath.Prob(chi2,19-nZero)
        if(True):
            index = mytuple.index(ele)
            Matrix[index][inner]=chiProb
            c1 = TCanvas('c1','c1',500,500)
            temp1.SetTitle('#chi Prob={}'.format(chiProb))
            temp1.Draw()
            c1.SaveAs('temp/{}-h{:0=2d}.pdf'.format(hGroup.GetName(),ele))

mytuple = (6, 16, 20, 25)
h_Mbc = list()
Matrix= list()
myfile = TFile.Open('sigPDFs.root')
n = 30
for index in range(n):
    histo = myfile.Get('h_{:0=2d}'.format(index))
    h_Mbc.append(histo)

for index in range(len(mytuple)):
    Lst = list()
    for inner in range(8):
        Lst.append(-999)
    Matrix.append(Lst)

hA = h_Mbc[0].Clone('hA')
hA.Add(h_Mbc[7].Clone())

hB = h_Mbc[1].Clone('hB')
hB.Add(h_Mbc[14].Clone())
hB.Add(h_Mbc[17].Clone())

hC = h_Mbc[2].Clone('hC')
hC.Add(h_Mbc[4].Clone())
hC.Add(h_Mbc[5].Clone())
hC.Add(h_Mbc[15].Clone())

hD = h_Mbc[3].Clone('hD')
hD.Add(h_Mbc[18].Clone())
hD.Add(h_Mbc[22].Clone())

hE = h_Mbc[9].Clone('hE')
hE.Add(h_Mbc[19].Clone())
hE.Add(h_Mbc[27].Clone())
hE.Add(h_Mbc[28].Clone())
hE.Add(h_Mbc[29].Clone())

hF = h_Mbc[11].Clone('hF')
hF.Add(h_Mbc[12].Clone())
hF.Add(h_Mbc[13].Clone())
hF.Add(h_Mbc[21].Clone())

hG = h_Mbc[23].Clone('hG')
hG.Add(h_Mbc[26].Clone())
hG.Add(h_Mbc[10].Clone())

hH = h_Mbc[8].Clone('hH')
hH.Add(h_Mbc[24].Clone())

compare(hA, 0)
compare(hB, 1)
compare(hC, 2)
compare(hD, 3)
compare(hE, 4)
compare(hF, 5)
compare(hG, 6)
compare(hH, 7)

print ',hA,hB,hC,hD,hE,hF,hH'
for index in range(len(mytuple)):
    lineString = 'h{},'.format(mytuple[index])
    for inner in range(8):
        value = Matrix[index][inner]
        if(value>.01):
            lineString+='{:.2f},'.format(value)
        else:
            lineString+=','
    print lineString
