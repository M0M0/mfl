#ifndef MFL_CL_INCLUDED
#define MFL_CL_INCLUDED

#ifndef NDEBUG
//#	define __CL_ENABLE_EXCEPTIONS
#endif//NDEBUG
#include <CL\cl.hpp>
#pragma comment(lib,"OpenCL.lib")

#ifndef NDEBUG
#include <iostream>
#include <cassert>
#endif

#if !defined(NDEBUG)&&!defined(__CL_ENABLE_EXCEPTIONS)
#define CL_CHECK(errcode,string) \
	if(int err_=(errcode)!=CL_SUCCESS){\
		std::cerr<<err_<<'\t'<<string<<'\n';\
		assert(false);\
	}
#else
#define CL_CHECK(errcode,string)
#endif

#ifdef __CL_ENABLE_EXCEPTIONS
#define MFL_CL_TRY_BEGIN try{
#define MFL_CL_TRY_END   }\
	catch(cl::Error err) {\
         std::cerr\
		<< "ERROR: "\
		<< err.what()\
		<< "("\
		<< err.err()\
		<< ")"\
		<< std::endl;\
	}
#else
#define MFL_CL_TRY_BEGIN
#define MFL_CL_TRY_END
#endif

#if defined(_WIN32)&&defined(__GLEW_H__) 
#define GetDC		wglGetCurrentDC()
#define GetContext  wglGetCurrentContext()
#else//_WIN32&&__GLEW_H__
#define GetDC		0		
#define GetContext  0
#endif//_WIN32&&__GLEW_H__

namespace MFL{
	namespace CL{
		enum Device { CPU, GPU };
		enum Context_Property { CL_ONLY, CL_GL_INTEROP };

		struct Wrap{

			std::vector<cl::Platform>	_platforms;
			cl::Context					_context;
			std::vector<cl::Device>		_devices;
			cl::CommandQueue			_queue;

			Wrap(){}



			Wrap(Device dev_, Context_Property prop_=CL_ONLY) {
				cl_int err = CL_SUCCESS;
				MFL_CL_TRY_BEGIN
				err = cl::Platform::get(&_platforms);
				CL_CHECK(err,"Could not acquire CL Platform!")
				cl_context_properties properties[7];

				properties[0] = CL_CONTEXT_PLATFORM;
				properties[1] = (cl_context_properties)_platforms[0]();

				if( prop_ == CL_ONLY )
					properties[2] = 0;
				else if( prop_ == CL_GL_INTEROP ){

					properties[2] = CL_WGL_HDC_KHR;
					properties[3] = (cl_context_properties)GetDC;
					properties[4] = CL_GL_CONTEXT_KHR;
					properties[5] = (cl_context_properties)GetContext;

					properties[6] = 0;

				}
				cl_device_type type;
				if( dev_ == CPU )
					type = CL_DEVICE_TYPE_CPU;
				else if( dev_ == GPU )
					type = CL_DEVICE_TYPE_GPU;

				_context = cl::Context(type, properties,nullptr,nullptr,&err);
				CL_CHECK(err,"Error creating Context!")
				_devices = _context.getInfo<CL_CONTEXT_DEVICES>(&err);
				CL_CHECK(err,"Error creating Devices!")
				_queue   = cl::CommandQueue(_context, _devices[0], 0, &err);
				CL_CHECK(err,"Error creating Commandqueue!")
				MFL_CL_TRY_END
			}
		};



	};
}

#endif//MFL_CL_INCLUDED