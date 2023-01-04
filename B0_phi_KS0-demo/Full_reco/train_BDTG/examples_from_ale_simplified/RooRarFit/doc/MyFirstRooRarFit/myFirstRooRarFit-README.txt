//////////////////////////////////////////////
// myFirstRooRarFit
//
// $Id: myFirstRooRarFit-README.txt,v 1.6 2007/06/29 08:37:43 zhanglei Exp $
//
// by: Karsten Koeneke (kkoeneke@slac.stanford.edu)
//
//////////////////////////////////////////////

*******************************************************
(0) Charge
*******************************************************
This is a short first-user introduction to RooRarFit. 
Please feel free to edit and extend it if you find a mistake or think
that something essential for a first time user is missing...

Extensive documentation can be found at:

http://rarfit.sourceforge.net/




*******************************************************
(1) Introduction
*******************************************************

  (A) What is RooRarFit?

RooRarFit is a configuration-file based wrappper for RooFit and more.
It enables you to perform simple or very elaborate fits with simply
writing a text-based configuration file with as little overhead as
possible. But the aim is to give you the full power of RooFit.



  (B) How do I get it?

(i) Preparations

You should be logged in to your home directory at SLAC (or any of the
remote sites, provided you obtained an afs token for slac, i.e.: klog
-c slac.stanford.edu). You will need a Babar release, e.g. analysis-26
(or your favorite and up-to-date release). If you don't already have a
release, follow these steps (from the Babar Workbook):

 newrel -s $BFROOT/build/<first_letter>/<username> -t analysis-26 ana26

This will install a new release for you. Now go to this new directory:

 cd ana26

and type:

 srtpath <enter> <enter>

Now, you will need to have a workdir:

 addpkg workdir

and finally you need to set up your workdir

 gmake workdir.setup



(ii) Installing RooRarFit

Now that you have an analysis release, you can obtain RooRarFit by
simply (after issuing srtpath):

 addpkg RooRarFit

You will need to compile it with

 gmake RooRarFit.all

Now you have a working RooRarFit installation.

(NB: If you have private RooFitCore and RooFitModels checked out,
please make sure the libs built for RooFitCore and RooFitModels are
the same type, shared or static, as for RooRarFit.
The version of RooFit works with RooRarFit is V01-09-02 and up.
Please check online reference to see if patches are needed for RooFit)



  (C) Overview


If you are already familiar with RooFit, you should be able to quickly
learn RooRarFit. If not, you should still be able to learn it rather
quickly.


	(i) The general structure

To perform a fit to a dataset is in principle a three-step procedure.

First, you will perform a fit (called PdfAct in the config file; see
later) to the individual dimesions (e.g.: deltaE and mES) and
categories (e.g.: signal and continuum background). This step is
usually a series of one-dimensional fits to individual Monte Carlo
samples. All parameters in your pdf will be floated (unless otherwise
specified) and RooRarFit will store the results of these fits in a
parameter file and it will also store the plots in a root file.

Second, you will perform a Maximum Likelihood Fit (called MLAct in the
config file; see later) to the full dataset (or a full Monte Carlo
simulation thereof). In this fit, only the pdf parameters you
explicitly specified (usually the signal yield, the background yield
and some pdf shape parameters, e.g. the ARGUS shape parameter) will be
varied. All other parameters are fixed. RooRarFit will use for a value
of these fixed parameters the ones determined in the first step and
stored in the parameter file. Since RooRarFit is reading this
parameter file, it also knows the errors associated with each fixed
parameter. You can specify also to perform a variation of these fixed
parameters in the end and determine the change in (e.g.) signal yield
and thus perform a systematic error study related to the fixing of pdf
parameters.

Third, you probably want to plot the fit result. This can be done by
doing e.g. Projection Plots, sPlots,...





*******************************************************
(2) Run your first fit
*******************************************************

  (A) Preparations

Before you start, make sure that you are loading the necessary RooFit
libraries, i.e. check that your RooLogon.C file (in the workdir)
contains the two lines:

  loadSrtLib("RooFitCore") ;
  loadSrtLib("RooFitModels") ;

Now, copy the file:

RooRarFit/doc/MyFirstRooRarFit/myFirstRooRarFit.config

to the workdir.




  (B) The PdfAct fit
Once these two steps are done, go to the workdir and issue the command:

 rarFit -A PdfAct myFirstRooRarFit.config

This starts the section PdfAct defined in the configuration file
myFirstRooRarFit.config which will perform one dimensional pdf fits to
individual Monte Carlo datasets. For example, it will fit a Crystal
Ball shape in mES to signal Monte Carlo and another independent fit is
e.g. a fit of a second order polynomial to a continuum Monte Carlo
sample in the variable (dimension) deltaE. Two files are created by
issuing this command. A text file containing the fit results
(including the fit uncertainties) in the .params directory (created by
RooRarFit, if not already existing) and a root file containing the
plots for all the individual fits in the results directory (also
created by RooRarFit, if not already existing).  You can also change
these two default directory names by setting the environment variables
PARAMDIR and RESULTDIR, e.g.:

setenv PARAMDIR ./FitParameters
setenv RESULTDIR ./FitResults

You can look at the plots stored in the results directory by starting
a root session and loading the file
"myFirstRooRarFit.mlFitter_Config.pdfPlot.PdfAct.root" just created;
i.e. you can do:

 bbrroot results/myFirstRooRarFit.mlFitter_Config.pdfPlot.PdfAct.root

and open a TBrowser in this root session

 TBrowser new

to browse in this file (of course you can also do that on the command
line inside root).

To understand how the pdf is actually created and what exactly is
done, just take a look at myFirstRooRarFit.config. It contains
extensive comments.



  (C) The MLAct fit

Now, you can run the full multidimensional maximum likelihood fit on
the full Monte Carlo simulated dataset (containing only signal and
continuum background events) by issuing the command (again in the
workdir):

 rarFit -A MLAct myFirstRooRarFit.config

The full fit will run and the results of this fit will be stored in a
(different from the previous) text file in the .params directory. The
results for all the parameters will be shown at the end of the
execution on the command line. Also, the systematic uncertainty on the
signal yield caused by fixing some of the pdf parameters are printed
to the command line. Again, for a better understanding, simply take a
look at the myFirstRooRarFit.config configuration file.



  (D) Plotting the results: SPlotAct and ProjAct
To obtain sPlots for the two dimensions mES and deltaE, issue the command

 rarFit -A SPlotAct myFirstRooRarFit.config

in the workdir.
For projection plots, issue the command

 rarFit -A ProjAct myFirstRooRarFit.config

in the workdir.

Both will create a .root file (which contains the desired plots) in
the results directory.






*******************************************************
(3) Producing Post Script files
*******************************************************

You will find a .cc file in
RooRarFit/doc/MyFirstRooRarFit/pdfplot_myFirstRooRarFit.cc which will
aid you in creating Post Script plots. In order to use it, copy it to
the workdir and issue the commands:

bbrroot -l -q -b pdfplot_myFirstRooRarFit.cc\(\"results/myFirstRooRarFit.mlFitter_Config.pdfPlot.PdfAct.root\"\)

bbrroot -l -q -b pdfplot_myFirstRooRarFit.cc\(\"results/myFirstRooRarFit.mlFitter_Config.sPlot.SPlotAct.root\"\)

bbrroot -l -q -b pdfplot_myFirstRooRarFit.cc\(\"results/myFirstRooRarFit.mlFitter_Config.projPlot.ProjAct.root\"\)

for creating .eps plots of the PdfAct, the SPlotAct and the ProjAct,
respectively.

