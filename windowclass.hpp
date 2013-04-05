// Implementation of a Basic Window including GL Attributes
// TODO :	
//			- change define CLASS_NAME to something more robust
//			- check if nullptr or NULL is better in the context of window API! ( currently using nullptr )
#pragma once
#ifndef _WIN32
#	error	"MFL::Window is purely Microsoft Windows Class."
#endif//_WIN32

#include "MFL_Common.hpp"
#include "MFL_Exception.hpp"

#include <Windows.h>
#include <Windowsx.h>

#include <map>
#include <string>


#define CLASS_NAME	TEXT("TEST_CLASS")
#define CLASS_TITLE TEXT("Test Title")

MFL_BEGIN

class Window
{
	typedef long							(* MessageHandler)(Window&,HWND,WPARAM,LPARAM);
	typedef std::map<long,MessageHandler>	MessageMap;
	typedef MessageMap::iterator			MessageIterator;

public:
	Window(HINSTANCE hInstance)
		:_hWnd(nullptr),_hInstance(hInstance),WC(hInstance),_ExitCode(-1),_IsExit(false)
	{
		RegisterMessageHandler(WM_DESTROY,&OnDestroy);
		RegisterMessageHandler(WM_CLOSE  ,&OnClose);
	}
	virtual	~Window()
	{
		// not needed but does not hurt

		_hInstance = nullptr;
	}

	virtual void Create(HWND hWndParent,DWORD dwStyles,int width=300,int height=300)
	{
		_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,CLASS_NAME,CLASS_TITLE,dwStyles,0,0,width,height,hWndParent,NULL,_hInstance,(void*)this);
		DWORD err = ::GetLastError();
		if(err>0||!_hWnd)
			THROW_RT_ERR("Window Creation failed with errorcode \'"+std::to_string(static_cast<_ULonglong>(err))+"\'!");
	}

	
	virtual void ShowWindow(int nCmdShow)
	{
		::ShowWindow(_hWnd,nCmdShow);
		::SetForegroundWindow(_hWnd);
		::SetFocus(_hWnd);
	}
	const bool handleMessages()
	{
		static MSG msg;

		if(!_hWnd&&!IsExit())
			THROW_RT_ERR("Messagehandler without Window called!");

		if(PeekMessage(&msg,_hWnd,0,0,PM_REMOVE)>0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}//while(PeekMessage(&msg,_hWnd,0,0,PM_REMOVE)>0);
		else if(IsExit()){
				if(msg.message!=WM_QUIT)
					THROW_RT_ERR("Window is marked for exit but quitmessage was not returned!");
				SetExitCode((long)msg.wParam);
				return false;
			}
		return true;
	}

	long ExitCode()
	{
		if(!IsExit())
			THROW_RT_ERR("ExitCode returned before Window was closed!");
		return _ExitCode;
	}
protected:
	virtual void Delete()
	{
		if(_hWnd){
			::DestroyWindow(_hWnd);
			_hWnd = nullptr;
		}
	}
	MessageHandler RegisterMessageHandler(long Msg,MessageHandler handler)
	{
		MessageHandler m = nullptr;
		auto it = _MsgHandlers.find(Msg);
		if(it != _MsgHandlers.end())
			m = it->second;
		_MsgHandlers[Msg] = handler;
		return m;
	}
private:
	// could be removed!
	// is it clever to have direct access?
	const bool IsExit() const
	{
		return _IsExit;
	}
	void SetExitCode(long ExitCode)
	{
		_ExitCode = ExitCode;
	}
	void SetExit(bool Set)
	{
		_IsExit = Set;
	}

	MessageIterator			GetMessageHandler(long Msg) { auto it = _MsgHandlers.find(Msg); return it; }
	const MessageIterator	NullMessageIterator()		{ return _MsgHandlers.end(); }

	static LRESULT CALLBACK MsgRouter(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		Window* wnd = nullptr;
		if(uMsg==WM_NCCREATE){
			wnd = reinterpret_cast<Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
			::SetWindowLongPtr(hWnd,GWLP_USERDATA,reinterpret_cast<long>(wnd));
		}
		else
			wnd = reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd,GWLP_USERDATA));
	
		if(wnd){
			MessageIterator it = wnd->GetMessageHandler(uMsg);
			if(it!=wnd->NullMessageIterator())
				return (it->second)((*wnd),hWnd,wParam,lParam);
		}

		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}

	static long	OnClose		(Window& wnd,HWND hWnd,WPARAM wParam,LPARAM lParam)
		{ wnd.Delete(); wnd.SetExit(true); return 0; }
	static long	OnDestroy	(Window& wnd,HWND hWnd,WPARAM wParam,LPARAM lParam)
		{ PostQuitMessage(0); return 0; }

	// data members
protected:
	HWND			_hWnd;

	struct WndClass{
		WNDCLASSEX		_wc;
		WndClass(HINSTANCE hInstance){
			_wc.cbSize			= sizeof(WNDCLASSEX);
			_wc.style			= CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
			_wc.lpfnWndProc		= &MsgRouter;
			_wc.cbClsExtra		= 0;
			_wc.cbWndExtra		= 0;
			_wc.hInstance		= hInstance;
			_wc.hIcon			= LoadIcon(nullptr,IDI_WINLOGO);
			_wc.hCursor			= LoadCursor(nullptr,IDC_ARROW);
			_wc.hbrBackground	= nullptr;
			_wc.lpszMenuName	= nullptr;
			_wc.lpszClassName	= CLASS_NAME;
			_wc.hIconSm			= nullptr;

			if(!RegisterClassEx(&_wc))
				THROW_RT_ERR("Error registering Class");
		}
		~WndClass()
			{UnregisterClass(CLASS_NAME,_wc.hInstance);}
	} WC;

private:
	HINSTANCE		_hInstance;
	MessageMap		_MsgHandlers;
	long			_ExitCode;
	bool			_IsExit;
};
MFL_END

#undef CLASS_NAME
#undef CLASS_TITLE