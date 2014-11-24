#include "Advanced2d.h"

extern void game_keyPress(int);
extern void game_keyRelease(int);

extern void game_mousePress(int);
extern void game_mouseRelease(int);
extern void game_mouseMotion(int,int);
extern void game_mouseMove(int,int);
extern void game_mouseWheel(int);

extern void game_joystickRelease(int);
extern void game_joystickPress(int);
extern void game_joystickAxis(LONG x,LONG y,LONG z, LONG z_rot);

extern void game_joystickXYAxisPress(int);
extern void game_joystickXYAxisRelease(int);
//extern void game_joystickAxisReleaseX(LONG x);
//extern void game_joystickAxisReleaseY(LONG y);

//connected with gui key to string
#define JS_XPLUS -1000
#define JS_XMINUS -1001
#define JS_YPLUS -1002
#define JS_YMINUS -1003

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#pragma once

namespace Advanced2D {

	class Input {

	private:
	public:
		HWND window;

		IDirectInput8 *di; 
		IDirectInputDevice8 *keyboard;		
		IDirectInputDevice8 *mouse;		
		LPDIRECTINPUTDEVICE8 g_pJoystick;

		char keyState[256]; 
		char old_keys[256];
		bool button_released[256];

		DIMOUSESTATE mouseState; 
		DIJOYSTATE2 js;				//joystick state 
		POINT position; 

	
		Input();
		virtual ~Input();
		void Update();
		bool GetMouseButton( char button );
		bool Init(HWND window);

		char GetKeyState(int key) { return keyState[key]; }
		long GetPosX() { return position.x; }
		long GetPosY() { return position.y; }
		long GetDeltaX() { return mouseState.lX; }
		long GetDeltaY() { return mouseState.lY; }
		long GetDeltaWheel() { return mouseState.lZ; }

		void UpdateMouse();
		void UpdateKeyboard();

		bool init_joystick();
		void UpdateJoystick();

	};

};