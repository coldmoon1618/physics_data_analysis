import csv
import numpy
from math import sqrt

exec(open("fcnNames.py").read())

scaleFactor = .19/.3

pkgBkgList = list()
incl_fit_NsigBox  = list()
excl_flt_NsigBox  = list()
excl_fix_NsigBox  = list()
only_fix_NsigBox  = list()
centralVal  = list()
statError   = list()
loSysErr    = list() 
hiSysErr    = list() 

Incl_fit_NsigBox  = 0.
Incl_fit_NtotBox  = 0.
Incl_fit_eSigSqr  = 0.
Excl_flt_NsigBox  = 0.
Excl_flt_NtotBox  = 0.
Excl_flt_eSigSqr  = 0.
Excl_fix_NsigBox  = 0.
Excl_fix_NtotBox  = 0.
Excl_fix_eSigSqr  = 0.
Only_fix_NsigBox  = 0.
Only_fix_NtotBox  = 0.
Only_fix_eSigSqr  = 0.
CentralVal  = 0.
StatErr     = 0.
LoSysErr    = 0.
HiSysErr    = 0.

# inclusive
with open("allMCPDF.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fcnType         = getFcnType(fcnNames[group])
        fullGroupStr    = array[3*group+fcnType]
        fitYieldList    = fullGroupStr[3].split('±')
        nSigFit         = scaleFactor*float(fitYieldList[0])
        nSigFitErr      = scaleFactor*float(fitYieldList[1])
        pkgBkgList.append('')
        pkgBkgList[group] += str(group) + ' & '
        pkgBkgList[group] += '{}±{}'.format( int(nSigFit) , int(nSigFitErr) ) + ' & '
        #pkgBkgList[group] += fullGroupStr[2] + ' & '

        sigBoxNsig  = scaleFactor*float(fullGroupStr[3].split('±')[0])
        sigBoxEsig  = scaleFactor*float(fullGroupStr[3].split('±')[1])
        purity      = float(fullGroupStr[2]) 
        incl_fit_NsigBox.append(sigBoxNsig)
        Incl_fit_NsigBox  += sigBoxNsig 
        Incl_fit_NtotBox  += sigBoxNsig/purity
        Incl_fit_eSigSqr  += sigBoxEsig * sigBoxEsig 

# float excl 
with open("exlB0_float.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fullGroupStr    = array[group]
        fitYieldList    = fullGroupStr[3].split('±')
        nSigFit         = scaleFactor*float(fitYieldList[0])
        nSigFitErr      = scaleFactor*float(fitYieldList[1])
        pkgBkgList[group] += '{}±{}'.format( int(nSigFit), int(nSigFitErr) ) + ' & '
        #pkgBkgList[group] += fullGroupStr[2] + ' & '

        sigBoxNsig  = scaleFactor*float(fullGroupStr[3].split('±')[0])
        sigBoxEsig  = scaleFactor*float(fullGroupStr[3].split('±')[1])
        purity      = float(fullGroupStr[2]) 
        excl_flt_NsigBox.append(sigBoxNsig)
        Excl_flt_NsigBox  += sigBoxNsig 
        Excl_flt_NtotBox  += sigBoxNsig/purity
        Excl_flt_eSigSqr  += sigBoxEsig * sigBoxEsig 

for group in range(n):
    pkgBkgList[group] += '{:.1f}'.format(100-100.*excl_flt_NsigBox[group]/incl_fit_NsigBox[group]) + ' & '

# fix excl
with open("exlB0_fixBkg.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fullGroupStr    = array[group]
        fitYieldList    = fullGroupStr[3].split('±')
        nSigFit         = scaleFactor*float(fitYieldList[0])
        nSigFitErr      = scaleFactor*float(fitYieldList[1])
        pkgBkgList[group] += '{}±{}'.format( int(nSigFit) , int(nSigFitErr) ) + ' & '
        #pkgBkgList[group] += fullGroupStr[2] + ' & '

        sigBoxNsig  = scaleFactor*float(fullGroupStr[3].split('±')[0])
        sigBoxEsig  = scaleFactor*float(fullGroupStr[3].split('±')[1])
        purity      = float(fullGroupStr[2]) 
        excl_fix_NsigBox.append(sigBoxNsig)
        Excl_fix_NsigBox  += sigBoxNsig 
        Excl_fix_NtotBox  += sigBoxNsig/purity
        Excl_fix_eSigSqr  += sigBoxEsig * sigBoxEsig

for group in range(n):
    pkgBkgList[group] += '{:.1f}'.format(100-100.*excl_fix_NsigBox[group]/incl_fit_NsigBox[group]) + ' & '

# fit B0 only
with open("justB0_fixSig.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fullGroupStr    = array[group]
        fitYieldList    = fullGroupStr[3].split('±')
        nSigFit         = scaleFactor*float(fitYieldList[0])
        nSigFitErr      = scaleFactor*float(fitYieldList[1])
        pkgBkgList[group] += '{}±{}'.format( int(nSigFit) , int(nSigFitErr) ) + ' & '
        #pkgBkgList[group] += fullGroupStr[2] + ' & '

        sigBoxNsig  = scaleFactor*float(fullGroupStr[3].split('±')[0])
        sigBoxEsig  = scaleFactor*float(fullGroupStr[3].split('±')[1])
        only_fix_NsigBox.append(sigBoxNsig)
        Only_fix_NsigBox  += sigBoxNsig 
        Only_fix_eSigSqr  += sigBoxEsig * sigBoxEsig

for group in range(n):
    pkgBkgList[group] += '{:.1f}'.format(100.*only_fix_NsigBox[group]/incl_fit_NsigBox[group]) + ' & ' 

# counting
with open("../count.csv") as f:
    mytable = csv.reader(f)
    array   = list(mytable)
    for group in range(n):
        fullGroupStr    = array[group]
        fitYieldList    = fullGroupStr[1].split('±')
        nSigFit         = scaleFactor*float(fitYieldList[0])
        nSigFitErr      = sqrt(scaleFactor*float(fitYieldList[0]))
        pkgBkgList[group] += '{}±{}'.format(int(nSigFit), int(nSigFitErr)) 

        CentralVal  += nSigFit
        StatErr     += nSigFitErr 
        lowerLimit  =  scaleFactor*float(fullGroupStr[2]) 
        upperLimit  =  scaleFactor*float(fullGroupStr[3]) 
        LoSysErr    += lowerLimit  
        HiSysErr    += upperLimit  
        centralVal.append(nSigFit)
        pkgBkgList[group] += '${}_{'+str(int(round(lowerLimit)))+'}^{+'+str(int(round(upperLimit)))+'}$ & '

for group in range(n):
    pkgBkgList[group] += '{:.1f}±{:.1f}'.format(100.*centralVal[group]/incl_fit_NsigBox[group],100.*sqrt(centralVal[group])/incl_fit_NsigBox[group] ) 
    pkgBkgList[group] += '\\\\ \\hline'

for ele in pkgBkgList:
    print(ele)

print('\\line')
print('total & {}±{} & {}±{} & {:0.1f} & {}±{} & {:0.1f} & {}±{} & {:0.1f} & {}±{}{} & {:0.1f}±{:.1f}{} '.format(
int(round(Incl_fit_NsigBox)),
int(round(sqrt(Incl_fit_eSigSqr))),
int(round(Excl_flt_NsigBox)),
int(round(sqrt(Excl_flt_eSigSqr))),
100-100.0*Excl_flt_NsigBox/Incl_fit_NsigBox,
int(round(Excl_fix_NsigBox)),
int(round(sqrt(Excl_fix_eSigSqr))),
100-100.0*Excl_fix_NsigBox/Incl_fit_NsigBox,
int(round(Only_fix_NsigBox)),
int(round(sqrt(Only_fix_eSigSqr))),
100.0*Only_fix_NsigBox/Incl_fit_NsigBox,
int(round(CentralVal)),
StatErr,
'${}_{'+str(int(round(LoSysErr)))+'}^{+'+str(int(round(HiSysErr)))+'}$',
100.0*CentralVal/Incl_fit_NsigBox,
100.0*StatErr/Incl_fit_NsigBox,
'${}_{'+'{:.3f}'.format(100.*LoSysErr/Incl_fit_NsigBox)
+'}^{+'+'{:.3f}'.format(100.*HiSysErr/Incl_fit_NsigBox)+'}$ \\\\ \\hline ',
))

#TODO: Modulize gen table scripts into direct-read vs fcnType-read
