// some basic utility for runtime exceptions
#ifndef MFL_EXCEPTION_INCLUDED
#define MFL_EXCEPTION_INCLUDED

#include <stdexcept>
#include <string>

/// Macro throwing runtime error when called.
/// Argument \a x has to be a C or C++ string.
/// \details Will also include functionname into the exception.
/// \warning Watch out __FUNCTION__ is nonstandard!
#define THROW_RT_ERR(x)	\
    throw std::runtime_error(std::string("\'") + __FUNCTION__ + "\' -> " + x)
/// Macro throwing runtime error when contition \a x is met.
/// Argument \a y has to be a C or C++ string.
#define THROW_RT_ERR_IF_FALSE(condition,x) if (!condition) THROW_RT_ERR(x)

#endif//MFL_EXCEPTION_INCLUDED
