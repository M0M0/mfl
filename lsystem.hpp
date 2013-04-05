#ifndef MFL_LSYSTEM_INCLUDED
#define MFL_LSYSTEM_INCLUDED

#include "MFL_Common.hpp"

#include <string>
#include <vector>
#include <map>

MFL_BEGIN

class LSystem{
protected:

	std::map<char,std::string>	    _ruleset;
	std::string						_state;

	const std::string				_startset;
	int								_niters;

public:
	LSystem(std::map<char,std::string>	Ruleset_in,
			std::string					Startset_in)
			:_niters(0), _startset(Startset_in),
			_state(Startset_in), _ruleset(Ruleset_in){}
										  
	int iterate(){
		std::vector<std::string> update;

		for(auto i:_state){
			auto j =_ruleset.find(i);
			if(j!=_ruleset.end())
				update.push_back(j->second);
			else
				update.push_back(std::string(1,i));
		}
		_state.clear();
		for(auto& i:update)
			_state += i;

		++_niters;
		return 0;
	}
	std::string getstate(){
		return _state;
	}
	int iterations(){
		return _niters;
	}

};

MFL_END

#endif //MFL_LSYSTEM_INCLUDED