inputList = open('input.list', 'r')

inFiles = inputList.readlines()

import subprocess
for file in inFiles:
    inputFile  = file.split('/')
    mcType = inputFile[-3]
    fInNum = inputFile[-1].split('_')[1]
    indStr = str(int(fInNum))
    output_file = 'Rec_out/'+str(mcType)+'_'+indStr+'.root'
    command = ['bsub','basf2 Reco_MC.py',file.rstrip(),output_file]
    print(output_file)
    subprocess.call(command)
