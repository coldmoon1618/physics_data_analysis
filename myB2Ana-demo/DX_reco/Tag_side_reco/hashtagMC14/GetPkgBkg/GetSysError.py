from ROOT import *
import numpy as np

nFitSig = [
4443,
40925,
28382,
121563,
11413,
6027,
79108,
64072,
6088,
29679,
10587,
1697,
15369,
8723,
3293,
5637,
10214,
13447,
3814,
7467,
4751,
6588,
3569,
5293,
7959,
1352,
9665,
13690,
7254,
10151
]


H_eps = list()
myfile = TFile.Open('epsilon.root')
n = 30
for index in range(n):
    histo = myfile.Get('H_{:0=2d}'.format(index))
    H_eps.append(histo)

for index in range(n):
    nMatch = 0
    nBin = 0
    print '------'
    print index
    for inner in range(2000):
        i = inner + 1
        if(nMatch<nFitSig[index]):
            nMatch+=H_eps[index].GetBinContent(i)
            nBin += 1

    print str(nBin)+','
    print 'nFitSig='+str(nFitSig[index])
    print 'nMatch='+str(nMatch)
    print 'cutoff='+str(int(round(nBin*.5)))+'MeV'
