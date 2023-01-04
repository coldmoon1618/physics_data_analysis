import csv
import numpy

exec(open("fcnNames.py").read())
myList = list()
with open("DataFit.csv") as f:
    mytable = csv.reader(f)
    array=list(mytable)
    for group in range(n):
        groupList = list()
        groupList.append(str(group)) 
        for fcn in range(3):
            index   = group*3+fcn
            row     = array[index]
            fitLst  = row[1].split('±')
            nSigVal = int(fitLst[0])
            nSigErr = int(fitLst[1])
            nSigFit = '{}±{}'.format(nSigVal,nSigErr)    
            purity  = '{:.3f}'.format(float(row[2]))
            groupList.append(nSigFit)
            groupList.append(purity)
        myList.append(groupList)

nTotYield = 0
nTotSgnBk = 0
nTotErrSq = 0
for ele in myList:
    print(ele[0],end=' ')
    fcn = fcnNames[myList.index(ele)]
    if(fcn=='Johnson'):
        print(ele[1],end=' & ')
        print(ele[2],end=' & ')
        print('Johnson',end='\\\\')
        errPrp = ele[1].split('±')
        nTotYield+=int(errPrp[0])
        nTotErrSq+=int(errPrp[1])**2
        nTotSgnBk+=float(errPrp[0])/float(ele[2])
    elif(fcn=='Gaussian'):
        print(ele[3],end=' & ')
        print(ele[4],end=' & ')
        print('Gaussian',end='\\\\')
        errPrp = ele[3].split('±')
        nTotYield+=int(errPrp[0])
        nTotErrSq+=int(errPrp[1])**2
        nTotSgnBk+=float(errPrp[0])/float(ele[4])
    elif(fcn=='CB'):
        print(ele[5],end=' & ')
        print(ele[6],end=' &')
        print('CrystalBall',end='\\\\')
        errPrp = ele[5].split('±')
        nTotYield+=int(errPrp[0])
        nTotErrSq+=int(errPrp[1])**2
        nTotSgnBk+=float(errPrp[0])/float(ele[6])
    print()

import math
print('nTot={}±{}'.format(nTotYield,int(round(math.sqrt(nTotErrSq)))))
print('purity={:.3f}'.format(float(nTotYield)/nTotSgnBk))
