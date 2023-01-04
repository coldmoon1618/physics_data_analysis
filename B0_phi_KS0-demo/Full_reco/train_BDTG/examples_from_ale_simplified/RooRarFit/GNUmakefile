# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile,v 1.4 2007/06/29 08:37:34 zhanglei Exp $
#################################################################
#++ library products				[build it with 'lib']

LIBREMOVEFILES := 
LIBTMPLFILES := 
LIBDDLORDERED := 

#################################################################
#++ extra binary products	[not in production, build it with extrabin]

EXTRABINS := 

$(addprefix $(bindir),$(EXTRABINS)): $(bindir)% : %.o

#################################################################
#++ binary products				[build it with 'bin']

ROOTONLYLIBS += -lHtml
SYSLIB += -lHtml
BINS := rarFit
BINCCFILES :=  rarFit.cc

#++ Binary rules		 [in production, build it with 'bin']

$(addprefix $(bindir),$(BINS)): $(bindir)% : %.o

#++ shell script products.. 			[build it with 'bin']
BINSCRIPTS := submitToy

#################################################################
#++ regression test scripts			[build it with 'test']


#################################################################
#++ include standard makefile from SoftRelTools.
include SoftRelTools/standard.mk

PACKAGELIST += RooRarFit
override LOADLIBES += -lRooRarFit -lRooFitModels -lRooFitCore
