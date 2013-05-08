#ifndef MFL_LSYSTEM_INCLUDED
#define MFL_LSYSTEM_INCLUDED

#include <string>
#include <map>

#include "../common.hpp"

MFL_BEGIN

class LSystem{
 public:
  LSystem(std::map<char,std::string> ruleset, std::string state)
      : iteration_count_(0),
	state_(state),
	ruleset_(ruleset) {
  }									       
  int Iterate() {
    std::vector<std::string> update;
    for (auto i : state_) {
      auto j = ruleset_.find(i);
      if (j != ruleset_.end()) update.push_back(j->second);
      else update.push_back(std::string(1,i));
    }
    _state.clear();
    for (auto& i : update) state_ += i;
    ++iteration_count;
    return 0;
  }
  std::string state() const { return state_; }
  int iteration_count() { return iteration_count; }

 protected:
  int iteration_count_;
  std::map<char,std::string> ruleset_;
  std::string state_;
};

MFL_END

#endif //MFL_LSYSTEM_INCLUDED
