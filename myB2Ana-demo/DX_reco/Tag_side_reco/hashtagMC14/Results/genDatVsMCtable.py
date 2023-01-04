import csv
import numpy

exec(open("fcnNames.py").read())

DatVsMClist = list()
DatNsig     = list()
DatNsigE    = list()
MC_Nsig     = list()
MC_NsigE    = list()

DatNsigBox  = 0. 
DatNtotBox  = 0.
MC_NsigBox  = 0.
MC_NtotBox  = 0.

with open("DataFit.csv") as f:
    mytable = csv.reader(f)
    array   =  list(mytable)
    for group in range(n):
        fcnType     = getFcnType(fcnNames[group])
        fullGroupStr= array[3*group+fcnType]
        outGroupStr = str(group) + ' & '
        outGroupStr+= '\import{'+'./}{dec/'+'{:0=2d}'.format(group)+'.tex} & '
        outGroupStr+= fullGroupStr[1] + ' & '
        outGroupStr+= fullGroupStr[2] + ' & '
        DatVsMClist.append(outGroupStr)

        fitYieldList    = fullGroupStr[1].split('±')
        nSigFit         = int(fitYieldList[0])
        nSigFitErr      = int(fitYieldList[1])
        DatNsig .append(nSigFit)
        DatNsigE.append(nSigFitErr)

        sigBoxNsig  = float(fullGroupStr[3].split('±')[0])
        purity      = float(fullGroupStr[2]) 
        DatNsigBox  += sigBoxNsig 
        DatNtotBox  += sigBoxNsig/purity

scaleFactor = .19/.3
with open("allMCPDF.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fcnType         = getFcnType(fcnNames[group])
        fullGroupStr    = array[3*group+fcnType]
        fitYieldList    = fullGroupStr[1].split('±')
        nSigFit         = int(round(scaleFactor*float(fitYieldList[0])))
        nSigFitErr      = int(round(scaleFactor*float(fitYieldList[1])))
        DatVsMClist[group] += '{}±{}'.format(nSigFit, nSigFitErr) + ' & '
        DatVsMClist[group] += fullGroupStr[2] + ' & '
        DatVsMClist[group] += getFcnFullName(fcnNames[group]) + ' \\\\'

        MC_Nsig .append(nSigFit)
        MC_NsigE.append(nSigFitErr)

        sigBoxNsig  = float(fullGroupStr[3].split('±')[0])
        purity      = float(fullGroupStr[2]) 
        MC_NsigBox  += sigBoxNsig 
        MC_NtotBox  += sigBoxNsig/purity

divideLines = [3, 5, 7, 12, 15, 20]
for ele in DatVsMClist:
    print(ele,'\\hline')
    if(DatVsMClist.index(ele) in divideLines):
        print('\\hline')

DatTot      = 0
DatErrSqr   = 0.0
MC_Tot      = 0
MC_ErrSqr   = 0.0

for group in range(n):
    DatTot += DatNsig[group]
    DatErrSqr   += DatNsigE[group] * DatNsigE[group]
    MC_Tot += MC_Nsig[group]
    MC_ErrSqr   += MC_NsigE[group] * MC_NsigE[group]

DatPurity = DatNsigBox/DatNtotBox
MC_Purity = MC_NsigBox/MC_NtotBox

import math
summaryLine = 'Estimated overall &&& {}±{} & '.format(DatTot, int(round(math.sqrt(DatErrSqr))))
summaryLine+= '{:.3f} & '.format(DatPurity)
summaryLine+= '{}±{} & '.format(MC_Tot, int(round(math.sqrt(MC_ErrSqr))))
summaryLine+= '{:.3f} & '.format(MC_Purity)
summaryLine+= '\\\\'
print(summaryLine)

