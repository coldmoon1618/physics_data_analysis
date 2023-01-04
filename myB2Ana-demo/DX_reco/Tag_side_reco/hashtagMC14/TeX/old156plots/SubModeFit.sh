#!/bin/csh

#===================================
# Try 1
# Tried and found unreliable with scaling factors
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==0&&SigProb>0.1'  --mynsig  8620 --mynbkg  76549 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==2&&SigProb>0.1'  --mynsig 49313 --mynbkg 379298 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==3&&SigProb>0.1'  --mynsig  5437 --mynbkg  42562 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==5&&SigProb>0.1'  --mynsig 84601 --mynbkg  30277 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==6&&SigProb>0.1'  --mynsig 20100 --mynbkg 138492 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==10&&SigProb>0.1' --mynsig 24708 --mynbkg 133406 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==11&&SigProb>0.1' --mynsig 23599 --mynbkg 162260 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==12&&SigProb>0.1' --mynsig  7462 --mynbkg 134479 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==13&&SigProb>0.1' --mynsig 82603 --mynbkg  39986 &
#python ConditionalFit.py --fInput 'Mode00_MC.root' --cutStr 'subMode==14&&SigProb>0.1' --mynsig 10499 --mynbkg  39793 &

#python GetSignf.py  8620  76549 1  0
#python GetSignf.py 49313 379298 1  2
#python GetSignf.py  5437  42562 1  3
#python GetSignf.py 84601  30277 1  5
#python GetSignf.py 20100 138492 1  6
#python GetSignf.py 24708 133406 1 10
#python GetSignf.py 23599 162260 1 11
#python GetSignf.py  7462 134479 1 12
#python GetSignf.py 82603  39986 1 13
#python GetSignf.py 10499  39793 1 14

#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==0&&SigProb>0.1"  --mynsig  2295 --mynbkg  25662 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==2&&SigProb>0.1"  --mynsig 22241 --mynbkg 153126 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==3&&SigProb>0.1"  --mynsig  2617 --mynbkg  17197 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==5&&SigProb>0.1"  --mynsig 38390 --mynbkg 179813 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==6&&SigProb>0.1"  --mynsig  6991 --mynbkg  50984 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==10&&SigProb>0.1" --mynsig  5691 --mynbkg  46901 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==11&&SigProb>0.1" --mynsig  5341 --mynbkg  64573 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==12&&SigProb>0.2" --mynsig  2173 --mynbkg  37366 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==13&&SigProb>0.1" --mynsig 34012 --mynbkg 243080 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==14&&SigProb>0.1" --mynsig  5293 --mynbkg  25549 &

#===================================
# Try 2
# Min purity at 40%
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==0&&SigProb>0.4"  --mynsig  1614 --mynbkg   8067 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==1&&SigProb>0.7"  --mynsig   525 --mynbkg   1259 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==2&&SigProb>0.1"  --mynsig 22241 --mynbkg 153126 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==3&&SigProb>0.2"  --mynsig  2216 --mynbkg   9332 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==4&&SigProb>0.7"  --mynsig  1165 --mynbkg   3119 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==5&&SigProb>0.1"  --mynsig 38390 --mynbkg 179813 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==6&&SigProb>0.2"  --mynsig  6262 --mynbkg  27096 &
#python ConditionalFit.py --fInput 'Mode00_Data.root' --cutStr 'subMode==7&&SigProb>0.6'  --mynsig   261 --mynbkg    591 &
#python ConditionalFit.py --fInput 'Mode00_Data.root' --cutStr 'subMode==8&&SigProb>0.1'  --mynsig  1168 --mynbkg   5278 &
#python ConditionalFit.py --fInput 'Mode00_Data.root' --cutStr 'subMode==9&&SigProb>0.6' & #BAD SHAPE
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==10&&SigProb>0.4" --mynsig  2760 --mynbkg   9648 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==11&&SigProb>0.5" --mynsig  5341 --mynbkg  64573 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==12&&SigProb>0.7" --mynsig  1487 --mynbkg   2423 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==13&&SigProb>0.2" --mynsig 30444 --mynbkg 137027 &
#python ConditionalFit.py --fInput "Mode00_Data.root" --cutStr "subMode==14&&SigProb>0.2" --mynsig  4367 --mynbkg  12528 &
