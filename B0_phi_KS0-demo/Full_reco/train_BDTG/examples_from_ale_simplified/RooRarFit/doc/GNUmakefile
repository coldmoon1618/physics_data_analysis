# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile,v 1.2 2007/06/29 08:37:41 zhanglei Exp $
#################################################################
# to build guide of RooRarFit
# in release dir type
# gmake RooRarFit.guide

guide: RooRarFitGuide

RooRarFitGuide: RooRarFitTexinfo RooRarFitDoxygen
#echo "Building documents for RooRarFit"
RooRarFitTexinfo:
	cd src; \
	texi2html -monolithic RooRarFit.texinfo; \
	texi2dvi RooRarFit.texinfo; \
	texi2pdf RooRarFit.texinfo; \
	mv RooRarFit.html RooRarFit.pdf ..
RooRarFitDoxygen:
	doxygen src/doxygen.cfg
	cd latex; make; mv refman.pdf ../refman.pdf
	if [ -e html/main.html ]; then \
		sed -e 's%RooRarFit Documentation%<a href="../RooRarFit.html">RooRarFit Documentation</a>%' html/main.html > html/main.html.tmp; \
		mv html/main.html.tmp html/main.html; \
	fi
	if [ -e html/index.html ]; then \
		sed -e 's%RooRarFit Documentation%<a href="../RooRarFit.html">RooRarFit Documentation</a>%' html/index.html > html/index.html.tmp; \
		mv html/index.html.tmp html/index.html; \
	fi

clean: cleanRooRarFitTexinfo cleanRooRarFitDoxygen

cleanRooRarFitTexinfo:
	cd src; \
	rm -f RooRarFit.aux RooRarFit.cp RooRarFit.dvi RooRarFit.fn \
	RooRarFit.ky RooRarFit.log RooRarFit.pg \
	RooRarFit.tp RooRarFit.vr RooRarFit.cps RooRarFit.toc RooRarFit.op
cleanRooRarFitDoxygen:
	rm -fr latex
