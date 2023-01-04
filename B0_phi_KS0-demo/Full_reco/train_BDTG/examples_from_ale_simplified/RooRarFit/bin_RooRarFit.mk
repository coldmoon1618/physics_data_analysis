# Please see SoftRelTools/HOWTO-dependency for documentation
# $Id: bin_RooRarFit.mk,v 1.3 2007/06/29 08:37:34 zhanglei Exp $
override LINK_RooRarFit         += RooRarFitGNUmakefile
override LINK_RooFitCore        += RooRarFitGNUmakefile
override LINK_RooFitModels      += RooRarFitGNUmakefile
#override LINK_RooFitBabar       += RooRarFitGNUmakefile
override LINK_ROOT		+= RooRarFitGNUmakefile
