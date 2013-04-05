#ifndef MFL_GL_VERTEXARRAY_INCLUDED
#define MFL_GL_VERTEXARRAY_INCLUDED

#include "MFL_GL_Instances.hpp"

namespace MFL{
namespace GL{
	// TODO: change how textures are read!
	enum VertexAttribute{ 
		Vertex = 0, Normal, Color, Texture0, Texture1, Texture2, Texture3, Texture4
	};

	class VertexArray:virtual public Base{
	public:
		VertexArray():Base(){
		}
		~VertexArray(){
			this->Delete();
		}
		void Gen(){
			if(!_set){
				_set = true;
				glGenVertexArrays(1, &_id);
				Singleton<Instances<VertexArray>>::getInstance()->Register(this);
			}
			else
				Singleton<Instances<VertexArray>>::getInstance()->Update();
		}
		void Delete(){
			if(_set){
				_set = false;
				glDeleteVertexArrays(1, &_id);
			}
			//Singleton<Instances<VertexArray>>::getInstance()->Update();
		}

		void Bind(){
			glBindVertexArray(_id);
		}
		static void Release(){
			glBindVertexArray(0);
		}
		static void EnableAttribute  (VertexAttribute attrib){
			glEnableVertexAttribArray(attrib);
		}
		static void DisableAttribute (VertexAttribute attrib){
			glDisableVertexAttribArray(attrib);
		}

		static void AttributePointer (VertexAttribute attrib, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer){
			glVertexAttribPointer(attrib,size,type,normalized,stride,pointer);
		}
		void AttributeIPointer(VertexAttribute attrib, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer){
			glVertexAttribIPointer(attrib,size,type,stride,pointer);
		}
		void AttributeLPointer(VertexAttribute attrib, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer){
			glVertexAttribLPointer(attrib,size,type,stride,pointer);
		}

		void AttributeDivisor(VertexAttribute attrib, GLuint divisor){
			glVertexAttribDivisor(attrib,divisor);
		}

		static void EnablePrimitiveRestart(){
			glEnable(GL_PRIMITIVE_RESTART);
		}
		static void DisablePrimitiveRestart(){
			glDisable(GL_PRIMITIVE_RESTART);
		}
		static void PrimitiveRestartIndex(GLuint index){
			glPrimitiveRestartIndex(index);
		}
		static void PrimitiveRestart(GLuint index){
			EnablePrimitiveRestart();
			PrimitiveRestartIndex(index);
		}

		GLboolean Is(){
			return glIsVertexArray(_id);
		}
	};
}
}
#endif//MFL_GL_VERTEXARRAY_INCLUDED