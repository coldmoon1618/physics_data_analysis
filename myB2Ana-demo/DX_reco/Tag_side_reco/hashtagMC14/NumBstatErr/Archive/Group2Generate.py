from ROOT import *
import ROOT.RooFit as rf
import math
import numpy as np

fixThres    = 5.29
lowBound    = 5.27   # Lower limit of both
upBound     = 5.29   # Upper limit of plot range

h1 = TH1F('h1','',50,-5,5)
h2 = TH1F('h2','',50,-5,5)

trueNsig    = 69439
purity      = .661
trueNtot    = int(trueNsig/purity)
trueNbkg    = trueNtot-trueNsig

print('mean')
print('nSig=',trueNsig,'nBkg=',trueNbkg, 'nTot=',trueNtot)
print()

for n in range(4):
    evntNsig    = np.random.poisson(trueNsig)
    evntNbkg    = np.random.poisson(trueNbkg)
    evntNtot    = evntNsig + evntNbkg

    print('nSig=',evntNsig,'nBkg=',evntNbkg, 'nTot=',evntNtot)
'''
for n in range(2000):
    Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound, upBound)
    mu          = RooRealVar('mu','',   5.28)
    lamb        = RooRealVar('lamb','', 0.0508) 
    gamma       = RooRealVar('gamma','',0.475) 
    delta       = RooRealVar('delta','',19.8)
    sig         = RooJohnson('sig','',  Mbc,mu,lamb,gamma,delta,lowBound)

    thres       = RooRealVar('thres','',fixThres)
    coef        = RooRealVar('coef','', -117.13)
    bkg         = RooArgusBG('bkg','',  Mbc, thres, coef)

    evntNsig    = np.random.poisson(trueNsig)
    evntNbkg    = np.random.poisson(trueNbkg)
    evntNtot    = evntNsig + evntNbkg

    nsig        = RooRealVar('nsig','', evntNsig)
    nbkg        = RooRealVar('nbkg','', evntNbkg)

    fit_nsig    = RooRealVar('nsig','', evntNsig, 0, 9e6)
    fit_nbkg    = RooRealVar('nbkg','', evntNbkg, 0, 9e6)

    model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
    fit_model   = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(fit_nsig,fit_nbkg))

    modelData   = model.generate(Mbc, evntNtot, rf.NumCPU(8))

    fit_model.fitTo(modelData, rf.NumCPU(8))
    fitNbkg     = fit_nbkg.getVal()
    fitNbkgErr  = fit_nbkg.getError()

    thePull = (fitNbkg-evntNbkg)/fitNbkgErr
    h1.Fill(thePull)
    thePull = (fitNbkg-trueNbkg)/fitNbkgErr
    h2.Fill(thePull)

outFile =  TFile('grp2Histo.root', 'recreate')
h1.Write('h1')
h2.Write('h2')
outFile.Close()
'''
