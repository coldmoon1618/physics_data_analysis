FSPs =[
'#pi^{0}#pi^{+}K^{0}_{S}',
'2#pi^{+}#pi^{-}',
'#pi^{+}#pi^{-}K^{+}',
'#pi^{+}K^{+}K^{-}',
'#pi^{0}2#pi^{+}#pi^{-}',
'#pi^{0}#pi^{+}#pi^{-}K^{+}',
'2#pi^{+}#pi^{-}K^{0}_{S}',
'#pi^{0}#pi^{+}K^{+}K^{-}',
'#pi^{+}K^{0}_{S}K^{+}K^{-}',
'2#pi^{0}2#pi^{+}#pi^{-}',
'2#pi^{0}#pi^{+}#pi^{-}K^{+}',
'#pi^{0}2#pi^{+}#pi^{-}K^{0}_{S}',
'3#pi^{+}2#pi^{-}',
'2#pi^{+}2#pi^{-}K^{+}',
'#pi^{0}2#pi^{+}2#pi^{-}K^{+}',
]

#------------------------------------------------
# Fixed for now, optional parse later if needed
fixThres    = 5.29
nBins       = 180
lowBound    = 5.21  # Lower limit of both
upRange     = 5.27  # Upper limit of sideband
upBound     = 5.3   # Upper limit of plot range
mySubModes = []

# TODO: Type in cut string for each
# Feed in through bash
import argparse
parser      = argparse.ArgumentParser()
parser.add_argument('--fInput',  nargs='?', type=str,   default = 'AllModes_MC.root',   help='fIn')
parser.add_argument('--cutStr',  nargs='?', type=str,   default = 'SigProb>0.8',        help='Full cut string')

arguments   = parser.parse_args()
fInName     = arguments.fInput
cutString   = arguments.cutStr
cutListStr  = str(cutString.split('&&')).replace(' ','')
titleString = fInName + cutListStr 
fOutFit     = 'quickFit/'+titleString + '_fit.pdf'
fOutPull    = 'quickFit/'+titleString + '_pull.pdf'

from ROOT import *
import ROOT.RooFit as rf

#------------------------------------------------
# RooRealVar
Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound,   5.3)
subMode     = RooRealVar('subMode','',     -999,     14)
decMode     = RooRealVar('decMode','',     -999,     36)
SigProb     = RooRealVar('SigProb','',     0,      1)
beamE       = RooRealVar('beamE','',       5.28, 5.3)

iMcType     = RooRealVar('iMcType','',     1,      6)
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

#Mbc_data    = RooDataSet('Mbc_data','', RooArgSet(Mbc, SigProb, iMcType, mySignal, isSignal, accMiss, accMissGamma, accWrongFSP), rf.Import(myTree), rf.Cut(cutString))
Mbc_data    = RooDataSet('Mbc_data','', RooArgSet(Mbc, SigProb, subMode, decMode), rf.Import(myTree), rf.Cut(cutString))

frame       = Mbc.frame(rf.Title(titleString)) 
Mbc_data.plotOn(frame)

#------------------------------------------------
# Define signal and background
# sig
mu          = RooRealVar('mu','',   5.2829,5.2,5.3)
lamb        = RooRealVar('lamb','', 0.0106,0,.1) 
gamma       = RooRealVar('gamma','',0.5814,0,9) 
delta       = RooRealVar('delta','',3.8237,0,9)
sig         = RooJohnson('sig','',  Mbc,mu,lamb,gamma,delta,lowBound)

# bkg
thres       = RooRealVar('thres','',fixThres)
coef        = RooRealVar('coef','', -100,-1e3,1e3)
bkg         = RooArgusBG('bkg','',  Mbc, thres, coef)

# Add sig and bkg
nsig        = RooRealVar('nsig','', 2e6, 0, 1e9)
nbkg        = RooRealVar('nbkg','', 2e6, 0, 1e9)
model       = RooAddPdf ('model','',RooArgList(sig,bkg), RooArgList(nsig,nbkg))
#------------------------------------------------
# Fit and plot 
# Fit
Mbc.setRange('sband',lowBound,upRange)
model.fitTo(Mbc_data)

# Fit + Data overlay 
c1          = TCanvas('c1','c1',800,600)
model.plotOn(frame, rf.Components('bkg'), rf.LineStyle(kDashed), rf.LineColor(kRed), rf.LineWidth(2))
model.plotOn(frame, rf.LineWidth(2))
model.plotOn(frame, rf.Components('sig'), rf.DrawOption('F'), rf.FillColor(kOrange), rf.MoveToBack())
model.paramOn(frame, rf.Layout(0.75,1,.93))
frame.getAttText().SetTextSize(0.03)
frame.Draw()
gPad.Update()
T = TText()
T.SetNDC()
T.SetTextSize(0.03)
T.SetTextFont(42);
T.SetTextAlign(21);
Mbc.setRange('sigBox',5.27,5.29)
sigInt = sig.   createIntegral(RooArgSet(Mbc),rf.NormSet(RooArgSet(Mbc)), rf.Range('sigBox')).getVal()
myS     = sigInt*nsig.getVal()
myTot   = Mbc_data.sumEntries('','sigBox')
myB     = myTot - myS
import math
significance = myS/(math.sqrt(myS+myB))
T.DrawText(.3,.8,'SigBox=[5.27,5.29]');
T.DrawText(.3,.75 ,'S={},B={}'.format(int(myS),int(myB)));
T.DrawText(.3,.7 ,'signf={}'.format(int(round(significance))));
T.DrawText(.3,.65 ,'purity={}%'.format(int(round(myS/(myS+myB)*100))));

c1.SaveAs(fOutFit)

# Pull distr
c2          = TCanvas('c2','c2',800,600)
frame2      = Mbc.frame(rf.Title(titleString+' pull')) 
hpull       = frame.pullHist();
frame2.addPlotable(hpull, 'PL')
frame2.Draw()
frame2.SetMinimum(-5)
frame2.SetMaximum(5)
gPad.Update()
c2.SaveAs(fOutPull)

# Print out arguments to pass for more precise fitting later
print('--mymu {:.4f} --mylamb {:.4f} --mygamma {:.4f} --mydelta {:.4f} --mynsig {} --mynbkg {}'.format(
mu.getVal(),lamb.getVal(),gamma.getVal(),delta.getVal(),int(round(nsig.getVal())),int(round(nbkg.getVal()))))
