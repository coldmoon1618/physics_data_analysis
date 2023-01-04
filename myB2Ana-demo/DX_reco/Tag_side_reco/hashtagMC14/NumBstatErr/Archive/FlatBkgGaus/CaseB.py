from ROOT import *
import ROOT.RooFit as rf
import math
import numpy as np

fixThres    = 5.29
lowBound    = 5.27   # Lower limit of both
upBound     = 5.29   # Upper limit of plot range

histo = TH1F('histo','',50,-5,5)

for n in range(30000):
    Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound, upBound)
    mean        = RooRealVar('mean','',   5.28)
    sigma       = RooRealVar('sigma','',  0.002, 1e-9,1)
    sig         = RooGaussian('sig','',  Mbc, mean, sigma)

    c0          = RooRealVar("c0","coefficient #0", 1.0,-1.,1.)
    c1          = RooRealVar("c1","coefficient #1", 0.1,-1.,1.)
    #c2         =RooRealVar("c2","coefficient #2",-0.1,-1.,1.)
    #bkg         = RooChebychev('bkg','',Mbc, RooArgList(c0,c1,c2))
    bkg         = RooChebychev('bkg','',Mbc, RooArgList(c0,c1))

    trueNsig    = 700 
    purity      = .7
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

    c   = TCanvas('c','',400,300)
    frame = Mbc.frame()
    model.plotOn(frame)
    fit_model.plotOn(frame,rf.LineColor(kRed))
    frame.Draw()
    c.SaveAs('CaseB_{}.pdf'.format(n))
    '''

    print('true values: ', trueNsig, trueNbkg)
    print('expt values: ', exptNsig, exptNbkg)
    print('expt values: ', exptNtot-fitNbkg,  fitNbkg)
    print()
    '''

outFile =  TFile('CaseB.root', 'recreate')
histo.Write('histo')
outFile.Close()
