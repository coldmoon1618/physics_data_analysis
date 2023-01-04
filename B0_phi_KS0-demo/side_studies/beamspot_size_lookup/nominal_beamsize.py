import basf2 as b2
import modularAnalysis as ma

#Deleted, replace with something new of nominal phase 3
#input_file  = '/home/belle2/zhai/B0_phi_KS0/sub00/mdst_000001_prod00014140_task00000001.root'
output_file = 'MC13a_nominal_mixed_beamspot.root'

path = b2.create_path()
ma.inputMdstList("default", [input_file], path=path)

ma.fillParticleListFromMC('Upsilon(4S):generator','',writeOut = True, path=path)

ma.variablesToNtuple('Upsilon(4S):generator', ['mcX','mcY','mcZ'], filename=output_file, treename='tree',path=path)

b2.process(path,0)
