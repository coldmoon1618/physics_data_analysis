AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

INCLUDES = -I$(includedir) -I$(OFFLINE_MAIN)/include -I$(ROOTSYS)/include

AM_LDFLAGS = -L$(libdir) -L$(OFFLINE_MAIN)/lib -L$(ROOTSYS)/lib

bin_PROGRAMS = makejets

makejets_SOURCES = \
  main.C \
  MakeJets.C \
  PHENIXFilter.cpp

nodist_makejets_SOURCES = \
  ml_training_data_mlp_Run13.h 

makejets_LDFLAGS = \
	$(AM_LDFLAGS) \
	`root-config --libs`

makejets_LDADD = \
	-lgfortran \
	-lCGAL \
	-lCGAL_Core \
	-lsiscone \
	-lsiscone_spherical \
	-lfastjetplugins \
	-lfastjettools \
	-lfastjet \
	-luspin \
	-lTMVA

noinst_HEADERS = \
  MakeJets.h \
  PHENIXFilter.h 

BUILT_SOURCES = \
  ml_training_data_mlp_Run12.h \
  ml_training_data_mlp_Run13.h

ml_training_data_mlp_Run12.h:\
	TMVARegression_MLP.weights.Run12.xml 
	cp $< .
	xxd -i $(<F) > temp2
	head -n -1 temp2 > $@
	rm $(<F)
	rm temp2

ml_training_data_mlp_Run13.h:\
	TMVARegression_MLP.weights.Run13.xml 
	cp $< .
	xxd -i $(<F) > temp1
	head -n -1 temp1 > $@
	rm $(<F)
	rm temp1

clean-local:
	rm -f $(BUILT_SOURCES)

