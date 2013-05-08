// Class: MFL::GLWindow
// A Window class with OpenGL context
// To be used together with a MFL::GLImplementation class
// TODO: - Maybe separate Errorlog so there is no need to set
//			it elsewhere.(?)
//		 - Add support for menues
//		 - Add function that dumps supported extensions!
//		 - switch to something more robust for depthbuffer bits and colorbits!
//		 - reset mouse buttons then losing focus
#pragma once

#include <memory>

#include <GL\glew.h>
#include <GL\wglew.h>

#include "MFL_WindowClass.hpp"
#include "MFL_GLInterface.hpp"
#include "MFL_Mouse.hpp"

#ifdef GLEW_STATIC
#  pragma comment(lib,"glew32s.lib")
#else
#  pragma comment(lib,"glew32.lib")
#endif

#pragma comment(lib,"opengl32.lib")

#ifdef USING_ERRORLOG
# include "MFL_Log.hpp"
# ifdef ERRORLOG_NAME
extern MFL::ErrorLog ERRORLOG_NAME;
# else
#   error "Errorlog specified but ERRORLOG_NAME is unspecified."		
# endif
# define LOG_MESSAGE(msg) ::ERRORLOG_NAME.Message(msg)	

#else
#  define LOG_MESSAGE(msg) 0
#endif


MFL_BEGIN

class GLWindow : public Window{
  typedef std::unique_ptr<GLInterface> GLIfPtr_t;
public:
  explicit GLWindow(HINSTANCE hInstance,GLInterface*&& ptr)
      : Window(hInstance),
        _GLptr(ptr),_hDC(nullptr),
        _hGLRC(nullptr),
        _mouse() {
    LOG_MESSAGE("Construction in progress...");
    RegisterMessageHandler(WM_SIZE,&OnResize);

    //  Key Messages
    RegisterMessageHandler(WM_KEYDOWN,&OnKeyPressed);

    //Mouse Messages
    RegisterMessageHandler(WM_MOUSEMOVE,&OnMouseMoved);
    RegisterMessageHandler(WM_LBUTTONDOWN,&OnMouseLPressed);
    RegisterMessageHandler(WM_LBUTTONUP,&OnMouseLReleased);
    RegisterMessageHandler(WM_RBUTTONDOWN,&OnMouseRPressed);
    RegisterMessageHandler(WM_RBUTTONUP,&OnMouseRReleased);
    RegisterMessageHandler(WM_MBUTTONDOWN,&OnMouseMPressed);
    RegisterMessageHandler(WM_MBUTTONUP,&OnMouseMReleased);
    RegisterMessageHandler(WM_MOUSELEAVE,&OnMouseLeave);

    LOG_MESSAGE("Construction successful.");
  }
  // TODO: choose GL Version and Color bit etc.
  void Create(HWND hWndParent, DWORD dwStyles, int width = 600,
	      int height = 600) {
    Window::Create(hWndParent,dwStyles,width,height);
    LOG_MESSAGE("Created dummy window.");

    _hDC = ::GetDC(_hWnd);
    THROW_RT_ERR_IF_FALSE(_hDC,"Could not create device context!");

    PIXELFORMATDESCRIPTOR pfd;
    THROW_RT_ERR_IF_FALSE(::SetPixelFormat(_hDC,1,&pfd),
			  "Could not set pixel format!");

    _hGLRC = ::wglCreateContext(_hDC);
    THROW_RT_ERR_IF_FALSE(_hGLRC,"Could not create GL rendering context!");

    THROW_RT_ERR_IF_FALSE(::wglMakeCurrent(_hDC,_hGLRC),
			  "Could not bind GL rendering context!");

    GLenum err = ::glewInit();
    if(err != GLEW_OK)
      THROW_RT_ERR(reinterpret_cast<const char*>(::glewGetErrorString(err))) ;


    LOG_MESSAGE( 
		std::string("Supporting GLVersion : ")
		+reinterpret_cast<const char*>(::glGetString(GL_VERSION))
		 );

    THROW_RT_ERR_IF_FALSE(_GLptr->FeaturesSupported(),
			  "Required OpenGL features not supported!");

    // Delete dummy Window
    Delete();
    //recreate Window
    Window::Create(hWndParent,dwStyles,width,height);
    LOG_MESSAGE("Recreated window.");
    _hDC = ::GetDC(_hWnd);
    THROW_RT_ERR_IF_FALSE(_hDC,"Could not create device context!");
    int pixelformat = -1;
    if (WGLEW_ARB_pixel_format) {
      int pixAttribs[] = { WGL_SUPPORT_OPENGL_ARB, TRUE,
			   WGL_DRAW_TO_WINDOW_ARB, TRUE,
			   WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			   WGL_COLOR_BITS_ARB, 32,
			   WGL_DEPTH_BITS_ARB, 16,
			   WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			   0 };
      UINT pixCount = 0;
      if (!wglChoosePixelFormatARB(_hDC,pixAttribs,nullptr,1,&pixelformat,
				   &pixCount) || pixelformat == -1) {
	THROW_RT_ERR("Could not choose pixel format!");
      }
    } else {
      THROW_RT_ERR("Could not choose pixel format!");
    }

    THROW_RT_ERR_IF_FALSE(::SetPixelFormat(_hDC,pixelformat,&pfd),
			  "Could not set pixel format!");


    if(WGLEW_ARB_create_context){
      GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,	1,
			 WGL_CONTEXT_MINOR_VERSION_ARB,	1,
#ifdef _DEBUG
			 WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB, 
#endif
			 0 };
      if(GLEW_VERSION_4_0){
	attribs[1] = 4;
	if(GLEW_VERSION_4_3) { attribs[3] = 3;
	} else if(GLEW_VERSION_4_2) { attribs[3] = 2;
	} else if(GLEW_VERSION_4_1) { attribs[3] = 1;
	} else { attribs[3] = 0; }
      }
      else if(GLEW_VERSION_3_0){
	attribs[1] = 3;
	if(GLEW_VERSION_3_3) { attribs[3] = 3;
	} else if(GLEW_VERSION_3_2) { attribs[3] = 2;
	} else if(GLEW_VERSION_3_1) { attribs[3] = 1;
	} else { attribs[3] = 0; }
      }
      else// TODO: Change Message
	THROW_RT_ERR("There was an unexpected error using \
                      WGLEW_ARB_create_context!");
#ifdef USING_ERRORLOG	
      ::ERRORLOG_NAME.Message("Creating GL %i.%i Context.",attribs[1],
			      attribs[3]);
#endif
      _hGLRC = wglCreateContextAttribsARB(_hDC,0,attribs);		
    }
    else
      _hGLRC = ::wglCreateContext(_hDC);	
			

    THROW_RT_ERR_IF_FALSE(_hGLRC,
			  "Could not create GL rendering context!");	
    THROW_RT_ERR_IF_FALSE(wglMakeCurrent(_hDC,_hGLRC),
			  "Could not bind GL rendering context!");
    LOG_MESSAGE("Setting up GL.");
    _GLptr->Setup();
    GetGLError(); // check for errors
    // registering draw function after opengl initialisation is complete!
    RegisterMessageHandler(WM_PAINT,&OnDraw);
    LOG_MESSAGE("Starting Messageloop.");
  }
  void Show(int nCmdShow) { Window::ShowWindow(nCmdShow); }
  void Update() { ::UpdateWindow(_hWnd); }

//==============================================================================
// Function checking for GL errors and if found throwing exceptions
//==============================================================================
  static int GetGLError() {
    std::string str;
    switch(glGetError()){
    case GL_NO_ERROR: return 0;
    case GL_INVALID_ENUM: str = "GL Invalid Enum.";
      break;
    case GL_INVALID_VALUE: str = "GL Invalid value.";
      break;
    case GL_INVALID_OPERATION: str = "GL Invalid Operation.";
      break;
    case GL_STACK_OVERFLOW: str = "GL Stack Overflow.";
      break;
    case GL_STACK_UNDERFLOW: str = "GL Stack Underflow.";
      break;
    case GL_OUT_OF_MEMORY: str = "GL Out of Memory.";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      str = "GL Invalid FrameBuffer Operation.";
      break;
    default:
      str = "Unknown value passed to GetGLErrorString.";
    }
    throw std::runtime_error(str);
  }

protected:
  void SwapBuffers()		{ ::SwapBuffers(_hDC); }
private:
  //=====================================================================================
  // Basic callback functions
  //=====================================================================================
  static long OnResize	(Window& wnd,HWND hWnd,WPARAM wParam,LPARAM lParam)
  {
    GLWindow& glwnd = static_cast<GLWindow&>(wnd);
    glwnd._GLptr->Resize(LOWORD(lParam),HIWORD(lParam));
    return 0;
  }
  static long OnDraw		(Window& wnd,HWND hWnd,WPARAM wParam,LPARAM lParam)
  {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    // swapping buffers as late as possible so the gpu has all the time he needs
    glwnd.SwapBuffers(); 
    glwnd._GLptr->Draw();
#ifdef _DEBUG
    GetGLError();
#endif
    return 0;
  }

//==============================================================================
// Keyboard function
//==============================================================================
  static long OnKeyPressed(Window& wnd,HWND hWnd,WPARAM wParam,LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._GLptr->KeyPressed( static_cast<int>(wParam+32) );
    return 0;
  }

//==============================================================================
// Function deallocating all window resources
//==============================================================================
  void Delete() {
    LOG_MESSAGE("Closing Window.");
    if(_hGLRC){
      THROW_RT_ERR_IF_FALSE(wglMakeCurrent(nullptr,nullptr),
			    "Could not release context!");
      THROW_RT_ERR_IF_FALSE(wglDeleteContext(_hGLRC),
			    "Could not delete context!");
      _hGLRC = nullptr;
    }
    if(_hDC){
      THROW_RT_ERR_IF_FALSE(ReleaseDC(_hWnd,_hDC),
			    "Could not release DC!");
      _hDC = nullptr;
    }
    Window::Delete();
  }

//==============================================================================
// Mouse functions
// using mouse struct in 'MFL_Mouse.hpp'
//==============================================================================

//==============================================================================
// Mousebutton pressed
//==============================================================================
  static long OnMouseLPressed(Window& wnd,HWND hWnd,WPARAM wParam,
			      LPARAM lParam){
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.LButton = BUTTON_DOWN;
    glwnd._mouse.UpdatePosition(lParam);
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }
  static long OnMouseRPressed(Window& wnd,HWND hWnd,WPARAM wParam,
			      LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.RButton = BUTTON_DOWN;
    glwnd._mouse.UpdatePosition(lParam);
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }
  static long OnMouseMPressed(Window& wnd,HWND hWnd,WPARAM wParam,
			      LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.MButton = BUTTON_DOWN;
    glwnd._mouse.UpdatePosition(lParam);
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }

//==============================================================================
// Mousebutton released
//==============================================================================
  static long OnMouseLReleased(Window& wnd,HWND hWnd,WPARAM wParam,
			       LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.LButton = BUTTON_UP;
    glwnd._mouse.UpdatePosition(lParam);
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }
  static long OnMouseRReleased(Window& wnd,HWND hWnd,WPARAM wParam,
			       LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.RButton = BUTTON_UP;
    glwnd._mouse.UpdatePosition(lParam);
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }
  static long OnMouseMReleased(Window& wnd,HWND hWnd,WPARAM wParam,
			       LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.MButton = BUTTON_UP;
    glwnd._mouse.UpdatePosition(lParam);
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }

//============================================================================
// Mouse moved
//============================================================================
  static long OnMouseMoved(Window& wnd,HWND hWnd,WPARAM wParam,LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.UpdatePosition(lParam);
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }

//==============================================================================
// Mouse left window
//==============================================================================
  static long OnMouseLeave(Window& wnd,HWND hWnd,WPARAM wParam,LPARAM lParam) {
    GLWindow& glwnd = reinterpret_cast<GLWindow&>(wnd);
    glwnd._mouse.LButton = BUTTON_UP;
    glwnd._mouse.RButton = BUTTON_UP;
    glwnd._mouse.MButton = BUTTON_UP;
    glwnd._GLptr->MouseFun(glwnd._mouse);
    return 0;
  }

  //data members
protected:
  Mouse		_mouse;

private:
  GLIfPtr_t	_GLptr;
  HDC			_hDC;
  HGLRC		_hGLRC;
};

MFL_END

#undef LOG_MESSAGE
 
