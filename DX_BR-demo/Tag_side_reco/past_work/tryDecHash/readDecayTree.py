import decayHash
from ROOT import Belle2

def listFSPs(treeStr):
    myTree = Belle2.DecayTree(treeStr,True)

    nNodes = myTree.getNumberOfDecayNodes();

    FSPs = []

    for i in range(nNodes):
        nodeStr = myTree.getDecayNode(i).print_node(); 
        nodeLst = nodeStr.split('\n')
        if(len(nodeLst)==2):
            pdg = int(nodeLst[0])
            FSPs.append(pdg)

    FSPs.sort()

    return FSPs
