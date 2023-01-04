=========================
Before explicit exclusion

full_output/
    Saved mode-based candidate files, downloaded from KEKCC.

full_mode_plots/
    Plot for every single mode, whether Fit or Histo, of every single channel.

PlotData.C
    To demo these bad channels.

Round1_cand.root
    Full cand output, temporarily ignored in favor of mode-based cand files.

=========================
After explicit exclusion

output/
    Saved mode-based candidate files, downloaded from KEKCC.

mode_plots/
    Only Fit plots for the channels to keep.

=========================
fit.sh
    Used for both before and after, based on FitModes.C

FitModes.C  
    Fit file based on mode-specific cand files.

TotalFit.C
    Quick fit attempt to verify that we have better yield than the old FEI note.
    totalFit.pdf

=========================
Work in progress

TwoFunctions.C
    Right now CB and Gaussian, unsuccesfful so far, try Johnson next...

CB_fit.C
    So far unsuccesfful.

EstimateYield.C
    Includes numbers both before and after exclusion.

FitMbcBDTcut.C
    Single Gaussian+ Argus fit, temporarily out of use.

Nov 15, 2021
