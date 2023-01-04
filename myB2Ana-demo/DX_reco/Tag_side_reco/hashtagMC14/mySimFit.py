from ROOT import *
import numpy as np
import ROOT.RooFit as rf

#------------------------------------------------
# Fixed for now, optional parse later if needed
fixThres    = 5.29
lowBound    = 5.21  # Lower limit of both
upRange     = 5.26  # Upper limit of sideband
upBound     = 5.3   # Upper limit of plot range

# Create master pdf
# ---------------------------------
Mbc         = RooRealVar('Mbc', 'Mbc',     lowBound, upBound)
beamE       = RooRealVar('beamE','',       5.28,     upBound)

iMcType     = RooRealVar('iMcType','',     -1,     6)
mySignal    = RooRealVar('mySignal','',    -1,     4)
groupID     = RooRealVar('groupID','',      0,  29)

thres       = RooRealVar('thres','',fixThres)

mean_00     = RooRealVar('mean_00','', 5.28,5.2,5.3)
sigma_00    = RooRealVar('sigma_00','',0.002,0,.2)
sig_00      = RooGaussian('sig_00', '', Mbc, mean_00, sigma_00);
coef_00     = RooRealVar('coef_00','', -100,-1e3,1e3)
bkg_00      = RooArgusBG('bkg_00','',  Mbc, thres, coef_00)
nsig_00     = RooRealVar('nsig_00','', 3e4, 0, 1e9)
nbkg_00     = RooRealVar('nbkg_00','', 1e4, 0, 1e9)

model_00    = RooAddPdf ('model_00','',RooArgList(sig_00,bkg_00), RooArgList(nsig_00,nbkg_00))

mean_01     = RooRealVar('mean_01','', 5.28,5.2,5.3)
sigma_01    = RooRealVar('sigma_01','',0.002,0,.2)
sig_01      = RooGaussian('sig_01', '', Mbc, mean_01, sigma_01);

thres_01    = RooRealVar('thres_01','',fixThres)
coef_01     = RooRealVar('coef_01','', -100,-1e3,1e3)
bkg_01      = RooArgusBG('bkg_01','',  Mbc, thres, coef_01)
nsig_01     = RooRealVar('nsig_01','', 3e4, 0, 1e9)
nbkg_01     = RooRealVar('nbkg_01','', 1e4, 0, 1e9)

model_01    = RooAddPdf ('model_01','',RooArgList(sig_01,bkg_01), RooArgList(nsig_01,nbkg_01))

mean_02     = RooRealVar('mean_02','', 5.28,5.2,5.3)
sigma_02    = RooRealVar('sigma_02','',0.002,0,.2)
sig_02      = RooGaussian('sig_02', '', Mbc, mean_02, sigma_02);
coef_02     = RooRealVar('coef_02','', -100,-1e3,1e3)
bkg_02      = RooArgusBG('bkg_02','',  Mbc, thres, coef_02)
nsig_02     = RooRealVar('nsig_02','', 3e4, 0, 1e9)
nbkg_02     = RooRealVar('nbkg_02','', 1e4, 0, 1e9)

model_02    = RooAddPdf ('model_02','',RooArgList(sig_02,bkg_02), RooArgList(nsig_02,nbkg_02))
 
#------------------------------------------------
# Import data
nBins = 60
fInName = 'Groups_MC.root'
Mbc.setBins(nBins)
myTree = TChain('tree')
myTree.Add(fInName)

Mbc_data_list = list()

#for i in range(30):
for i in range(3):
    Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID=={}'.format(i)))) 
    #Mbc_data_list.append(RooDataSet('Mbc_data','', RooArgSet(Mbc, groupID, mySignal, iMcType), rf.Import(myTree), rf.Cut('groupID=={}&&mySignal>0.5'.format(i)))) 
frame       = Mbc.frame() 
#Mbc_data.plotOn(frame)

# Create category observables for splitting
# ----------------------------------------------------------------------------------
 
# Define categories that can be used for splitting
grp = RooCategory("grp", "grp")
grp.defineType("group00")
grp.defineType("group01")
grp.defineType("group02")

Mbc_data = RooDataSet("combData", "combined data", RooArgSet(Mbc), rf.Index(grp), 
rf.Import("group00",Mbc_data_list[0]), 
rf.Import("group01",Mbc_data_list[1]),
rf.Import("group02",Mbc_data_list[2]),
) 

#
simSig = RooSimultaneous("simSig","simultaneous pdf",grp)

# associate model with the physics state and model_ctl with the control state
#simSig.addPdf(sig_00,"group00")
#simSig.addPdf(sig_01,"group01")
#simSig.addPdf(sig_02,"group02")

#
simBkg = RooSimultaneous("simBkg","simultaneous pdf",grp)

# associate model with the physics state and model_ctl with the control state
#simBkg.addPdf(sig_00,"group00")
#simBkg.addPdf(sig_01,"group01")
#simBkg.addPdf(sig_02,"group02")

nsig     = RooRealVar('nsig','', 3e6, 0, 1e9)
nbkg     = RooRealVar('nbkg','', 1e6, 0, 1e9)

simPdf = RooSimultaneous("simPdf","simultaneous pdf",grp)
simPdf.addPdf(model_00,"group00")
simPdf.addPdf(model_01,"group01")
simPdf.addPdf(model_02,"group02")
#simPdf   = RooAddPdf ('simPdf','',RooArgList(simSig,simBkg), RooArgList(nsig,nbkg))
# Perform simultaneous fit of model
simPdf.fitTo(Mbc_data) 

frame0 = Mbc.frame(rf.Title("group00"))
#Mbc_data_list[0].plotOn(frame0)
#simSig.plotOn(frame0,rf.Slice(grp,"group00"),rf.ProjWData(grp,Mbc_data))
Mbc_data.plotOn(frame0)
simPdf.plotOn(frame0,rf.ProjWData(grp,Mbc_data))
simPdf.plotOn(frame0,rf.ProjWData(grp,Mbc_data),rf.Components("bkg_00,bkg_01,bkg_02"))


#frame1 = Mbc.frame(rf.Title("group01"))
#Mbc_data_list[1].plotOn(frame1)
#simSig.plotOn(frame1,rf.Slice(grp,"group01"),rf.ProjWData(grp,Mbc_data))

#frame2 = Mbc.frame(rf.Title("group02"))
#Mbc_data_list[2].plotOn(frame2)
#simSig.plotOn(frame2,rf.Slice(grp,"group02"),rf.ProjWData(grp,Mbc_data))


c = TCanvas("rf501_simultaneouspdf","rf403_simultaneouspdf",800,800) 
c.Divide(2,2)
c.cd(1)
frame0.Draw()
c.cd(2)
#frame1.Draw()
c.cd(3)
#frame2.Draw()
c.cd(4)
gPad.Update()
nsig_00.Print()
''' 
# Import ingredients in a workspace
w = RooWorkspace("w", "w")
w.Import(RooArgSet(sig, grp))
 
# Make Sim builder tool
sct = RooSimWSTool(w)
 
model_sim = sct.build("model_sim", "sig", rf.SplitParam("Mbc", "grp"))
 
# Print tree structure of model
model_sim.Print("t")
 
# Adjust model_sim parameters in workspace
w.var("Mbc_group26").Print()

c1 = TCanvas('c1','',500,400)
Mbc_data.plotOn(frame, rf.Cut("groupID==26"))
Mbc_data.plotOn(frame, rf.Cut("groupID==27"), rf.MarkerColor(kRed))
frame.Draw()
gPad.Update()
''' 
