import basf2 as b2
import modularAnalysis as ma
import vertex as vt

#input_file = '/home/belle2/zhai/B0_phi_KS0/sub00/udst_000001_prod00012457_task10020000001.root'
input_file = '/home/belle2/zhai/B0_phi_KS0/sub00/udst_000001_prod00012916_task72948000001.root'

path = b2.create_path()
ma.inputMdstList("default", [input_file], path=path)

ma.applyCuts('B0:TCPV_qqs0','',path=path)
#ma.matchMCTruth('B0:TCPV_qqs0',path=path)

#ma.fillParticleList('K-', 'kaonID>0.05 and nCDCHits>0 and thetaInCDCAcceptance>0', path=path)
ma.fillParticleList('K-', '', path=path)
ma.reconstructDecay('phi:reco -> K+ K-',  '0.97 < M < 1.1',  path=path)
vt.treeFit('phi:reco',     conf_level=0, updateAllDaughters = True,  path=path)
ma.matchMCTruth('phi:reco',path=path)
ma.variablesToNtuple('phi:reco',['isSignal','M','pt','useCMSFrame(p)'],filename='test_phi.root',treename='tree',path=path)

'''
ma.variablesToNtuple('B0:TCPV_qqs0', 
        ['M','Mbc','deltaE',
        'useCMSFrame(p)',
], filename="test_MC_uDST.root", treename="tree", path=path)
'''

b2.process(path, max_event=0)
