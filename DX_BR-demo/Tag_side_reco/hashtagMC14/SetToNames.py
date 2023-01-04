import pdg
from readDecayTree import *
import sys
if len(sys.argv) != 2:
    print('Usage: basf2 SetToNames.py iMode')
    print('')
    sys.exit(1)

iMode   = int(sys.argv[1])
inputSet = 'decSets/posSet_mode{}.txt'.format(iMode)
B_pos_lst = list()
readInLsts(inputSet,B_pos_lst)
print('Input set: {}'.format(inputSet))

with open('decSets/FSPs/{}.tex'.format(iMode),'w') as tex:
    print('\\begin{table}[h!]',file=tex)
    print('\\begin{center}',file=tex)
    print('\\begin{tabular}{cl}',file=tex)
    print('\\hline',file=tex)
    print('submode& FSPs\\\\',file=tex)
    print('\\hline',file=tex)
    for ele in B_pos_lst:
        nameset = pdg.to_names(json.loads(ele))
        print(B_pos_lst.index(ele),end=' & ',file=tex)
        print('$',end='',file=tex)
        for name in nameset:
            if(name=='mu+'):
                print('\mu^+',end=' ',file=tex)
            elif(name=='mu-'):
                print('\mu^-',end=' ',file=tex)
            elif(name=='e+'):
                print('e^+',end=' ',file=tex)
            elif(name=='e-'):
                print('e^-',end=' ',file=tex)
            elif(name=='pi+'):
                print('\pi^+',end=' ',file=tex)
            elif(name=='pi-'):
                print('\pi^-',end=' ',file=tex)
            elif(name=='K+'):
                print('K^+',end=' ',file=tex)
            elif(name=='K-'):
                print('K^-',end=' ',file=tex)
            elif(name=='K_S0'):
                print('K_S^0',end=' ',file=tex)
            elif(name=='pi0'):
                print('\pi^0',end=' ',file=tex)
            elif(name=='anti-p-'):
                print('{p}',end=' ',file=tex)
            elif(name=='p+'):
                print('p',end=' ',file=tex)
            else:
                print()
                print(name)
                print('mu+',name=='mu+')
                print('mu-',name=='mu-')
                print('e+',name=='e+')
                print('e-',name=='e-')
                print(name,end=' ',file=tex)
        print('$ \\\\',file=tex)
    print('\\hline',file=tex)
    print('\\end{tabular}',file=tex)
    print("\\label{{tab:Mode{:0=2d}FSPs}}".format(iMode),file=tex)
    print('\\caption{{Mode{:0=2d} FSPs.}}'.format(iMode),file=tex)
    print('\\end{center}',file=tex)
    print('\\end{table}',file=tex)
