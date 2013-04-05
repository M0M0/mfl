#ifndef MFL_PERFORMANCE_INCLUDED
#define MFL_PERFORMANCE_INCLUDED


//windows only!

#ifdef _WIN32
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>
#else
#	error "There is no version fitting your OS."
#endif//_WIN32



namespace MFL{

	class Time{
		__int64 freq;
		__int64 time1, time2;
	public:
		Time(){
#ifdef _WIN32
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
#endif//_WIN32
		}
		void start(){
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&time1);
#endif//_WIN32
		}
		void end(){
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&time2);
#endif//_WIN32
		}
		template <typename T>
		T getseconds (){
			return static_cast<T>(static_cast<double>(time2-time1)/static_cast<double>(freq));
		}
#ifdef _OSTREAM_
		void print(){
			std::cout << getseconds<double>() <<'\n';
		}
#endif//_OSTREAM_
	};

};

#endif//MFL_PERFORMANCE_INCLUDED