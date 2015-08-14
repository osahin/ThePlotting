#ifndef MakeUp_h
#define MakeUp_h
#include <TPad.h>
#include <TH1D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TString.h>
#include <vector>
#include "THStack.h"
#include "TLatex.h"
namespace plotting{
  class MakeUp{
  public:
    static const int font_helv     = 42;     /* Helvetica */ 
    static const int ratio_color   = kRed+4; /* marker and line will be set to this color */ 
    void AddLegend(THStack* s1);    
    void AddLegend(TH1D* h);
    void AddLegend(TH1D* h,const TString&);
    void rock(TPad* t){
      if(Reverse){
	  cms->SetX(0.55);
	  info->SetX(0.55);
	  legend->SetX1(0.135);
	  legend->SetX2(0.35);
	} else {
	  cms->SetX(0.135);
	  info->SetX(0.135);
	}
      init = false;
      t->cd();
      legend->Draw();
      t->SetLogy();
      t->SetTicky(1);
      t->SetTickx(1);
      t->Update();
      t->cd();
      cms->Draw();
    }
    MakeUp(){
      init = false;
      TeXCMS();
      TeXinfo();
      Reverse = false;
    }; 
    ~MakeUp(){}; 
    bool Reverse;    
  private:
    TLegend* legend;
    bool init; 
    TString AddSampleName(const TString &SampName);    
    void initlegend();
    void TeXCMS ();
    void TeXinfo (TString cms_energy = "8",TString lumi = "19.5");
    TLatex* cms;
    TLatex* info;
  };  
};
#endif