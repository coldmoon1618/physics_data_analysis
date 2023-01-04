/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooRarFit
 *    File: $Id: RooRelBreitWigner.cc,v 1.2 2008/01/15 06:54:28 zhanglei Exp $
 * Authors:                                                                  
 *                                             
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --
// This is an implementation of Relativistic Breit Wigner for Spin 0,1,2 particles
// with Blatt-Weisskopf form factors and barrier functions for RooRarFit. 
// The meson radius is set to 3.1 GeV^-1 which is similar to the EvtGen number 
// but may not be appropriate for every meson.

#include "RooRarFit/rarVersion.hh"

#include "Riostream.h"
#include <math.h>
#include <complex>

#include "RooRarFit/RooRelBreitWigner.hh"
#include "RooAbsReal.h"
#include "RooRealVar.h"

//------------------------------------------------------------------
RooRelBreitWigner::RooRelBreitWigner(const char *name, const char *title,
				     RooAbsReal& _x, RooAbsReal& _mean,
				     RooAbsReal& _width, Int_t _spin) :
  RooAbsPdf(name,title),
  x("x","Dependent",this,_x),
  mean("mean","Mean",this,_mean),
  width("width","Width",this,_width),
  spin(_spin)
{
}

//------------------------------------------------------------------
RooRelBreitWigner::RooRelBreitWigner(const RooRelBreitWigner& other, 
				     const char* name) : 
  RooAbsPdf(other,name), 
  x("x",this,other.x), 
  mean("mean",this,other.mean),
  width("width",this,other.width)
{
  spin = other.spin;
}

//------------------------------------------------------------------
Double_t RooRelBreitWigner::evaluate() const
{
  Double_t temp = mean*getWidth();
  std::complex<double> T(x*x,0.0);
  std::complex<double> denom(mean*mean-x*x,-1*temp);
  T = T /denom;     // Transition probability
  return(pow(abs(T),2)); // Amplitude (arbitrary scale)
}

//------------------------------------------------------------------
Double_t RooRelBreitWigner::getWidth() const
{
  Double_t q  = getQ(x);
  Double_t q0 = getQ(mean);
  Double_t result(0.0);

  if (q>0 && q0>0 && x>0 && mean>0) {
    result = width * getQterm(q,q0) * (mean/x) 
      * (getFF(q) / getFF(q0));
  }
  return (result);
}

//------------------------------------------------------------------
Double_t RooRelBreitWigner::getQterm(Double_t q, Double_t q0) const
{
  Double_t result = pow(q/q0,2*spin+1);
  return (result); // result = (q/q0)^(2*spin+1);
}

//------------------------------------------------------------------
Double_t RooRelBreitWigner::getFF(Double_t q) const
{
  
  const Double_t radius =  3.1; // Meson Radius Parameter, GeV^{-1}
  Double_t z = q * radius;
  Double_t result(0.0);

  if (spin==1) {
    result = 1.0/(1+z*z);
  } else if (spin==2) {
    result = 1.0/(9 + 3*z*z + z*z*z*z);
  }

  return (result); //square of the Blatt-Weisskopf form factor
}

//-----------------------------------------------------
Double_t RooRelBreitWigner::getQ(Double_t mass) const
{
  // Assumes B as initial particle. Assumes kaon/pion as final state 
  // particles but has only a minor effect if resonance mass is not 
  // near kaon/pion/inial meson mass.
  const Double_t m_Kaon = 0.493677; // GeV/c^2
  const Double_t m_Pion = 0.13957018;
  const Double_t m_initial = 5.279;
  const Double_t m_Spectator = m_Pion;

  if (mass < (m_Kaon+m_Pion)) {return(0);}
  if (mass > (m_initial - m_Spectator)) {return(0);}

  const Double_t mDaugSumSq  = (m_Kaon+m_Pion)*(m_Kaon+m_Pion);
  const Double_t mDaugDiffSq = (m_Kaon-m_Pion)*(m_Kaon-m_Pion);

  Double_t q = sqrt((mass*mass-mDaugSumSq)*(mass*mass-mDaugDiffSq))/(2*mass);
  return(q);
}
