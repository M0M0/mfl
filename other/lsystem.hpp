#ifndef MFL_LSYSTEM_INCLUDED
#define MFL_LSYSTEM_INCLUDED

#include <string>
#include <map>
#include <vector>

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
    state_.clear();
    for (auto& i : update) state_ += i;
    ++iteration_count_;
    return 0;
  }
  std::string state() const { return state_; }
  unsigned int iteration_count() const { return iteration_count_; }

 protected:
  unsigned int iteration_count_;
  std::string  state_;
  std::map<char,std::string> ruleset_;
};

MFL_END

#endif //MFL_LSYSTEM_INCLUDED
