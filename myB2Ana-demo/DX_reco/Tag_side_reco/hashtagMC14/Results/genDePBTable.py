import csv
import numpy
import math

exec(open("fcnNames.py").read())

scaleFactor = .19/.3

pkgBkgList  = list()

centralVal  = list()
statError   = list()
loSysErr    = list() 
hiSysErr    = list() 

CentralVal  = 0
StatErrSqr  = 0
LoSysErr    = 0
HiSysErr    = 0

with open("../count.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fullGroupStr    = array[group]
        fitYieldList    = fullGroupStr[1].split('±')
        nSigFit         = int(round(scaleFactor*float(fitYieldList[0])))
        nSigFitErr      = int(round(scaleFactor*float(fitYieldList[1])))
        pkgBkgList.append('')
        pkgBkgList[group] += str(group) + ' & '
        pkgBkgList[group] += '{}±{}'.format(nSigFit, nSigFitErr) + ' & '

        CentralVal  += nSigFit
        StatErrSqr  += nSigFitErr * nSigFitErr
        lowerLimit  =  scaleFactor*float(fullGroupStr[2]) 
        upperLimit  =  scaleFactor*float(fullGroupStr[3]) 
        LoSysErr    += lowerLimit * lowerLimit 
        HiSysErr    += upperLimit * upperLimit 

print(CentralVal/3449.01,"±",math.sqrt(StatErrSqr)/3449.01,' -',math.sqrt(LoSysErr)/3449.01, ' +',math.sqrt(HiSysErr)/3449.01)
print('{:.2f}±{:.2f} -{:.2f} +{:.2f}'.format(CentralVal/3449.01,math.sqrt(StatErrSqr)/3449.01,math.sqrt(LoSysErr)/3449.01, math.sqrt(HiSysErr)/3449.01))

for ele in pkgBkgList:
    print(ele)

