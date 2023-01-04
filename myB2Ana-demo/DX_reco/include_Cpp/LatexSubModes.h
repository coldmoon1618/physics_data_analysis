#pragma GCC diagnostic ignored "-Wc++17-extensions"
//Heavily work in progress
inline const int nDecModes = 25;
inline int nSubModes[nDecModes]={
    15,  0,  0,  0,  0,
    0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,
    0,  0,  0,  0,  0
};
inline int indexFEI[nDecModes]={
    0,  1,  2,  3,  4,
    14, 15, 16, 17, 18,
    19, 20, 20, 22, 23,
    24, 25, 26, 27, 28, 
    29, 30, 31, 32, 35  
};
//std::vector<std::string> *myStrs;
//myStrs->push_back("#pi^{0}#pi^{+}K^{0}_{S}");
inline const int nTotSubModes = 15;//Needs to manually update this 
//Build a for loop in a simple macro to get the total number of submodes
//Might merge this into the main LatexDecStrings.
//Then [Major] primDau (Sub) FSPs
inline const char *FSPs[nTotSubModes] = {
    "#pi^{0}#pi^{+}K^{0}_{S}",
    "2#pi^{+}#pi^{-}",
    "#pi^{+}#pi^{-}K^{+}",
    "#pi^{+}K^{+}K^{-}",
    "#pi^{0}2#pi^{+}#pi^{-}",
    "#pi^{0}#pi^{+}#pi^{-}K^{+}",
    "2#pi^{+}#pi^{-}K^{0}_{S}",
    "#pi^{0}#pi^{+}K^{+}K^{-}",
    "#pi^{+}K^{0}_{S}K^{+}K^{-}",
    "2#pi^{0}2#pi^{+}#pi^{-}",
    "2#pi^{0}#pi^{+}#pi^{-}K^{+}",
    "#pi^{0}2#pi^{+}#pi^{-}K^{0}_{S}",
    "3#pi^{+}2#pi^{-}",
    "2#pi^{+}2#pi^{-}K^{+}",
    "#pi^{0}2#pi^{+}2#pi^{-}K^{+}",
}; 
