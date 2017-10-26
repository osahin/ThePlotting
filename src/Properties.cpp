/*please see the header file for class definition
Author: OS
Do not modify the code (it is tested and it works properly) unless you really know what you are doing and unless it is stated with capital letters (i.e 
YOU CAN ADD YOUR REVISION HISTORY HERE)
1.1 Adding weighting
ozgur[no_spam].sahin[spam_not]@cern.ch 
*/

#include "Properties.h"
#include <TString.h>
#include <iostream>
#include <fstream>
#include <string>
#include <TObjString.h>
#include <TList.h>
#include <TObject.h>
#include <cstdlib>
using std::cout;
using std::cin;
using std::endl;
void plotting::properties::readConfig(TString & config_path, int trial) {
  std::ifstream config_file (config_path.Data());
  //I believe 4 times is enough for someone to write correct path of the config file
  int items = 0;
  if(!config_file.is_open() && trial < 4) {
    cout << "could not find the config file at \n" << config_path << "\n please enter the path of the config file (i.e. ./config.txt)" << endl; 
    cin >> config_path;
    trial++;
    plotting::properties::readConfig(config_path, trial);
  } else if(!config_file.is_open()) {
    cout<< "you could not even manage to enter the correct path of the config file in four tries. I weep for the future of science... " << endl;
    exit(0);
  }
  std::string line;
  TObjArray* list;
  double weight = 0;
  TString * sub_dir = new TString;

  while(getline(config_file,line)) {
    items = 0;
    if('#' != line[0]){ //lines starting with "#" reserved for the comments
      list = ((TString)line).Tokenize(" ");
      items = list->GetEntriesFast();
      if(!(items == 2 || items == 3) ) {
	cout << "please check the config file structure: <sample path> (<sub dir>) <sample type>" << endl; exit(0);
      }
      sample_prop prop;
      TIter next(list);//unfortunately TString can not be stored (or can not be converted to TObject) as TObject so TObjString it is...
      prop.path=(&((TObjString*)next())->String());//The first word is the path and the second one is type of the sample- these two words are seperated with empty space
      weight = -1;
      if(items == 3){
	cout << items << endl;
	(sub_dir) =(& ((TObjString*)next())->String()); 
	if((sub_dir) -> BeginsWith('*')) {
	  std::cout << (TString)(sub_dir->Strip(TString::kLeading,'*')) << std::endl;
	  weight = TString(sub_dir->Strip(TString::kLeading,'*')).Atof();
	  std::cout <<" weight " << weight << std::endl;
	  sub_dir = 0;
	}
      }
      else sub_dir = 0;

      prop.weight = weight;
      prop.sub_dir = sub_dir;
      TString type = ((TObjString*)next())->String();
      cout << "type:"<< type << endl;
      if(type == "bkg")
	prop.sample_type = bkg;
      else if(type == "sig"){
	prop.sample_type = sig;
      } else if(type == "dat"){
	prop.sample_type = dat;
      }
      else {
	cout << " The type of the sample is not defined properly (sig, dat, bkg) "<< endl; exit(0);
      } 
      property->push_back(prop);
    }
  }
  config_file.close();
}
