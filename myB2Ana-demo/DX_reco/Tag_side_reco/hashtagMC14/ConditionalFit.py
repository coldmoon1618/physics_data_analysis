#------------------------------------------------
# Fixed for now, optional parse later if needed
fixThres    = 5.29
lowBound    = 5.21  # Lower limit of both
upRange     = 5.26  # Upper limit of sideband
upBound     = 5.3   # Upper limit of plot range

import argparse
parser      = argparse.ArgumentParser()
# Fit functions
parser.add_argument('--mym0',    nargs='?', type=float, default = -999,                 help='Init or fix mean')
parser.add_argument('--mymean',  nargs='?', type=float, default = -999,                 help='Init or fix mean')
parser.add_argument('--mysigma', nargs='?', type=float, default = -999,                 help='Init or fix sigma')
parser.add_argument('--mymu',    nargs='?', type=float, default = -999,                 help='Init or fix mu')
parser.add_argument('--mylamb',  nargs='?', type=float, default = -999,                 help='Init or fix lamb')
parser.add_argument('--mygamma', nargs='?', type=float, default = -999,                 help='Init or fix gamma')
parser.add_argument('--mydelta', nargs='?', type=float, default = -999,                 help='Init or fix delta')
parser.add_argument('--myalpha', nargs='?', type=float, default = -999,                 help='Init or fix alpha')
parser.add_argument('--myn',     nargs='?', type=float, default = -999,                 help='Init or fix nsig')
parser.add_argument('--mycoef',  nargs='?', type=float, default = -999,                 help='Init or fix Argus coef')
parser.add_argument('--fixSig',  nargs='?', type=int,   default = 0,                    help='Sig: Fixed = 1, init = 0')
parser.add_argument('--fixBkg',  nargs='?', type=int,   default = 0,                    help='Bkg: Fixed = 1, init = 0')
parser.add_argument('--mynsig',  nargs='?', type=int,   default = -999,                 help='Init or fix nsig')
parser.add_argument('--mynbkg',  nargs='?', type=int,   default = -999,                 help='Init or fix nbkg')
parser.add_argument('--cutStr',  nargs='?', type=str,   default = 'groupID==3',         help='Full cut string')

# Fit configs 
parser.add_argument('--fInput',  nargs='?', type=str,   default = 'Groups_MC.root',     help='fIn')
parser.add_argument('--nPltBin', nargs='?', type=int,   default = 180,                  help='Num plot bins for unbinned fit')
parser.add_argument('--sigPDF',  nargs='?', type=str,   default = 'Johnson',            help='SigPDF function')
parser.add_argument('--fitName', nargs='?', type=str,   default = 'fitResult.root',     help='Output name for fit results' )
parser.add_argument('--saveHist',nargs='?', type=int,   default = 0,                    help='Save h_Mbc')
parser.add_argument('--saveFit', nargs='?', type=int,   default = 1,                    help='Save fit root')
parser.add_argument('--writeCSV',nargs='?', type=int,   default = 1,                    help='Write to results.csv')
parser.add_argument('--writeFix',nargs='?', type=int,   default = 0,                    help='Output fix fit params')
parser.add_argument('--scaleLum',nargs='?', type=int,   default = 0,                    help='Apply 190/300 scale factor when writing out results.csv')

arguments   = parser.parse_args()
mym0        = arguments.mym0  
mymean      = arguments.mymean
mysigma     = arguments.mysigma
mymu        = arguments.mymu
mylamb      = arguments.mylamb
mygamma     = arguments.mygamma
mydelta     = arguments.mydelta
myalpha     = arguments.myalpha
myn         = arguments.myn   
mycoef      = arguments.mycoef
fixSig      = arguments.fixSig>.5
fixBkg      = arguments.fixBkg>.5
mynsig      = arguments.mynsig
mynbkg      = arguments.mynbkg
cutStr      = arguments.cutStr

fInName     = arguments.fInput
nBins       = arguments.nPltBin
sigPDF      = arguments.sigPDF
fitName     = arguments.fitName
pyCutString = arguments.cutStr.replace('&&',' and ').replace('||',' or ')

titleString = '' 
isMC = False 
isSkimmed = False
if(fInName.split('_')[1]=='MC.root'):
    isMC = True
    titleString = 'MC_' 
else:
    titleString = 'Data_'
if(fInName.split('_')[0]=='Groups'):
    isSkimmed = True

titleString+=cutStr.replace('&&','and').replace('||','or') 

from ROOT import *
import ROOT.RooFit as rf

#------------------------------------------------
# RooRealVar
Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound, upBound)
groupID     = RooRealVar('groupID','',     -999,     30)
decMode     = RooRealVar('decMode','',      0,36)
subMode     = RooRealVar('subMode','',      0,100)

beamE       = RooRealVar('beamE','',       5.28,     upBound)

iMcType     = RooRealVar('iMcType','',     -1,     6)
mySignal    = RooRealVar('mySignal','',    -1,     4)
isSignal    = RooRealVar('isSignal','',    -1,     1)
accMiss     = RooRealVar('accMiss','',     -1,     1)
accMissGamma= RooRealVar('accMissGamma','',-1,     1)
accWrongFSP = RooRealVar('accWrongFSP','', -1,     1)

#------------------------------------------------
# Import data
Mbc.setBins(nBins)
myTree = TChain('tree')
myTree.Add(fInName)
beamE_data  = RooDataSet('beamE_data','', RooArgSet(beamE), rf.Import(myTree))
beamE_histo = beamE_data.binnedClone()
beamE_pdf   = RooHistPdf('beamE_pdf','',RooArgSet(beamE),beamE_histo, 2)

if(isMC):
    Mbc_data    = RooDataSet('Mbc_data','', RooArgSet(Mbc, beamE, groupID, subMode, decMode, mySignal, iMcType), rf.Import(myTree), rf.Cut(cutStr))
elif(isSkimmed):
    Mbc_data    = RooDataSet('Mbc_data','', RooArgSet(Mbc, beamE, groupID), rf.Import(myTree), rf.Cut(cutStr))
else:
    Mbc_data    = RooDataSet('Mbc_data','', RooArgSet(Mbc, beamE, subMode, decMode), rf.Import(myTree), rf.Cut(cutStr))

frame       = Mbc.frame(rf.Title(titleString)) 
Mbc_data.plotOn(frame)

#------------------------------------------------
# Define signal and background
# sig
if(sigPDF=='Johnson'):
    mu          = RooRealVar('mu','',   5.28,5.2,5.3)
    lamb        = RooRealVar('lamb','', 0.02,0,.1) 
    gamma       = RooRealVar('gamma','',1.27,0,9) 
    delta       = RooRealVar('delta','',5   ,0,20)
    if(mymu>-900):
        mu      = RooRealVar('mu','',   mymu    ,5.2,5.3)
        if(fixSig):
            mu  = RooRealVar('mu','',   mymu)
    if(mylamb>-900):
        lamb    = RooRealVar('lamb','', mylamb  ,0,.1)
        if(fixSig):
            lamb= RooRealVar('lamb','', mylamb)
    if(mygamma>-900):
        gamma   = RooRealVar('gamma','',mygamma, 0,9)
        if(fixSig):
            gamma=RooRealVar('gamma','',mygamma)
    if(mydelta>-900):
        delta   = RooRealVar('delta','',mydelta, 0,20)
        if(fixSig):
            delta=RooRealVar('delta','',mydelta)
    sig         = RooJohnson('sig','',  Mbc,mu,lamb,gamma,delta,lowBound)
elif(sigPDF=='Gaus'):
    mean        = RooRealVar('mean','', 5.28,5.2,5.3)
    sigma       = RooRealVar('sigma','',0.002,0,.2)
    if(mymean>-900):
        mean    = RooRealVar('mean','',mymean, 5.2,5.3)
        if(fixSig):
            mean= RooRealVar('mean','',mymean)
    if(mysigma>-900):
        sigma   = RooRealVar('sigma','',mysigma,0,.2)
        if(fixSig):
            sigma=RooRealVar('sigma','',mysigma)
    sig         = RooGaussian('sig', '', Mbc, mean, sigma);
elif(sigPDF=='CB'):
    m0          = RooRealVar('m0','',5.28,5.27,5.29)
    sigma       = RooRealVar('sigma','',0.003,0,.2)
    n           = RooRealVar('n','',3,0,6)
    alpha       = RooRealVar('alpha','',1.5,0,3)
    if(mym0>-900):
        m0      = RooRealVar('m0','',mym0, 5.2,5.3)
        if(fixSig):
            m0  = RooRealVar('m0','',mym0)
    if(mysigma>-900):
        sigma   = RooRealVar('sigma','',mysigma,0,.2)
        if(fixSig):
            sigma=RooRealVar('sigma','',mysigma)
    if(myn>-900):
        n       = RooRealVar('n','',myn,0,6)
        if(fixSig):
            n   = RooRealVar('n','',myn)
    if(myalpha>-99):
        alpha   = RooRealVar('alpha','',myalpha,0,3)
        if(fixSig):
            alpha=RooRealVar('alpha','',myalpha)
    sig         = RooCBShape('sig','',Mbc,m0,sigma,alpha,n)

# bkg
thres       = RooRealVar('thres','',fixThres)
coef        = RooRealVar('coef','', -100,-1e3,1e3)
if(mycoef>-900):
    coef    = RooRealVar('coef','', mycoef,-1e3,1e3)
    if(fixBkg):
        coef= RooRealVar('coef','',mycoef)

if(isMC):
    bkg     = RooArgusBG('bkg','',  Mbc, thres, coef)
else:
    bkg     = RooArgusBG('bkg','',  Mbc, beamE, coef)

# Add sig and bkg
nsig        = RooRealVar('nsig','', 3e4, 0, 1e9)
nbkg        = RooRealVar('nbkg','', 1e4, 0, 1e9)
if(mynsig>-900):
    nsig    = RooRealVar('nsig','', mynsig, 0, 1e9)
if(mynbkg>-900):
    nbkg    = RooRealVar('nbkg','', mynbkg, 0, 1e9)
model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
#------------------------------------------------
# Fit and plot 
# Fit
Mbc.setRange('sband',lowBound,upRange)
if(not isMC):
    bkg.fitTo(Mbc_data, rf.Range('sband'), rf.ConditionalObservables(RooArgSet(beamE)), rf.NumCPU(8))
    fitresult = model.fitTo(Mbc_data, rf.ConditionalObservables(RooArgSet(beamE)),rf.Save(), rf.NumCPU(8))
else:
    bkg.fitTo(Mbc_data, rf.Range('sband'), rf.NumCPU(8))
    fitresult = model.fitTo(Mbc_data,rf.Save(), rf.NumCPU(8))

# Fit + Data overlay 
c1          = TCanvas('c1','c1',800,700)
pad1        = TPad('pad1','pad1',0.0,.30,1.0,1.0)
pad2        = TPad('pad2','pad2',0.0,.05,1.0,.3 )
pad1.Draw()
pad1.cd()
if(isMC):
    model.plotOn(frame, rf.Components('bkg'), rf.LineStyle(kDashed), rf.LineColor(kRed), rf.LineWidth(2), rf.NumCPU(8))
    model.plotOn(frame, rf.LineWidth(2), rf.NumCPU(8))
else:
    model.plotOn(frame, rf.Components('bkg'), rf.LineStyle(kDashed), rf.LineColor(kRed), rf.LineWidth(2), rf.ProjWData(beamE_data,kTRUE))
    model.plotOn(frame, rf.LineWidth(2), rf.ProjWData(beamE_data,kTRUE))
# Can't use multiple core when doing ProjWData
#model.plotOn(frame, rf.Components('sig'), rf.DrawOption('F'), rf.FillColor(kOrange), rf.MoveToBack(), rf.ProjWData(beamE_data,kTRUE))
model.paramOn(frame, rf.Layout(0.76,1,.9))
frame.getAttText().SetTextSize(0.03)
frame.Draw()
gPad.Update()

Mbc.setRange('sigBox',5.27,5.29)
nFitSig = nsig.getVal()
sigInt  = sig.   createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
myS     = sigInt*nFitSig
mySerr  = sigInt*nsig.getError()
eNSig   = nsig.getError()
myTot   = Mbc_data.sumEntries('','sigBox')
import math
purity = myS*1.0/(myTot); 

import numpy as np
if(arguments.saveHist>.5):

    mcTree = TChain('tree')
    mcTree.Add('Groups_MC.root')
    Entries = mcTree.GetEntries()
    mySignalLst=np.empty((1), dtype='i')
    mcTree.SetBranchAddress('mySignal',mySignalLst)
    iMcTypeLst=np.empty((1), dtype='i')
    mcTree.SetBranchAddress('iMcType',iMcTypeLst)
    groupIDLst=np.empty((1), dtype='i')
    mcTree.SetBranchAddress('groupID',groupIDLst)
    decModeLst=np.empty((1), dtype='float')
    MbcLst=np.empty((1), dtype='float')
    mcTree.SetBranchAddress('Mbc',MbcLst)

    lowBound    = 5.21  # Lower limit of both
    upBound     = 5.3   # Upper limit of plot range
    h_Mbc = TH1F('h_Mbc','',nBins,lowBound,upBound)
    nHistBkg = 0
    for i in range(Entries):
        mcTree.GetEntry(i)
        mySignal= mySignalLst[0]
        iMcType= iMcTypeLst[0]
        groupID= groupIDLst[0]
        if((eval(pyCutString)) and iMcType!=1):
            h_Mbc.Fill(MbcLst[0])
            nHistBkg+=1
    h_Mbc.SetLineColor(kGreen+2)
    h_Mbc.SetLineWidth(2)
    nFitBkg = int(round(nbkg.getVal()))
    h_Mbc.Scale(nFitBkg*1.0/nHistBkg)
    h_Mbc.Draw('HIST same')

# Pull distr
c1.cd()
pad2.Draw()
pad2.cd()
frame2      = Mbc.frame() 
hpull       = frame.pullHist();
frame2.addPlotable(hpull, 'P')
frame2.Draw()
frame2.SetMinimum(-5)
frame2.SetMaximum(5)
gPad.Update()

if(arguments.saveFit>.5):
    myFile = TFile('FitGroupsResults/'+fitName,'recreate')
    fitresult.Write('fitresult')
    frame   .Write('frame')
    frame2  .Write('frame2')
    if(arguments.saveHist>.5):
        h_Mbc   .Write('h_Mbc')
    myFile  .Close()

if(arguments.writeCSV>.5):
    x = open('results.csv', 'a')
    if(arguments.scaleLum>.5 and isMC):
        nFitSig = int(round(.19/.3*nFitSig)) 
        eNSig   = int(round(.19/.3*eNSig)) 
        myS     = int(round(.19/.3*myS))
        mySerr  = int(round(.19/.3*mySerr))
    sigBoxStr = '{:.0f}±{:.0f}'.format(myS,mySerr)
    #           fitName     nSigTotal        purity   sigBoxStr
    x.writelines('{} , {:.0f}±{:.0f} , {:.3f} , {}\n'.format(fitName, nFitSig, eNSig, purity, sigBoxStr))
    x.close()

if(sigPDF=='Gaus'):
    sigPDFName = 'Gaussian'
else:
    sigPDFName = sigPDF

if(arguments.writeFix>.5):
    y = open('paramFixFit.sh','a')
    y.writelines('python3 ConditionalFit.py --fInput "AllModes_MC.root" --cutStr "iMcType==0&&{}" --saveHist 0 '.format(cutStr))
    y.writelines('--sigPDF "{}" --fitName {} --fixSig 1 '.format(sigPDF,fitName,sigPDFName))
    if(sigPDF=='Johnson'):
        y.writelines('--mymu {:.4f} --mylamb {:.4f} --mygamma {:.4f} --mydelta {:.4f} \n'.format(mu.getVal(), lamb.getVal(), gamma.getVal(), delta.getVal() ))
        y.close()
    elif(sigPDF=='Gaus'):
        y.writelines('--mymean {:.4f} --mysigma {:.4f} \n'.format(mean.getVal(), sigma.getVal() ))
        y.close()
    elif(sigPDF=='CB'):
        y.writelines('--mym0 {:.4f} --mysigma {:.4f} --myn {:.4f} --myalpha {:.4f} \n'.format(m0.getVal(), sigma.getVal(), n.getVal(), alpha.getVal() ))
        y.close()
