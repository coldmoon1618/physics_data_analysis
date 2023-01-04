#!/bin/csh

# Step 1:   Ftting for MC, these are final
# Step 2:   Draft fit for Data, obtain estimate of fitting params
# Step 3:   Use these fitting params as initial values for final fit 
# Note:     Scale factor extrapolated for 0.1, too hard to fit

# /quickFit
#python FixedThresFit.py --cutStr 'SigProb>0.9' &
#python FixedThresFit.py --cutStr 'SigProb>0.8' & 
#python FixedThresFit.py --cutStr 'SigProb>0.7' &
#python FixedThresFit.py --cutStr 'SigProb>0.6' &
#python FixedThresFit.py --cutStr 'SigProb>0.5' &
#python FixedThresFit.py --cutStr 'SigProb>0.4' &
#python FixedThresFit.py --cutStr 'SigProb>0.3' &
#python FixedThresFit.py --cutStr 'SigProb>0.2' &

# /quickFit
#python FixedThresFit.py --cutStr 'SigProb>0.9' --fInput AllModes_Data.root &
#python FixedThresFit.py --cutStr 'SigProb>0.8' --fInput AllModes_Data.root & 
#python FixedThresFit.py --cutStr 'SigProb>0.7' --fInput AllModes_Data.root &
#python FixedThresFit.py --cutStr 'SigProb>0.6' --fInput AllModes_Data.root &
#python FixedThresFit.py --cutStr 'SigProb>0.5' --fInput AllModes_Data.root &
#python FixedThresFit.py --cutStr 'SigProb>0.4' --fInput AllModes_Data.root &
#python FixedThresFit.py --cutStr 'SigProb>0.3' --fInput AllModes_Data.root &
#python FixedThresFit.py --cutStr 'SigProb>0.2' --fInput AllModes_Data.root &

# /fit_plots
#python ConditionalFit.py --cutStr 'SigProb>0.9' --fInput AllModes_Data.root --mygamma 0.04 --mydelta 9 --mynsig 71555  --mynbkg 48308  --mycoef -49 &
#python ConditionalFit.py --cutStr 'SigProb>0.8' --fInput AllModes_Data.root --mygamma 1.45 --mydelta 8 --mynsig 108201 --mynbkg 82428  --mycoef -67 & 
#python ConditionalFit.py --cutStr 'SigProb>0.7' --fInput AllModes_Data.root --mygamma 2.06 --mydelta 8 --mynsig 142689 --mynbkg 136154 --mycoef -74 &
#python ConditionalFit.py --cutStr 'SigProb>0.6' --fInput AllModes_Data.root --mygamma 2.70 --mydelta 8 --mynsig 179646 --mynbkg 216769 --mycoef -76 &
#python ConditionalFit.py --cutStr 'SigProb>0.5' --fInput AllModes_Data.root --mygamma 3.76 --mydelta 8 --mynsig 220320 --mynbkg 340427 --mycoef -76 &
#python ConditionalFit.py --cutStr 'SigProb>0.4' --fInput AllModes_Data.root --mygamma 3.14 --mydelta 6 --mynsig 270284 --mynbkg 538067 --mycoef -72 &
#python ConditionalFit.py --cutStr 'SigProb>0.3' --fInput AllModes_Data.root --mygamma 3.14 --mydelta 6 --mynsig 300000 --mynbkg 700000 --mycoef -75 &
#python ConditionalFit.py --cutStr 'SigProb>0.2' --fInput AllModes_Data.root --mygamma 3.14 --mydelta 6 --mynsig 350000 --mynbkg 800000 --mycoef -75 &
