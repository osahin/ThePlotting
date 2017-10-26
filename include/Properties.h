/* simple class to read the config file which includes the sample path and type information
Author: OS
Do not modify the code (it is tested and it works properly) unless you really know what you are doing and unless it is stated with capital letters (i.e 
YOU CAN ADD YOUR REVISION HISTORY HERE)
ozgur[no_spam].sahin[spam_not]@cern.ch 
*/
#ifndef Properties_h
#define Properties_h
#include <THStack.h>
#include <TString.h>
#include <map>
#include <vector>
#include "Histo.h"

namespace plotting{
  //make this class singleton later 
  class properties{
  public:
    //    std::map<TString, plotting::Histo_t> sample_prop;
    ~properties(){};
    properties(){};
    properties(TString config_path) {
      property = new  std::vector<sample_prop>;
      readConfig(config_path);
    };
    typedef struct {
      TString *sub_dir;
      double weight;
      TString * path;
      plotting::Histo_t sample_type;
    } sample_prop;
    std::vector<sample_prop> * property;  
  private:
    void readConfig(TString & config_path , int trial = 0);
  };  
};
#endif
