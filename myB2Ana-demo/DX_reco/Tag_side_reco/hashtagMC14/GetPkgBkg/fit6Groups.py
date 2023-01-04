from ROOT import *
import numpy as np
import ROOT.RooFit as rf

# start 7:30am Mar 9
#------------------------------------------------
# Fixed for now, optional parse later if needed
fixThres    = 5.29
lowBound    = 5.21  # Lower limit of both
upRange     = 5.26  # Upper limit of sideband
upBound     = 5.3   # Upper limit of plot range

# ---------------------------------
Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound, upBound)
beamE       = RooRealVar('beamE','',       5.28,     upBound)

iMcType     = RooRealVar('iMcType','',     -1,     6)
mySignal    = RooRealVar('mySignal','',    -1,     4)
groupID     = RooRealVar('groupID','',      0,  29)
thres       = RooRealVar('thres','',fixThres)

# ---------------------------------
m0_01       = RooRealVar('m0_01','', 5.28,5.2,5.3)
sigma_01    = RooRealVar('sigma_01','',0.002,0,.2)
n_01        = RooRealVar('n','',3,0,6)
alpha_01    = RooRealVar('alpha','',1.5,0,3)
sig_01      = RooCBShape('sig_01','',Mbc,m0_01,sigma_01,alpha_01,n_01)
coef_01     = RooRealVar('coef_01','', -100,-1e3,1e3)
bkg_01      = RooArgusBG('bkg_01','',  Mbc, thres, coef_01)
nsig_01     = RooRealVar('nsig_01','', 3e4, 0, 1e9)
nbkg_01     = RooRealVar('nbkg_01','', 1e4, 0, 1e9)
model_01    = RooAddPdf ('model_01','',RooArgList(sig_01,bkg_01), RooArgList(nsig_01,nbkg_01))

# ---------------------------------
mu_02       = RooRealVar('mu_02','', 5.28,5.2,5.3)
lamb_02     = RooRealVar('lamb','', 0.02,0,.1) 
gamma_02    = RooRealVar('gamma','',1.27,0,9) 
delta_02    = RooRealVar('delta','',5   ,0,20)
sig_02      = RooJohnson('sig_02','',  Mbc,mu_02,lamb_02,gamma_02,delta_02,lowBound)
coef_02     = RooRealVar('coef_02','', -100,-1e3,1e3)
bkg_02      = RooArgusBG('bkg_02','',  Mbc, thres, coef_02)
nsig_02     = RooRealVar('nsig_02','', 3e4, 0, 1e9)
nbkg_02     = RooRealVar('nbkg_02','', 1e4, 0, 1e9)
model_02    = RooAddPdf ('model_02','',RooArgList(sig_02,bkg_02), RooArgList(nsig_02,nbkg_02))

# ---------------------------------
mu_03       = RooRealVar('mu_03','', 5.28,5.2,5.3)
lamb_03     = RooRealVar('lamb','', 0.02,0,.1) 
gamma_03    = RooRealVar('gamma','',1.27,0,9) 
delta_03    = RooRealVar('delta','',5   ,0,20)
sig_03      = RooJohnson('sig_03','',  Mbc,mu_03,lamb_03,gamma_03,delta_03,lowBound)
coef_03     = RooRealVar('coef_03','', -100,-1e3,1e3)
bkg_03      = RooArgusBG('bkg_03','',  Mbc, thres, coef_03)
nsig_03     = RooRealVar('nsig_03','', 3e4, 0, 1e9)
nbkg_03     = RooRealVar('nbkg_03','', 1e4, 0, 1e9)
model_03    = RooAddPdf ('model_03','',RooArgList(sig_03,bkg_03), RooArgList(nsig_03,nbkg_03))

# ---------------------------------
m0_06       = RooRealVar('m0_06','', 5.28,5.2,5.3)
sigma_06    = RooRealVar('sigma_06','',0.002,0,.2)
n_06        = RooRealVar('n','',3,0,6)
alpha_06    = RooRealVar('alpha','',1.5,0,3)
sig_06      = RooCBShape('sig_06','',Mbc,m0_06,sigma_06,alpha_06,n_06)
coef_06     = RooRealVar('coef_06','', -100,-1e3,1e3)
bkg_06      = RooArgusBG('bkg_06','',  Mbc, thres, coef_06)
nsig_06     = RooRealVar('nsig_06','', 3e4, 0, 1e9)
nbkg_06     = RooRealVar('nbkg_06','', 1e4, 0, 1e9)
model_06    = RooAddPdf ('model_06','',RooArgList(sig_06,bkg_06), RooArgList(nsig_06,nbkg_06))
 
# ---------------------------------
m0_07       = RooRealVar('m0_07','', 5.28,5.2,5.3)
sigma_07    = RooRealVar('sigma_07','',0.002,0,.2)
n_07        = RooRealVar('n','',3,0,6)
alpha_07    = RooRealVar('alpha','',1.5,0,3)
sig_07      = RooCBShape('sig_07','',Mbc,m0_07,sigma_07,alpha_07,n_07)
coef_07     = RooRealVar('coef_07','', -100,-1e3,1e3)
bkg_07      = RooArgusBG('bkg_07','',  Mbc, thres, coef_07)
nsig_07     = RooRealVar('nsig_07','', 3e4, 0, 1e9)
nbkg_07     = RooRealVar('nbkg_07','', 1e4, 0, 1e9)
model_07    = RooAddPdf ('model_07','',RooArgList(sig_07,bkg_07), RooArgList(nsig_07,nbkg_07))
 
# ---------------------------------
mean_09     = RooRealVar("mean_09",'',5.28,5.2,5.3)
sigma_09    = RooRealVar('sigma_09','',0.002,0,.2)
sig_09      = RooGaussian('sig_09', '', Mbc, mean_09, sigma_09);
coef_09     = RooRealVar('coef_09','', -100,-1e3,1e3)
bkg_09      = RooArgusBG('bkg_09','',  Mbc, thres, coef_09)
nsig_09     = RooRealVar('nsig_09','', 3e4, 0, 1e9)
nbkg_09     = RooRealVar('nbkg_09','', 1e4, 0, 1e9)
model_09    = RooAddPdf ('model_09','',RooArgList(sig_09,bkg_09), RooArgList(nsig_09,nbkg_09))

#------------------------------------------------
# Import data
nBins = 180 
fInName = '../Groups_MC.root'
Mbc.setBins(nBins)
myTree = TChain('tree')
myTree.Add(fInName)

Mbc_data_list = list()

Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID==1&&mySignal>0.5'))) 
Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID==2&&mySignal>0.5'))) 
Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID==3&&mySignal>0.5'))) 
Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID==6&&mySignal>0.5'))) 
Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID==7&&mySignal>0.5'))) 
Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID==9&&mySignal>0.5'))) 
#for i in range(3):
    #Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID=={}'.format(i)))) 
    #Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID=={}&&mySignal>0.5'.format(i)))) 
frame       = Mbc.frame() 
#Mbc_data.plotOn(frame)

# Create category observables for splitting
# ----------------------------------------------------------------------------------
 
# Define categories that can be used for splitting
grp = RooCategory("grp", "grp")
grp.defineType("group01")
grp.defineType("group02")
grp.defineType("group03")
grp.defineType("group06")
grp.defineType("group07")
grp.defineType("group09")

Mbc_data = RooDataSet("combData", "combined data", RooArgSet(Mbc), rf.Index(grp), 
rf.Import("group01",Mbc_data_list[0]), 
rf.Import("group02",Mbc_data_list[1]),
rf.Import("group03",Mbc_data_list[2]),
rf.Import("group06",Mbc_data_list[3]), 
rf.Import("group07",Mbc_data_list[4]),
rf.Import("group09",Mbc_data_list[5]),
) 

# TODO: maybe eventually only one set of nsig and nbkg
#simSig = RooSimultaneous("simSig","simultaneous pdf",grp)
#simBkg = RooSimultaneous("simBkg","simultaneous pdf",grp)
#simPdf   = RooAddPdf ('simPdf','',RooArgList(simSig,simBkg), RooArgList(nsig,nbkg))
#Mbc_data_list[0].plotOn(frame)
#simSig.plotOn(frame,rf.Slice(grp,"group00"),rf.ProjWData(grp,Mbc_data))

simPdf = RooSimultaneous("simPdf","simultaneous pdf",grp)
simPdf.addPdf(model_01,"group01")
simPdf.addPdf(model_02,"group02")
simPdf.addPdf(model_03,"group03")
simPdf.addPdf(model_06,"group06")
simPdf.addPdf(model_07,"group07")
simPdf.addPdf(model_09,"group09")
fitresult = simPdf.fitTo(Mbc_data, rf.NumCPU(8)) 

frame = Mbc.frame(rf.Title("groups: 1,2,3,6,7,9"))
Mbc_data.plotOn(frame)
simPdf.plotOn(frame,rf.ProjWData(grp,Mbc_data), rf.NumCPU(8))
simPdf.plotOn(frame,rf.ProjWData(grp,Mbc_data),rf.Components("bkg_01,bkg_02,bkg_03,bkg_06,bkg_07,bkg_09"),rf.LineColor(kRed),rf.LineStyle(kDashed), rf.NumCPU(8))

c = TCanvas("c","c",800,600) 
frame.Draw()
gPad.Update()

Mbc.setRange('sigBox',5.27,5.29)
sigInt_01   = sig_01.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
boxNsig_01  = sigInt_01*nsig_01.getVal()
boxEsig_01  = sigInt_01*nsig_01.getError()
sigInt_02   = sig_02.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
boxNsig_02  = sigInt_02*nsig_02.getVal()
boxEsig_02  = sigInt_02*nsig_02.getError()
sigInt_03   = sig_03.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
boxNsig_03  = sigInt_03*nsig_03.getVal()
boxEsig_03  = sigInt_03*nsig_03.getError()
sigInt_06   = sig_06.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
boxNsig_06  = sigInt_06*nsig_06.getVal()
boxEsig_06  = sigInt_06*nsig_06.getError()
sigInt_07   = sig_07.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
boxNsig_07  = sigInt_07*nsig_07.getVal()
boxEsig_07  = sigInt_07*nsig_07.getError()
sigInt_09   = sig_09.createIntegral(RooArgSet(Mbc), RooArgSet(Mbc), 'sigBox').getVal()
boxNsig_09  = sigInt_09*nsig_09.getVal()
boxEsig_09  = sigInt_09*nsig_09.getError()

x = open('results.csv', 'a')
x.writelines('{:.0f},{:.0f}, {:.0f}±{:.0f}\n'.format(boxNsig_01,boxEsig_01,boxNsig_01,boxEsig_01))
x.writelines('{:.0f},{:.0f}, {:.0f}±{:.0f}\n'.format(boxNsig_02,boxEsig_02,boxNsig_02,boxEsig_02))
x.writelines('{:.0f},{:.0f}, {:.0f}±{:.0f}\n'.format(boxNsig_03,boxEsig_03,boxNsig_03,boxEsig_03))
x.writelines('{:.0f},{:.0f}, {:.0f}±{:.0f}\n'.format(boxNsig_06,boxEsig_06,boxNsig_06,boxEsig_06))
x.writelines('{:.0f},{:.0f}, {:.0f}±{:.0f}\n'.format(boxNsig_07,boxEsig_07,boxNsig_07,boxEsig_07))
x.writelines('{:.0f},{:.0f}, {:.0f}±{:.0f}\n'.format(boxNsig_09,boxEsig_09,boxNsig_09,boxEsig_09))
x.close()

myFile = TFile('fitresult.root','recreate')
#fitresult.Write('fitresult')
frame   .Write('frame')
myFile  .Close()
