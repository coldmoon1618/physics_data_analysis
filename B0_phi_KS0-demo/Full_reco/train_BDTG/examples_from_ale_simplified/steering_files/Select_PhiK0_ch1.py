#!/usr/bin/env python
# -*- coding: utf-8 -*-

######################################################
#
# Reconstruct B0 -> phi[K+K-] Ks[pi+ pi-]
#
# Author: A. Gaz
#
######################################################

import sys

if len(sys.argv) != 1:
    sys.exit('\n Usage: basf2 Select_PhiK0_ch1.py\n'
    )

import os
import basf2
import modularAnalysis as ma
import variables.collections as vc
import variables.utils as vu
import vertex as vx
import flavorTagger as ft
import stdV0s as stdv
from stdCharged import *
from stdPhotons import *


# set global tags

use_central_database('analysis_tools_release-04')
#use_central_database('online_proc11')
#use_central_database('data_reprocessing_proc11')


# defile systematic tests
sys_tests = ['def', 'scale', 'scaleUp', 'scaleDown', 'Efficiency']

# Select the systematic variation from the list
test = 'Efficiency'

if test not in sys_tests:
    print("Unknown systematic test {}".format(test))
    exit(1)


# create path
my_path = basf2.Path()

outFileName = 'OUTFILE'
dir1 = 'DIR1'
dir2 = 'DIR2'
dir3 = 'DIR3'
suffix = 'INSUFFIX'


# set the input files
filenameIN = '/chai/sgt3/users/gaz/Belle2/Analysis/PhiK_win2020/'
filenameIN += dir1
filenameIN += '/'
filenameIN += dir2
filenameIN += '/'
filenameIN += dir3
filenameIN += '/'
filenameIN += '*'
filenameIN += suffix
filenameIN += '.root'

inputMdstList('default', [filenameIN], path=my_path)


stdPi(listtype='all', path=my_path)
stdK(listtype='all', path=my_path)
stdPhotons(listtype='all', path=my_path)
stdPhotons(listtype='loose', path=my_path)


# apply scaling/shift momenta/efficiency

if test == 'def':
    pass
elif test == 'scale':
    ma.trackingMomentum(inputListNames=['pi+:all', 'K+:all'], scale=1.00062, path=my_path)
elif test == 'scaleUp':
    ma.trackingMomentum(inputListNames=['pi+:all', 'K+:all'], scale=1.001, path=my_path)
elif test == 'scaleDown':
    ma.trackingMomentum(inputListNames=['pi+:all', 'K+:all'], scale=0.999, path=my_path)
elif test == 'Efficiency':
    ma.trackingEfficiency(inputListNames=['pi+:all', 'K+:all'], fraction=0.0091, path=my_path)



# load Ks candidates directly from default list
stdv.stdKshorts(path=my_path)


# reconstruct candidates

ma.reconstructDecay('phi:all -> K-:all K+:all', cut='1.00 < M < 1.05', path=my_path)
vx.vertexKFit('phi:all', 0.0, '', '', path=my_path)
ma.matchMCTruth(list_name='phi:all', path=my_path)

ma.reconstructDecay('B0:temp -> phi:all K_S0:merged', cut='5.2 < Mbc < 5.29 and abs(deltaE) < 0.5', path=my_path)
vx.vertexKFit('B0:temp', 0.0, '', '', path=my_path)
ma.matchMCTruth(list_name='B0:temp', path=my_path)


# get event shape and continuum suppression

ma.buildEventShape(inputListNames=['pi+:all', 'gamma:all'], foxWolfram=True, path=my_path)
ma.buildRestOfEvent(target_list_name='B0:temp', path=my_path)

ma.appendROEMask('B0:temp', 'IPtracks', 'abs(d0) < 0.05 and abs(z0) < 0.1', '', path=my_path)
ma.buildContinuumSuppression('B0:temp', 'IPtracks', path=my_path)


# tag vertex
vx.TagV(list_name='B0:temp', useFitAlgorithm='noConstraint', path=my_path)


# add FT for continuum suppression
ft.flavorTagger(
     particleLists=['B0:temp'],
     weightFiles='B2nunubarBGx1',
     path=my_path)


# perform cuts on the final list if needed
cutAndCopyList('B0:ch1', 'B0:temp', '5.2 < Mbc < 5.29 and abs(deltaE) < 0.5 and foxWolframR2 < 0.6', True, path=my_path)


# rank signal candidate by signal vertex probability
ma.rankByHighest(particleList='B0:ch1',
                 variable='chiProb',
                 path=my_path)

# MVAExpert
my_path.add_module('MVAExpert', listNames=['B0:ch1'],
                   extraInfoName='FastBDT', identifier='MVAFastBDT_PhiKs_BGx1.root')

variables.addAlias('out_FastBDT', 'extraInfo(FastBDT)')
variables.addAlias('csv_FastBDT', 'transformedNetworkOutput(FastBDT,0.,1.0)')


my_path.add_module('MVAExpert', listNames=['B0:ch1'],
                   extraInfoName='FastBDT1', identifier='MVAFastBDT_PhiKs_BGx1_1.root')

variables.addAlias('out_FastBDT1', 'extraInfo(FastBDT1)')
variables.addAlias('csv_FastBDT1', 'transformedNetworkOutput(FastBDT1,0.,1.0)')


my_path.add_module('MVAExpert', listNames=['B0:ch1'],
                   extraInfoName='FastBDT2', identifier='MVAFastBDT_PhiKs_BGx1_2.root')

variables.addAlias('out_FastBDT2', 'extraInfo(FastBDT2)')
variables.addAlias('csv_FastBDT2', 'transformedNetworkOutput(FastBDT2,0.,1.0)')


my_path.add_module('MVAExpert', listNames=['B0:ch1'],
                   extraInfoName='FastBDT3', identifier='MVAFastBDT_PhiKs_BGx1_3.root')

variables.addAlias('out_FastBDT3', 'extraInfo(FastBDT3)')
variables.addAlias('csv_FastBDT3', 'transformedNetworkOutput(FastBDT3,0.,1.0)')



# more aliases

variables.addAlias('FBDT_flTag_qr', 'qrOutput(FBDT)')
variables.addAlias('FANN_flTag_qr', 'qrOutput(FANN)')

variables.addAlias('phiCosHel', 'cosHelicityAngle(0, 0)')

variables.addAlias('mothID', 'genMotherID(0)')
variables.addAlias('gdMothID', 'genMotherID(1)')
variables.addAlias('gdgdMothID', 'genMotherID(2)')
variables.addAlias('mothPDG', 'genMotherPDG(0)')
variables.addAlias('gdMothPDG', 'genMotherPDG(1)')
variables.addAlias('gdgdMothPDG', 'genMotherPDG(2)')

variables.addAlias('kaonBID', 'pidPairProbabilityExpert(321, 211, ALL)')


cs_extra_vars = [
    "cosTBTO",
    "cosTBz",
    "thrustOm",
    "thrustBm",
    'KSFWVariables(et)',
    'KSFWVariables(mm2)',
    'KSFWVariables(hso00)',
    'KSFWVariables(hso04)',
    'KSFWVariables(hso10)',
    'KSFWVariables(hso14)',
    'KSFWVariables(hso20)',
    'KSFWVariables(hso22)',
    'KSFWVariables(hso24)',
    'KSFWVariables(hoo0)',
    'KSFWVariables(hoo1)',
    'KSFWVariables(hoo2)',
    'KSFWVariables(hoo3)',
    'KSFWVariables(hoo4)',
]

mc_custom_vars = ['mothID', 'gdMothID', 'gdgdMothID', 'mothPDG', 'gdMothPDG', 'gdgdMothPDG']


# save variables to file

trk_vars = ['charge', 'kaonBID'] + vc.kinematics + vc.mc_truth + vc.pid + mc_custom_vars + vc.track + vc.track_hits
res_vars = vc.inv_mass + vc.vertex + vc.mc_vertex + vc.kinematics + vc.mc_truth + mc_custom_vars
Ks_vars = res_vars + ['goodBelleKshort']
B_vars = res_vars + vc.deltae_mbc + vc.event_shape + cs_extra_vars + vc.tag_vertex + vc.mc_tag_vertex + \
    ['isNotContinuumEvent', 'phiCosHel', 
     'out_FastBDT', 'csv_FastBDT', 'out_FastBDT1', 'csv_FastBDT1', 'out_FastBDT2', 'csv_FastBDT2', 'out_FastBDT3', 'csv_FastBDT3', 
     'FBDT_flTag_qr', 'FANN_flTag_qr'] 


B_vars += vu.create_aliases_for_selected(list_of_variables=res_vars,
                                         decay_string='B0:ch1 -> ^phi:all K_S0:merged',
                                         prefix='B0_phi') + \
          vu.create_aliases_for_selected(list_of_variables=Ks_vars,
                                         decay_string='B0:ch1 -> phi:all ^K_S0:merged',
                                         prefix='B0_Ks') + \
          vu.create_aliases_for_selected(list_of_variables=trk_vars,
                                         decay_string='B0:ch1 -> [phi:all -> ^K+:all ^K-:all] [K_S0:merged -> pi+ pi-]',
                                         prefix=['B0_phi_K1','B0_phi_K2']) + \
          vu.create_aliases_for_selected(list_of_variables=trk_vars,
                                         decay_string='B0:ch1 -> [phi:all -> K+:all K-:all] [K_S0:merged -> ^pi+ ^pi-]',
                                         prefix=['B0_Ks_pi1','B0_Ks_pi2']) 

variablesToNtuple(decayString='B0:ch1',
                  variables=B_vars,
                  filename=outFileName,
                  treename='B0_ch1',
                  path=my_path)

# Process the events
basf2.process(my_path)

# print out the summary
print(statistics)
