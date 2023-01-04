origin  ssh://git@stash.desy.de:7999/b2a/alegaz_phikstar_2020.git

This README file gives a sketchy overview of the contents of the directories containing
the code that is relevant for the Summer 2020 analysis of B -> phi K(*).

For more details and information, please contact:

  gaz@hepl.phys.nagoya-u.ac.jp


In the following <MODE> indicates one of: 'PhiKp', 'PhiKstp', 'PhiKs', and 'PhiKst0'



 steering_files:
==================

The following steering files are utilized for this analysis:

*) Skim_PhiKAll_ch1.py

this is a "private skim" steering file, aimed at slimming down generic MC collections
using a loose selection for the four modes investigated in this analysis.


*) TrainBDT.py

This is used for training the FastBDT continuum suppression algorithm


*) Select_<MODE>_ch1.py

These are the main steering files, aimed at creating the root files that are
analyzed in the following stage (see analysis_macros directory)



 analysis_macros:
==================

*) AddSignalMCInfoFast.C

This is a macro that looks at the MC truth information stored in the rootuples,
identifies the nonresonant events in the generic MC, and tries to compensate for
the shortcomings of the current MCTruth association.


*) <MODE>Ana.h and <MODE>Ana.h

These are the main classes (created starting from the MakeClass() method of TTree), that
contain the main selection cuts and produce the reduced rootuples.


*) DumpFlatFile<MODE>.C

These macros create the text files that are then used as input for the maximum
likelihood fit.



 RooRarFit:
============

This directory contains the code of the RooFit parser RooRarFit.

Compile with:

 make -f GNUmakefile.standalone

the executable appears in:

 tmp/RooRarFit/rarFit



 ML_fit:
=========

For each mode, two configuration files are provided:

*) dsd_<MODE>_ch1.config

This specifies the content of the input (text) files, the definition of the variables,
the binning for the plots, ...


*) <MODE>_ch1.config

This is the actual configuration file that specifies the components, input samples,
pdf parameterization, fit options, ...

