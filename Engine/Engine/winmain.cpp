#include "Advanced2D.h"

HINSTANCE g_hInstance;
HWND g_hWnd;
int g_nCmdShow;

//declare global engine object
Advanced2D::Engine *g_engine;

bool gameover;

//window event callback function
LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		gameover = true;
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MSG msg;
	srand((unsigned int)time(NULL));
	g_hInstance = hInstance;
	g_nCmdShow = nCmdShow;
	DWORD dwStyle, dwExStyle;	

	//Create engine object first! 
	g_engine = new Advanced2D::Engine();

	//let main program have a crack at things before window is created
	if (!game_preload())
	{
		MessageBox(g_hWnd, "Error in game preload.", "MUGEe Error", MB_OK);
		return 0; 
	}	

	//get window caption string from engine
	char title[255];
	sprintf(title, "%s", g_engine->getAppTitle().c_str());

	//set window dimensions
	RECT windowRect;
	windowRect.left = (long)0;
	windowRect.right = (long)g_engine->getScreenWidth();
	windowRect.top = (long)0;
	windowRect.bottom = (long)g_engine->getScreenHeight();

	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//fill the struct with info
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)WinProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= title;
	wc.hIconSm			= NULL;

	//set up the window with the class info
	RegisterClassEx(&wc);

	//set up the screen in windowed or fullscreen mode?
	if (g_engine->getFullscreen()) 
	{
		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = g_engine->getScreenWidth();
		dm.dmPelsHeight = g_engine->getScreenHeight();
		dm.dmBitsPerPel = g_engine->getColorDepth();
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(g_hWnd, "Display mode failed.\n\nTry changing the screen resolution:\n\n- via the options menu\n- or via System\\config.txt\n\n and restart.", "MUGEe Error", MB_OK);
			g_engine->setScreenWidth(1024);
			g_engine->setScreenHeight(768);
			windowRect.right = (long)g_engine->getScreenWidth();
			windowRect.bottom = (long)g_engine->getScreenHeight();
			dwStyle = WS_OVERLAPPEDWINDOW;
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			ShowCursor(FALSE);
			//dm.dmPelsHeight = g_engine->getScreenHeight();
			//dm.dmBitsPerPel = g_engine->getColorDepth();
			g_engine->setFullscreen(false);
		}

		dwStyle = WS_POPUP;
		dwExStyle = WS_EX_APPWINDOW;
		ShowCursor(FALSE);
	}
	else
	{
		dwStyle = WS_OVERLAPPEDWINDOW;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		ShowCursor(FALSE);
	}

	//adjust window to true requested size
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	//create the program window
	g_hWnd = CreateWindowEx( 0,
		title,								        //window class
		title,								        //title bar
		dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,	  
		0, 0,										//x,y coordinate
		windowRect.right - windowRect.left,			//width of the window
		windowRect.bottom - windowRect.top,			//height of the window
		0,											//parent window
		0,											//menu
		g_hInstance,									//application instance
		0);											//window parameters


	//was there an error creating the window?
	if (!g_hWnd)
	{
		MessageBox(g_hWnd, "Failed to create program window.", "MUGEe Error", MB_OK);
		return 0;
	}

	//display the window
	ShowWindow(g_hWnd, g_nCmdShow);
	UpdateWindow(g_hWnd);

	//initialize the engine
	g_engine->setWindowHandle(g_hWnd);
	if (!g_engine->Init(g_engine->getScreenWidth(), g_engine->getScreenHeight(), g_engine->getColorDepth(), g_engine->getFullscreen()))
	{
		MessageBox(g_hWnd, "Error initializing MUGEe", "MUGEe Error", MB_OK);
		g_engine->save_log();
		return 0;
	}
	else
	{
		g_engine->log_text.push_back("Initializing engine and game completed.");
		g_engine->e_font->write_stringvector(g_engine->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
		g_engine->save_log();
	}


	// main message loop
	gameover = false;
	while (!gameover)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		g_engine->Update();
	}

	game_end();

	delete g_engine;

	return 1;

}
