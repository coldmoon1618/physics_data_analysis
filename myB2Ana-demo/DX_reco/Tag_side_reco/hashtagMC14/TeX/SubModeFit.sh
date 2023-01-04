#!/bin/csh

#python ConditionalFit.py --fInput 'AllModes_MC.root' --cutStr 'SigProb>0.5'  --mynsig  300000 --mynbkg  500000 
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'SigProb>0.5'  --mynsig  300000 --mynbkg  500000 
# Uniformm 0.5 cut, then discard below 40% later
# Initial values don't matter much, not updated compared to second try
## Mode00&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==0'  --mynsig  1614 --mynbkg   8067 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==1'  --mynsig   525 --mynbkg   1259 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==2'  --mynsig 22241 --mynbkg 153126 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==3'  --mynsig  2216 --mynbkg   9332 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==4'  --mynsig  1165 --mynbkg   3119 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==5'  --mynsig 38390 --mynbkg 179813 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==6'  --mynsig  6262 --mynbkg  27096 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==7'  --mynsig   261 --mynbkg    591 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==8'  --mynsig  1168 --mynbkg   5278 &
##python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==9'  --mynsig   261 --mynbkg    591  #BAD SHAPE&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==10' --mynsig  2760 --mynbkg   9648 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==11' --mynsig  5341 --mynbkg  64573 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==12' --mynsig  1487 --mynbkg   2423 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==13' --mynsig 30444 --mynbkg 137027 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==0&&subMode==14' --mynsig  4367 --mynbkg  12528 &
## Mode01&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==0'  --mynsig  1614 --mynbkg   8067 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==1'  --mynsig  1000 --mynbkg    300 --nPltBin 60 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==2'  --mynsig 22241 --mynbkg 153126 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==3'  --mynsig  2216 --mynbkg   9332 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==4'  --mynsig  1165 --mynbkg   3119 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==5'  --mynsig 38390 --mynbkg 179813 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==6'  --mynsig  6262 --mynbkg  27096 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==7'  --mynsig   261 --mynbkg    591 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==8'  --mynsig   268 --mynbkg    278 --nPltBin 60 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==9'  --mynsig  1168 --mynbkg   5278&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==10' --mynsig   960 --mynbkg    248 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==11' --mynsig  1141 --mynbkg    573 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==12' --mylamb 0.1 --mygamma 0.6 --mynsig  87 --mynbkg   223 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==13' --mynsig 30444 --mynbkg 137027 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==1&&subMode==14' --mynsig   937 --mynbkg   1805 &
## Mode03&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==0'  --mynsig  1614 --mynbkg   8067 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==1'  --mynsig   525 --mynbkg   1259 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==2'  --mynsig 22241 --mynbkg 153126 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==3'  --mynsig   100 --mynbkg    100 --nPltBin 60 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==4'  --mynsig  1165 --mynbkg   3119 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==5'  --mynsig 38390 --mynbkg 179813 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==6'  --mynsig   100 --mynbkg    100 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==7'  --mynsig   261 --mynbkg    591 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==8'  --mynsig  1168 --mynbkg   5278 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==9'  --mynsig   261 --mynbkg    591&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==10' --mynsig  2760 --mynbkg   9648 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==11' --mynsig  5341 --mynbkg  64573 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==12' --mynsig  1487 --mynbkg   2423 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==13' --mynsig 30444 --mynbkg 137027 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==3&&subMode==14' --mynsig   100 --mynbkg    100 --nPltBin 60&
## Mode04&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==4'            --mynsig   100 --mynbkg    400 --skipSub 1 --nPltBin 60&
## Mode14&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==14'           --mynsig 12345 --mynbkg    126 --skipSub 1 &
## Mode16&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==16'           --mynsig   100 --mynbkg    400 --skipSub 1 --nPltBin 60&
## Mode18&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==18'           --mynsig  2900 --mynbkg  22400 --skipSub 1 &
## Mode20&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==20'           --mynsig   500 --mynbkg    400 --skipSub 1 &
## Mode22&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==22'           --mynsig  9000 --mynbkg   4000 --skipSub 1 &
## Mode23&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==23'           --mynsig  9000 --mynbkg   4000 --skipSub 1 &
## Mode24&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==24'           --mynsig   400 --mynbkg    100 --skipSub 1 --nPltBin 60 &
## Mode30&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==30'           --mynsig   666 --mynbkg    496 --skipSub 1 --nPltBin 60 &
## Mode31&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==31'           --mynsig   666 --mynbkg   1496 --skipSub 1 --nPltBin 60 &
## Mode25&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==25&&subMode==0'           --mynsig  6666 --mynbkg   6996 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==25&&subMode==1'           --mynsig  6666 --mynbkg   6996 &
## Mode26&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==26&&subMode==0'           --mynsig  1666 --mynbkg   1996 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==26&&subMode==1'           --mynsig  1666 --mynbkg   1996 &
## Mode27&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==27&&subMode==0'           --mynsig  1666 --mynbkg   1996 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==27&&subMode==1'           --mynsig  1666 --mynbkg   1996 &
## Mode28&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==28&&subMode==0'           --mynsig  1666 --mynbkg   1996 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==28&&subMode==1'           --mynsig  1666 --mynbkg   1996 &
## Mode15&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==0'  --mynsig   614 --mynbkg   867 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==1'  --mynsig   614 --mynbkg   867 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==2'  --mynsig   614 --mynbkg   867 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==3'  --mynsig   614 --mynbkg   867 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==4'  --mynsig   614 --mynbkg     7 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==5'  --mynsig   614 --mynbkg   867 --nPltBin 60&
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==6'  --mynsig   614 --mynbkg   867 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==7'  --mynsig   614 --mynbkg   867 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==8'  --mynsig   614 --mynbkg   867 --nPltBin 60 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==9'  --mynsig   614 --mynbkg   867 --nPltBin 60 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==10'  --mynsig   614 --mynbkg   867 --nPltBin 60 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==11'  --mynsig   614 --mynbkg   867 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==12'  --mynsig   614 --mynbkg   867 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==13'  --mynsig   614 --mynbkg   867 --nPltBin 60 &
#python ConditionalFit.py --fInput 'AllModes_Data.root' --cutStr 'decMode==15&&subMode==14'  --mynsig    24 --mynbkg     2 --nPltBin 60 &
