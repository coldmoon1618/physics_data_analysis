#!/usr/bin/env python3

#######################################################
# This tutorial demonstrates how to print out the
# contents of the StoreArray<MCParticle>.
# location: analysis/examples/tutorials/B2A902-PrintOutMCParticles.py
#
# This is useful for debugging purposes, e.g. to check if
# the MC files contain the desired processes/decays.
######################################################

import basf2 as b2
import modularAnalysis as ma

# create path
my_path = b2.create_path()

# load input ROOT file
ma.inputMdst('default','/home/belle2/zhai/B0_phi_KS0/B02phiKS0MC/100_2k_evts/0/signalevents_2000.mdst.root', path=my_path)

# print out the contents of the StoreArray<MCParticle>
ma.printPrimaryMCParticles(path=my_path)

max_events = 1
# Process the events
b2.process(my_path, max_events)

print(b2.statistics)
