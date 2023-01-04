#!/bin/csh

bsub basf2 BuildDecSet.py $1 2 0
bsub basf2 BuildDecSet.py $1 2 1
bsub basf2 BuildDecSet.py $1 2 2
bsub basf2 BuildDecSet.py $1 2 3
bsub basf2 BuildDecSet.py $1 2 4
bsub basf2 BuildDecSet.py $1 2 5
bsub basf2 BuildDecSet.py $1 2 6
bsub basf2 BuildDecSet.py $1 2 7
bsub basf2 BuildDecSet.py $1 2 8
bsub basf2 BuildDecSet.py $1 2 9
