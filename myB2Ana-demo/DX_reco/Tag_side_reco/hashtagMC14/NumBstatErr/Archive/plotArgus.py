from ROOT import *
import ROOT.RooFit as rf
import math

fixThres    = 5.29
lowBound    = 5.23   # Lower limit of both
upBound     = 5.29   # Upper limit of plot range

Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound, upBound)
mu          = RooRealVar('mu','',   5.28)
lamb        = RooRealVar('lamb','', 0.0508) 
gamma       = RooRealVar('gamma','',0.475) 
delta       = RooRealVar('delta','',19.8)
sig         = RooJohnson('sig','',  Mbc,mu,lamb,gamma,delta,lowBound)

thres       = RooRealVar('thres','',fixThres)
coef        = RooRealVar('coef','', -25)
bkg         = RooArgusBG('bkg','',  Mbc, thres, coef)

trueNsig    = 69451
trueNbkg    = 95980 
trueNtot    = trueNsig + trueNbkg
nsig        = RooRealVar('nsig','', trueNsig)
nbkg        = RooRealVar('nbkg','', trueNbkg)

fit_nsig    = RooRealVar('nsig','', trueNsig, 0, 9e6)
fit_nbkg    = RooRealVar('nbkg','', trueNbkg, 0, 9e6)

model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
fit_model   = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(fit_nsig,fit_nbkg))

n = 0
titleString = 'Example {}'.format(n)
frame1      = Mbc.frame(rf.Title(titleString)) 

modelData   = model.generate(Mbc, int(trueNtot), rf.NumCPU(8))
modelData.plotOn(frame1)
model.plotOn(frame1)
model.plotOn(frame1, rf.Components('bkg'), rf.LineStyle(kDashed))

c1 = TCanvas('c1','')
frame1.Draw()
c1.Update()
