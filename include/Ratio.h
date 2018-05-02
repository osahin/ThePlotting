/* Template class for ratio plots 
Author: OS
*/
#ifndef Ratio_h
#define Ratio_h
#include <TPad.h>
#include <TH1D.h>
#include <THStack.h>
#include <TString.h>
#include <TAttAxis.h>
#include "TColorGradient.h"
#include "TStyle.h"
#include <cmath>
#include <iostream>
#include <TLine.h>
#include "MakeUp.h"
#include "Pad.h"
namespace plotting{
  template<class dat_type, class bkg_type, class sig_type>  class Ratio : Pad<dat_type,bkg_type,sig_type>{
  private:
  public:
  Ratio(SizePad_t s, PositionPad_t p, TString t):Pad<dat_type,bkg_type,sig_type>(s,p,t){};
    ~Ratio(){};
    TPad* DrawPlot(dat_type*,bkg_type*,sig_type* signal = 0);
  };
  template<class dat_type, class bkg_type, class sig_type> TPad* plotting::Ratio<dat_type,bkg_type,sig_type>::DrawPlot(dat_type* data,bkg_type* background, sig_type* signal){
    TPad* pad1= this->AllocPad(data->GetName(),background->GetName());
    pad1->Draw();
    TString name = (TString)"Ratio plot of "+data->GetName()+" vs "+background->GetName();
    dat_type* ratio = (dat_type*)data->Clone(name);
    pad1->cd();
    ratio->SetStats(0);
    ratio->SetTitle("");
    ratio->Divide(background);
    double min;
    double max;
    ratio->GetYaxis()->SetTitle      ((TString)"#frac{data}{MC}");
    ratio->GetYaxis()->SetLabelFont(42); //font in pixels
    ratio->GetYaxis()->SetLabelSize(0.11); //in pixels
    ratio->GetYaxis()->SetTitleFont  (42);
    ratio->GetYaxis()->CenterTitle   ();
    ratio->GetYaxis()->SetTitleSize  (0.15); 
    ratio->GetYaxis()->SetTitleOffset(0.33);
    ratio->GetYaxis()->SetTickSize(0.02);
    ratio->SetMarkerStyle(1);
    ratio->SetMarkerStyle(1);
    if(Pad<dat_type,bkg_type,sig_type>::pos==kTop) {
      ratio->SetLabelSize(0,"X");
    } else {
      ratio->GetXaxis()->SetLabelSize  (0.12); //in pixel
      ratio->GetXaxis()->SetLabelFont  (42);
      ratio->GetXaxis()->SetTitle      (background->GetTitle());
      ratio->GetXaxis()->SetTitleFont  (42); 
      ratio->GetXaxis()->SetTitleSize  (0.150); 
      ratio->GetXaxis()->SetTitleOffset(0.94);
    }
    ratio->GetYaxis()->SetNdivisions(505);
    ratio->GetXaxis()->SetNdivisions(505);
    if(fabs(1.-ratio->GetMinimum())>(fabs(ratio->GetMaximum()-1.)) && ratio->GetMinimum()!= 0){
      double max=1.+fabs(1.-ratio->GetMinimum())-ratio->GetBinError(ratio->GetMinimumBin());
      if(max>2) { 
	ratio->SetMaximum(3.001); 
	ratio->SetMinimum(-0.001); 
      } else if(max>1.5){ 
	ratio->SetMaximum(2.001); 
	ratio->SetMinimum(-0.001); 
      } else{ 
	ratio->SetMaximum(1.001); 
	ratio->SetMinimum(-0.001); 
      }
    } else {
      double max=fabs(ratio->GetMaximum()+ratio->GetBinError(ratio->GetMaximumBin()));
      if(max>2){ 
	ratio->SetMaximum(3.001); ratio->SetMinimum(-0.001); 
      } else if(max>1.5){ 
	ratio->SetMaximum(2.001); 
	ratio->SetMinimum(-0.001); 
      } else { 
	ratio->SetMaximum(1.5001); ratio->SetMinimum(0.4999); 
      }
    }
    ratio->SetMaximum(2.001); 
    ratio->SetMinimum(-0.001); 
    pad1->Update();
    pad1->SetTicky();
    pad1->SetTickx();
    ratio->SetMarkerColor(MakeUp::ratio_color);
    ratio->SetLineColor(MakeUp::ratio_color);
    ratio->SetFillColor(MakeUp::ratio_color);
    ratio->SetMarkerStyle(20);
    ratio->Draw("E");
    TLine* line = new TLine(ratio->GetXaxis()->GetXmin(),1,ratio->GetXaxis()->GetXmax(),1);
    line->SetLineColorAlpha(1,0.3);
    line->Draw();
    pad1->Update();
    return pad1;
  }
};
#endif 
