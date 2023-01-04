import csv
import numpy

nTotMode = numpy.zeros(36)
with open("fitResults.csv") as f:
    mytable = csv.reader(f)
    array=list(mytable)
    for ele in array:
        if(array.index(ele)>0):
            nTotMode[int(ele[0])]+=int(ele[2])
    for ele in array:
        print(ele[0],end=' & ')
        print(ele[1],end=' & ')
        print(ele[2],end=' & ')
        print(ele[3],end=' & ')
        print(ele[4],end=' & ')
        print(ele[5],end=' & ')
        print(ele[6],end=' & ')
        if(array.index(ele)==0):
            print(' relYield \\\\')
        else:
            print('{:.1f} \\\\'.format(100*int(ele[2])/nTotMode[int(ele[0])]))

for i in [0,1,3]:
    print('decMode',i,':',int(nTotMode[i]))
