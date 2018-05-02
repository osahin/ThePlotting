/*please see the header file for class definition
Author: OS
Do not modify the code (it is tested and it works properly) unless you really know what you are doing and unless it is stated with capital letters (i.e 
YOU CAN ADD YOUR REVISION HISTORY HERE)
ozgur[no_spam].sahin[spam_not]@cern.ch 
*/

#include "Dir.h"
#include <TString.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <TCanvas.h>
#include <ctime>
#include <TText.h>
using std::cout;
using std::cin;
using std::endl;
//Simple method to save canvases with the given option (*.pdf,*.jpeg,*.png etc) by conserving the initial directory structure
//YOU CAN OPTIMIZE THIS FUNCTION
void plotting::Dir::SaveCanvas(const TString &option, const std::vector<TString> & dirName, TCanvas* c1 )
{
  bool eq = false;
  //if the previous file is from the same directory no need to open a new one...  
  if (prev_path.size() > 0 )
    {
      eq = std::equal(dirName.begin(), dirName.end(), prev_path.begin());
    }
  if(eq)
    {
        c1->SaveAs((TString)c1->GetName()+"."+option);
//      c1->Print("plots.pdf","pdf");
    }
  else {
    if(chdir(pDir)) { cout << "can not go back to the plotting directory" << endl; exit(0);}
    std::vector<TString>::const_iterator dir_it;
    for(dir_it = dirName.begin(); dir_it != dirName.end(); dir_it++)
      {
	if(chdir(*dir_it))
	  {
	    mkdir(*dir_it,S_IRWXU|S_IRGRP|S_IXGRP);
	    if(chdir(*dir_it)) { cout << "can not enter to the plotting directory" << endl; exit(0);}
	  }
	
      }
    c1->SaveAs((TString)c1->GetName()+"."+option);
//    c1->Print("plots.pdf","pdf");
  }
  prev_path = dirName;
}

void plotting::Dir::CreateFolder( TCanvas * title){
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
  TString introt (" (plots generated ");
  introt = introt + TString(buffer) + TString(")");
  TText * thePlotting = new TText (0.1,0.2, "The Plotting Tool");
  TText * thePlottingt = new TText (0.1,0.1, introt.Data());
  
  title->cd();
  thePlotting->Draw();
  thePlottingt->Draw();
  title->Print("plots.pdf(","pdf");
  
}
void plotting::Dir::EndFolder(TCanvas * end){
  TText * thePlotting = new TText (0.1,0.2, "The Plotting Tool EOF");
  thePlotting->Draw();
  end->Print("plots.pdf)","pdf"); 
}
