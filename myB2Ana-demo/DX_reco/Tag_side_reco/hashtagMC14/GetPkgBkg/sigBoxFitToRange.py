# Use the fit values from sigBoxes and print out range of nFit
# Work in progress
Value = [
41927,
29178,
121088,
83598,
68445,
30387
]
Error = [
385,
311,
499,
510,
416,
204
]

for i in range(6):
    print(int(round(Value[i]-2.5*Error[i])),'-',int(round(Value[i]+2.5*Error[i])))
