fcnNames = [
'CB',
'Johnson',
'Johnson',
'Johnson',

'CB',
'CB',

'Gaussian',
'Johnson',

'Gaussian',
'Johnson',
'Johnson',
'Gaussian',
'Johnson',

'CB',
'Gaussian',
'Gaussian',
'Gaussian',
 
'Gaussian',
'Gaussian',
'Gaussian',
'Gaussian',
]

n = len(fcnNames)

def getFcnType(fcnStr):
    if(fcnStr=='Johnson'):
        return 0
    elif(fcnStr=='Gaussian'):
        return 1
    elif(fcnStr=='CB'):
        return 2
def getFcnFullName(fcnStr):
    if(fcnStr=='Johnson'):
        return  'Johnson'
    elif(fcnStr=='Gaussian'):
        return 'Gaussian'
    elif(fcnStr=='CB'):
        return 'Crystal Ball'


