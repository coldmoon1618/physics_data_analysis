import decayHash
from ROOT import Belle2
import json
from collections import Counter

def readInLsts(fIn,B_lst):
    #print('Reading in',fIn,'as list')
    f = open(str(fIn),'r')
    Lines = f.readlines()
    for line in Lines:
        B_lst.append(line.split('\n')[0])

def readInSets(fIn,B_set):
    #print('Reading in',fIn,'as set')
    f = open(str(fIn),'r')
    Lines = f.readlines()
    for line in Lines:
        B_set.add(line)

def updateSets(fOut,B_set):
    print('Updating',fOut)
    B_lst = list(B_set)
    B_lst.sort(key=lambda item: (len(json.loads(item)), sum(map(abs, json.loads(item)))))
    with open(str(fOut),'w') as f:
        for ele in B_lst:
            print(ele, end='',file=f)

def drawTree(treeStr):
    myTree = Belle2.DecayTree(treeStr,True)
    print(myTree.to_string())

def decayKshort(FSPs):
    while(310 in FSPs):
        FSPs.remove(310)
        FSPs.append(211)
        FSPs.append(-211)
        FSPs.sort(key=lambda item: (abs(item),item))
    return FSPs

def compareLists(mcList,feiList):
    nMissPi0 = 0
    nMissGamma=0
    c1 = Counter(mcList)
    c2 = Counter(feiList)
    missPars = list((c1-c2).elements())
    xtraPars = list((c2-c1).elements())
    misPID   = set()
    for par in missPars:
        if(par==111):
            nMissPi0  +=1
        elif(par==22):
            nMissGamma+=1
        else:
            misPID.add(abs(par))
    for par in xtraPars:
        if(par!=111 and par!=22):
            misPID.add(abs(par))

    return c1, c2, missPars, xtraPars, misPID, nMissPi0, nMissGamma 

def listFSPs(treeStr):
    myTree = Belle2.DecayTree(treeStr,True)
    nNodes = myTree.getNumberOfDecayNodes();
    FSPs = []
    setVal = 0
    for i in range(nNodes):
        node    = myTree.getDecayNode(i)
        #dauList = node.daughters
        nodeStr = node.print_node(); 
        nodeLst = nodeStr.split('\n')
        if(i==0):
            if  (node.pdg== 521):
                setVal = 1
            elif(node.pdg==-521):
                setVal =-1
        # Lists look like ['the','particles',''] Extra empty string caused by newline
        if(len(nodeLst)==2 or (len(nodeLst)==4 and int(nodeLst[0])==111)):   #pi0
            pdg = int(nodeLst[0])
            if(pdg != 22):
                FSPs.append(pdg)
    FSPs.sort(key=lambda item: (abs(item),item))
    return FSPs,setVal

def getnFSgamma(treeStr):
    myTree = Belle2.DecayTree(treeStr,True)
    nNodes = myTree.getNumberOfDecayNodes();
    nTotPi0 = 0
    nTotGamma = 0
    for i in range(nNodes):
        node    = myTree.getDecayNode(i)
        nodePDG = node.pdg
        if(nodePDG==111):
            nTotPi0+=1
        elif(nodePDG==22):
            nTotGamma+=1
    nFSgamma = nTotGamma - 2*nTotPi0 
    '''
    if(nFSgamma>0):
        print ('nTot={}, nPi0={}, nFS={}'.format(nTotGamma,nTotPi0,nFSgamma))
        print(myTree.to_string())
    '''
    return nFSgamma
