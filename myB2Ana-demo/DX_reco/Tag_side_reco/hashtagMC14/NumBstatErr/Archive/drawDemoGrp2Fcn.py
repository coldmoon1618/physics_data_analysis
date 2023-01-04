from ROOT import *
import ROOT.RooFit as rf
import math
import numpy as np

fixThres    = 5.29
lowBound    = 5.27   # Lower limit of both
upBound     = 5.29   # Upper limit of plot range

histo = TH1F('histo','',50,-5,5)
for n in range(4):
    Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound, upBound)
    mu          = RooRealVar('mu','',   5.28)
    lamb        = RooRealVar('lamb','', 0.0508) 
    gamma       = RooRealVar('gamma','',0.475) 
    delta       = RooRealVar('delta','',19.8)
    sig         = RooJohnson('sig','',  Mbc,mu,lamb,gamma,delta,lowBound)

    thres       = RooRealVar('thres','',fixThres)
    coef        = RooRealVar('coef','', -117.13)
    bkg         = RooArgusBG('bkg','',  Mbc, thres, coef)

    expNsig    = np.random.poisson(700)
    expNbkg    = np.random.poisson(300) 
    expNtot    = expNsig + expNbkg
    nsig        = RooRealVar('nsig','', expNsig)
    nbkg        = RooRealVar('nbkg','', expNbkg)

    fit_nsig    = RooRealVar('nsig','', expNsig, 0, 9e6)
    fit_nbkg    = RooRealVar('nbkg','', expNbkg, 0, 9e6)

    model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
    fit_model   = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(fit_nsig,fit_nbkg))

    titleString = 'Example {}'.format(n)
    frame1      = Mbc.frame(rf.Title(titleString)) 

    modelData   = model.generate(Mbc, int(expNtot), rf.NumCPU(8))
    Mbc.setBins(16)
    modelData.plotOn(frame1)
    model.plotOn(frame1)
    model.plotOn(frame1, rf.Components('bkg'), rf.LineStyle(kDashed))

    fit_model.fitTo(modelData, rf.NumCPU(8))
    fitNbkg     = fit_nbkg.getVal()
    fitNbkgErr  = fit_nbkg.getError()

    thePull     = (fitNbkg-expNbkg)/fitNbkgErr
    labelStr    = 'trueNbkg=300, exptNbkg = {}, pull= {:.3f}'.format(expNbkg, thePull)
    print('pull: ',thePull)
    histo.Fill(thePull)

    fit_model.plotOn(frame1, rf.LineColor(kRed))
    fit_model.plotOn(frame1, rf.LineColor(kRed), rf.Components('bkg'), rf.LineStyle(kDashed))
    fit_model.paramOn(frame1, rf.Layout(0.76,1,.9))
    c1 = TCanvas('c1','')
    frame1.SetTitle(labelStr)
    frame1.Draw()
    #text = TText(.8,.9,labelStr)
    #text.Draw('same')
    c1.SaveAs('Sample_{}.pdf'.format(n))
