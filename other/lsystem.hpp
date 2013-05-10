#ifndef MFL_LSYSTEM_INCLUDED
#define MFL_LSYSTEM_INCLUDED

#include <string>
#include <map>
#include <list>

#include "../common.hpp"

MFL_BEGIN

class LSystem{
 public:
  LSystem(std::map<char,std::string> ruleset, std::string state)
      : iteration_count_(0),
	state_(state.begin(),state.end()),
	ruleset_(ruleset) {}
  int Iterate() {
    std::map<char,std::string>::iterator rule;
    auto iter = state_.begin();
    while (iter != state_.end()) {
      rule = ruleset_.find(*iter);
      if (rule != ruleset_.end()) {
	iter = state_.erase(iter); // iter now points to the next position
	// no change in iter necessary since it points to the following position
        state_.insert(iter,rule->second.begin(),rule->second.end());
      } else ++iter;
    }
    ++iteration_count_;
    return 0;
  }
  std::string state() const {
    return std::string(state_.begin(),state_.end());
  }
  unsigned int iteration_count() const { return iteration_count_; }

 protected:
  unsigned int iteration_count_;
  std::list<char> state_;
  std::map<char,std::string> ruleset_;
};

MFL_END

#endif //MFL_LSYSTEM_INCLUDED
