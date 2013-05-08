#ifndef MFL_GLINTERFACE_INCLUDED
#define MFL_GLINTERFACE_INCLUDED

#include "common.hpp"

#include "mouse.hpp"

#include <GL\glew.h>
#include <string>

MFL_BEGIN

class GLInterface{
 public:
  virtual ~GLInterface() {}
  virtual bool FeaturesSupported() = 0;
  virtual void Setup() = 0;
  virtual void Draw() =  0;

  virtual void Resize(int width,int height) = 0;
  virtual void KeyPressed(int) = 0;
  virtual void MouseFun(Mouse const& mouse) = 0;
};

MFL_END

#endif//MFL_GLINTERFACE_INCLUDED
