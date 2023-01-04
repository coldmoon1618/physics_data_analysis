/******************************************************************
 * Project: RooFit                                                *
 * Package: RooFitBabar                                           *
 *    File: $Id: RooLass.cc,v 1.1 2010/06/15 13:09:57 fwilson Exp $
 * Authors:                                                       *
 * Fergus Wilson, RAL, fwilson@slac.stanford.edu                  *
 *                                                                *
 * Copyright (c) 2006, RAL                                        *
 *                                                                *
 ******************************************************************/

// -- CLASS DESCRIPTION [PDF] --

//#include "BaBar/BaBar.hh"
#include <iostream>
#include <math.h>
#include <complex>

#include "RooRarFit/RooLass.hh"
#include "RooAbsReal.h"
#include "RooRealVar.h"

using std::cout;
using std::cerr;
using std::endl;

RooLass::RooLass(const char * name, const char * title, 
		 RooAbsReal & fitVariable, RooAbsReal & theMean,
		 RooAbsReal & theWidth, RooAbsReal & effectiveRange,
		 RooAbsReal & scatteringLength, RooAbsReal & turnOffValue)
  : RooAbsPdf(name,title),
    x("x","Dependent",this,fitVariable),
    mean("mean","Mean",this,theMean),
    width("width","Width",this,theWidth),
    effRange("effRange","Effective Range",this,effectiveRange),
      scatLen("scatLen","Scattering Length",this,scatteringLength),
    turnOffVal("turnOffVal","Turn Off Value",this,turnOffValue)
{
}

RooLass::RooLass(const RooLass & other, const char * name)
  : RooAbsPdf(other,name),
    x("x",this,other.x),
    mean("mean",this,other.mean),
    width("width",this,other.width),
    effRange("effRange",this,other.effRange),
    scatLen("scatLen",this,other.scatLen),
    turnOffVal("turnOffVal",this,other.turnOffVal)
{
}

Double_t RooLass::getQ(Double_t mass) const
{

  const Double_t m_Kaon = 0.493677;
  const Double_t m_Pion = 0.13957018;
  const Double_t m_Bmeson = 5.279;
  const Double_t m_Spectator = m_Pion;

  if (mass < (m_Kaon+m_Pion)) {return(0);}
  if (mass > (m_Bmeson - m_Spectator)) {return(0);}

  const Double_t mDaugSumSq  = (m_Kaon+m_Pion)*(m_Kaon+m_Pion);
  const Double_t mDaugDiffSq = (m_Kaon-m_Pion)*(m_Kaon-m_Pion);

  Double_t q  = sqrt((mass*mass-mDaugSumSq)*(mass*mass-mDaugDiffSq))/(2*mass);
  return(q);
}

Double_t RooLass::evaluate() const
{
  Double_t mass = x;

  Double_t q  = getQ(mass);
  if (q==0) {return(0);}

  Double_t q0 = getQ(mean);
  if (q0==0) {return(0);}

  Double_t rho  = 2*q/mass;
  Double_t rho0 = 2*q0/mean;

  Double_t g0sqr = mean*mass*width/rho0;
  // K matrix for BW resonance
  Double_t Khat = g0sqr/(mean*mean - mass*mass);

  if (mass <= turnOffVal) {
    // K matrix for S-wave background + BW resonance
    Khat += ((scatLen*mass)/(2+scatLen*effRange*q*q));
  }

  std::complex<double> T(Khat,0.0);
  std::complex<double> denom(1.0,-Khat*rho);
  T = T / denom; // Transition probability

  return(pow(abs(T),2)); // Amplitude (arbitrary scale);
}
