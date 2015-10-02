#include "MakeUp.h"
#include <iostream>
#include <TString.h>
#include <TLegend.h>
#include <TH1D.h>
#include "THStack.h"
#include <TList.h>
#include "TObject.h"
#include "TCollection.h"
#include "TLatex.h"
using namespace std;
//modifiy the sample name strings, please make sure you add the list the new sample
//PLEASE CREATE A MAP FOR THIS I AM TOO LAZY TO DO IT NOW
TString plotting::MakeUp::AddSampleName(const TString &SampName)
{
  TString tex;
  if(SampName.Contains("dilep",TString::kIgnoreCase)){
    tex = " #font[42]{t#bar{t}#rightarrow}#font[132]{#it{l#bar{l}}}";
  } else if(SampName.Contains("singlelep",TString::kIgnoreCase) || SampName.Contains("onelep",TString::kIgnoreCase)) {
    tex =" #font[42]{1#font[12]{l} top}";
  } else if(SampName.Contains("wjets",TString::kIgnoreCase)) {
    tex =" #font[42]{W+jets}"; 
  } else if(SampName.Contains("QCD",TString::kIgnoreCase)){
    tex =" #font[42]{QCD}";
  } else if(SampName.Contains("Rare",TString::kIgnoreCase)) {
    tex =" #font[42]{rare}";
  } else if(SampName.Contains("drell",TString::kIgnoreCase)) {
    tex =" #font[42]{Z/#gamma*#rightarrow}#font[132]{#it{l#bar{l}}}";
  } else if(SampName.Contains("T2tb-mStop200mLSP25",TString::kIgnoreCase)) {
    tex =" #font[42]{T2tb (200,25)}";
  } else if(SampName.Contains("T2tb-mStop175mLSP50",TString::kIgnoreCase)) {
    tex =" #font[42]{T2tb (175,50)}";
  } else if(SampName.Contains("T2tb-mStop325mLSP100",TString::kIgnoreCase)) {
    tex =" #font[42]{T2tb (325,100)}";
  } else if(SampName.Contains("T2tb-mStop200mLSP25",TString::kIgnoreCase)) {
    tex =" #font[42]{T2tb (200,25)}";
  } else if(SampName.Contains("TTbar",TString::kIgnoreCase) && SampName.Contains("600-1100",TString::kIgnoreCase)) {
    tex =" #font[42]{t#bar{t} (600-1100) }";
  } else if(SampName.Contains("TTbar",TString::kIgnoreCase) && SampName.Contains("1100-1700",TString::kIgnoreCase)) {
    tex =" #font[42]{t#bar{t} (600-1700) }";
  } else if(SampName.Contains("TTbar",TString::kIgnoreCase) && SampName.Contains("1700-2500",TString::kIgnoreCase)) {
    tex =" #font[42]{t#bar{t} (1700-2500) }";
  } else if(SampName.Contains("TTbar",TString::kIgnoreCase) && SampName.Contains("2500-100000",TString::kIgnoreCase)) {
    tex =" #font[42]{t#bar{t} (2500-10^{5}) }";
  } else if(SampName.Contains("TTbar",TString::kIgnoreCase) && SampName.Contains("0-600",TString::kIgnoreCase)) {
    tex =" #font[42]{t#bar{t} (0-600) }";
  } else if(SampName.Contains("BosonJets",TString::kIgnoreCase) && SampName.Contains("600-1100",TString::kIgnoreCase)) {
    tex =" #font[42]{W+Jets (600-1100) }";
  } else if(SampName.Contains("BosonJets",TString::kIgnoreCase) && SampName.Contains("1100-1800",TString::kIgnoreCase)) {
    tex =" #font[42]{W+Jets (1100-1800) }";
  } else if(SampName.Contains("BosonJets",TString::kIgnoreCase) && SampName.Contains("1800-2700",TString::kIgnoreCase)) {
    tex =" #font[42]{W+Jets (1800-2700) }";
  } else if(SampName.Contains("BosonJets",TString::kIgnoreCase) && SampName.Contains("2700-3700",TString::kIgnoreCase)) {
    tex =" #font[42]{W+Jets (2700-3700) }";
  } else if(SampName.Contains("BosonJets",TString::kIgnoreCase) && SampName.Contains("3700-100000",TString::kIgnoreCase)) {
    tex =" #font[42]{W+Jets (3500-10^{5}) }";
  } else if(SampName.Contains("BosonJets",TString::kIgnoreCase) && SampName.Contains("300-600",TString::kIgnoreCase)) {
    tex =" #font[42]{W+Jets (300-600) }";
  } else if(SampName.Contains("t2t",TString::kIgnoreCase) && SampName.Contains("900",TString::kIgnoreCase)) {
    tex =" #font[42]{T2tt (900-100) }";
  } else if(SampName.Contains("ttbar",TString::kIgnoreCase) && SampName.Contains("all",TString::kIgnoreCase)) {
    tex ="{t#bar{t}}";
  } else if(SampName.Contains("bosonjets",TString::kIgnoreCase) && SampName.Contains("all",TString::kIgnoreCase)) {
    tex ="{W+Jets}";
  } else tex = " #font[42]{"+SampName+"}";
  return tex;
}
//Add new legends for the legend objects
void plotting::MakeUp::AddLegend(TH1D* h) {
  if(!init) {
    initlegend();
  }
  legend->AddEntry(h,AddSampleName(h->GetName()));
}
void plotting::MakeUp::AddLegend(TH1D* h,const TString & samp_name) {
  if(!init){
    initlegend();
  }
  legend->AddEntry(h,AddSampleName(samp_name));
}
void plotting::MakeUp::AddLegend(THStack *s1) {
  if(!init) {
    initlegend();
  }
  TIter next1(s1->GetHists(),kIterBackward);
  while (TObject *obj = next1()){
    legend->AddEntry((TH1D*)obj,AddSampleName(((TH1D*)obj)->GetName()),"lf");
  }
}
//The position (right) of the legend will be overwritten, 
void plotting::MakeUp::initlegend(){
  init = true;
  legend = new TLegend(0.60,0.60,0.84,0.925);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.038);
  legend->SetEntrySeparation(0.15);
}
//The TeX
void plotting::MakeUp::TeXCMS() {
  cms = new TLatex(0.135,0.89,"CMS private");
  cms->SetNDC();
  cms->SetTextFont(62);
  cms->SetTextColor(46);
  cms->SetTextSize(0.035);
}
//here converting cms_energy and lumi from int and double to string does not worth the effort. Trust me...
void plotting::MakeUp::TeXinfo(TString cms_energy,TString lumi){
  info = new TLatex(0.135,0.82,"#int L dt = "+lumi+" fb^{-1}, #sqrt{s}="+cms_energy+" TeV");
  info->SetNDC();
  info->SetTextFont(132);
  info->SetTextColor(1);
  info->SetTextSize(0.035);
}
