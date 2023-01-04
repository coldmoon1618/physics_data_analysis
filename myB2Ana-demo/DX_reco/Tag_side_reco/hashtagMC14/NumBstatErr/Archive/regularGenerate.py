from ROOT import *
import ROOT.RooFit as rf
import math

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
    coef        = RooRealVar('coef','', -25)
    bkg         = RooArgusBG('bkg','',  Mbc, thres, coef)

    trueNsig    = 69439
    purity      = .661
    trueNtot    = trueNsig/purity
    trueNbkg    = trueNtot-trueNsig
    nsig        = RooRealVar('nsig','', trueNsig)
    nbkg        = RooRealVar('nbkg','', trueNbkg)

    fit_nsig    = RooRealVar('nsig','', trueNsig, 0, 9e6)
    fit_nbkg    = RooRealVar('nbkg','', trueNbkg, 0, 9e6)

    model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
    fit_model   = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(fit_nsig,fit_nbkg))

    titleString = 'Try'
    frame1      = Mbc.frame(rf.Title(titleString)) 

    modelData   = model.generate(Mbc, int(trueNtot), rf.NumCPU(8))
    modelData.plotOn(frame1)
    model.plotOn(frame1)

    fit_model.fitTo(modelData, rf.NumCPU(8))
    fitNbkg     = fit_nbkg.getVal()
    fitNbkgErr  = fit_nbkg.getError()

    print('true: ',int(trueNbkg))
    print('fit : ',fitNbkg,'+/-',fitNbkgErr,' ')

    thePull = (fitNbkg-trueNbkg)/fitNbkgErr
    print('pull: ',thePull)
    histo.Fill(thePull)

outFile =  TFile('regHisto.root', 'recreate')
histo.Write('histo')
outFile.Close()
#fit_model.plotOn(frame1, rf.LineColor(kRed))
#fit_model.paramOn(frame1, rf.Layout(0.76,1,.9))

#c1 = TCanvas('c1','')
#frame1.Draw()
#c1.Update()

