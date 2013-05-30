#ifndef MFL_LSYSTEM_INCLUDED
#define MFL_LSYSTEM_INCLUDED

#include <string>
#include <map>
#include <vector>

#include "../common.hpp"

MFL_BEGIN

class LSystem {
 public:
  LSystem(std::map<char,std::string> ruleset, std::string state)
      : iteration_count_(0),
	state_(state.begin(),state.end()),
	ruleset_(ruleset) {}
  int Iterate() {
    std::map<char,std::string>::iterator rule;
    std::vector<char> newstate;
    for (auto iter = state_.begin(); iter != state_.end(); ++iter) {
      rule = ruleset_.find(*iter);
      if (rule != ruleset_.end()) {
	newstate.insert(newstate.end(),rule->second.begin(),rule->second.end());
      }
    }
    std::swap(state_,newstate);
    ++iteration_count_;
    return 0;
  }
  std::string state() const {
    return std::string(state_.begin(),state_.end());
  }
  unsigned int iteration_count() const {
    return iteration_count_;
  }

 protected:
  unsigned int iteration_count_;
  std::vector<char> state_;
  std::map<char,std::string> ruleset_;
};

MFL_END

#endif //MFL_LSYSTEM_INCLUDED
