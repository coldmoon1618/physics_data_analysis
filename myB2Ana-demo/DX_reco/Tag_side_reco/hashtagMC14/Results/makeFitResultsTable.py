import csv
import numpy

nTotMode = numpy.zeros(36)
myList = list()
with open("sigPDF.csv") as f:
    mytable = csv.reader(f)
    array=list(mytable)
    for group in range(30):
        groupList = list()
        groupList.append(str(group)) 
        for fcn in range(3):
            index   = group*3+fcn
            row     = array[index]
            nSigVal = int(round(19./30*int(row[3])))
            nSigErr = int(round(19./30*int(row[4])))
            nSigFit = '{}±{}'.format(nSigVal,nSigErr)    
            purity  = '{:.3f}'.format(float(row[6]))
            groupList.append(nSigFit)
            groupList.append(purity)
        myList.append(groupList)

print('\\begin{tabular}{c|cc|cc|cc}')
print('groupID & Johnson && Gaussian&& Crystal Ball&\\\\')
for ele in myList:
    print(ele[0],end=' & ')
    print(ele[1],end=' & ')
    print(ele[2],end=' & ')
    print(ele[3],end=' & ')
    print(ele[4],end=' & ')
    print(ele[5],end=' & ')
    print(ele[6],end='\\\\')
    print()
    print('\\hline')
print('\\end{tabular}')

