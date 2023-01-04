AUTOMAKE_OPTIONS = foreign

INCLUDES = -I. -I$(srcdir) -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

lib_LTLIBRARIES = \
  libPythiaParticles.la

noinst_HEADERS = \
  PythiaParticlesLinkDef.h

include_HEADERS = \
  PythiaParticles.h

libPythiaParticles_la_SOURCES = \
  PythiaParticles.C \
  PythiaParticles_Dict.C

libPythiaParticles_la_LIBADD = -L$(OFFLINE_MAIN)/lib -lphool -lPHPythia -L$(ROOTSYS)/lib -lPhysics -lHist -lTree

# Rule for generating CINT dictionaries from class headers.
PythiaParticles_Dict.C: \
  PythiaParticles.h \
  PythiaParticlesLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

clean-local:
	rm -f *Dict*

