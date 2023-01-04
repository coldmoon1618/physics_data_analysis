import sys
if len(sys.argv) != 5:
    print('Usage: p3 GetSignf.py nSig nBkg iCut subMode')
    print('')
    sys.exit(1)

nSig    = int(sys.argv[1])
nBkg    = sys.argv[2]
iCut    = int(sys.argv[3])
subMode = int(sys.argv[4])

nSigRatio = [.58, .58, .59, .61, .62, .64, .66, .69, .72, .75]
nBkgRatio = [.85, .85, .85, .85, .86, .87, .89, .91, .96, 1.06]

nSigData = nSig*nSigRatio[iCut]
nBkgData = nSig*nBkgRatio[iCut]
import math
signf = int(round(nSigData/(math.sqrt(nSigData+nBkgData))))
purity = int(round(nSigData/(nSigData+nBkgData)*100))

print('{} & {} & {} & {} \\\\'.format(subMode, nSig, signf,purity))
#print('python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode=={}&&SigProb>0.1" --mynsig {} --mynbkg {} &'.format(
subMode,int(round(nSigData)),int(round(nBkgData))))
