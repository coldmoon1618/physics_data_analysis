from math import comb

P_pass = 0.
hitEff = 0.98 
minHit = 21
print('hitEfficiency=',hitEff*100,'%\n')

for i in [5,10,20,21,22,23,40,55]:
    P_pass = 0.
    nLayer = i
    print('Reached outmost layer=',nLayer)

    for k in range(minHit,nLayer+1):
        poss = comb(nLayer,k)*(hitEff**k)*(1-hitEff)**(nLayer-k)
#        print(k,'\t',poss)
        P_pass+=poss
    print('P_pass=',P_pass,'\n')

