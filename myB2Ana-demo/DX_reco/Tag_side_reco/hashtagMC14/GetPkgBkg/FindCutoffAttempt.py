from ROOT import *
import numpy as np

nBin = [
1397,
1448,
2000,
230,
1320,
1332,
1355,
1362,
1325,
1386,
1339,
1185,
1258,
1255,
1280,
1339,
1407,
1437,
1264,
1328,
1492,
1405,
1410,
228,
763,
220,
1387,
1441,
1298,
1322
]


h_eps = list()
myfile = TFile.Open('epsilon.root')
n = 30
for index in range(n):
    histo = myfile.Get('h_{:0=2d}'.format(index))
    h_eps.append(histo)

for index in range(n):
    nLower = 0
    nUpper = 0
    nMatch = 0
    nBins= 0
    print '------'
    print index
    for inner in range(2000):
        i = inner + 1
        if(inner<1.2*nBin[index]):
            nUpper+=h_eps[index].GetBinContent(i)
            if(inner<nBin[index]):
                nMatch+=h_eps[index].GetBinContent(i)
                nBins+= 1
                if(inner<0.8*nBin[index]):
                    nLower+=h_eps[index].GetBinContent(i)

    print 'nBin='+str(nBins)
    print 'nMatch='+str(nMatch) 
    print 'nLower='+str(nLower) + ' ' +str(100*nLower/nMatch-100)+'%'
    print 'nUpper='+str(nUpper) + ' ' +str(100*nUpper/nMatch-100)+'%'

# This upper and lower thing is probably for B0 stuff
