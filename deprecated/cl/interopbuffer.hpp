#ifndef MFL_GL_INTEROPBUFFER_INCLUDED
#define MFL_GL_INTEROPBUFFER_INCLUDED

namespace MFL{
namespace CL{

	template <typename T>
	struct interop{
		std::vector<T>	hostdata;
		MFL::GL::Buffer glbuffer;
		cl::Buffer		clbuffer;


		void constructBuffers(const cl::Context& context){
			if(glbuffer.Is())
				glbuffer.Delete();

			glbuffer.Construct(GL_ARRAY_BUFFER,datasize(),GL_STREAM_DRAW);
			MFL_CL_TRY_BEGIN;
			clbuffer = cl::BufferGL(context,CL_MEM_READ_WRITE,glbuffer());
			MFL_CL_TRY_END;
		}
		size_t datasize(){
			return sizeof(T)*size();
		}
		size_t size(){
			return hostdata.size();
		}
		void clacquire(cl::CommandQueue& queue){
			CL_CHECK(::clEnqueueAcquireGLObjects(queue(),1,&this->clbuffer(),0,nullptr,nullptr),"Error in InteropBuffer acquisition!\n" );
		}
		void clrelease(cl::CommandQueue& queue){
			CL_CHECK(::clEnqueueReleaseGLObjects(queue(),1,&this->clbuffer(),0,nullptr,nullptr),"Error in InteropBuffer release!\n");
		}
	};

}
}

#endif//MFL_GL_INTEROPBUFFER_INCLUDED