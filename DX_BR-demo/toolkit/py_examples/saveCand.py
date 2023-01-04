inputList = open('input.list', 'r')

inFiles = inputList.readlines()

import subprocess
for file in inFiles:
    inputFile  = file.split('/')
    mcType = inputFile[-3]
    fInNum = inputFile[-1].split('_')[1]
    indStr = str(int(fInNum))
    RecOut_file  = 'Rec_out/'+str(mcType)+'_'+indStr+'.root'
    candOut_file = 'cand_out/'+str(mcType)+'_cand_'+indStr+'.root'
    command = ['bsub','root -b -l -q SaveSingleCand.C("{}","{}","{}")'.format(RecOut_file,candOut_file,mcType)]
    print(candOut_file)
    subprocess.call(command)
