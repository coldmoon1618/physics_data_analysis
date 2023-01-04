/*****************************************************************************
 * Package: RooRarFit                                                        *
 *    File: $Id: RooBallack.cc,v 1.3 2007/06/29 08:37:34 zhanglei Exp $      *
 * Authors:                                                                  *
 *    Karsten Koeneke, Massachusetts Institute of Technology, Cambridge, USA *
 *                                                                           *
 * Copyright (c) 2006, Massachsetts Institute of Technology, Cambridge, USA  *
 *****************************************************************************/

// This is an implementation for the Ballack function for RooFit

#include "RooRarFit/rarVersion.hh"

#include <iostream>
#include <math.h>
#include <TMath.h>

#include "RooRarFit/RooBallack.hh"
#include "RooRealVar.h"
#include "RooRealConstant.h"

RooBallack::RooBallack(const char *name, const char *title,
		       RooAbsReal& _x, RooAbsReal& _mean, 
		       RooAbsReal& _width, RooAbsReal& _tail,
		       RooAbsReal& _alpha, RooAbsReal& _n)
  :
  RooAbsPdf(name, title),
  x("x", "x", this, _x),
  mean("mean",   "mean",  this, _mean),
  width("width", "width", this, _width),
  tail("tail",   "tail",  this, _tail),
  alpha("alpha", "alpha", this, _alpha),
  n("n", "n", this, _n)
{
}

RooBallack::RooBallack(const RooBallack& other, const char* name) :
  RooAbsPdf(other, name), 
  x("x", this, other.x), 
  mean("mean", this, other.mean),
  width("width", this, other.width), 
  tail("tail", this, other.tail), 
  alpha("alpha", this, other.alpha),
  n("n", this, other.n)
{
}

Double_t RooBallack::evaluate() const 
{
  // build the functional form

  double qa=0,qb=0,qc=0,qx=0,qy=0;

  double A=0,B=0,C=0,a=0,c1=0,c2=0;
  double sigTail=0, sigMean=0;

  a = sqrt(log(4.));


  // Make a symmetric function around x=0
  if(x<0) {
    sigTail = -tail;
    sigMean = -mean;
  }
  else {
    sigTail = tail;
    sigMean = mean;
  }

  if(TMath::Abs(tail) < 1.e-7) {
    qc = 0.5*TMath::Power(((x-sigMean)/width),2);
    A = 1.e7;
  }
  else {
    qa = sigTail*a;
    qb = sinh(qa)/qa;
    qx = (x-sigMean)/width*qb;
    qy = 1.+sigTail*qx;
  
    //---- Cutting curve from right side
    if( qy > 1.E-7) {
      qc = 0.5*(TMath::Power((log(qy)/sigTail),2) + sigTail*sigTail);
    }   
    else {
      qc = 15.0;
    }

    // Preparing the polynomial tail
    A = alpha*width*a/sinh(sigTail*a);
  }

  // Determine the side ofthe tail and polynomial
  if( x>=sigMean-fabs(A) && sigTail<0 ) {
    return exp(-qc);
  }
  if( x<=sigMean+fabs(A) && sigTail>0 ) {
    return exp(-qc);
  }
  else {
    if( (1+alpha) > 1.e-7 ) {
      B = -0.5*(TMath::Power( (log(1+alpha)/sigTail), 2 ) + sigTail*sigTail);
    }
    else {
      B = -15.0;
    }

    C = n*A*sigTail*sigTail*(1+alpha)*(TMath::Power( (sigMean+A), n-1. ));
    c2 = - alpha*log(1+alpha)*exp(B)/C;
    c1 = exp(B) - c2*(TMath::Power( (sigMean+A), n));

    return c1 + c2*(TMath::Power(x,n));
  }

}
