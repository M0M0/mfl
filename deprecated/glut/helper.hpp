#ifndef MFL_GLUT_HELPER_INCLUDED
#define MFL_GLUT_HELPER_INCLUDED

//#define GLEW_BUILD
#include <GL\glew.h>
#pragma comment(lib,"glew32.lib")

#include <iostream>

#define FREEGLUT_STATIC
#include <GL\glut.h>
#include <GL\GL.h>

#include "MFL_LinAlg.hpp"
#include "MFL_Graphics3D.hpp"

namespace MFL{

	struct GLUT_SETUP{
	
		static MFL::LinAlg::Mat4f				projection;
		static MFL::Graphics3D::Camera<float>	camera;
		

		static float* getCameraProjection(){
			return (float*)&(projection*camera.getMatrix());
		}

		static void Keyboard(unsigned char key,int x, int y){
			switch(key){
			case 'w':
				camera.move( 0.3f,0,0,1);
				break;
			case 's':
				camera.move(-0.3f,0,0,1);
				break;
			case 'd':
				camera.move(-0.3f,1,0,0);
				break;
			case 'a':
				camera.move( 0.3f,1,0,0);
				break;
			case 'r':
				camera.rotateX(-0.03f);
				break;
			case 'f':
				camera.rotateX( 0.03f);
				break;
			case 'q':
				camera.rotateY(-0.03f);
				break;
			case 'e':
				camera.rotateY( 0.03f);
				break;

			default:
				;
			}
		}
		static void Resize(int w,int h){
			if(h==0)
				h=1;
			glViewport(0,0,w,h);
			projection = MFL::Graphics3D::Projection::Perspective( 70.f, float(w)/float(h),1.f, 10000.f );
		}
		GLUT_SETUP(){}
		GLUT_SETUP(int argc,char* argv[],char* Windowname,void (*drawfunc)() = nullptr, unsigned int displaymode = GLUT_RGBA){
			glutInit(&argc, argv);
			glutInitDisplayMode(displaymode);
			glutInitWindowSize(800, 600);
			glutCreateWindow(Windowname);

			GLenum err = glewInit();
			if (GLEW_OK != err) {
				std::cerr << "GLEW Error: "<< glewGetErrorString(err) << std::endl;
				assert(false);
			}

			glutKeyboardFunc(Keyboard);
			glutReshapeFunc(Resize);

			if(drawfunc)
				glutDisplayFunc(drawfunc);
		}

		static void EnableLoop(){
			glutMainLoop();
		}
	};
	// initiate with some reasonable default values!
	MFL::LinAlg::Matrix<float,4>    GLUT_SETUP::projection = MFL::LinAlg::Matrix<float,4>();
	MFL::Graphics3D::Camera<float>  GLUT_SETUP::camera     = MFL::Graphics3D::Camera<float>(MFL::LinAlg::Vector3<float>(0,0,0),MFL::LinAlg::Vector3<float>(0,0,-1));

}

#endif//MFL_GLUT_HELPER_INCLUDED