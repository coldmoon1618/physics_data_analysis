This is the working area on PHENIX pp collision data to resolve jet substructure info

/GenSimTree
	Directly reads makejets output from lajoie area and output newTree roots
	/outTree
		newTrees for each ckin, where ckin 170, 190, 210, 240 splitted into /early and /late
		newTree_Runxx_ckinxx_hadrons.root
/TaxiTree
	Run my own trkClus files on taxi and then make my own trees
	/makeJetsArea 
		build and install makejets
	/taxiOut_run13
		Soft link the taxi trkClus output to here
	/jetOut_run13
		makejets output over the taxi trkClus to this folder
	/outTree_run13
	/runMakeJets_run13
		The perl and condor things to do the makejets process
/ReadTree
	/read_run13
		Read and compare sim and data
		Divide different substructure info such as nc distr, opening angle distr, z_g distr in pT bins
/plots
	/plot_run13
		Organize important plots into dirs and give descriptions



YZhai Junly 14, 2019
