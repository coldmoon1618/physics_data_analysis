import root_pandas
from decayHash import DecayHashMap

myBplus = root_pandas.read_root('output/charged_0_mainOut.root','B_plus')
BplusMap = DecayHashMap('output/charged_0_mc1Hash.root', removeRadiativeGammaFlag=True)

candidate2 = myBplus.iloc[2][["decHash", "decHashEx"]].values

Bplus = BplusMap.get_reconstructed_decay(*candidate2)
print(Bplus.to_string())
