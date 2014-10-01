// desiBal.cpp : Defines the entry point for the application.
//



#include"stdafx.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

Graphics* graphics;
CSkeletonBasics *skeleton;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);



 	// TODO: Place code here.


	MSG       msg = { 0 };
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DESIBAL, szWindowClass, MAX_LOADSTRING);

	skeleton = new CSkeletonBasics();
	
	MyRegisterClass(hInstance);

	graphics = new Graphics();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		delete graphics;
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DESIBAL));
	
	const int eventCount = 1;
	HANDLE hEvents[eventCount];

	// Main message loop
	while (WM_QUIT != msg.message)
	{
		hEvents[0] = skeleton->m_hNextSkeletonEvent;

		// Check to see if we have either a message (by passing in QS_ALLEVENTS)
		// Or a Kinect event (hEvents)
		// Update() will check for Kinect events individually, in case more than one are signalled
		MsgWaitForMultipleObjects(eventCount, hEvents, FALSE, INFINITE, QS_ALLINPUT);

		// Explicitly check the Kinect frame event since MsgWaitForMultipleObjects
		// can return for other reasons even though it is signaled.
			skeleton->Update(graphics);

		
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If a dialog message will be taken care of by the dialog proc
			if ((hWnd != NULL) && IsDialogMessageW(hWnd, &msg))
			{
				continue;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	
	if(graphics) delete graphics;

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;//MessageRouter;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra = DLGWINDOWEXTRA;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DESIBAL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCE(IDC_DESIBAL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   
   hInst = hInstance; // Store instance handle in our global variable

   RECT rect = { 0, 0, 800, 600 };
   AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
  hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0,
	rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInst, NULL);

  // hWnd = CreateDialogParamW(  hInstance, szTitle, NULL, (DLGPROC)WndProc, reinterpret_cast<LPARAM>(skeleton));

   if (!hWnd)
   {
      return FALSE;
   }


   if (!graphics->Init(hWnd)){
	   return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);


   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//return MessageRouter(hWnd, message, wParam, lParam);
	
	PAINTSTRUCT ps;

	CSkeletonBasics* pThis = NULL;

	switch (message)
	{

	case WM_CREATE:
		


		break;
	
	case WM_PAINT:
		//pThis = reinterpret_cast<CSkeletonBasics*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		pThis = reinterpret_cast<CSkeletonBasics*>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

		
		// Bind application window handle
		skeleton->m_hWnd = hWnd;

		// Init Direct2D
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &(skeleton->m_pD2DFactory));

		// Look for a connected Kinect, and create it if found
		BeginPaint(hWnd, &ps);
		skeleton->CreateFirstConnected();
		/*
		// TODO: Add any drawing code here...
		graphics->BeginDraw();
		graphics->ClearScreen(0.0f, 0.0f, 0.5f);
		for (int i = 0; i < 1000; i++){
			graphics->DrawCircle(rand()%800, rand()%600, rand()%100,
				(rand()%100) / 100.0f,
				(rand() % 100) / 100.0f,
				(rand() % 100) / 100.0f,
				(rand() % 100) / 100.0f);

		}

		graphics->EndDraw();
		*/
		EndPaint(hWnd, &ps);
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return skeleton->MessageRouter(hWnd, message, wParam, lParam);

}