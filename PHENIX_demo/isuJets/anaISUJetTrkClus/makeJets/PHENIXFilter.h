#ifndef PHENIXFilter_h
#define PHENIXFilter_h

//-----------------------------------------------------------------------------
//
//  Check whether a particle is in PHENIX central arm acceptance (++ field)
//
//  RETURN VALUE:
//  in acceptance:     return 0-7 corresponding to sector
//  not in acceptance: return <0
//
//-----------------------------------------------------------------------------


int PHENIXFilter(double pt, double phi, double pz, double zvtx, float charge);

#endif
