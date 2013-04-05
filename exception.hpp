// some basic utility for runtime exceptions
#ifndef MFL_EXCEPTION_INCLUDED
#define MFL_EXCEPTION_INCLUDED

#include <stdexcept>
#include <string>

// watch out __FUNCTION__ is nonstandard (why does __func__ not work?)
#define THROW_RT_ERR(x)				throw std::runtime_error(\
										std::string("\'")+__FUNCTION__+"\'"\
										+"\n -> "\
										+x)

#define THROW_RT_ERR_IF_FALSE(x,y)	if(!(x)) THROW_RT_ERR(y)

#endif//MFL_EXCEPTION_INCLUDED