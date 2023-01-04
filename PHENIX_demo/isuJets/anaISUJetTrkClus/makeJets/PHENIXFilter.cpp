#include "PHENIXFilter.h"
#include <math.h>
#include <gsl/gsl_math.h>

//c++
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <exception>

using namespace std;
double phiPHENIX(const double phi) {
  if (phi > 1.5*M_PI)
    return phi - 2*M_PI;
  else if (phi < -0.5*M_PI)
    return phi + 2*M_PI;
  else
    return phi;
}

int PHENIXFilter(double pt, double phi_in, double pz, double zvtx, float charge)
{
  //cout<<"In filter, charge = "<<charge<<endl;
  // ------------- initialize ------------------------------
  // Phi acceptance of each arm
  double phi_bot_w = -0.589; // MC info form dcgeom96.f
  double phi_top_w =  0.982;
  double phi_bot_e =  3.731; // MC info form dcgeom96.f
  double phi_top_e =  2.160;
  // PHENIX convention -pi/2 .. 3pi/2
  double phi = phiPHENIX(phi_in); 

  // Theta acceptance
  const double theta_min = 1.23;  // From data
  const double theta_max = 1.92;
  // slopes for each line tuned ++ field (Run-4/5/6)
  double sl1 =  0.309;    // CRK cut
  double sl2 =  0.206;    // DCH cut
  double sl3 =  0.275;    // PC3 cut
  double slz =  0.004224; // Z_theta slope

  double theta = (M_PI/2.0);
  if(pz!=0.0) {
    theta = atan(pt/fabs(pz)); 
    if(pz<0.0) theta = M_PI - theta;
  } 

  // --------------- ACCEPTANCE CHECK -------------------------

  // --------------------- Theta acceptance ----------------------
  const double th1 = theta-zvtx*slz;
  if (th1>theta_max || th1<theta_min) return -3;
  // --------------------- Phi acceptance ----------------------
  int phi_arm = -1;         //  Flag to fall in phi acceptance
  const double q_pt = charge/pt;
  const double phi1 = phiPHENIX(phi-q_pt*sl1);
  const double phi2 = phiPHENIX(phi-q_pt*sl2);
  const double phi3 = phiPHENIX(phi-q_pt*sl3);

  if (phi2>phi_top_e && phi2<phi_bot_e &&
      phi1>phi_top_e && phi1<phi_bot_e) // Fall into East phi acceptance
  {
    //emcal sectors
    if(phi3>phi_top_e &&
        phi3<(phi_bot_e - phi_top_e)/4.0 + phi_top_e)
    {
      phi_arm = 3; // E3
    }
    else if(phi3>(phi_bot_e - phi_top_e)/4.0 + phi_top_e &&
        phi3<2.0*(phi_bot_e - phi_top_e)/4.0 + phi_top_e)
    {
      phi_arm = 2; // E2
    }
    else if(phi3>2.0*(phi_bot_e - phi_top_e)/4.0 + phi_top_e&&
        phi3<3.0*(phi_bot_e - phi_top_e)/4.0 + phi_top_e)
    {
      phi_arm = 1; // E1
    }
    else if(phi3>3.0*(phi_bot_e - phi_top_e)/4.0 + phi_top_e &&
        phi3<phi_bot_e)
    {
      phi_arm = 0; // E0
    }
  }
  else if (phi2>phi_bot_w && phi2<phi_top_w &&
      phi1>phi_bot_w && phi1<phi_top_w)  // Fall into West phi acceptance
  {
    // emcal sectors
    if(phi3>phi_bot_w &&
        phi3<(phi_top_w - phi_bot_w)/4.0 + phi_bot_w)
    {
      phi_arm = 4; // W0
    }
    else if(phi3>(phi_top_w - phi_bot_w)/4.0 + phi_bot_w &&
        phi3<2.0*(phi_top_w - phi_bot_w)/4.0 + phi_bot_w)
    {
      phi_arm = 5; // W1
    }
    else if(phi3>2.0*(phi_top_w - phi_bot_w)/4.0 + phi_bot_w &&
        phi3<3.0*(phi_top_w - phi_bot_w)/4.0 + phi_bot_w)
    {
      phi_arm = 6; // W2
    }
    else if(phi3>3.0*(phi_top_w - phi_bot_w)/4.0 + phi_bot_w &&
        phi3<phi_top_w)
    {
      phi_arm = 7; // W3
    }
  }
  if (phi_arm == -1) return -4;

  // --------------------- Good track ----------------------------
  return phi_arm;
}
