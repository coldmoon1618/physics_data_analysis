
*) Compile with:

make -f GNUmakefile.standalone

If everything works correctly the executable rarFit is created in
the directory tmp/

Using ROOT 6.02, running rarFit raises the error:

Error in <TCling::RegisterModule>: cannot find dictionary module RooRarFitCling_rdict.pcm

To get rid of it, just create a symbolic link in the tmp directory to
RooRarFit/RooRarFitCling_rdict.pcm



*) Parsing problems in roorarfit:

It seems like a bug has been introduced since ROOT 5.34.xx, so that RooFit cannot
correcly parse numbers in exponential form any longer.
To fix that, change the line:

&& (!first || !((c=='-'||c=='+') && (isdigit(cnext)||cnext=='.'||haveINF)))) {

into:

&& (!((c=='-'||c=='+') && (isdigit(cnext)||cnext=='.'||haveINF)))) {

in RooStreamParser.cxx and recompile ROOT.

See also:

https://sft.its.cern.ch/jira/browse/ROOT-4683

Eventually, in order to use the package with the standard pre-compiled versions of
ROOT, I added a modified version of RooStreamParser to the package.


*) Problem with the projection plots: 

If a component does not have any floating parameters, it is taken as constant in the
projection plot. 

A quick and dirty solution could be commenting out the following lines:

  if (canOpt) {
    setAttribute("ConstantExpression") ;
  }
  
  // If yes, list node eligible for caching, if not test nodes one level down
  if (canOpt||getAttribute("CacheAndTrack")) {
    
    if (!cacheList.find(*this) && dependsOnValue(observables) && !observables.find(*this) ) {
      
      // Add to cache list
      cxcoutD(Optimization) << "RooAbsArg::findConstantNodes(" << GetName() << ") adding self to list of constant nodes" << endl ;

      cacheList.add(*this,kFALSE) ;
    }
  }
  
in RooAbsArg.cxx.

A slightly more elegant one consists in setting at least one parameter to "not constant"
in the total pdf and in the components, so these are not put in the list of cached pdf's
and then are plotted correctly.

I introduced this workaround in the getProjPlot method of rarMLFitter.cc (from v6 on).

