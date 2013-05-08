#ifndef MFL_PERFORMANCE_INCLUDED
#define MFL_PERFORMANCE_INCLUDED

//DEPRICATED USE CLOCK!
//windows only!

#ifndef _WIN32
#  error "There is no version fitting your OS."
#else
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <Windows.h>
#endif//_WIN32

namespace MFL{

  class Time{
    __int64 freq;
    __int64 time1, time2;
  public:
    Time() {
      QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    }
    void start(){
      QueryPerformanceCounter((LARGE_INTEGER*)&time1);
    }
    void end(){
      QueryPerformanceCounter((LARGE_INTEGER*)&time2);
    }
    template <typename T>
    T getseconds (){
      return static_cast<T>(static_cast<double>(time2 - time1)
			    / static_cast<double>(freq));
    }
#ifdef _OSTREAM_
    void print(){
      std::cout << getseconds<double>() <<'\n';
    }
#endif//_OSTREAM_
  };
  
};

#endif//MFL_PERFORMANCE_INCLUDED
