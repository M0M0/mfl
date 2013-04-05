#ifndef MFL_CLOCK_INCLUDED
#define MFL_CLOCK_INCLUDED

#include "MFL_Common.hpp"

#include <chrono>

MFL_BEGIN

class clock{
	typedef std::chrono::system_clock			clock_t;
	typedef std::chrono::time_point<clock_t>	tp_t;

	tp_t t0,t1;
	bool _pause;

public:
	clock():t0(),t1(),_pause(false){}

	void start(){
		t0 = clock_t::now();
	}

	void pause(){
		_pause = true;
	}
	void unpause(){
		_pause = false;
		start();
	}

	bool isactive() const{
		return !_pause;
	}

	float delta_ms() {
		using namespace std::chrono;

		if(_pause)
			return 0.f;

		t1 = clock_t::now();

		float delta = static_cast<float>( duration_cast<milliseconds>(t1-t0).count() );
		t0 = t1;

		return delta;
	}
	float delta_s() {
		using namespace std::chrono;

		if(_pause)
			return 0.f;

		t1 = clock_t::now();

		float delta = static_cast<float>( duration_cast<seconds>(t1-t0).count() );
		t0 = t1;

		return delta;
	}
};

MFL_END

#endif//MFL_CLOCK_INCLUDED