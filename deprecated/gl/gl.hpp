#ifndef MFL_GL_INCLUDED
#define MFL_GL_INCLUDED


namespace MFL{
namespace GL{

	// wrapper for 2 GL buffers swapped after every draw
	// Unfinished -- Does not work
	/*class SwapBuffer{
	protected:
		Buffer _buff[2];
		int    _swapped;
	public:
		SwapBuffer(){}
		SwapBuffer(GLenum BUFFER_TYPE, GLenum DRAW_TYPE,GLsizeiptr size, const GLvoid * ptr = 0){
			_swapped = 0;
			_buff[0].Construct(BUFFER_TYPE,DRAW_TYPE,size,ptr);
			_buff[1].Construct(BUFFER_TYPE,DRAW_TYPE,size);
		}
		int swap(){
			return ++_swapped%=2;
		}
		void Bind(){
			_buff[_swapped].Bind();
		}
		void Unbind(){
			_buff[_swapped].Unbind();
		}
		GLuint operator()(){
			return _buff[_swapped]();
		}
	};*/


}
}
#endif//MFL_GL_INCLUDED