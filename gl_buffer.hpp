#ifndef MFL_GL_BUFFER_INCLUDED
#define MFL_GL_BUFFER_INCLUDED

#include "MFL_GL_Instances.hpp"

namespace MFL{
namespace GL{
	// wrapper for a GL Buffer
	class Buffer:virtual public Base{
	protected:
		GLenum _target;
	public:
		Buffer():Base(){
		}
		~Buffer(){
			this->Delete();
		}

		void Gen(){
			if(!_set){
				_set = true;
				glGenBuffers(1, &_id);
				Singleton<Instances<Buffer>>::getInstance()->Register(this);
			}
		}
		void Delete(){
			if(_set){
				_set = false;
				glDeleteBuffers(1, &_id);
			}
			//Singleton<Instances<Buffer>>::getInstance()->Update();
		}

		void Construct(GLenum target, GLsizeiptr size,GLenum DRAW_TYPE,const GLvoid * ptr = nullptr){
			this->Gen();
			_target = target;
			this->Data(size,DRAW_TYPE,ptr);

		}
		GLboolean Is(){
			return glIsBuffer(_id);
		}

		void ChangeTarget(GLenum target){
			_target = target;
		}

		void Bind(GLenum target){
			glBindBuffer(target,  _id);
		}
		void Bind(){
			this->Bind(_target);
		}

		void BindRange(GLenum target,GLuint index,GLuint offset,GLsizeiptr size){
			glBindBufferRange(target,index,_id,offset,size);
		}
		void BindBase(GLenum target,GLuint index){
			glBindBufferBase(target,index,_id);
		}

		static void Release(GLenum target){
			glBindBuffer(target,0);
		}
		void Release(){
			this->Release(_target);
		}

		static void Data(GLenum target,GLsizeiptr size,GLenum DRAW_TYPE,const GLvoid * ptr=nullptr){
			glBufferData(target, size, ptr, DRAW_TYPE);
		}
		void Data(GLsizeiptr size,GLenum DRAW_TYPE,const GLvoid * ptr=nullptr){
			this->Bind();
			Data(_target,size,DRAW_TYPE,ptr);
		}

		static void SubData(GLenum target,GLintptr offset,GLsizeiptr size,const GLvoid * ptr){
			glBufferSubData(target,offset,size,ptr);
		}
		void SubData(GLintptr offset,GLsizeiptr size,const GLvoid * ptr){
			this->Bind();
			SubData(_target,offset,size,ptr);
		}

		static GLvoid* Map(GLenum target,GLenum access){
			return glMapBuffer(target,access);
		}
		GLvoid* Map(GLenum access){
			this->Bind();
			return Map(_target,access);
		}

		static GLboolean Unmap(GLenum target){
			return glUnmapBuffer(target);
		}
		GLboolean Unmap(){
			this->Bind();
			Unmap(_target);
		}

		static void* MapRange(GLenum target,GLintptr offset,GLsizeiptr length,GLenum access){
			return glMapBufferRange(target,offset,length,access);
		}
		void* MapRange(GLintptr offset,GLsizeiptr length,GLenum access){
			this->Bind();
			return MapRange(_target,offset,length,access);
		}

		static void FlushedMappedRange(GLenum target,GLintptr offset,GLsizeiptr length){
			glFlushMappedBufferRange(target,offset,length);
		}
		void FlushedMappedRange(GLintptr offset,GLsizeiptr length){
			FlushedMappedRange(_target, offset,length);
		}

		static void CopySubData(GLenum rtarget,GLenum wtarget,GLintptr roffset, GLintptr woffset,GLsizeiptr size){
			glCopyBufferSubData(rtarget,wtarget,roffset,woffset,size);
		}
		void WriteBufferSubData(GLenum wtarget,GLintptr roffset, GLintptr woffset,GLsizeiptr size){
			this->Bind();
			CopySubData(_target,wtarget,roffset,woffset,size);
		}
		void ReadBufferSubData(GLenum rtarget,GLintptr roffset, GLintptr woffset,GLsizeiptr size){
			this->Bind();
			CopySubData(rtarget,_target,roffset,woffset,size);
		}

		static GLint GetParameter(GLenum target,GLenum pname){
			GLint data;
			glGetBufferParameteriv(target,pname,&data);
			return data;
		}
		GLint GetParamteriv(GLenum pname){
			this->Bind();
			return this->GetParameter(_target,pname);
		}
		static GLint64 GetParameter64(GLenum target,GLenum pname){
			GLint64 data;
			glGetBufferParameteri64v(target,pname,&data);
			return data;
		}
		GLint64 GetParameter64(GLenum pname){
			this->Bind();
			return this->GetParameter64(_target,pname);
		}

		static GLvoid* GetPointer(GLenum target){
			GLvoid * params;
			glGetBufferPointerv(target,GL_BUFFER_MAP_POINTER,&params);
			return params;
		}
		GLvoid* GetPointer(){
			this->Bind();
			return this->GetPointer(_target);
		}

		static void GetSubData(GLenum target,GLintptr offset,GLsizeiptr size,GLvoid* data){

			glGetBufferSubData(target,offset,size,data);

		}
		void GetSubData(GLintptr offset,GLsizeiptr size,GLvoid* data){
			this->Bind();
			this->GetSubData(_target,offset,size,data);
		}



	};

}
}
#endif//MFL_GL_BUFFER_INCLUDED