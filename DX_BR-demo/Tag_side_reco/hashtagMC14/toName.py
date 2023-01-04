import pdg
import sys
if len(sys.argv) != 2:
    print('Usage: basf2 toName.py m_PDG')                       
    sys.exit(1)
m_PDG = int(sys.argv[1])
print(pdg.to_name(m_PDG))
sys.exit(0)
