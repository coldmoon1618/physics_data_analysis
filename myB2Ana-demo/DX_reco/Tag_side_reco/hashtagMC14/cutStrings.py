cutStrings =[
'decMode==0&&subMode==2',
'decMode==0&&(subMode==3||subMode==6||subMode==8||subMode==10)',
'decMode==0&&(subMode==5||subMode==13)',
'decMode==0&&subMode==14',

'decMode==1&&(subMode==1||subMode==2||subMode==5)',
'decMode==1&&(subMode==3||subMode==13||subMode==14)',

'decMode==3&&(subMode==2||subMode==13)',
'decMode==3&&subMode==5',

'decMode==15&&(subMode==0||subMode==3)',
'decMode==15&&(subMode==1||subMode==6||subMode==7||subMode==9)',
'decMode==15&&subMode==2',
'decMode==15&&(subMode==5||subMode==8||subMode==12)',
'decMode==15&&(subMode==10||subMode==11)',

'decMode==25&&subMode==0',
'decMode==25&&subMode==1',
'(decMode==27&&subMode==1)||(decMode==28&&subMode==1)',

'decMode==14',
'decMode==16',
'decMode==20',
'decMode==22',
'decMode==23'
]

n = len(cutStrings)
for index in range(n):
    cutStrings[index]=cutStrings[index].replace('&&',' and ').replace('||',' or ')
