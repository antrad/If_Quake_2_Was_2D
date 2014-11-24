#include "Advanced2D.h"

namespace Advanced2D {

	Input::Input()
	{
		this->g_pJoystick = NULL;
		this->mouse = NULL;
		this->keyboard = NULL;
		this->di = NULL;
	}

	bool Input::Init( HWND hwnd )
	{
		//save window handle
		window = hwnd;		

		//create DirectInput object
		DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, NULL );

		if (this->di)
		{
			g_engine->log_text.push_back("DirectInput initialized.");
			g_engine->e_font->write_stringvector(g_engine->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
		}
		else
		{
			g_engine->log_text.push_back("Failed initializing DirectInput.");
			g_engine->e_font->write_stringvector(g_engine->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
			return false;
		}		

		//initialize keyboard
		di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
		if (this->keyboard)
		{
			g_engine->log_text.push_back("Keyboard detected.");
			g_engine->e_font->write_stringvector(g_engine->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);			
		}
		else
		{
			g_engine->log_text.push_back("Keyboard NOT detected.");
			g_engine->e_font->write_stringvector(g_engine->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
			return false;
		}
		keyboard->SetDataFormat( &c_dfDIKeyboard );
		keyboard->SetCooperativeLevel( window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		keyboard->Acquire();

		//initialize mouse
		di->CreateDevice(GUID_SysMouse, &mouse, NULL);
		if (this->keyboard)
		{
			g_engine->log_text.push_back("Mouse detected.");
			g_engine->e_font->write_stringvector(g_engine->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);			
		}
		else
		{
			g_engine->log_text.push_back("Mouse NOT detected.");
			g_engine->e_font->write_stringvector(g_engine->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
			return false;
		}
		mouse->SetDataFormat(&c_dfDIMouse);
		mouse->SetCooperativeLevel(window, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
		mouse->Acquire();		

		return true;
	}	

	Input::~Input()
	{
		if (di) di->Release();
		if (keyboard) keyboard->Release();
		if (mouse) mouse->Release();
		if (g_pJoystick) g_pJoystick->Release();

	}

	void Input::Update()
	{
		//poll state of the keyboard
		keyboard->Poll();
		if (!SUCCEEDED(keyboard->GetDeviceState(256,(LPVOID)&keyState)))
		{
			//keyboard device lost, try to re-acquire
			keyboard->Acquire();
		}

		//poll state of the mouse
		mouse->Poll();
		if (!SUCCEEDED(mouse->GetDeviceState(sizeof(DIMOUSESTATE),&mouseState)))
		{
			//mouse device lose, try to re-acquire
			mouse->Acquire();
		}

		//get mouse position on screen
		GetCursorPos(&position);
		ScreenToClient(window, &position);

		this->UpdateKeyboard();
		this->UpdateMouse();

		if (this->g_pJoystick) UpdateJoystick();

	}

	void Input::UpdateMouse()
	{
		static bool OldMouse[4]={0,0,0,0};

		static int oldPosX = 0;
		static int oldPosY = 0;

		int deltax = GetDeltaX();
		int deltay = GetDeltaY();

		//check mouse position
		if (GetPosX() != oldPosX || GetPosY() != oldPosY)
		{
			game_mouseMove(GetPosX(),GetPosY());
			oldPosX = GetPosX();
			oldPosY = GetPosY();
		}

		//check mouse motion
		if (deltax != 0 || deltay )
		{
			game_mouseMotion(deltax,deltay);
		}

		//check mouse wheel
		int wheel = GetDeltaWheel();
		if (wheel != 0) game_mouseWheel(wheel);

		//check mouse buttons
		for (int n=0; n<4; n++) 
		{
			if (GetMouseButton(n))
			{
				OldMouse[n]=GetMouseButton(n);

				//if button pressed skip mouse button (GUI has advantage)
				if (g_engine->gui!=NULL)
				{
					if (g_engine->gui->CheckButtonsPress(n)) return;
				}

				game_mousePress(n);

			}
			else if (OldMouse[n]!=0)
			{
				OldMouse[n]=GetMouseButton(n);

				//if button release skip mouse button (GUI has advantage)
				if (g_engine->gui!=NULL)
				{
					if (g_engine->gui->CheckButtonsRelease(n)) return;
				}

				game_mouseRelease(n);
				//no button pressed, release attached button, delte dropbox options
				if (g_engine->getGameState()!=PLAY) g_engine->gui->setSelectedButton(NULL);

			}
		}

	}//UpdateMouse()

	void Input::UpdateKeyboard()
	{
		for (int n=0; n<255; n++)
		{
			if (GetKeyState(n) & 0x80)//check for key press
			{
				game_keyPress(n);
				old_keys[n] = GetKeyState(n);
			} 
			else if (old_keys[n] & 0x80)//check for release
			{
				game_keyRelease(n);
				button_released[n] = true;
				old_keys[n] = GetKeyState(n);
			}
		}
	}

	bool Input::GetMouseButton( char button )
	{
		return (mouseState.rgbButtons[button] & 0x80);
	}

	////////-----------------------------------------------------------------------------
	// Name: EnumObjectsCallback()
	// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
	//       joystick. This function enables user interface elements for objects
	//       that are found to exist, and scales axes min/max values.
	//-----------------------------------------------------------------------------
	BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
		VOID* pContext )
	{
		HWND hDlg = ( HWND )pContext;

		static int nSliderCount = 0;  // Number of returned slider controls
		static int nPOVCount = 0;     // Number of returned POV controls

		// For axes that are returned, set the DIPROP_RANGE property for the
		// enumerated axis in order to scale min/max values.
		if( pdidoi->dwType & DIDFT_AXIS )
		{
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof( DIPROPRANGE );
			diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin = -1000;
			diprg.lMax = +1000;

			// Set the range for the axis
			if( FAILED( g_engine->p_input->g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
				return DIENUM_STOP;

		}    
		return DIENUM_CONTINUE;
	}

	struct DI_ENUM_CONTEXT
	{
		DIJOYCONFIG* pPreferredJoyCfg;
		bool bPreferredJoyCfgValid;
	};

	BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
	{
		DI_ENUM_CONTEXT* pEnumContext = ( DI_ENUM_CONTEXT* )pContext;
		HRESULT hr;

		// Skip anything other than the perferred joystick device as defined by the control panel.  
		// Instead you could store all the enumerated joysticks and let the user pick.
		if(pEnumContext->bPreferredJoyCfgValid && !IsEqualGUID(pdidInstance->guidInstance,pEnumContext->pPreferredJoyCfg->guidInstance))
			return DIENUM_CONTINUE;

		// Obtain an interface to the enumerated joystick.
		hr = g_engine->p_input->di->CreateDevice( pdidInstance->guidInstance, &g_engine->p_input->g_pJoystick, NULL );

		// If it failed, then we can't use this joystick. (Maybe the user unplugged
		// it while we were in the middle of enumerating it.)
		if( FAILED(hr) ) return DIENUM_CONTINUE;

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return DIENUM_STOP;
	}

	bool Input::init_joystick()
	{
		HRESULT hr;

		DIJOYCONFIG PreferredJoyCfg = {0};
		DI_ENUM_CONTEXT enumContext;
		enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
		enumContext.bPreferredJoyCfgValid = false;

		IDirectInputJoyConfig8* pJoyConfig = NULL;
		if(FAILED(hr=di->QueryInterface(IID_IDirectInputJoyConfig8,(void**)&pJoyConfig)))
			return false;

		PreferredJoyCfg.dwSize = sizeof( PreferredJoyCfg );
		if(SUCCEEDED(pJoyConfig->GetConfig(0,&PreferredJoyCfg,DIJC_GUIDINSTANCE))) enumContext.bPreferredJoyCfgValid = true;
		SAFE_RELEASE( pJoyConfig );//This function is expected to fail if no joystick is attached

		// Look for a simple joystick we can use for this sample program.
		if(FAILED(hr=di->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,&enumContext,DIEDFL_ATTACHEDONLY)))
			return false;		

		// Make sure we got a joystick
		if(NULL==g_pJoystick) return false;

		// Set the data format to "simple joystick" - a predefined data format 
		// A data format specifies which controls on a device we are interested in,
		// and how they should be reported. This tells DInput that we will be
		// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
		if(FAILED(hr=g_pJoystick->SetDataFormat(&c_dfDIJoystick2)))
			return false;

		// Set the cooperative level to let DInput know how this device should
		// interact with the system and with other DInput applications.
		if(FAILED(hr=g_pJoystick->SetCooperativeLevel(g_engine->getWindowHandle(),DISCL_EXCLUSIVE|DISCL_FOREGROUND)))
			return false;

		// Enumerate the joystick objects. The callback function enabled user
		// interface elements for objects that are found, and sets the min/max
		// values property for discovered axes.
		if(FAILED(hr=g_pJoystick->EnumObjects(EnumObjectsCallback,( VOID* )g_engine->getWindowHandle(),DIDFT_ALL)))
			return false;

		return true;

	}

	void Input::UpdateJoystick()
	{
		if( NULL == g_pJoystick ) return;

		static char old_keys[128];

		HRESULT hr;
		TCHAR strText[512] = {0}; // Device state text		

		// Poll the device to read the current state
		hr = g_pJoystick->Poll();
		if( FAILED( hr ) )
		{
			// DInput is telling us that the input stream has been interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We just re-acquire and try again.
			hr = g_pJoystick->Acquire();
			while(hr==DIERR_INPUTLOST) hr = g_pJoystick->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			return;
		}

		// Get the input's device state
		if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) )
			return; // The device should have been acquired during the Poll()

		// Display joystick state to dialog
		// Fill up text with which buttons are pressed
		for( int i = 0; i < 128; i++ )
		{
			if( js.rgbButtons[i] & 0x80 )
			{
				game_joystickPress(i);
				old_keys[i] = js.rgbButtons[i] & 0x80;
			}
			else if (old_keys[i] & 0x80)//check for release
			{
				game_joystickRelease(i);
				old_keys[i] = js.rgbButtons[i] & 0x80;
			}
		}

		static bool x_plus = false;
		static bool x_minus = false;
		static bool y_plus = false;
		static bool y_minus = false;

		if (js.lX==1000)
		{
			game_joystickXYAxisPress(JS_XPLUS);
			x_plus = true;
		}
		else if(x_plus)
		{
			game_joystickXYAxisRelease(JS_XPLUS);
			x_plus = false;
		}

		if (js.lX==-1000)
		{
			game_joystickXYAxisPress(JS_XMINUS);
			x_minus = true;
		}
		else if(x_minus)
		{
			game_joystickXYAxisRelease(JS_XMINUS);
			x_minus = false;
		}

		if (js.lY==-1000)
		{
			game_joystickXYAxisPress(JS_YPLUS);
			y_plus = true;
		}
		else if(y_plus)
		{
			game_joystickXYAxisRelease(JS_YPLUS);
			y_plus = false;
		}

		if (js.lY==1000)
		{
			game_joystickXYAxisPress(JS_YMINUS);
			y_minus = true;
		}
		else if(y_minus)
		{
			game_joystickXYAxisRelease(JS_YMINUS);
			y_minus = false;
		}		

		game_joystickAxis(js.lX,js.lY,js.lZ,js.lRz);

	}

};
