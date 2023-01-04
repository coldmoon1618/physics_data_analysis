# Plain PyROOT handing of mdst, only works locally,
# Not valid to use on GRID
# New handling of decay chain used in SaveDecayChain.py 
# inspired by software/analysis/scripts/variables/MCGenTopo.py
# print to terminal version of SkimTree.C
# Jan 15, 2021

import basf2 as b2
import modularAnalysis as ma
import ROOT
from ROOT.Belle2 import MCParticle

input_file = '/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/0/signalevents_2000.mdst.root'
output_file = 'Info.txt'

fOut = open(output_file,"w+")

myParti     = MCParticle()
arr         = ROOT.TClonesArray("Belle2::MCParticle")

mdstTree    = ROOT.TChain("tree")
mdstTree.Add(input_file)

nEntries = mdstTree.GetEntries()
mdstTree.SetBranchAddress("MCParticles",ROOT.AddressOf(arr))

event_num   = []
dec_step    = []
pdg         = []
first_dau   = []
last_dau    = []
nDaughters  = []

print(nEntries," entries")

fOut.write('step    PDG     firstDaughter   lastDaughter    nDau\n')
#for i in range(nEntries):
for i in range(4):
    mdstTree.GetEntry(i)
    nParticles=arr.GetEntries()
    event_num.append(i+1)
    fOut.write('event '+str(event_num[i])+' \n')
    for j in range(nParticles):
        myParti     = arr.At(j)
        dec_step.append(j+1)
        pdg.append(myParti.getPDG())
        first_dau.append(myParti.getFirstDaughter())
        last_dau.append(myParti.getLastDaughter())
        nDaughters.append(myParti.getNDaughters())

        fOut.write(str(dec_step[j])+'  '+str(pdg[j])+'  '+str(first_dau[j])+'   '+str(last_dau[j])+'   '+str(nDaughters[j])+'\n')
fOut.close()
