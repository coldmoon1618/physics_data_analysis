import csv
import numpy as np
from ROOT import TMath

def getW(Combo):
    w = 0.
    for ind in Combo:
        i = ind - 2
        w+= 1.0/eCoef[i]**2
        print('[{}]({}): coef=-{} +/- {}    {}        {}'.format(decMode[i],subMode[i],negCoef[i],eCoef[i],nSig[i],decStr[i]))
    return w

def getMu(Combo, w):
    Sum = 0.
    for ind in Combo:
        i = ind - 2
        Sum += negCoef[i]/eCoef[i]**2
    print('mu={}'.format(Sum/w))
    return Sum/w

def getChi2(Combo,mu):
    chi2 = 0.
    for ind in Combo:
        i = ind -2 
        chi2 += (mu-negCoef[i])**2/eCoef[i]**2
    print('chi2={}'.format(chi2))
    return chi2

def getChiProb(Combo):
    w   = getW(Combo)
    mu  = getMu(Combo, w)
    chi2= getChi2(Combo, mu)
    return TMath.Prob(chi2,len(Combo)-1)

nRow = 57
decMode = np.empty((nRow), dtype="int")
subMode = np.empty((nRow), dtype="int")
nSig    = np.empty((nRow), dtype="float")
eNsig   = np.empty((nRow), dtype="float")
purity  = np.empty((nRow), dtype="int")
negCoef = np.empty((nRow), dtype="float")
eCoef   = np.empty((nRow), dtype="float")
decStr  = list()

with open("coefChart.csv") as f:
    mytable = csv.reader(f)
    array=list(mytable)
    for ele in array:
        index = array.index(ele)
        if(index>0):
            index-=1
            decMode [index] = ele[0]
            subMode [index] = ele[1]
            nSig    [index] = ele[2]
            eNsig   [index] = ele[3]
            purity  [index] = ele[4]
            negCoef [index] = ele[5]
            eCoef   [index] = ele[6]
            decStr.append(ele[7])

Combo = list()

#Mode00
Combo.append((4,6,7,8))
Combo.append((5,10))

#Mode01
Combo.append((12,13,15))
Combo.append((14,16,18,20,21))
Combo.append((17,19))

#Mode03
Combo.append((22,26))

#Mode15
Combo.append((28,31))
Combo.append((29,35,37))
Combo.append((33,34,36,40))
Combo.append((38,39))

#ModeJPsi
Combo.append((46,48))
Combo.append((47,49))

#ModeOthers
Combo.append((56,57))

for ele in Combo:
    print('chiProb={}'.format(getChiProb(ele))) 
    print(' ')

