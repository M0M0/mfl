#ifndef MFL_MOUSE_INCLUDED
#define MFL_MOUSE_INCLUDED

#include "MFL_Common.hpp"

MFL_BEGIN

#define BUTTON_UP	0
#define BUTTON_DOWN	1

// Mousestate struct
struct Mouse{
	bool LButton;
	bool RButton;
	bool MButton;

	bool XButton1;
	bool XButton2;

	struct {
		short X;
		short Y;
	} Last,Current;

	Mouse():LButton(BUTTON_UP),RButton(BUTTON_UP),MButton(BUTTON_UP){
		Last.X = 0;
		Last.Y = 0;
		Current.X = 0;
		Current.Y = 0;
	}

	void UpdatePosition(LONG_PTR lParam){
		Last = Current;
		Current.X = GET_X_LPARAM(lParam);
		Current.Y = GET_Y_LPARAM(lParam);
	}
};

MFL_END

#endif//MFL_MOUSE_INCLUDED