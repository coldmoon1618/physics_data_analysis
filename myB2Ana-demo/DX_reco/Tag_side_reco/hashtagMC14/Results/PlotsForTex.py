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
'Gaussian',
'Gaussian',
'Gaussian',
]
n = len(fcnNames)
for i in range(n):
    print('\includegraphics[width=5.3cm]{'+'fit_plots/Data_group{:0=2d}_{}.pdf'.format(i,fcnNames[i])+'}')
    print('\includegraphics[width=5.3cm]{'+'fit_plots/AllMC_group{:0=2d}_{}.pdf'.format(i,fcnNames[i])+'}')
    print('\includegraphics[width=5.3cm]{'+'fit_plots/Matched_group{:0=2d}_{}.pdf'.format(i,fcnNames[i])+'}')
    print()
