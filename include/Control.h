/* template class to plot Control plots 
Author: OS
*/
#pragma once
#include <TPad.h>
#include <TH1D.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TString.h>
#include <TGaxis.h>
#include <TStyle.h>
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
    bool normalize = true;
    bool fom = false;
    const double top_offset = 1.2;
    if(data){
      makeup.AddLegend(data,"data - SB","PE0");
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
    //    pad2->SetLogy();
    //        background->Sumw2();
    TH1D* fomh;
    TH1D* sumhb;
    TH1D* fomhd;
    TClass * cl = gROOT->GetClass(background->ClassName());
    if(fom && signal && data){
      gStyle->SetOptStat(kFALSE);
      cl = gROOT->GetClass(signal->ClassName());
      if (cl->InheritsFrom("THStack")) {        
        TH1D* sumhs = new TH1D( *(TH1D*)signal->GetHists()->At(0));
        int    maxBin = data-> GetSize()-2;
        fomhd = (TH1D*) sumhs->Clone("fom");
        for (int ind = 1; ind < maxBin+1; ind++) {
          double bkg_cont = std::sqrt(data->GetBinContent(ind));
          if(bkg_cont < 0.5) bkg_cont = 0.5;
          fomhd->SetBinContent(ind,sumhs->GetBinContent(ind)/100. / bkg_cont );
        }
      }
    }
    if(fom && signal){
      gStyle->SetOptStat(kFALSE);
      if (cl->InheritsFrom("THStack")) {      
        sumhb = new TH1D( *(TH1D*)background->GetHists()->At(0));
        for ( int ih = 1; ih < background->GetHists()->GetEntries(); ih++){
          sumhb->Add((TH1D*)background->GetHists()->At(ih));
        }
        cl = gROOT->GetClass(signal->ClassName());
        if (cl->InheritsFrom("THStack")) {        
          TH1D* sumhs = new TH1D( *(TH1D*)signal->GetHists()->At(0));
          int    maxBin = sumhb-> GetSize()-2;
          fomh = (TH1D*) sumhs->Clone("fom");
          for (int ind = 1; ind < maxBin+1; ind++) {
            double bkg_cont = std::sqrt(sumhb->GetBinContent(ind));
            if(bkg_cont < 0.5) bkg_cont = 0.5;
            fomh->SetBinContent(ind,sumhs->GetBinContent(ind)/100. / bkg_cont );
          }
        }
      }
    }
    if(normalize)
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
    //    sigLegend->Sumw2();
    if (normalize){
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
    background->GetYaxis()->SetLabelSize(0.04); //in pixels
    background->GetYaxis()->SetTitleSize(0.05); //in pixels
    background->GetXaxis()->SetLabelSize(0.04); //in pixels
    background->GetXaxis()->SetTitleSize(0.05); //in pixels
    background->GetYaxis()->SetTickSize(0.02);
    background->GetXaxis()->SetTickSize(0.02);
    if(Pad<dat_type,bkg_type,sig_type>::pos==kTop){
      background->GetXaxis()->SetLabelSize(0);    
    } else background->GetXaxis()->SetTitle(background->GetTitle());
    background->GetYaxis()->SetTitle("n events");
    if (normalize) background->GetYaxis()->SetTitle("a.u.");
    background->GetYaxis()->SetTitleOffset(1.20); 
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
    if(data){ 
        if (normalize == true){
            data->DrawNormalized("SAME,EPX0");
        } else {
            data->Draw("SAME,EPX0");
        }
        
    }
    if(signal){
      signal->Draw("SAME,HIST,NOSTACK");
      //      signal->SetMinimum(1);
    }
    pad2->Update();
    //Set the visible range on Y axis 
    if(data){
        if (normalize){
            background->SetMaximum(0.21);
            background->GetYaxis()->SetRangeUser(0.,0.21);
        } else if(background->GetMaximum() > data->GetMaximum()) {    
            background->SetMaximum(background->GetMaximum()*top_offset);
            background->SetMinimum(0.7);
            background->GetYaxis()->SetRangeUser(0.5,background->GetMaximum());
            pad2->Update();
        } else {
            background->SetMaximum(data->GetMaximum()*top_offset);
            if(background->GetMaximum() > 1.) background->SetMinimum(0.001);
            background->GetYaxis()->SetRangeUser(0.5,data->GetMaximum());
            pad2->Update();
        }
    } else if(signal){
      if(background->GetMaximum() > signal->GetMaximum()) {    
	background->SetMaximum(background->GetMaximum()*top_offset);
	background->SetMinimum(0.0);
	background->GetYaxis()->SetRangeUser(0.0,background->GetMaximum());
	pad2->Update();
      } else {
	background->SetMaximum(signal->GetMaximum()*top_offset);
	if(background->GetMaximum() > 1.) background->SetMinimum(0.7);
       	background->GetYaxis()->SetRangeUser(0.5,signal->GetMaximum());
	pad2->Update();
      }

    } else {    
      if(background->GetMaximum() > 1.) background->SetMinimum(0.9);
      else background->SetMinimum(0.02);
      background->SetMaximum(background->GetMaximum()*top_offset);
      background->GetYaxis()->SetRangeUser(0.9,background->GetMaximum());
      pad2->Update();
    }
    makeup.Reverse=Reverse;
    makeup.rock(pad2);
    pad2->RedrawAxis();
    if(fom && signal){
      TCanvas *fom_plot = new TCanvas("fom_plot","fom_plot", 600, 600);

      fomh->GetXaxis()->SetTitle("MVA cut");
      fomh->GetYaxis()->SetTitle("s/#sqrt(b)");
      fomh->Draw("hist");
      fomh->SetLineColor(kBlue);
      fomh->SetMinimum(0);
      fomh->SetMaximum(1.);
      
      fomhd->Draw("same,hist");
      TLegend* legend = new TLegend(0.1,0.7,0.48,0.9);
      legend->AddEntry(fomh,"b=MC","l");
      legend->AddEntry(fomhd,"b=Data","l");
      legend->AddEntry("gr","Graph with error bars","lep");
      legend = new TLegend(0.60,0.60,0.84,0.925);
      legend->SetBorderSize(0);
      legend->SetFillColorAlpha(0,0.3);
      //  legend->SetFillStyle(0);
      legend->SetTextSize(0.038);
      legend->SetEntrySeparation(0.15);
      legend->Draw();

      fom_plot->Update();
      fom_plot->SaveAs((TString)background->GetName()+"_fom.pdf");


    }



    return pad2;
  }
};


