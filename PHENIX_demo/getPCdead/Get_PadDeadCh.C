//
// Generate the list of dead channels for the pad chamber.
// This uses the Run_XXX_PadCal.root file from OnlCal, which
// you can usually find in HPSS at
//
//         /home/phnxsink/runX/oncalhistos/Run_XXXXXX.tar.gz
//
// (or ask Chris Pinkenburg where they are if you can't find them).
//
void Get_PadDeadCh(const char *fname = "tar_files/13/Run_398132_PadCal.root")
{
  gSystem->Load("libpad.so");
  PadAddressObject *addressObj = new PadAddressObject;

  TFile *infile = new TFile(fname,"READ");

  TH2F *padFemPadxPadz[80] = {0};

  TH2F *pad1w = new TH2F("pad1w","PC1.W padx vs padz",216,0,216,160,0,160);
  TH2F *pad1e = new TH2F("pad1e","PC1.E padx vs padz",216,0,216,160,0,160);
  TH2F *deadpad1w = new TH2F("deadpad1w","PC1.W padx vs padz deadmap",216,0,216,160,0,160);
  TH2F *deadpad1e = new TH2F("deadpad1e","PC1.E padx vs padz deadmap",216,0,216,160,0,160);

  TString hname;
  for (int ihist=0; ihist<80; ihist++)
    {
      hname = "padFemPadxPadz"; hname += ihist;
      //cout << hname << endl;
      padFemPadxPadz[ihist] = (TH2F*)infile->Get( hname );
    }

  const int NPADZ = 108;
  const int NPADX = 20;
  //const double HITS_THRESHOLD = 25.0;
  //const double HITS_THRESHOLD = 12.0;
  //const double HITS_THRESHOLD = 18.0;
  //const double HITS_THRESHOLD = 10.0;
  const double HITS_THRESHOLD = 5.0;

  ofstream badchfile("tempdead");

  TH1F *hpadhits = new TH1F("hpadhits","num pad hits",10000,0,10000);
  hpadhits->SetLineColor(2);
  int nbad_padchannels = 0;

  for (int ihist=0; ihist<80; ihist++)
    {
      if ( padFemPadxPadz[ihist]==0 )
        {
          //cout << "Missing padFemPadxPadz" << ihist << endl;
          continue;
        }

      int packet_id = ihist + 4001;
      if ( ihist > 48 )
        {
          packet_id += 16;
        }
      cout << "pktid " << ihist << "\t" << packet_id << endl;

      for (int ipadz=0; ipadz<108; ipadz++)
        {
          for (int ipadx=0; ipadx<20; ipadx++)
            {
              Double_t npadhits = padFemPadxPadz[ihist]->GetBinContent( ipadz+1, ipadx+1 );
              hpadhits->Fill( npadhits );
              int ch_id = addressObj->getChannelid(0,ipadz,ipadx);
              //cout << ch_id << "\t" << ihist << "\t" << ipadz << "\t" << ipadx << endl;

              int side = addressObj->getSide(packet_id);
              int gpadz = side*108 + ipadz;
              int gpadx = ((packet_id-4001)%8)*20 + ipadx;

              //if ( (packet_id==4009 || npadhits < HITS_THRESHOLD) && packet_id != 4010 )
              if ( npadhits < HITS_THRESHOLD && packet_id != 4010 )
                {
                  badchfile << packet_id << "\t" << ch_id << "\t" << 1 << endl;
                  nbad_padchannels++;

                  if ( packet_id<=4016 )
                    {
                      deadpad1w->Fill( gpadz, gpadx );
                    }
                  else if ( packet_id>4016 && packet_id<=4032 )
                    {
                      deadpad1e->Fill( gpadz, gpadx );
                    }
                }

              // fill 2d hist of whole sector
              if ( packet_id<=4016 )
                {
                  pad1w->Fill( gpadz, gpadx, npadhits );
                }
              else if ( packet_id>4016 && packet_id<=4032 )
                {
                  pad1e->Fill( gpadz, gpadx, npadhits );
                }
            }
        }
    }

  badchfile.close();

  // deadpad file must have the number of bad channels on 1st line
  ofstream final_badchfile("dead");
  final_badchfile << nbad_padchannels << endl;
  final_badchfile.close();

  TCanvas *ac = new TCanvas("ac","ac",550,425);
  hpadhits->Draw();
  gPad->SetLogy(1);

  TCanvas *bc = new TCanvas("c_pad1w","pad1w",425,550);
  gStyle->SetPalette(1);
  pad1w->Draw("colz");
  gPad->SetLogz(1);

  TCanvas *cc = new TCanvas("c_pad1e","pad1e",425,550);
  pad1e->Draw("colz");
  gPad->SetLogz(1);

  TCanvas *dc = new TCanvas("c_deadpad1w","pad1w dead",425,550);
  deadpad1w->Draw("colz");
  gPad->SetLogz(1);

  TCanvas *ec = new TCanvas("c_deadpad1e","pad1e dead",425,550);
  deadpad1e->Draw("colz");
  gPad->SetLogz(1);

  gSystem->Exec("cat tempdead >> dead");
}

