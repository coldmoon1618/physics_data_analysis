#!/bin/csh

touch decSets/posSet_mode$1.txt
touch decSets/negSet_mode$1.txt

basf2 CombineSets.py $1 2 0
basf2 CombineSets.py $1 2 1
basf2 CombineSets.py $1 2 2
basf2 CombineSets.py $1 2 3
basf2 CombineSets.py $1 2 4
basf2 CombineSets.py $1 2 5
basf2 CombineSets.py $1 2 6
basf2 CombineSets.py $1 2 7
basf2 CombineSets.py $1 2 8
basf2 CombineSets.py $1 2 9
