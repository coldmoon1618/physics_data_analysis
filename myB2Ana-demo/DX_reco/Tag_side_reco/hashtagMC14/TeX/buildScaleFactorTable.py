nBkgData = [1612676,  898173,  546072, 344939, 219948, 138005,  83314,  48414]
nSigData = [ 403821,  320364,  262240, 215733, 176546, 140838, 107739,  71406]

nBkgMC   = [3650435, 2027378, 1225835, 764916, 477873, 290115, 166255,  88187]
nSigMC   = [1312014, 1014990,  810166, 647983, 512197, 393150, 286690, 183295]

cutStr   = [.2,.3,.4,.5,.6,.7,.8,.9]

print()
print()
print('\\begin{table}[h!]')
print('\\begin{center}')
print('\\begin{tabular}{ccccc}')
print('\\hline')
print('Cuts & nSigData & nSigMC & nBkgData & nBkgMC\\\\')
for i in range(len(cutStr)):
    print('SigProb$>${} & {} & {} & {} & {}\\\\'.format(cutStr[i],nSigData[i],nSigMC[i],nBkgData[i],nBkgMC[i]))
print('\\hline')
print('\\end{tabular}')
print('\\caption{Fit yields for Data(156/fb) and MC(300/fb)}')
print('\\end{center}')
print('\\end{table}')
print()

print('\\begin{table}[h!]')
print('\\begin{center}')
print('\\begin{tabular}{ccc}')
print('\\hline')
print('Data/MC & nSigRatio & nBkgRaio\\\\')
for i in range(len(cutStr)):
    sigRatio    = nSigData[i]/nSigMC[i]/156*300
    bkgRatio    = nBkgData[i]/nBkgMC[i]/156*300
    print('SigProb$>${} & {:.2f} & {:.2f}\\\\'.format(cutStr[i],sigRatio,bkgRatio))
print('\\hline')
print('\\end{tabular}')
print('\\caption{Scale factor for nSig and nBkg normalized by int. lumi}')
print('\\end{center}')
print('\\end{table}')
print()
