import csv
import numpy as np
from math import sqrt

exec(open("fcnNames.py").read())

scaleFactor = .19/.3

nSigTotal   = np.empty((3), dtype='float')
eSigSqr     = np.empty((3), dtype='float')

nSigBoxSig  = np.empty((3), dtype='float')
nSigBoxTot  = np.empty((3), dtype='float')

with open("allMCPDF.csv") as f:
    mytable = csv.reader(f)
    array=list(mytable)
    for group in range(n):
        outGroupStr = str(group) + ' & '
        for fcnType in range(3):
            fullGroupStr= array[3*group+fcnType]

            fitYieldList    = fullGroupStr[1].split('±')
            nSigFit         = int(round(scaleFactor*float(fitYieldList[0])))
            nSigFitErr      = int(round(scaleFactor*float(fitYieldList[1])))

            outGroupStr += '{}±{}'.format(nSigFit, nSigFitErr) + ' & '
            if(fcnType<2):
                outGroupStr += fullGroupStr[2] + ' & '
            else:
                outGroupStr += fullGroupStr[2] 

            nSigTotal[fcnType]  += scaleFactor*float(fitYieldList[0])
            eSigSqr[fcnType]    += scaleFactor*float(fitYieldList[1])
            nSigBoxSig[fcnType] += scaleFactor*float(fitYieldList[0])
            nSigBoxTot[fcnType] += scaleFactor*float(fitYieldList[0])/float(fullGroupStr[2]) 
        outGroupStr += '\\\\ \\hline'
        print(outGroupStr)

print('\\hline')
print('total & {}±{} & {:.3f} & {}±{} & {:.3f} & {}±{} & {:.3f}\\\\ \\hline'.format(  
int(round(nSigTotal[0])), int(round(sqrt(eSigSqr[0]))), nSigBoxSig[0]/nSigBoxTot[0],
int(round(nSigTotal[1])), int(round(sqrt(eSigSqr[1]))), nSigBoxSig[1]/nSigBoxTot[1],
int(round(nSigTotal[2])), int(round(sqrt(eSigSqr[2]))), nSigBoxSig[2]/nSigBoxTot[2]
))

