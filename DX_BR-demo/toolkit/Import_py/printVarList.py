##########################################################################
from variables import variables as vm
import variables.collections as vc
import variables.utils as vu

from buildVars import *

B_MC_vars = setB_MCVars()

for ele in B_MC_vars:
    print(ele)
