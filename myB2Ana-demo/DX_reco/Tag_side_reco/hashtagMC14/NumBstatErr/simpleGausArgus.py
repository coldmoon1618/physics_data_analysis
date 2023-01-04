from ROOT import *
import ROOT.RooFit as rf
import math
import sys
import numpy as np

fixThres    = 5.29
lowBound    = 5.21  # Lower limit of both
upRange     = 5.27  # Upper limit of sideband
upBound     = 5.3   # Upper limit of plot range

h_true = TH1F('h_true','',50,-5,5)
h_expt = TH1F('h_expt','',50,-5,5)
h_sgBx = TH1F('h_sgBx','',50,-5,5)
f = open("exampleExpts.txt", "w")

for n in range(1000):
    # Signal function
    Mbc         = RooRealVar('Mbc', 'Mbc', lowBound, upBound)
    mean        = RooRealVar('mean','', 5.28,5.2,5.3)
    sigma       = RooRealVar('sigma','',0.002,0,.2)
    sig         = RooGaussian('sig', '', Mbc, mean, sigma);

    # Bkg function
    thres       = RooRealVar('thres','',fixThres)
    coef        = RooRealVar('coef','', -25)
    bkg         = RooArgusBG('bkg','',  Mbc, thres, coef)

    # True values
    trueNsig    = 600 
    purity      = .06  # WARNING: not the correct definition yet
    trueNtot    = 8600 #trueNsig/purity
    trueNbkg    = trueNtot-trueNsig

    # Expt values
    exptNsig    = np.random.poisson(trueNsig)
    exptNbkg    = np.random.poisson(trueNbkg)
    exptNtot    = exptNsig + exptNbkg

    # expt model 
    nsig        = RooRealVar('nsig','', exptNsig)
    nbkg        = RooRealVar('nbkg','', exptNbkg)
    model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
    modelData   = model.generate(Mbc, int(trueNtot), rf.NumCPU(8))
    
    # fit model
    fit_nsig    = RooRealVar('nsig','', exptNsig, 0, 9e6)
    fit_nbkg    = RooRealVar('nbkg','', exptNbkg, 0, 9e9)
    fit_model   = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(fit_nsig,fit_nbkg))

    # Obtain sigBox nBkg before fitting
    Mbc.setRange('sigBox',5.27,5.29)
    exptBkgInt      = bkg.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
    exptBkgSgBx     = exptNbkg * exptBkgInt

    # Fitting and yields
    fit_model.fitTo(modelData, rf.NumCPU(8))
    fitNbkg     = fit_nbkg.getVal()
    fitNbkgErr  = fit_nbkg.getError()

    # Obtain sigBox nBkg after fitting
    fit_BkgInt      = bkg.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
    fit_BkgSgBx     = fitNbkg   *fit_BkgInt
    fit_BkgSgBxErr  = fitNbkgErr*fit_BkgInt

    # Calculate and save pull values 
    thePull_true = (fitNbkg-trueNbkg)/math.sqrt(fit_BkgSgBx)
    h_true.Fill(thePull_true)
    thePull_expt = (fitNbkg-exptNbkg)/fitNbkgErr
    h_expt.Fill(thePull_expt)
    thePull_SgBx = (fit_BkgSgBx-exptBkgSgBx)/fit_BkgSgBxErr
    h_sgBx.Fill(thePull_SgBx)

    # For graphical demos
    if(n<100):
        titleString = 'PseudoExpt_{}'.format(n)
        frame   = Mbc.frame(rf.Title(titleString)) 
        model.plotOn(frame)
        fit_model.plotOn(frame, rf.LineColor(kRed))

        c = TCanvas('c{}'.format(n),'',800,600)
        frame.Draw()

        # Not sure why there's problem with plotting data points
        # But this is not the focus of this study
        #frame1  = Mbc.frame(rf.Title(titleString)) 
        #modelData.plotOn(frame1)
        #frame1.Draw('same')

        c.SaveAs('demoPlots/{}.pdf'.format(n))

        print('===================================================',file=f)
        print(n,                                                    file=f)
        print('true bkg: '  , int(trueNbkg),                        file=f)
        print('expt bkg: '  , int(exptNbkg),                        file=f)
        print('',                                                   file=f)

        print('fit  bkg: '  , int(fitNbkg),'+/-', int(fitNbkgErr),  file=f)
        print('true pull: ' , thePull_true,                         file=f)
        print('expt pull: ' , thePull_expt,                         file=f)
        print('',                                                   file=f)

        print('exptBkgSgBx' , int(exptBkgSgBx),                     file=f)
        print('fitBkgSgBx',int(fit_BkgSgBx),'+/-',int(fit_BkgSgBxErr),file=f)
        print('sigbox pull:', thePull_SgBx,                         file=f)
       
        print('',                                                   file=f)

outFile =  TFile('PullDistrs.root', 'recreate')
h_true.Write('h_true')
h_expt.Write('h_expt')
h_sgBx.Write('h_sgBx')
outFile.Close()
