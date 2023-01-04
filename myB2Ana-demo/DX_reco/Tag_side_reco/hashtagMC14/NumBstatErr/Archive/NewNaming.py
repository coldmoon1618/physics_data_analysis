from ROOT import *
import ROOT.RooFit as rf
import math
import numpy as np

fixThres    = 5.29
lowBound    = 5.27   # Lower limit of both
upBound     = 5.29   # Upper limit of plot range

histo = TH1F('histo','',50,-5,5)

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

    trueNsig    = 69439
    purity      = .661
    trueNtot    = int(trueNsig/purity)
    trueNbkg    = trueNtot-trueNsig

    exptNsig    = np.random.poisson(trueNsig)
    exptNbkg    = np.random.poisson(trueNbkg)
    exptNtot    = exptNsig + exptNbkg

    nsig        = RooRealVar('nsig','', exptNsig)
    nbkg        = RooRealVar('nbkg','', exptNbkg)

    fit_nsig    = RooRealVar('nsig','', exptNsig, 0, 9e6)
    fit_nbkg    = RooRealVar('nbkg','', exptNbkg, 0, 9e6)

    model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
    fit_model   = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(fit_nsig,fit_nbkg))

    modelData   = model.generate(Mbc, exptNtot, rf.NumCPU(8))

    fit_model.fitTo(modelData, rf.NumCPU(8))
    fitNbkg     = fit_nbkg.getVal()
    fitNbkgErr  = fit_nbkg.getError()

    thePull = (fitNbkg-exptNbkg)/fitNbkgErr
    histo.Fill(thePull)

#outFile =  TFile('grp2Histo.root', 'recreate')
outFile =  TFile('exptFit_grp2Histo.root', 'recreate')
histo.Write('histo')
outFile.Close()
