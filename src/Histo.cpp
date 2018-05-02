/*
Please see Histo.h file for the class definition
Author:OS
*/
#include "Histo.h"
#include <TString.h>
#include <TH1D.h>
#include <TKey.h>
#include <TFile.h>
#include <iostream>
#include <cstdlib>
using namespace std;
//simple method that sets the integral for the given value to Hist container 
void plotting::Histo::Set_integral(Hist* hist_it,int min, int max)  {  
  (*hist_it).integ_reg=(*hist_it).h->Integral(min,max);
}
/*
For each given path iteratively loop over the directories
*/
void plotting::Histo::fill_hist (Histo_t* sample_type, TString* file_path, TString* sub_dir){
  vector<TString> dirName;
  float min = 0., max = 100.; //numbers and generally the structure are dummy, will add the proper treatment later
  sample_name = new TString(*file_path);
  sample_name->Remove(0,sample_name->Last('/')+1);
  sample_name->Remove(sample_name->First('.'),sample_name->Length()-1);
  cout <<" sample name: " <<*sample_name ;
  if(sub_dir) cout << "    sub dir: " << *sub_dir;
  cout << endl;
  TFile *File = new TFile(*file_path,"READ");
  if ( File->IsZombie() ) {
    cout << "File in path \n"<< *file_path << "could not opened successfully\n";
    exit(1);
    } 
  Loop_histos((TDirectory*) File,sample_type,dirName,sub_dir);
  File->Close();
}
//loop over the histograms and fill the container.
void plotting::Histo::Loop_histos(TDirectory * dir,Histo_t * type, vector<TString>& dirName, TString* sub_dir, int dir_depth,  TString dName, TString mName){ //pass pointer later
  if (sub_dir){
    if(!gDirectory->cd((TString)dir->GetPath()+*sub_dir)) cout << " Could not open the subdir... Continue... " << endl;  
    dir=gDirectory;
  }
  TDirectory *cur_dir = gDirectory;   
  TString hName="";
  TIter nextkey(dir->GetListOfKeys()); 
  TKey *key;
  while ((key=(TKey*)nextkey())) {
    if(key->IsFolder() && !TString(key->GetClassName()).Contains("TTree")){
      if(0 == dir_depth ) {
	  dName = (TString)key->GetName();
	  mName = dName;
	  (&dirName)->push_back((TString)key->GetName());
	} else {
	  (&dirName)->push_back((TString)key->GetName());
	  dName = dName+(TString)"_"+(TString)key->GetName();  
	}
      dir_depth++;
      dir->cd(key->GetName());
      TDirectory *subdir = gDirectory;
      plotting::Histo::Loop_histos(subdir,type,dirName,0,dir_depth, dName,mName);
      dir_depth--;
      dName = mName;
      (&dirName)->pop_back();
      cur_dir->cd();
    }
    else if( ((TString)key->GetClassName()).Contains("TH1")){ //||((TString)key->GetClassName()).Contains("TH2") ) {
      Hist hist_container;
      if(key->GetClassName()=="TH2D"){//replace this with find function
	hist_container.h2= (TH2D*)(key->ReadObj())->Clone();
	hist_container.h2->SetDirectory(0);
	//	if(weight > -0.1)   hist_container.h2->Multiply(hist_container.h2,weight);
      } else {	
	hist_container.h= (TH1D*)(key->ReadObj())->Clone("new");
	hist_container.h->Sumw2();        
	hist_container.h->SetDirectory(0);
	//	hist_container.h->Rebin(2);
	Set_integral(&hist_container,hist_container.h->GetMinimumBin(),hist_container.h->GetMaximumBin());
	if(weight > -0.1)   hist_container.h->Scale(weight);
	if (*type==bkg) {
	  hist_container.h->SetFillColorAlpha(col,0.7);
	  hist_container.h->SetLineColorAlpha(1.,1.);
	  hist_container.h->SetLineWidth(1);
	} else if (*type==sig) {
	  hist_container.h->SetLineWidth(3);
	  hist_container.h->SetLineStyle(line_style);
	  hist_container.h->SetLineColorAlpha(col,0.6); 
	}
	else if (*type==dat) {
	  hist_container.h->SetLineColorAlpha(1.,1.);
	  hist_container.h->SetLineWidth(2);
	}
      }
      hist_container.sample_name=sample_name;      
      hist_container.dir=dirName;
      hist_container.type=type;
      hName = (*sample_name)+(TString)"_";
      for(int ind = 0; ind < dirName.size();ind++)
	hName += dirName.at(ind)+(TString)"_";
      hName+=(TString)key->GetName(); 
      /* sanitizing hname */
      hName.ReplaceAll(" ","_");
      hName.ReplaceAll("\\","_");
      hName.ReplaceAll("/","_");
      hName.ReplaceAll("[","_");
      hName.ReplaceAll("]","_");
      hName.ReplaceAll("#","_");
      hName.ReplaceAll("{","_");
      hName.ReplaceAll("}","_");
      hName.ReplaceAll("(","_");
      hName.ReplaceAll(")","_");
      hist_container.h->SetName(hName);
      dir_container->push_back(hist_container);                  
    }
    else continue;
  }
  //the logic reaches this point either after looping over the histograms in a dir or while finalizing the loop over dirs. Therefore, this "if" statement is needed
  if(dir_container->size()) { 
    hists->push_back(*dir_container);
    dir_container=new Hists_container;
  }
}
int plotting::Histo::Print() const {
  cout<<"Hello world!"<<endl;
  return 0;
}
