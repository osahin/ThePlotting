/* template class to plot Control plots 
Author: OS
*/
#pragma once
#include <TPad.h>
#include <TH1D.h>
#include <THStack.h>
#include <TString.h>
#include <cmath>
#include "MakeUp.h"
#include <TROOT.h>
#include <TClass.h>
#include <iostream>
namespace plotting{
  template<class dat_type, class bkg_type, class sig_type>  class Control :Pad<dat_type,bkg_type,sig_type>{
  private:
    MakeUp makeup;  
  public:
    Control(SizePad_t s, PositionPad_t p, TString t):Pad<dat_type,bkg_type,sig_type>(s,p,t){};
    TPad* DrawPlot(dat_type*,bkg_type* data = 0,sig_type* signal = 0);
  };
  template<class dat_type, class bkg_type, class sig_type> 
  TPad* plotting::Control<dat_type,bkg_type,sig_type>::DrawPlot(dat_type* data,bkg_type* background, sig_type* signal){
    //Assumptions:
    //dat_type is derived from TH1
    //background will be used to style TPad
    bool Reverse = false;
    if(data){
      makeup.AddLegend(data,"data","PE0");
    }
    makeup.AddLegend(background);
    if(signal){ 
      makeup.AddLegend(signal,"l");
    }
    TPad* pad2;
    if(data) pad2 = Pad<dat_type,bkg_type,sig_type>::AllocPad(data->GetName(),      background->GetName()+(TString)"_hs");
    else     pad2 = Pad<dat_type,bkg_type,sig_type>::AllocPad(background->GetName(),background->GetName()+(TString)"_hs");
    pad2->Draw();
    pad2->cd();


    TClass * cl = gROOT->GetClass(background->ClassName());
    if(false)
    if (cl->InheritsFrom("THStack")) {      
      TH1D* sumh = new TH1D( *(TH1D*)background->GetHists()->At(0));
      THStack* stack = new THStack( "bkg", background->GetTitle());
      TH1D* tmph;
      
      for ( int ih = 1; ih < background->GetHists()->GetEntries(); ih++){
	sumh->Add((TH1D*)background->GetHists()->At(ih));
      }

      if(sumh->GetMaximumBin()>(sumh->GetXaxis()->GetLast()-sumh->GetXaxis()->GetFirst())/2) Reverse = true; //to plot the legend on the empty side
      
      for ( int ih = 0; ih < background->GetHists()->GetEntries(); ih++){
	tmph = new TH1D( *(TH1D*)background->GetHists()->At(ih));
	tmph->Scale( 1. / sumh->Integral(0, sumh->GetNbinsX()+1));
	
	stack->Add( tmph);
      }
      
      background = new THStack( *stack);
    }
    background->Draw("HIST");
    

    if(signal)    cl = gROOT->GetClass(signal->ClassName());
    THStack* sigLegend;
    TH1D* sigLegend2;
    if (false){
      std::cout << "entering the signal  normalization " << std::endl;
    if (cl->InheritsFrom("THStack")) {   
      TH1D* sumh = new TH1D( *(TH1D*)signal->GetHists()->At(0));
      THStack* stack = new THStack( "signal", signal->GetTitle());      
      for ( int ih = signal->GetHists()->GetEntries()-1; ih >= 0 ; ih--){
        sumh = new TH1D( *(TH1D*)signal->GetHists()->At(ih));
	
	TH1D* tmph = new TH1D( *(TH1D*)signal->GetHists()->At(ih));
	tmph->Scale( 1. / sumh->Integral(0, sumh->GetNbinsX()+1));
	
	tmph->SetFillStyle(0);
	tmph->SetFillColor( sumh->GetLineColor());
	tmph->SetLineColor( sumh->GetLineColor());
	tmph->SetLineWidth(2);
	
	//	if (ih % 2 == 0) tmph->SetLineStyle(7);
	stack->Add( tmph);
      }
      signal = new THStack( *stack);
      std::cout << " passed the normalization phase"  << std::endl;
    }
    }


    background->GetXaxis()->SetNdivisions(505);
    background->GetYaxis()->SetLabelFont(42); //font in pixels
    background->GetYaxis()->SetLabelSize(0.058); //in pixels
    background->GetYaxis()->SetTitleSize(0.058); //in pixels
    background->GetXaxis()->SetLabelSize(0.058); //in pixels
    background->GetXaxis()->SetTitleSize(0.058); //in pixels
    background->GetYaxis()->SetTickSize(0.02);
    background->GetXaxis()->SetTickSize(0.02);
    if(Pad<dat_type,bkg_type,sig_type>::pos==kTop){
      background->GetXaxis()->SetLabelSize(0);    
    } else background->GetXaxis()->SetTitle(background->GetTitle());
    background->GetYaxis()->SetTitle("events GeV");
    background->GetYaxis()->SetTitleOffset(0.87); 
    background->GetXaxis()->SetTitleOffset(1.0); 
    background->SetTitle("");
    if(data) {
	data->SetMarkerStyle(20);
	data->SetMarkerSize(1.0);
	data->SetLineColor(1);
	if(data->GetMaximumBin()>(data->GetXaxis()->GetLast()-data->GetXaxis()->GetFirst())/2) Reverse = true; //to plot the legend on the empty side
      }
    //    else
    //      if(background->GetMaximumBin()>(data->GetXaxis()->GetLast()-data->GetXaxis()->GetFirst())/2) Reverse = true; //to plot the legend on the empty side


    background->GetXaxis()->SetNdivisions(505);
    pad2->cd();
    pad2->Update();
    if(data){ data->Draw("SAME,EPX0"); std::cout << " data drawn " << std::endl;}
    if(signal){
      signal->Draw("SAME,HIST,NOSTACK");
      //      signal->SetMinimum(1);
    }
    pad2->Update();
    //Set the visible range on Y axis 
    if(data){
      if(background->GetMaximum() > data->GetMaximum()) {    
	background->SetMaximum(background->GetMaximum()*9);
	background->SetMinimum(0.7);
	background->GetYaxis()->SetRangeUser(0.5,background->GetMaximum());
	pad2->Update();
      } else {
	background->SetMaximum(data->GetMaximum()*9);
	if(background->GetMaximum() > 1.) background->SetMinimum(0.7);
       	background->GetYaxis()->SetRangeUser(0.5,data->GetMaximum());
	pad2->Update();
      }
    } else {    
      if(background->GetMaximum() > 1.) background->SetMinimum(0.9);
      else background->SetMinimum(0.02);
      background->SetMaximum(background->GetMaximum()*9);
      background->GetYaxis()->SetRangeUser(0.9,background->GetMaximum());
      pad2->Update();
    }
    makeup.Reverse=Reverse;
    makeup.rock(pad2);
    pad2->RedrawAxis();
    return pad2;
  }
};


