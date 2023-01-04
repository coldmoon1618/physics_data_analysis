#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import basf2
from modularAnalysis import *
from stdCharged import *
import basf2_mva
import subprocess

train_data = './PhiKst0_cs_training/PhisKst0_mergedTraining_cs_BGx1.root'


# Variables for training.
trainVars2 = [
    "cosTBTO",
    "cosTBz",
    "thrustOm",
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
    "foxWolframR1",
    "harmonicMomentThrust0",
    "harmonicMomentThrust1",
    "harmonicMomentThrust3",
    "cleoConeThrust3",
    "cleoConeThrust4",
    "cleoConeThrust5",
    "cleoConeThrust6",
    "cleoConeThrust7",
    "cleoConeThrust8",
    "thrust",
    "thrustAxisCosTheta",
    "FBDT_flTag_qr"#qrOutput(FBDT)
]

general_options = basf2_mva.GeneralOptions()
general_options.m_datafiles = basf2_mva.vector(train_data)
general_options.m_treename = "B0_ch1"
general_options.m_identifier = "MVAFastBDT_PhiKst0_BGx1.root"
general_options.m_variables = basf2_mva.vector(*trainVars2)
general_options.m_target_variable = "isNotContinuumEvent"
general_options.m_transformToProbability = False


fastbdt_options = basf2_mva.FastBDTOptions()


# Train a MVA method and store the weightfile (MVAFastBDT_ch1.root) locally.
basf2_mva.teacher(general_options, fastbdt_options)


general_options.m_identifier = "MVAFastBDT_PhiKst0_BGx1_2.root"
general_options.m_variables = basf2_mva.vector(*trainVars2)

basf2_mva.teacher(general_options, fastbdt_options)
