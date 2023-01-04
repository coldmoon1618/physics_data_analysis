import csv
import numpy
from math import sqrt

def intSqrt(number):
    return int(round(sqrt(number)))

MC_nSig     = 0
MC_eSigSqr  = 0.
Data_nSig   = 0
Data_eSigSqr= 0.

dataNsig= list()
mcNsig  = list()

n = 27
myRows = list()
for group in range (n):
    myRows.append('')

with open("data_fit.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fullGroupStr    = array[group+1]

        myRows[group]  += fullGroupStr[0] + ' & '

        fitYieldList    = fullGroupStr[1].split('±')
        nSigFit         = fitYieldList[0]
        nSigFitErr      = fitYieldList[1]
        myRows[group]  += '{}±{}'.format(nSigFit, nSigFitErr) + ' & '

        Data_nSig   += int(nSigFit)
        Data_eSigSqr+= int(nSigFitErr) * int(nSigFitErr)

        dataNsig.append(float(nSigFit))

with open("mc_fit.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fullGroupStr    = array[group+1]

        fitYieldList    = fullGroupStr[1].split('±')
        nSigFit         = fitYieldList[0]
        nSigFitErr      = fitYieldList[1]
        myRows[group]  += '{}±{}'.format(nSigFit, nSigFitErr) + ' & '

        MC_nSig     += int(nSigFit)
        MC_eSigSqr  += int(nSigFitErr) * int(nSigFitErr)

        mcNsig.append(float(nSigFit))

for group in range(n):
    myRows[group]  += '{:.3f} \\\\ \\hline'.format(dataNsig[group]/mcNsig[group]) 

myRows.insert(0,'     modes    & Data & MC & scaleFactor\\\\ \\hline')
myRows.insert(0,'\\hline')
myRows.append('\\hline')
myRows.append('total & {}±{} & {}±{} & \\\\ \\hline'.format(Data_nSig,intSqrt(Data_eSigSqr), MC_nSig,intSqrt(MC_eSigSqr)))
# This could be converted into a function: input args: fIn, myList, nSigCounter, eSigSqrCounter       

for ele in myRows:
    print(ele)
