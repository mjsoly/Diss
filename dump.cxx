#include <fstream>


void dump(const char *fname = "YourRootFile.root",const char *nname = "Coincidences")


{

TString dir = gSystem->UnixPathName(__FILE__);
dir.ReplaceAll("/time.cxx","/");
ofstream output_file1;
ofstream output_file2;
output_file1.open("time.txt");
output_file2.open("sinogram.txt");

  if (!fname || !(*fname) || !nname || !(*nname)) return; // just a precaution
  
  TFile *f = TFile::Open(fname, "READ");
  if ((!f) || f->IsZombie()) { delete f; return; } 

 TTree *t; f->GetObject(nname, t);
  if (!t) { delete f; return; } // just a precaution

Double_t time2;
Double_t time1;
Float_t sinogramTheta;
Float_t sinogramS;
Float_t sourcePosX1;
Float_t sourcePosY1;
Int_t n_bins_theta = 800;
Int_t n_bins_S = 1500;
TH2F* h2 = new TH2F("h2", "h2 title", n_bins_theta, 0.0, 180, n_bins_S, -200, 200);


t->SetBranchAddress("time2", &time2);
t->SetBranchAddress("time1", &time1);
t->SetBranchAddress("sinogramTheta", &sinogramTheta);
t->SetBranchAddress("sinogramS", &sinogramS);
t->SetBranchAddress("sourcePosX1", &sourcePosX1);
t->SetBranchAddress("sourcePosY1", &sourcePosY1);
   Int_t nentries = (Int_t)t->GetEntries();
   for (Int_t i=0; i<nentries; i++) {
      t->GetEntry(i);
      output_file1 << time2 << "\t" << time1 << "\t" << time2-time1<<"\n";
      //output_file2 << sinogramTheta * 57.2958 << "\t" << sinogramS << "\t"<< sourcePosX1 << "\t" << sourcePosY1 << "\n";
      h2->Fill(sinogramTheta*57.2958, sinogramS);
    
   }

for (Int_t i = 0; i<n_bins_theta; i++){
    TH1F* h1 = (TH1F*)h2->ProjectionY("lols", i);
    Float_t theta = (180/n_bins_theta)*i;
    //output_file2 << theta << " " ;
    for(Int_t j = 1; j<n_bins_S; j++){
        Float_t S = h1->GetBinContent(j);
        
        output_file2 << S << " ";
}
    output_file2 << "\n";
} 


  
  delete f; // no longer needed (automatically deletes "t")
}
